#include<stdio.h>
#include<termios.h>

static struct termios	terminal_backup;

void RawModeOff(){
tcsetattr(0,TCSANOW,&terminal_backup);
}

void RawModeOn(){
struct termios terminal;
tcgetattr(0,&terminal);
terminal_backup=terminal;
terminal.c_lflag&=~(ICANON);
//terminal.c_cc[VMIN]=1;
tcsetattr(0,TCSANOW,&terminal);
return;
}


int main(){
RawModeOn();
char c;

do{
c=getchar();
printf("%c",c);
}while(c!='#');

RawModeOff();
return 0;
}





