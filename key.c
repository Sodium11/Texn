#include"rawinput.h"
int main(){
RawModeOn();
printf("%d\n",rawinput());
printf("%d\n",rawinput());
printf("%d\n",rawinput());

RawModeOff();
return 0;
}
