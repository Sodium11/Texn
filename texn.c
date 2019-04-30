#include<stdio.h>
#include"rawinput.h"
#include"CGR.h"

#define WIDTH 30
#define HEIGHT 30
#define MAP_P(x,y) (x+y*CGR_getWidth())
int isAlpNum(char a){
if(a>='A'&&a<='Z')
return 1;
if(a>='a'&&a<='z')
return 1;
if(a>='0'&&a<='9')
return 1;
return 0;
}

int main(){
CGR_init(WIDTH,HEIGHT);
FILE *fp=fopen("output.txt","w");
int p=0;
int filesize=0;
int linesize[100]={0};
char data[1000]={0};
int wid=CGR_getWidth();
int hei=CGR_getHeight();
int x=0,y=0;
int x_offset=0,y_offset=0;
while (1){
	char input=rawinput();
	if(input==24)
	break;

	if(input==10){
	x=0;
	y++;
	filesize++;
	data[p++]='\n';
	}

	if(isAlpNum(input)){
	CGR_setChar(x,y,input);
	x++;
	data[p++]=input;
	filesize++;
	linesize[y]++;
	}
	CGR_draw();
}

data[filesize]='\0';
fprintf(fp,"%s\n",data);
fclose(fp);
CGR_end();
RawModeOff();
return 0;
}
