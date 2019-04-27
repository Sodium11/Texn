#include<stdio.h>
#include"rawinput.h"
void p_screen(int size,char *data){
for(int p=0;p<size;p++){
char buf=data[p];
if(buf=='\0')
buf='\n';
printf("%c",buf);
}
return;
}



int main(){
FILE *fp=fopen("output.txt","w");
int p=0;
char str[100];
char data[1000];
while (1){
int l=rawstr(str);
if (str[0]=='q')
break;
for (int i=0;i<l+1;i++){
data[p]=str[i];
p++;
}
//printf("%d:%s",l,str);
p_screen(p,data);
fprintf(fp,"%s\n",str);
}
fclose(fp);
RawModeOff();
return 0;
}
