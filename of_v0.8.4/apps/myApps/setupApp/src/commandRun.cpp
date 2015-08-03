#include "CommandRun.h"


//--------------------------------------------------------------
CommandRun::CommandRun( int ID ) {
    _ID = ID;
    
    // コマンドの実行結果
    _commandStatusCode = -1;
}

//--------------------------------------------------------------
void CommandRun::setup() {
    cout << "CommandRun-> setup" << "\n";
    
    char shellCommand[1024] = "sh setup.sh";
    _commandStatusCode = system(shellCommand);
    _commandStatusCode = WEXITSTATUS(_commandStatusCode);
}

//--------------------------------------------------------------
void CommandRun::update() {
}