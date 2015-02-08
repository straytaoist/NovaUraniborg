/*

OLED from:
http://www.ebay.co.uk/itm/Yellow-128X64-0-96-Blue-SPI-Serial-OLED-LCD-Display-Module-for-Arduino-STM32-51-/371153566472

GND  GND
VCC  3.3V
CLK  D13
MOSI D11
D/C  D9
CS   D10

RTC from:
http://www.amazon.co.uk/gp/product/B00K67X496

SDA A4
SCL A5

*/

#include <Wire.h>
#include "RTClib.h"
#include <Time.h>
#include <math.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);
RTC_DS1307 RTC;

void setup() {
  Wire.begin();
  RTC.begin();
  DateTime now = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

char * universal_time(char * UT) {
  DateTime now = RTC.now();
  sprintf(UT, "%2u:%2u:%2u", now.hour(), now.minute(), now.second());
  for ( int i = 0; i < 8; i++) {
    if (i != 10) {
      if (UT[i] == ' ') UT[i] = 'O';
      if (UT[i] == '0') UT[i] = 'O';
    }
  }
  return UT;
}

char * local_sidereal_time(char * ST) {
  DateTime now = RTC.now();

  float UT, J0, JD, JC, mm, ss;
  mm = now.minute();
  mm = mm / 60;
  ss = now.second();
  ss = ss / 3600;
  UT = now.hour() + mm + ss;
  float ll, ll2;
  unsigned long ll3;
  ll =  floor(275 * now.month() / 9) + now.day() + 1721013.5;
  ll2 = now.year() + floor((now.month() + 9) / 12);
  ll3 = 367;
  ll3 = ll3 * now.year();
  J0 = ll3 - floor(ll2 * 7 / 4) + ll;
  JD = J0 + (UT / 24);
  JC = J0 - 2451545.0;
  JC = JC / 36525;
  float GST0;
  GST0 = 100.4606184 + 36000.77004 * JC + 0.000387933 * JC * JC - 2.583e-8 * JC * JC * JC;
  while (GST0 > 360) {
    GST0 -= 360;
  }
  float GST, LST;
  GST = GST0 + 360.98564724 * UT / 24;
  LST = GST + 0.1732;
  while (LST > 360) {
    LST -= 360;
  }
   float mins_into_day, mins, secs;
  mins_into_day = LST * 4;
  int hours, minutes, seconds;
  hours = mins_into_day / 60;
  mins = ((mins_into_day - (hours * 60)) * 60) / 60;
  minutes = mins;
  secs = (mins - floor(mins)) * 60;
  seconds = secs;
  sprintf(ST, "%2u:%2u:%2u", hours, minutes, seconds);
  for ( int i = 0; i < 8; i++) {
    if (ST[i] == ' ') ST[i] = '0';
    if (ST[i] == '0') ST[i] = '0';
  }
  return ST;
}

void loop() {
  char ut[9];
  char st[9];

  u8g.firstPage();
  u8g.setFont(u8g_font_unifont);

  do {
    local_sidereal_time(st);
    universal_time(ut);
    u8g.drawStr(30, 15, ut);
    u8g.drawStr(30, 35, st);
    //u8g.drawStr(30, 35, "10:53:12");

  } while(u8g.nextPage());
  delay(1000);
}
