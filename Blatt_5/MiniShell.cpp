//
// Created by berkan on 26.11.21.
//
#include "MiniShell.h"

MiniShell::MiniShell() {
    m_exited = false;
    greeting();
}

MiniShell::~MiniShell() {
    cout << "Programm wurde beendet" << endl;
}

void MiniShell::greeting() {
    cout << "Willkommen" << endl;
}

string MiniShell::getUsername() {
    return getenv("LOGNAME");
}

string MiniShell::getCurrentDir() {
    char directory[256];
    getcwd(directory,256);

    return directory;
}

void MiniShell::execIO() {
    std::cout << getUsername()  << ":" <<  getCurrentDir() << "$ ";

    string input;
    std::getline(std::cin, input);

    argVecToArgArr(cropInput(input));

    string progName = m_args[0];

    if(progName == "exit"){
        m_exited = true;
    } else if(progName=="showenv"){
        m_args[0] = "env";
    } else if(progName == "export"){ // export implementieren
        putenv(m_args[1]);
    } else if (progName == "cd") {  // Ausfuehrung von CD
        char* homedir;
        if ((homedir = getenv("HOME")) == nullptr) {
            homedir = getpwuid(getuid())->pw_dir;
        }
        if (m_args[1] == nullptr) {
            chdir(homedir);
        } else {
            chdir(m_args[1]);
        }
    } else {

        /* Blatt 4 Execute
        // Kindprozesse erstellen
        if ((pid = fork()) < 0) {
            std::cout << "Fork Error" << std::endl;
        } else if (pid == 0) {
            execvp(m_args[0], m_args);
            exit(EXIT_SUCCESS);
        } else {
            int status;
            if (wait(&status) >= 0) {
                std::cout << "Child Process exited with " << WEXITSTATUS(status) << std::endl;
            }
        }
    */

        pid_t pid[128];
        //Filedescriptor-Vector mit Filedeskriptoren mit Read_End(0) und Write_End(1)
        int fdVec[128][2];

        int size = countPipes();
        for(int i=0; i<size; i++) {
            pipe(fdVec[i]);
        }

        myFork(0, size, fdVec, pid);
        for (int i=0; i<size+1; i++) {
            if (pid[i] > 0) {
                int status;
                for (int j=0; j<size; j++) {
                    close(fdVec[j][0]);
                    close(fdVec[j][1]);
                }
                waitpid(pid[i], &status, 0);
                cout << "Child " << i << " Process exited with " << WEXITSTATUS(status) << std::endl;
            }
        }
    }
}

vector<string> MiniShell::cropInput(string &input) {
    string spaceDelimeter = " ";
    size_t pos = 0;
    vector<string> croppedVec;

    while((pos = input.find(spaceDelimeter)) != string::npos){
        croppedVec.push_back(input.substr(0,pos));
        input.erase(0,pos + 1);
    }
    croppedVec.push_back(input.substr(0,pos));

    return croppedVec;
}

// Call by Reference implementieren... Das ist unsauber!!!
void MiniShell::argVecToArgArr(vector<string> input) {
    vector<char*> argumentsVec;
    for (int i = 0; i < input.size(); i++) {
        if(input[i][0] == '$'){
            m_args[i] = resolveEnviron(&input[i][0]);
        } else {
            m_args[i] = &input[i][0];
        }
    }
    m_args[input.size()] = nullptr;
}

char* MiniShell::resolveEnviron(char *token) {
    if(token!=nullptr){
        return getenv(&token[1]);
    } else {
        return nullptr;
    }
}

void MiniShell::myFork(int i, int size, int (*fdVec)[2], pid_t (*pid)) {
    splitStrForPipe();

    if( (pid[i]=fork()) == 0) {
        if ( (i > 0) && (i < size) ) {
            dup2(fdVec[i-1][0], STDIN_FILENO); //Auslesen vorheriger Pipe
            dup2(fdVec[i][1], STDOUT_FILENO); //Beschreiben der aktuellen Pipe
        } else if (i == 0) {
            dup2(fdVec[i][1], STDOUT_FILENO); //Beschreiben der aktuellen Pipe
        } else {
            dup2(fdVec[i-1][0], STDIN_FILENO); //Auslesen vorheriger Pipe
        }

        //Filedeskriptoren schliessen
        for (int j=0; j<size; j++) {
            close(fdVec[j][0]);
            close(fdVec[j][1]);
        }
        execvp(m_argsForPipe[0], m_argsForPipe);

        exit(EXIT_SUCCESS);
    } else if (i < size) {
        myFork(i+1, size, fdVec, pid);
    }
}

int MiniShell::countPipes() {
    char* spaceDelimeter = "|";
    int count = 0;
    int i=0;

    while(m_args[i] != nullptr) {
        if(strcmp(m_args[i],spaceDelimeter) == 0) {
            count++;
        }
        i++;
    }
    return count;
}

void MiniShell::splitStrForPipe() {
    char* spaceDelimeter = "|";
    size_t pos = 0;

    while(m_args[pos] != nullptr) {
        if(strcmp(m_args[pos],spaceDelimeter) == 0){
            break;
        } else {
            m_argsForPipe[pos] = m_args[pos];
        }
        pos++;
    }
    m_argsForPipe[pos] = nullptr;

    //beginning zu Stelle nach Pipe Symbol verschieben // Stackoverflow
    memmove(m_args, m_args+pos+1, sizeof(m_args) - sizeof(*m_args));
}



