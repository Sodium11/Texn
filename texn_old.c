//Sodium11.for.gitserver@gmail.com
#include<stdio.h>
#include<rawinput.h>
#include<CGR.h>
#include<texn.h>

int main(int argc,char ** argv){
CGR_init(WIDTH,HEIGHT);
//CGR_initF("setting.cgr");
CGR_setChar(0,0,CURSOR);
CGR_draw();

const int wid=CGR_getWidth();
const int hei=CGR_getHeight();
editordata ed=emptyED(wid,hei);

FILE *fp;
if(argc>=2){
fp=fopen(argv[1],"r");
	if(fp){
		if(texnload(fp,&(ed.txt))==-1){
			return -1;
		}
		fclose(fp);
	}
update(&ed);
CGR_draw();
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
		insertline(&(ed.txt),GlobalY(&ed)+1);
		for (int i=ed.x;i<linelen(&ed,GlobalY(&ed));i++){
			ed.txt.data[MAP_P(i-ed.x,GlobalY(&ed)+1)]=ed.txt.data[MAP_P(i,GlobalY(&ed))];
			ed.txt.data[MAP_P(i,GlobalY(&ed))]='\0';
		}
		int size=linelen(&ed,GlobalY(&ed));
		ed.txt.linesize[GlobalY(&ed)]-=size-ed.x;
		ed.txt.linesize[GlobalY(&ed)+1]+=size-ed.x;
		ed.txt.data[MAP_P(ed.x,ed.y)]='\0';
		moveCursor('d',&ed);
		ed.x=0;
		}
	}

	if (input==DEL_KEY){
		if(ed.x>0){
		//delete in a line
			for(int i=0;i<linelen(&ed,GlobalY(&ed))-GlobalX(&ed);i++)
				ed.txt.data[GlobalP(&ed)+i-1]=ed.txt.data[GlobalP(&ed)+i];
			ed.txt.linesize[GlobalY(&ed)]--;
			ed.txt.data[MAP_P(linelen(&ed,GlobalY(&ed)),GlobalY(&ed))]='\0';
			moveCursor('l',&ed);
		}else if(GlobalY(&ed)>0){
		//delete through lines
			if(linelen(&ed,GlobalY(&ed))>=0){
				int size=linelen(&ed,GlobalY(&ed));
				int n=linelen(&ed,GlobalY(&ed)-1);
				for (int i=0;i<size;i++){
					ed.txt.data[MAP_P(i+n,GlobalY(&ed)-1)]=ed.txt.data[MAP_P(i,GlobalY(&ed))];
					ed.txt.data[MAP_P(i,GlobalY(&ed))]='\0';
				}
				ed.txt.data[MAP_P(size+n,GlobalY(&ed)-1)]='\0';
				ed.txt.linesize[GlobalY(&ed)-1]+=size;
				delline(&(ed.txt),GlobalY(&ed));
				moveCursor('u',&ed);
				ed.x=n;
			}
		}
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
		insert(&(ed.txt),GlobalX(&ed),GlobalY(&ed),input);
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
