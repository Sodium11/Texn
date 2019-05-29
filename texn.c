#include<stdio.h>
#include"rawinput.h"
#include"CGR.h"

#define WIDTH 50
#define HEIGHT 38
#define MAP_P(x,y) ((x)+(y)*CGR_getWidth())
#define DATA_SIZE WIDTH*HEIGHT
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

void update(editordata *ed){
for (int y=0;y<(ed->txt).linenum;y++)
for(int x=0;x<(ed->txt).linesize[y];x++)
CGR_setChar(x,y,(ed->txt).data[MAP_P(x,y)]);
return;
}

void moveCursor(char direction,editordata *ed){
switch(direction){
case 'u':
	CGR_setChar(ed->x,ed->y,(ed->txt).data[MAP_P(ed->x,ed->y)]);
	if(ed->y>0)
	(ed->y)--;
	//CGR_setChar(0,*y,'*');
break;
case 'd':
//	CGR_setChar(0,ed->y,' ');
	CGR_setChar(ed->x,ed->y,(ed->txt).data[MAP_P(ed->x,ed->y)]);
	if((ed->y)<(ed->height)-1)
	(ed->y)++;
	//CGR_setChar(0,*y,'*');
break;
case 'r':
	CGR_setChar(ed->x,ed->y,(ed->txt).data[MAP_P(ed->x,ed->y)]);
        //update(ed);
	if((ed->x)>=(ed->txt).linesize[ed->y]){
		if(ed->y<(ed->txt).linenum){
		moveCursor('d',ed);
		ed->x=0;
		}
	}else{
	(ed->x)++;
	}
break;
case 'l':
	CGR_setChar(ed->x,ed->y,(ed->txt).data[MAP_P(ed->x,ed->y)]);
        //update(ed);
        if((ed->x)<=0){
		if(ed->y>0){
		moveCursor('u',ed);
		ed->x=(ed->txt.linesize[ed->y]);
		}
	}else{
	(ed->x)--;
	}
break;

default:
break;
}
CGR_draw();
return;
}

int isAlpNum(char a){
if(a>='A'&&a<='Z')
return 1;
if(a>='a'&&a<='z')
return 1;
if(a>='0'&&a<='9')
return 1;
return 0;
}

int texnload(FILE *fp,texndata *txt){
int x=0,y=0;
char ch;
while((ch=fgetc(fp))!=EOF) {
	printf("%c",ch);
	if(ch=='\n'||ch=='\r'){
	x=0;
	y++;
	}else{
	txt->data[MAP_P(x,y)]=ch;
	txt->linesize[y]++;
	x++;
	}
}
txt->linenum=y;
return 0;
}

int texnsave(FILE *fp,texndata txt){
int sum=0;
for(int i=0;i<txt.linenum;i++){
//printf("%d,",txt.linesize[i]);
sum+=txt.linesize[i];
for(int j=0;j<txt.linesize[i];j++)
fprintf(fp,"%c",txt.data[MAP_P(j,i)]);
fprintf(fp,"\n");
}
return sum;
}

int main(int argc,char ** argv){
CGR_init(WIDTH,HEIGHT);
CGR_setChar(0,0,CURSOR);
CGR_draw();

const int wid=CGR_getWidth();
const int hei=CGR_getHeight();
editordata ed=emptyED(wid,hei);

FILE *fp;
if(argc>=2){
fp=fopen(argv[1],"r");
	if(fp){
	texnload(fp,&(ed.txt));
	update(&ed);
/*	for(int i=0;i<ed.txt.linenum;i++)
	for(int j=0;j<ed.txt.linesize[i];j++)
	CGR_setChar(j,i,ed.txt.data[MAP_P(j,i)]);
*/
	CGR_draw();
	fclose(fp);
	}
}
int p=0;
//texndata txt=ed.txt;
//main loop
while (1){
	char input=rawinput();

	if(input==EXIT_KEY)
	break;

	if(input==SAVE_KEY){
	FILE *savefile;
	if (argc<2)
	savefile=fopen("output.txt","w");
	else
	savefile=fopen(argv[1],"w");
	int f_size=texnsave(savefile,ed.txt);
//	printf("%d",f_size);
	fclose(savefile);
	//fprintf(fp,"%s\n",txt.data);
	}
	if(input==ENTER_KEY){
		if(ed.y<hei){
//		txt.data[MAP_P(ed.x,ed.y)]='\n';
//		txt.linesize[ed.y]++;
		ed.txt.linenum++;
		moveCursor('d',&ed);
		ed.x=0;
		}
	}

	if (input==DEL_KEY){
		if(ed.x>0){
		p--;

		for (int i=ed.x;i<ed.txt.linesize[ed.y];i++)
			ed.txt.data[MAP_P(i,ed.y)]=ed.txt.data[MAP_P(i+1,ed.y)];
		ed.txt.data[MAP_P((ed.txt.linesize[ed.y]-1),(ed.y))]='\0';
		ed.txt.linesize[ed.y]--;
		}
		moveCursor('l',&ed);
	}

if(input==27)
if(rawinput()==91){
char ctlkey=rawinput();
	if (ctlkey==ARROW_UP){
		if(ed.y>0)
		moveCursor('u',&ed);
	}
	if(ctlkey==ARROW_DOWN){
		if(ed.y<(ed.txt.linenum)){
			if(ed.y>hei-1)
			ed.y=hei-1;
		moveCursor('d',&ed);
		}
	}
	if(ctlkey==ARROW_RIGHT){
		if(ed.x<=(ed.txt).linesize[ed.y]){
			//ed.x++;
			moveCursor('r',&ed);
		}
	}
	if(ctlkey==ARROW_LEFT){
		if(ed.x>=0){
			//ed.x--;
			moveCursor('l',&ed);
		}
	}
}
	if(input>=0x20&&input<=0x7E){
	CGR_setChar(ed.x,ed.y,input);
	ed.txt.data[MAP_P(ed.x,ed.y)]=input;
	ed.txt.linesize[ed.y]++;
	moveCursor('r',&ed);
	}
	CGR_setChar(ed.x,ed.y,CURSOR);
	CGR_draw();
}

CGR_end();
RawModeOff();
return 0;
}
