#include<stdio.h>
#include"CGR.h"
int main(){
CCH_init(82,22);
char dir=' ';
CCH_setChar(0,0,'a');
CCH_draw();
int x=0,y=0;
do{
CCH_reset();

dir=getchar();
switch(dir){
case'u':
if(y>0)y--;
break;

case'd':
if(y<CCH_getHeight()-1)y++;
break;

case'l':
if(x>0)x--;
break;

case'r':
if(x<CCH_getWidth()-1)x++;
break;
}

if(dir=='u'||dir=='d'||dir=='l'||dir=='r'){
CCH_setChar(x,y,'a');
CCH_draw();
}

}while(dir!='e');
CCH_end();
return 0;
}
