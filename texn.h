#ifndef TEXN_H
#define TEXN_H

#define WIDTH 50
#define HEIGHT 38
#define MAP_P(x,y) ((x)+(y)*CGR_getWidth())
#define DATA_SIZE 100000
#define LINE 100

#define EXIT_KEY 24
#define SAVE_KEY 19
#define ENTER_KEY 10
#define DEL_KEY 127
#define ARROW_UP 65
#define ARROW_DOWN 66
#define ARROW_RIGHT 67
#define ARROW_LEFT 68

#define CURSOR '|'


typedef struct {
int linenum;
int linesize[LINE];
char data[DATA_SIZE];
}texndata;

texndata emptyTD(){
texndata txt;
txt.linenum=1;
for(int i=0;i<LINE;i++)
txt.linesize[i]=0;
for(int i=0;i<DATA_SIZE;i++)
txt.data[i]='\0';
return txt;
}

typedef struct{
texndata txt;
int x;
int y;
int width;
int height;
int x_offset;
int y_offset;
}editordata;

editordata emptyED(int width,int height){
editordata ed;
ed.txt=emptyTD();
ed.x=0;
ed.y=0;
ed.width=width;
ed.height=height;
ed.x_offset=0;
ed.y_offset=0;
return ed;
}

int linelen(editordata *ed,int l){
return ed->txt.linesize[l];
}

int GlobalY(editordata *ed){
return ed->y+ed->y_offset;
}

void moveCursor(char direction,editordata* ed){
int x=ed->x;
int y=ed->y;
int linenum=ed->txt.linenum;
int width=ed->width;
int height=ed->height;
int Gy=GlobalY(ed);
switch(direction){
case'u':
if(y>0)y--;
break;

case'd':
if(y<linenum) y++;
break;

case'r':
if(x<linelen(ed,Gy))x++;
break;

case'l':
if(x>0)x--;
break;

default:
break;
}
ed->x=x;
ed->y=y;
return;
}
#endif
