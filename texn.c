#include<stdio.h>
#include"rawinput.h"
#include"CGR.h"

#define WIDTH 30
#define HEIGHT 30
#define MAP_P(x,y) (x+y*CGR_getWidth())

#define EXIT_KEY 24
#define SAVE_KEY 19
#define ENTER_KEY 10
#define DEL_KEY 127

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
CGR_init(WIDTH,HEIGHT);
CGR_draw();
FILE *fp;
if(argc>=2)
fp=fopen(argv[1],"w");
else
fp=fopen("output.txt","w");
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

	if(input==EXIT_KEY)
	break;

	if(input==SAVE_KEY){
	data[filesize]='\0';
	fprintf(fp,"%s\n",data);
	}
	if(input==ENTER_KEY){
	x=0;
	y++;
	filesize++;
	data[p++]='\n';
	}

if (input==DEL_KEY){
	if(x>0){
	p--;
	x--;
	CGR_setChar(x,y,' ');
	}
}

	if(input>=0x20&&input<=0x7E){
	CGR_setChar(x,y,input);
	x++;
	data[p++]=input;
	filesize++;
	linesize[y]++;
	}
	CGR_draw();
}


fclose(fp);
CGR_end();
RawModeOff();
return 0;
}
