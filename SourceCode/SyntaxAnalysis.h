//
//  SyntaxAnalysis.h
//  Syntax Analysis
//
//  Created by 陈子源 on 2019/5/29.
//  Copyright © 2019 陈子源. All rights reserved.
//

#ifndef SYNTAXANALYSIS_H
#define SYNTAXANALYSIS_H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

ifstream fileIn;                    // 所读取的tokenOut文件
int numOfLine = 0;                  // 当前读取的行数
string token = "";                  // 当前的token
string space = "-|";                // 用来形成树结构的缩进

void openFile (string fileName);    // 打开指定文件
void writeFile (string stringOut);  // 输入指定内容到SyntaxOut.txt
void errorOut (string error);       // 输出错误信息
void closeFile();                   // 关闭tokenOut文件

string readLine();                  // 读取各行 找到下一个能包含token的长句 返回token
string readToken (string sentence); // 读取包含token的长句 返回token

void match (string str);            // 判断token与str是否相同 不相同则返回错误

void start();                       // 用状态机的思想来构建每一条语法规则
void threadSpec();
void featureSpec();
void parameterSpec();
void portSpec();
void iOtype();
void flowSpec();
void flowSourceSpec();
void flowSinkSpec();
void flowPathSpec();
void association();
void splitter();
void reference();

void openFile (string fileName) {
    fileIn.open(fileName);
    if (fileIn.fail()) {
        cout << "Open File Error!\nPlease put the target file in the lexical analysis code folder!" << endl;
        exit(1);
    }
}

void writeFile (string stringOut) {
    ofstream fileOut("SyntaxOut.txt", ios::app);
    fileOut << stringOut << endl;
    fileOut.close();
}

void errorOut (string error) {
    ofstream fileOut("SyntaxOut.txt", ios::app);
    fileOut << error << endl;
    cout << error << endl;
    fileOut.close();
    exit(1);
}

void closeFile() {
    fileIn.close();
}

string readLine() {
    string tokenOut = "";
    string line = "";
    int space = 1;
    while (true) {
        tokenOut = "";
        getline(fileIn, line);
        if (line.size() > 0 && line[0] == 'L') {
            numOfLine ++;
        } else if (line.size() > 3 && line[3] == 'S') {
            if (numOfLine > 9) { space = 2; }  // 对两位数的numOfLine 要做处理
            if (line[10 + space] == '-') {
                string err = "Error : lexical error in " + to_string(numOfLine) + " line";
                errorOut(err);
            } else if (line[10 + space] == '|') {
                tokenOut = readToken(line.substr(11 + space));
                break;
            }
        } else if (line.size() > 0 && line[0] == 'D') {  // 在此处读取完tokenOut
            tokenOut = "END";
            break;
        }
    }
    return tokenOut;
}

string readToken(string sentence) {
    string tokenOut = "";
    switch (sentence[0]) {
        case 'I':
            tokenOut = "identifier, name" + sentence.substr(16);
            break;
        case 'D':
            tokenOut = "decimal, value" + sentence.substr(14);
            break;
        case 'S':
            tokenOut = sentence.substr(9);
            break;
        case 'R':
            tokenOut = sentence.substr(16);
            break;
    }
    return tokenOut;
}

void match(string str) {
    if (token == "END") {  // 当token为END的时候，就是tokenOut读完的情况
        if (str == "END") {
            writeFile("\nSyntax Analysis Complete");
            cout << "Syntax Analysis Successfully Complete" << endl;
        } else {
            errorOut("Error: tokenOut file ends at a improper position in " + to_string(numOfLine) + " line");
        }
        exit(0);
    }
    if (str == "identifier"){
        if (token.substr(0, 10) != str) {
            errorOut("Error: syntax error in " + to_string(numOfLine) + " line\n" + "Unexpected token: " + token);
        } else {
            writeFile(space + token);
        }
    } else if (str == "decimal") {
        if (token.substr(0, 7) != str) {
            errorOut("Error: syntax error in " + to_string(numOfLine) + " line\n" + "Unexpected token: " + token);
        } else {
            writeFile(space + token);
        }
    } else if (str == "END") {
        if (token != str) {
            errorOut("Error: syntax error in " + to_string(numOfLine) + " line\n" + "Unexpected token: " + token);
        } else {
            writeFile("\nEnd of SyntaxOut");
        }
    } else{
        if (token != str) { // 如果当前拿到的token和所要识别的token不一样
            errorOut("Error: syntax error in " + to_string(numOfLine) + " line\n" + "Unexpected token: " + token);
        } else {
            writeFile(space + token);
        }
    }
}

void start() {
    token = readLine();
    do {
        threadSpec();
        token = readLine();
    } while (token != "END");
    match("END");
}

