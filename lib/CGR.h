//Character Graphics Renderer  by Sodium11.for.gitserver@gmail.com
#ifndef CGR_H
#define CGR_H
#include<stdio.h>
#include<stdlib.h>
#define SCREEN_SPLITTER '='
#define LINE_FEED '\n'

int CGR_width;
int CGR_height;
char *CGRScreen;

void CGR_reset(){
int x,y;
for(y=0;y<CGR_height;y++){
        for(x=0;x<CGR_width;x++){
        CGRScreen[x+y*CGR_width]=' ';
        }
}
}


void CGR_init(int width,int height){
CGR_width=width;
CGR_height=height;
CGRScreen=(char *)malloc(CGR_width*CGR_height);
if(CGRScreen==NULL){
perror("CGRScreen definition error.");
exit(0);
}
CGR_reset();
}

void CGR_initF(char* filename){
int width,height;
FILE *fp=fopen(filename,"r");
fscanf(fp,"%d %d",&width,&height);
fclose(fp);
CGR_width=width;
CGR_height=height;
CGRScreen=(char *)malloc(CGR_width*CGR_height);
if(CGRScreen==NULL){
perror("CGRScreen definition error.");
exit(0);
}
CGR_reset();
}

char CGR_getChar(int x,int y){
if(x<0||y<0||x>=CGR_width||y>=CGR_height){
perror("invalid position");
return -1;
}
return CGRScreen[x+y*CGR_width];
}

int CGR_setChar(int x,int y,char character){
if(x<0||y<0||x>=CGR_width||y>=CGR_height){
perror("invalid position");
return -1;
}
CGRScreen[x+y*CGR_width]=character;
return 0;
}

void CGR_draw(){
//top border
int i;
for(i=0;i<CGR_width;i++){
putchar(SCREEN_SPLITTER);
}
putchar(LINE_FEED);

//draw
int x,y;
for(y=0;y<CGR_height;y++){
	for(x=0;x<CGR_width;x++){
	putchar(CGRScreen[x+y*CGR_width]);
	}
putchar(LINE_FEED);
}

//bottom border
for(i=0;i<CGR_width;i++){
putchar(SCREEN_SPLITTER);
}
putchar(LINE_FEED);
}

void CGR_end(){
free(CGRScreen);
return;
}

int CGR_getWidth(){
return CGR_width;
}

int CGR_getHeight(){
return CGR_height;
}
#endif

