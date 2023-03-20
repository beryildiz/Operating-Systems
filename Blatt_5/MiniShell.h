//
// Created by berkan on 26.11.21.
//

#ifndef BLATT_5_MINISHELL_H
#define BLATT_5_MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <limits.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <pwd.h>
#include <wait.h>
#include <pthread.h>
#include "curl/curl.h"
using namespace std;

class MiniShell{
private:
    char* m_args[128];
    char* m_argsForPipe[128];
    bool m_exited;
    char* resolveEnviron(char* token);

    string getCurrentDir();
    string getUsername();
    void argVecToArgArr(vector<string> input);
    void greeting();

    void myFork(int i, int size, int (*fdVec)[2], pid_t (*pid));
    void splitStrForPipe();
    int countPipes();
    vector<string> cropInput(string &input);

public:
    MiniShell();
    ~MiniShell();
    void execIO();
    bool getExited(){ return m_exited;}

};



#endif //BLATT_5_MINISHELL_H
