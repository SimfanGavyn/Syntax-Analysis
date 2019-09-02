//
//  main.cpp
//  Syntax Analysis
//
//  Created by 陈子源 on 2019/5/29.
//  Copyright © 2019 陈子源. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include "SyntaxAnalysis.h"

string nowTime() {
    time_t rawtime;
    struct tm *timeInfo;
    string out = "";
    time(&rawtime);
    timeInfo = localtime(&rawtime);
    out += to_string(timeInfo -> tm_year + 1900) + "/" + to_string(timeInfo -> tm_mon + 1) + "/" +
           to_string(timeInfo -> tm_mday) + " " + to_string(timeInfo -> tm_hour) + ":" +
           to_string(timeInfo -> tm_min) + ":" + to_string(timeInfo -> tm_sec);
    return out;
}

void buildUpEnvironment(string fileName) {
    ofstream fileOut ("SyntaxOut.txt");
    string out = "THREAD COMPILATION: " + fileName + "  " + nowTime();
    out += "\nSTART:\n";
    fileOut << out << endl;
    fileOut.close();
}

void syntaxAnalysis(string fileName) {
    openFile(fileName);
    buildUpEnvironment(fileName);
    start();
    closeFile();
}

int main(int argc, const char * argv[]) {
    string fileName = argv[1];
    syntaxAnalysis(fileName);
    return 0;
}
