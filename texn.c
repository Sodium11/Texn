#include<stdio.h>
#include"lib/CGR.h"
#include"lib/rawinput.h"
#define VERSION "v0.24"
#define DEBUG 0
#define FLEXIBLE_FILE_EDIT 1
#define DEFAULT_TXTROW 8000
#define DEFAULT_TXTCOLUMN 100
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68


const int S_width=50;
const int S_height=30;

int cx=0;
int cy=0;
int c='|';

int vx=0;
int vy=0;

void draw(int TXTline,int TXTrow,char *TXTmap,int x,int y){
int w=CGR_getWidth();
int h=CGR_getHeight();
for(int i=0;i<w;i++)
for(int j=0;j<h;j++)
if(i<TXTline && j<TXTrow){
char data=TXTmap[(i+x)+(j+y)*TXTline];
if(data=='\n'||data=='\0')
	CGR_setChar(i,j,' ');
else
	CGR_setChar(i,j,data);
}
return;
}

void normal_exit(){
RawModeOff();
CGR_end();
exit(0);
}

void error_exit(int TXTline,int TXTrow,char *TXTmap,const char* str){
printf("%s\n",str);
FILE *backup=fopen("backup.txt","w");
if(backup){
int p=0;
while(p<TXTrow*TXTline){
if(TXTmap[p]=='\0')
        break;
fprintf(backup,"%c",TXTmap[p]);
if(TXTmap[p]=='\n')
        p=((p/TXTline)+1)*TXTline;
else
        p++;
}
fclose(backup);
}
printf("Data saved in backup.txt");
RawModeOff();
CGR_end();
exit(1);
}

int openfile(char* filename,unsigned int TXTline,unsigned int TXTrow,char *TXTmap){
FILE *fp=fopen(filename,"r");
if(!fp){
printf("FILE ERROR");
return -1;
}
char buf;
int x=0;
int y=0;
while(1){
	buf=getc(fp);

	if(buf==EOF) break;

	TXTmap[x+y*TXTline]=buf;

	if(buf=='\n'){
		x=0;
		y++;
	}else{
		x++;
	}
}
fclose(fp);
return 0;
}

int savefile(char* filename,unsigned int TXTline,unsigned int TXTrow,char *TXTmap){
FILE *fp=fopen(filename,"r");
if(fp){
fclose(fp);
printf("Overwrite on %s?(y/n)\n",filename);
char answer=rawinput();
if(answer!='y')return -1;
}
fp=fopen(filename,"w");
int p=0;
while(p<TXTrow*TXTline){
if(TXTmap[p]=='\0')
        break;
fprintf(fp,"%c",TXTmap[p]);
if(TXTmap[p]=='\n')
        p=((p/TXTline)+1)*TXTline;
else
        p++;
}
fclose(fp);
return 0;
}