void threadSpec() {
    match("thread");
    token = readLine(); match("identifier");
    string threadName = token;
    token = readLine();
    if (token == "features") {
        match("features");
        space = "-" + space;
        featureSpec();
        space.erase(0, 1);
    }
    if (token == "flows") {
        match("flows");
        space = "-" + space;
        flowSpec();
        space.erase(0, 1);
        token = readLine();
    }
    if (token == "properties") {
        match("properties");
        token = readLine();
        space = "-" + space;
        association();
        space.erase(0, 1);
        token = readLine(); match(";");
        token = readLine();
    }
    match("end");
    token = readLine(); match(threadName);  // 要与一开头的identifier的内容一致
    token = readLine(); match(";");
}

void featureSpec() {
    token = readLine();
    if (token == "none") {
        match("none");
        token = readLine(); match(";");
        token = readLine();
    } else {
        match("identifier");
        token = readLine(); match(":");
        space = "-" + space;
        iOtype();
        space.erase(0, 1);
        if (token == "parameter") {
            space = "-" + space;
            parameterSpec();
            space.erase(0, 1);
        } else {
            space = "-" + space;
            portSpec();
            space.erase(0, 1);
        }
    }
}

void iOtype() {
    token = readLine();
    if (token == "out") {
        match("out");
        token = readLine();
    } else {
        match("in");
        token = readLine();
        if (token == "out") {
            match("out");
            token = readLine();
        }
    }
}

void portSpec() {
    if (token == "data") {
        match("data");
        token = readLine(); match("port");
        token = readLine();
        if (token.substr(0, 10) == "identifier") {
            space = "-" + space;
            reference();
            space.erase(0, 1);
        }
    } else {
        match("event");
        token = readLine();
        if (token == "data") {
            match("data");
            token = readLine(); match("port");
            token = readLine();
            if (token.substr(0, 10) == "identifier") {
                space = "-" + space;
                reference();
                space.erase(0, 1);
            }
        } else {
            match("port");
            token = readLine();
        }
    }
    if (token == "{") {
        match("{");
        space = "-" + space;
        token = readLine();
        do {
            association();
            token = readLine();
        } while (token == "none" || token.substr(0, 10) == "identifier");
        space.erase(0, 1);
        match("}");
        token = readLine();
    }
    match(";");
    token = readLine();
}

void parameterSpec() {
    match("parameter");
    token = readLine();
    if (token.substr(0, 10) == "identifier") {
        space = "-" + space;
        reference();
        space.erase(0, 1);
    }
    if (token == "{") {
        match("{");
        space = "-" + space;
        token = readLine();
        do {
            association();
            token = readLine();
        } while (token == "none" || token.substr(0, 10) == "identifier");
        space.erase(0, 1);
        match("}");
        token = readLine();
    }
    match(";");
    token = readLine();
}

void reference() {
    match("identifier");
    token = readLine();
    while (token == "::") {
        match("::");
        token = readLine(); match("identifier");
        token = readLine();
    }
}

void association() { 
    if (token == "none") {
        match("none");
    } else {
        match("identifier");
        token = readLine();
        if (token == "::") {
            match("::");
            token = readLine(); match("identifier");
            token = readLine();
        }
        space = "-" + space;
        splitter();
        space.erase(0, 1);
        token = readLine();
        if (token == "constant") {
            match("constant");
            token = readLine();
        }
        match("access");
        token = readLine(); match("decimal");
    }
}

void splitter() {
    if (token == "=>") {
        match("=>");
    } else {
        match("+=>");
    }
}

void flowSpec() {
    token = readLine();
    if (token == "none") {
        match("none");
        token = readLine(); match(";");
    } else {
        match("identifier");
        token = readLine(); match(":");
        token = readLine(); match("flow");
        token = readLine();
        if (token == "source") {
            space = "-" + space;
            flowSourceSpec();
            space.erase(0, 1);
        } else if (token == "sink") {
            space = "-" + space;
            flowSinkSpec();
            space.erase(0, 1);
        } else {
            space = "-" + space;
            flowPathSpec();
            space.erase(0, 1);
        }
    }
}

void flowSourceSpec() {
    match("source");
    token = readLine(); match("identifier");
    token = readLine();
    if (token == "{") {
        match("{");
        space = "-" + space;
        token = readLine();
        do {
            association();
            token = readLine();
        } while (token == "none" || token.substr(0, 10) == "identifier");
        space.erase(0, 1);
        match("}");
        token = readLine();
    }
    match(";");
}

void flowSinkSpec() {
    match("sink");
    token = readLine(); match("identifier");
    token = readLine();
    if (token == "{") {
        match("{");
        space = "-" + space;
        token = readLine();
        do {
            association();
            token = readLine();
        } while (token == "none" || token.substr(0, 10) == "identifier");
        space.erase(0, 1);
        match("}");
        token = readLine();
    }
    match(";");
}

void flowPathSpec() {
    match("path");
    token = readLine(); match("identifier");
    token = readLine(); match("->");
    token = readLine(); match("identifier");
    token = readLine(); match(";");
}

#endif /* SYNTAXANALYSIS_H */
