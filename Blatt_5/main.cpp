#include "MiniShell.h"

ostream& operator<<(ostream& os, const vector<string>& v){
    for(int i = 0; i < v.size(); i++){
        os << v.at(i) << endl;
    }
    return os;
}

int main() {

    MiniShell minishell;

    while(!minishell.getExited()){
        minishell.execIO();
    }

    return 0;
}







