#include<Servo.h>

Servo lower;
Servo upper;
Servo mike;

int lvalue,i,j,k,uvalue,lpr,upr,mvalue,mpr;
int a,b,c;


void setup()
{
  Serial.begin(9600);
  
  lower.attach(4);
  upper.attach(11);
  mike.attach(3);
  
  lower.write(0);
  lpr=0;
  upr=180;
  mpr=180;
  mike.write(180);
  upper.write(160);
  delay(5);
  
}

void loop()
{
  while(Serial.available()<=0);
   if(Serial.available() >=3)
   {
    a=Serial.read();
    b=Serial.read();
    c=Serial.read();
    uvalue=word(a);
    lvalue=word(b);
    mvalue=word(c);
    if(lvalue<lpr||uvalue>upr||mvalue>mpr)
    {
      if(lvalue<lpr)
    {
      for(i=lpr;i>lvalue;i--)
      {
        lower.write(i);
        delay(25);
      }
      lpr=lvalue;
    }
     if(mvalue>mpr)
    {
      for(k=mpr;k<mvalue;k++)
      {
        mike.write(k);
        delay(25);
      }
      mpr=mvalue;
    }
    if(uvalue>upr)
      {
      for(j=upr;j<uvalue;j++)
      {
        upper.write(j);
        delay(25);
      }
      upr=uvalue;
    }
    }
    else
    {
     if(uvalue<upr)
    {
      for(j=upr;j>uvalue;j--)
      {
        upper.write(j);
        delay(25);
      }
      upr=uvalue; 
    }
    if(mvalue<mpr)
    {
      for(k=mpr;k>mvalue;k--)
      {
        mike.write(k);
        delay(25);
      }
      mpr=mvalue;
    }
    if(lvalue>lpr)
    {
      for(i=lpr;i<lvalue;i++)
      {
        lower.write(i);
        delay(25);
      }
      lpr=lvalue;
    }
    }
   }
  
}

  
