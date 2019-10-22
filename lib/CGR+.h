#ifndef CGR_H
#include"CGR.h"
#endif

#ifndef CGR_PLUS_H
#define CGR_PLUS_H

int abs(int n){
	return (n<0)?-n:n;
}

void CGR_line(int x1,int y1,int x2,int y2,char ch){
if(x1>x2){//swap
int z=x1;
x1=x2;
x2=z;
z=y1;
y1=y2;
y2=z;
}
int x=x1;
int y=y1;
int i;
CGR_setChar(x1,y1,ch);
CGR_setChar(x2,y2,ch);
for(i=0;i<x2-x1-1;i++){
x++;
y=((x-x1)*y2+(x2-x)*y1)/(x2-x1);
//printf("line(%d,%d)",x,y);
CGR_setChar(x,y,ch);
}
return;
}

void CGR_square(int x,int y,int width,int height,char ch){
if(x<0||y<0){
perror("invalid x,y");
exit(0);
}
int i;
for(i=0;i<width;i++)
	CGR_setChar(x+i,y,ch);
for(i=0;i<height;i++){
	CGR_setChar(x,y+i,ch);
	CGR_setChar(x+width-1,y+i,ch);
}
for(i=0;i<width;i++)
        CGR_setChar(x+i,y+height-1,ch);

return;
}

int CGR_setNumber(int x,int y,int n){
int pos=1;
if(n<0)return -1;
if(n<10){
CGR_setChar(x,y,n+'0');
return 1;
}

while(n>=pos)pos*=10;
pos/=10;
int p=0;
while(pos>0){
int a=n%(pos*10)/pos;
CGR_setChar(x+p,y,a+'0');
p++;
pos/=10;
}
return p;
}

void CGR_setString(int x,int y,int len,char *str){
int i;
for(i=0;i<len;i++)
	CGR_setChar(x+i,y,str[i]);
return;
}

/*
enum color{
RED,
GREEN,
YELLOW,
BLUE,
MAGENTA,
CYAN,
RESET
};

void CGR_color(int colormode){
switch(colormode){
case RED:
printf("\x1b[31m");
break;

case GREEN:
printf("\x1b[32m");
break;

case YELLOW:
printf("\x1b[33m");
break;

case BLUE:
printf("\x1b[34m");
break;

case MAGENTA:
printf("\x1b[35m");
break;

case CYAN:
printf("\x1b[36m");
break;

case RESET:
printf("\x1b[0m");
break;
}

}
*/
#endif