int main(int argc,char** argv){
printf("Texn %s\n",VERSION);
CGR_init(S_width,S_height);
CGR_draw();
unsigned int TXTline=DEFAULT_TXTCOLUMN;
unsigned int TXTrow=DEFAULT_TXTROW;
char* TXTmap;
char* txtfilename="test.txt";
if(argc==1){ //no file
	printf("NEW FILE");
	TXTmap=malloc(TXTrow*TXTline);
	for(int p=0;p<TXTrow*TXTline;p++)
		TXTmap[p]='\0';
}else if(argc==2){ //open file
	//new fileopen start
	TXTmap=malloc(TXTrow*TXTline);
	openfile(argv[1],TXTline,TXTrow,TXTmap);
	txtfilename=argv[1];
	draw(TXTline,TXTrow,TXTmap,vx,vy);
	//new fileopen end
}
CGR_setChar(cx-vx,cy-vy,c);
CGR_draw();

char input=' ';
RawModeOn();
while(1){//main loop
input=rawinput();
if(input==19){//save file
if(savefile(txtfilename,TXTline,TXTrow,TXTmap)==0)
printf("SAVED\n");
else
printf("NOTsaved\n");
}

if(input==24)//exit ctrl+x
	normal_exit();

if(input==27){//Control
input=rawinput();
input=rawinput();
if(DEBUG)printf("Control:%d",input);
switch(input){

case KEY_UP://up
if(TXTmap[cx+(cy-1)*TXTline]=='\0'){
int found=0;
for(int i=cx-1;i>=0;i--){
        if(TXTmap[i+(cy-1)*TXTline]!='\0'){
        cx=i;
        cy--;
        found=1;
        break;
        }
}
if(found==0)printf("NULL LETTER");
break;
}
if(cy>0){
cy--;
if(cy<vy)
vy--;
}
break;

case KEY_DOWN://down
if(TXTmap[cx+(cy+1)*TXTline]=='\0'){
int found=0;
for(int i=cx-1;i>=0;i--){
	if(TXTmap[i+(cy+1)*TXTline]!='\0'){
	cx=i;
	cy++;
	found=1;
	break;
	}
}
if(found==0)printf("NULL LETTER");
break;
}
if(cy<TXTrow-1){
if(cy-vy<S_height-1){
cy++;
if(TXTmap[cx+cy*TXTline]=='\0'){
cx=0;
}
}else
vy++;
}else{
printf("HEIGHT OVER");
}
break;

case KEY_RIGHT://right
	if (TXTmap[cx+cy*TXTline]=='\n'){
		cx=0;
		cy++;
		if(cy>=vy+S_height)
		vy++;
	}else{
	cx++;
	vx++;
	}
break;

case KEY_LEFT://left
	if(cx>0){
	cx--;
	if(cx<vx)vx--;
	}else{
	if(cy>0){
		cx=0;
		cy--;
		if(cy<vy)vy--;
		while(TXTmap[cx+cy*TXTline]!='\n'){
		cx++;
		if(cx>=S_width-1)error_exit(TXTline,TXTrow,TXTmap,"over");
		}
	}
	}
	break;
case 80://view help F1
	printf("%s",VERSION);
break;
}
}else if(input>=0x20&&input<=0x7E){//normal input
	int tmp=cx;
	while(TXTmap[tmp+cy*TXTline]!='\0')tmp++;
	while(tmp>cx){
	TXTmap[tmp+cy*TXTline]=TXTmap[tmp+cy*TXTline-1];
	tmp--;
	}
	TXTmap[cx+cy*TXTline]=input;
	if(cx<TXTline-1){
		cx++;
		if(cx>S_width-1)
			vx++;
	}else{
		printf("OVER");
	}
	if(DEBUG)printf("%d,%d",cx,cy);
}else{
	if(DEBUG)printf("function:%d",input);
	if(input==0xA){//Enter
		if(cy<TXTrow-1){//row down shift
			cy++;
			for(int y=TXTrow-1;y>vy+cy;y--)
				for(int x=0;x<TXTline;x++)
					TXTmap[x+y*TXTline]=TXTmap[x+(y-1)*TXTline];
			for(int x=cx;x<TXTline;x++){
				TXTmap[(x-cx)+(vy+cy)*TXTline]=TXTmap[x+(vy+cy-1)*TXTline];
				TXTmap[x+(vy+cy-1)*TXTline]='\0';
			}
			TXTmap[cx+(cy-1)*TXTline]='\n';
			cx=0;
		}else
		printf("HEIGHT OVER");
		if(cy>=vy+S_height)vy++;

	}
	if(input==0x7F){//delete
		if(cx>0){
		cx--;
		if(cx<vx)vx--;
		for(int i=cx;i<TXTline-1;i++)
		TXTmap[i+cy*TXTline]=TXTmap[(i+1)+cy*TXTline];
		}else if(cy>0){
			cy--;
			if(cy<vy)vy--;
			while(TXTmap[cx+cy*TXTline]!='\n')cx++;
                        for(int x=0;x<TXTline;x++)
                                TXTmap[(x+cx)+(vy+cy)*TXTline]=TXTmap[x+(vy+cy+1)*TXTline];
                        for(int y=vy+cy+1;y<TXTrow-1;y++)
                                for(int x=0;x<TXTline;x++)
                                        TXTmap[x+y*TXTline]=TXTmap[x+(y+1)*TXTline];
		}
	}
}
draw(TXTline,TXTrow,TXTmap,vx,vy);
CGR_setChar(cx-vx,cy-vy,c);
CGR_draw();
}


CGR_end();
return 0;
}
