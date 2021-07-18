#include "stm32f10x.h"
u8 h_total(u8 h1,u8 h2,u8 h3);
float class_hour(u8 grade,u8 cout,u8 total);
float result1=0,result2=0,result3=0,result4=0;
 int main(void)
 {	
	result1=class_hour( 161, 38, h_total(48,64,48));
	result2=class_hour( 162, 46, h_total(48,64,48));
	result3=class_hour( 151, 50, h_total(64,64,32));
	result4=class_hour( 152, 38, h_total(64,64,32));
	while(1);
  
	
 }

u8 h_total(u8 h1,u8 h2,u8 h3)
 {
	 return h1+h2+h3;
 }
 float class_hour(u8 grade,u8 cout,u8 total)
 {
	 if(grade==151||grade==152)
	 {
		 if(cout<=40)return (float)total*1.0;
		 else return (float)total*(1.0+(cout-40)*0.01);
	 }
	 if(grade==161||grade==162)
  {
	  if(cout<=40)return (float)total*1.5;
	  else return (float)total*(1.0+(cout-40)*0.01+0.5);
  }	 
}	 
	 