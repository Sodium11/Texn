//texn.h by Sodium11.for.gitserver@gmail.com
#ifndef TEXN_H
#define TEXN_H

#define WIDTH 50
#define HEIGHT 38
#define MAP_P(x,y) ((x)+(y)*CGR_getWidth())
#define DATA_SIZE 1000000
#define LINE 1000

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

int GlobalX(editordata *ed){
return ed->x+ed->x_offset;
}

int GlobalY(editordata *ed){
return ed->y+ed->y_offset;
}

int GlobalP(editordata *ed){
return MAP_P(GlobalX(ed),GlobalY(ed));
}

void update(editordata *ed){
for (int y=0;y<ed->height;y++)
for(int x=0;x<ed->width;x++)
CGR_setChar(x,y,(ed->txt).data[MAP_P(x+ed->x_offset,y+ed->y_offset)]);
return;
}

int texnload(FILE *fp,texndata *txt){
int x=0,y=0;
char ch;
while((ch=fgetc(fp))!=EOF) {
	if(ch=='\r')
	continue;
        if(ch=='\n'){
        x=0;
        y++;
	if(y>=LINE){
	perror("Too much lines");
	return -1;
	}
	continue;
        }
	printf("%d %c\n",ch,ch);
        txt->data[MAP_P(x,y)]=ch;
        txt->linesize[y]++;
        x++;
}
txt->linenum=y;
return 0;
}

int texnsave(FILE *fp,texndata txt){
int sum=0;
for(int i=0;i<=txt.linenum;i++){
sum+=txt.linesize[i];
for(int j=0;j<txt.linesize[i];j++)
fprintf(fp,"%c",txt.data[MAP_P(j,i)]);
fprintf(fp,"\n");
}
return sum;
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
if(y>0&&GlobalY(ed)>0)
y--;
else if(ed->y_offset>0)
ed->y_offset--;

if(x>linelen(ed,GlobalY(ed)))
	x=linelen(ed,GlobalY(ed));
break;

case'd':
if(Gy<linenum) y++;
if(y>=ed->height){
y--;
ed->y_offset++;
}
if(x>linelen(ed,GlobalY(ed)))
	x=linelen(ed,GlobalY(ed));
break;

case'r':
if(x<linelen(ed,Gy))x++;
else if(Gy<linenum){
x=0;
y++;
}
break;

case'l':
if(x>0)x--;
else if(Gy>0){
x=linelen(ed,Gy-1);
y--;
}
break;

default:
break;
}
ed->x=x;
ed->y=y;
return;
}

void insert(texndata *txt,int x,int y,char ch){
txt->linesize[y]++;
int size=txt->linesize[y];
int p=MAP_P(x,y);
for (int i=MAP_P(size,y);i>p;i--)
	txt->data[i]=txt->data[i-1];
txt->data[p]=ch;
return;
}

void insertline(texndata *txt,int y){
if(y<txt->linenum)
insertline(txt,y+1);
else
txt->linenum++;
for(int i=0;i<txt->linesize[y];i++)
txt->data[MAP_P(i,y+1)]=txt->data[MAP_P(i,y)];
txt->linesize[y+1]=txt->linesize[y];
for(int i=0;i<txt->linesize[y];i++)
txt->data[MAP_P(i,y)]='\0';
txt->linesize[y]=0;
return;
}

void delline(texndata *txt,int y){
int size=txt->linesize[y];
for(int i=0;i<size;i++)
txt->data[MAP_P(i,y)]='\0';
txt->linesize[y]=0;
if(y<txt->linenum){
for(int i=0;i<txt->linesize[y+1];i++)
txt->data[MAP_P(i,y)]=txt->data[MAP_P(i,y+1)];
txt->linesize[y]=txt->linesize[y+1];
delline(txt,y+1);
}else{
txt->linenum--;
}
return;
}
#endif
