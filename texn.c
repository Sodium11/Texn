#include<stdio.h>
#include"lib/CGR.h"
#include"lib/rawinput.h"

#define DEBUG 0

const int S_width=50;
const int S_height=30;

int cx=0;
int cy=0;
int c='|';

int vx=0;
int vy=0;

void update(int TXTline,int TXTrow,char *TXTmap,int x,int y){
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
exit(-1);
return;
}


int main(int argc,char** argv){
CGR_init(S_width,S_height);
CGR_draw();
unsigned int TXTline=1;
unsigned int TXTrow=1;
char* TXTmap;
if(argc==1){ //no file
printf("NEW FILE");
TXTline=100;
TXTrow=2000;
TXTmap=malloc(TXTrow*TXTline);
for(int p=0;p<TXTrow*TXTline;p++)
	TXTmap[p]='\0';
for(int p=0;p<TXTrow;p++)
	TXTmap[p*TXTline]='\n';

}else if(argc==2){ //open file
FILE *fp=fopen(argv[1],"r");
if(!fp){
printf("FILE ERROR");
return -1;
}
char filechar;
unsigned int linelen=0;
char buffer[10000];
int p=0;
do{
	filechar=getc(fp);
	if(filechar==EOF)
	break;

	buffer[p++]=filechar;
	if(filechar=='\n'){
	TXTline=(TXTline>linelen)?TXTline:linelen;
	linelen=0;
	TXTrow++;
	}else{
	linelen++;
	}

}while(filechar!=EOF);
fclose(fp);

if(DEBUG)printf("%d %d %s",TXTline,TXTrow,buffer);
TXTmap=malloc(TXTrow*TXTline);
for(int p=0;p<TXTrow*TXTline;p++)
	TXTmap[p]='\0';
p=0;
int x=0;
int y=0;
while(buffer[p]!='\0'){
TXTmap[x+y*TXTline]=buffer[p];
if(buffer[p]=='\n'){
x=0;
y++;
}else{
x++;
}
p++;
}
update(TXTline,TXTrow,TXTmap,vx,vy);
CGR_setChar(cx-vx,cy-vy,c);
CGR_draw();
}

char input=' ';


while(1){
input=rawinput();
if(input==19){//save file
FILE* fp=fopen("test.txt","w");
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
printf("SAVED");
}

if(input==24)//exit ctrl+x
	break;

if(input==27){//Control
input=rawinput();
input=rawinput();
if(DEBUG)printf("Control:%d",input);
switch(input){
case 65://up
if(cy>0){
cy--;
if(cy<vy)
vy--;
}
break;

case 66://down
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

case 67://right
	if (TXTmap[cx+cy*TXTline]=='\n'){
		cx=0;
		cy++;
		if(cy>=vy+S_height)
		vy++;
	}else{
	if(cx<S_width-1)cx++;
	}
break;

case 68://left
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
		TXTmap[cx+cy*TXTline]='\n';
		cx=0;
		if(cy<TXTrow-1)
		cy++;
		else
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
		}
	}
}
update(TXTline,TXTrow,TXTmap,vx,vy);
CGR_setChar(cx-vx,cy-vy,c);
CGR_draw();
}


CGR_end();
return 0;
}
