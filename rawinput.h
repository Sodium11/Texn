#ifndef RAWIN_H
#define RAWIN_H
#include<termios.h>

static struct termios	terminal_backup;
int raw_mode=0;
void RawModeOff(){
tcsetattr(0,TCSANOW,&terminal_backup);
raw_mode=0;
return;
}

void RawModeOn(){
struct termios terminal;
tcgetattr(0,&terminal);
terminal_backup=terminal;
terminal.c_lflag&=~(ICANON);
//terminal.c_cc[VMIN]=1;
tcsetattr(0,TCSANOW,&terminal);
raw_mode=1;
return;
}

char rawinput(){
if(raw_mode==0)
RawModeOn();
char ch=-1;
scanf("%c",&ch);
return ch;
}

int rawstr(char *str){
int p=0;
char buf;
while((buf = rawinput())!='\n')
str[p++]=buf;
str[p]='\0';
return p;
}
#endif
