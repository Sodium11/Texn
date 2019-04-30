#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include"CGR.h"
int main(){
struct termios tp,backup;
tcgetattr(0,&backup);
tcgetattr(0,&tp);
cfmakeraw(&tp);
tcsetattr(0,TCSANOW,&tp);//raw mode on
char ch;
CGR_init(59,40);
do{
ch=getchar();
//putchar(ch);
//printf("%d\n",ch);
CGR_setChar(0,0,ch);
CGR_draw();
}while(ch!='-');
CGR_end();
tcsetattr(0,TCSANOW,&backup);//raw mode off
return 0;
}
