//Sodium11.for.gitserver@gmail.com
#include<stdio.h>
#include"rawinput.h"
#include"CGR.h"
#include"texn.h"
/*
void dshift(texndata *txt,int start,int end,int n){
if(n>=0){
for (int i=end;i<=start;i--)
	txt->data[i+n]=txt->data[i];
}else{
for(int i=start;i>=end;i++)
	txt->data[i+n]=txt->data[i];
}
return;
}
*/
int isAlpNum(char a){
if(a>='A'&&a<='Z')
return 1;
if(a>='a'&&a<='z')
return 1;
if(a>='0'&&a<='9')
return 1;
return 0;
}

int main(int argc,char ** argv){
CGR_initF("setting.cgr");
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
	CGR_draw();
	fclose(fp);
	}
}
int p=0;
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
	fclose(savefile);
	}
	if(input==ENTER_KEY){
		if(ed.y<hei){
		ed.txt.linenum++;
		moveCursor('d',&ed);
		ed.x=0;
		}
	}

	if (input==DEL_KEY){
		if(ed.x>=0){
			for(int i=0;i<linelen(&ed,GlobalY(&ed))-GlobalX(&ed);i++)
				ed.txt.data[GlobalP(&ed)+i-1]=ed.txt.data[GlobalP(&ed)+i];
			ed.txt.linesize[GlobalY(&ed)]--;
			ed.txt.data[MAP_P(linelen(&ed,GlobalY(&ed)),GlobalY(&ed))]='\0';
			moveCursor('l',&ed);
		}else
		moveCursor('l',&ed);
	}

if(input==27)
if(rawinput()==91){
char ctlkey=rawinput();
	if(ctlkey==ARROW_UP)
		moveCursor('u',&ed);
	if(ctlkey==ARROW_DOWN)
		moveCursor('d',&ed);
	if(ctlkey==ARROW_RIGHT)
		moveCursor('r',&ed);
	if(ctlkey==ARROW_LEFT)
		moveCursor('l',&ed);
}
	if(input>=0x20&&input<=0x7E){
		CGR_setChar(ed.x,ed.y,input);
		ed.txt.data[MAP_P(ed.x,ed.y)]=input;
		ed.txt.linesize[ed.y]++;
		for(int i=linelen(&ed,GlobalY(&ed))-GlobalX(&ed);i>ed.x;i--)
			ed.txt.data[GlobalP(&ed)+i]=ed.txt.data[GlobalP(&ed)+i-1];
		moveCursor('r',&ed);
	}
	update(&ed);
	CGR_setChar(ed.x,ed.y,CURSOR);
	CGR_draw();
}

CGR_end();
RawModeOff();
return 0;
}
