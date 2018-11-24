#include <Arduino.h>
#include "L9110.h"

/*
            < ARDUINO PRO MICRO >
             
                |TX0   RAW| Batteries +
                |RX1   GND| Batteries -
                |GND   RST|
                |GND   VCC| 5V
                |2     #A3| 
      L9110 BIA |3~    #A2| 
                |4#    #A1| 
      L9110 BIB |5~    #A0| 
      L9110 AIA |6~#    15|
                |7      14| 
                |8#     16| 
      L9110 AIB |9~#  #~10| 
~ PWM
# Analog
*/

/*
              < ARDUINO MICRO >
               
                |MOSI  SCK|
                |SS   MISO|
                |TX0   VIN| Batteries +
                |TX1   GND| Batteries -
                |RST   RST| 
                |GND    5V| 5V
                |2        | 
                |3~       | 
                |4     #A5| 
                |5~    #A4| 
                |6~    #A3| 
                |7     #A2|
                |8     #A1| 
      L9110 BIA |9~    #A0| 
      L9110 BIB |10~  AREF| 
      L9110 AIA |11~   3V3| 
      L9110 AIB |12~   ~13| 

*/

L9110::L9110(int aia, int aib, int bia, int bib){
  this->_aia=aia;
  this->_aib=aib;
  this->_bia=bia;
  this->_bib=bib;
  pinMode(this->_aia,OUTPUT);
  pinMode(this->_aib,OUTPUT);
  pinMode(this->_bia,OUTPUT);
  pinMode(this->_bib,OUTPUT);
}

void L9110::forward(int l, int r, int t)
{
  digitalWrite(_aia,l);
  digitalWrite(_aib,0);
  digitalWrite(_bia,r);
  digitalWrite(_bib,0);
  delay(t);
}

void L9110::reverse(int l, int r, int t)
{
  digitalWrite(_aia,0);
  digitalWrite(_aib,l);
  digitalWrite(_bia,0);
  digitalWrite(_bib,r);
  delay(t);
}

void L9110::rotate(int l, int r, int t)
{
  if (t>0)
  {
    digitalWrite(_aia,0);
    digitalWrite(_aib,l);
    digitalWrite(_bia,r);
    digitalWrite(_bib,0);
  }
  else
  {
    digitalWrite(_aia,l);
    digitalWrite(_aib,0);
    digitalWrite(_bia,0);
    digitalWrite(_bib,r);
  }
  delay(abs(t));
}

void L9110::brake(int t)
{
  digitalWrite(_aia,0);
  digitalWrite(_aib,0);
  digitalWrite(_bia,0);
  digitalWrite(_bib,0);
  delay(t);
}

void L9110::drive(int l, int r, int t)
{
  if (l>0)
  {
    digitalWrite(_aia,l);
    digitalWrite(_aib,0);
  } else {
    digitalWrite(_aia,0);
    digitalWrite(_aib,-l);
  }
  
  if (r>0)
  {
    digitalWrite(_bia,r);
    digitalWrite(_bib,0);    
  } else {
    digitalWrite(_bia,0);
    digitalWrite(_bib,-r);
  }
  
  delay(t);
}
