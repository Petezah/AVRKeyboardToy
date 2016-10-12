// TinyBasicPlus.h

#ifndef _TINYBASICPLUS_H_
#define _TINYBASICPLUS_H_

enum BASICRunState
{
    InitStart,
    WarmStart,
    Idle,
    Run,
    Terminate
};

void injectln(char* line);
void performBASICWarmStart();
void setupBASIC();
BASICRunState execBASIC(bool triggerRunExec = false);

#endif //_TINYBASICPLUS_H_
