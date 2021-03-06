#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>

//#include <heltec.h>
#include <SSD1306Wire.h>

#include <Ticker.h>
#include <Cube.h>
#include <Triangle.h>
#include <digitalInput.h>

#include "confidential.h" // Wifi crendential

Ticker toggler;
Ticker newValueTicker;

//SSD1306Wire *display = Heltec.display;
SSD1306Wire *display;

// 'Nouveau projet', 128x64px
const uint8_t vertic [] PROGMEM = {
  0x0F, 0x00, 0x31, 0x00, 0x41, 0x00, 0x81, 0x00, 0x81, 0x00, 0x01, 0x01, 
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
  0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x04, 0x01, 0x18, 0x01, 
  0xE0, 0x01
};
const uint8_t horiz [] PROGMEM = {
  0xE0, 0x0F, 0x00, 0xFF, 0x1F, 0x18, 0x00, 0x00, 0x00, 0x10, 0x04, 0x00, 
  0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 
  0x08, 0x01, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x04, 0x01, 
  0x00, 0x00, 0x00, 0x03, 0xFF, 0x0F, 0x00, 0xFE, 0x00
};
const uint8_t inc45 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x11, 
  0x00, 0x00, 0x80, 0x10, 0x00, 0x00, 0x40, 0x30, 0x00, 0x00, 0x20, 0x20, 
  0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 
  0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
  0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
  0x02, 0x04, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x06, 0x01, 0x00, 0x00, 
  0x84, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
  0x10, 0x00, 0x00, 0x00
};
const uint8_t inc135 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x1C, 0x03, 0x00, 0x00, 
  0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
  0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 
  0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 
  0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 
  0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x60, 0x1C, 0x00, 0x00, 0x80, 0x07, 
  0x00, 0x00, 0x00, 0x00
};
const uint8_t centre [] PROGMEM = {
  0x7C, 0x00, 0xFE, 0x00, 0xFF, 0x01, 0xFF, 0x01, 0xEF, 0x01, 0xFF, 0x01, 
  0xFF, 0x01, 0xFE, 0x00, 0x7C, 0x00
};

const uint8_t epd_bitmap_Nouveau_projet [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x3C, 0x02, 0x81, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x02, 0x42, 0x04, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x03, 0x22, 
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0x00, 0x02, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x02, 0x0A, 0x08, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x06, 
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x02, 0x02, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x10, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF8, 0xF8, 0xFF, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xFC, 0x01, 
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x20, 0x00, 0xFE, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xFE, 0x03, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xDE, 0x03, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x08, 0x00, 0xFE, 0x03, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0xFE, 0x03, 0x20, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0xFC, 0x01, 
  0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xF8, 0x7F, 0xF8, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x03, 0x02, 
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x80, 0x02, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x41, 0x02, 0x02, 0x10, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x02, 0x06, 
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x11, 0x02, 0x1A, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x04, 0xE2, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00
  };

float graphValue[128];
bool newValue = false;
int XValue= 0;
bool maxValueReached = false;



digitalInput PRGButton(0,INPUT_PULLUP);
byte State = 0;



// pt2D_t cube_pt[] = {
//   {37.6867235218178,12.7373447043636},
//   {73.7274204169332,5.42419319435559},
//   {73.7274204169332,58.5758068056444},
//   {37.6867235218178,51.2626552956365},
//   {59.2251850602793,18.9549629879441},
//   {85.9013334604115,15.9671111534705},
//   {85.9013334604115,48.0328888465295},
//   {59.2251850602793,45.0450370120559},
// };




  
// pt2D_t cube_pt[] = 
//   {
//     {64-28,32-28},  //1
//     {64+28,32-28},  //2
//     {64+28,32+28},  //3
//     {64-28,32+28},  //4

//     {64-16,32-16},  //5
//     {64+16,32-16},  //6
//     {64+16,32+16},  //7
//     {64-16,32+16}   //8
//   };

Cube cube;
Cube cube2;
Triangle triangle;

void toggle(){
  static bool moved = false;
  //Serial.println("moved");

  if (moved)
  {
    cube.Translate({1,0,0});


  }else
  {
    
    cube.Translate({-1,0,0});
    
  }
  moved = !moved;
  
}
void setup() {
  // put your setup code here, to run once:

  pinMode(16,OUTPUT);
  digitalWrite(16,LOW);
  delay(1);
  digitalWrite(16,HIGH);
  Serial.begin(115200);
  //Heltec.begin(true,true,true,true,868E6);
  display = new SSD1306Wire(0x3c,4,15,OLEDDISPLAY_GEOMETRY::GEOMETRY_128_64);
  if (display->init())
  {
    Serial.println("Display init done");
  } else
  {
    Serial.println("Display init failed");
    /* code */
  }
  
  display->flipScreenVertically();
  
  display->clear();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    display->drawString(0,0,"Waiting for Wifi");
    display->display();
    delay(100);

  }
  display->clear();

  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() {
    display->clear();
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->drawString(display->getWidth() / 2, display->getHeight() / 2 - 20, "OTA Update");
    display->display();
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    display->drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
    display->display();
  });

  ArduinoOTA.onEnd([]() {
    display->clear();
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->drawString(display->getWidth() / 2, display->getHeight() / 2, "Restart");
    display->display();
  });
  
  display->drawXbm(0,0,128,64,epd_bitmap_Nouveau_projet);

  
  display->drawXbm(64-9/2,32-9/2,9,9,centre);

  display->drawXbm(64-9/2,32-37/2,9,37,vertic);
  display->display();
  delay(500);
  display->drawXbm(64-31/2,32-31/2,31,31,inc45);
  display->display();
  delay(500);

  display->drawXbm(64-37/2,32-9/2,37,9,horiz);
  display->display();
  delay(500);

  display->drawXbm(64-31/2,32-31/2,31,31,inc135);
  display->setColor(OLEDDISPLAY_COLOR::BLACK);
  display->drawXbm(64-9/2,32-37/2,9,37,vertic);
  display->setColor(OLEDDISPLAY_COLOR::WHITE);
  display->display();
  delay(500);
  display->setColor(OLEDDISPLAY_COLOR::BLACK);
  display->drawXbm(64-31/2,32-31/2,31,31,inc45);
  display->setColor(OLEDDISPLAY_COLOR::WHITE);
  display->drawXbm(64-9/2,32-37/2,9,37,vertic);
  display->display();

  delay(1000);

  display->display();

  delay(1000);
  toggler.attach(2,toggle);

  cube2.Translate({1,0,0});
  triangle.Translate({-2,0,0});
  PRGButton.loop();
}

void loop() {
  // put your main code here, to run repeatedly:
  ArduinoOTA.handle();

  //acquisition des donn??es
  PRGButton.loop();
  
  if (PRGButton.frontDesceandant())
  {
    State++;
  }
  switch (State)
  {
  case 0:
    cube.calcul();
    cube2.calcul();
    triangle.calcul();

    display->clear();

      // //face A
      // if (shoelace(cube_pt[0],cube_pt[1],cube_pt[2],cube_pt[3])>=0)
      // {
      //   display->drawLine(cube_pt[0].x,cube_pt[0].y,cube_pt[1].x,cube_pt[1].y);
      //   display->drawLine(cube_pt[1].x,cube_pt[1].y,cube_pt[2].x,cube_pt[2].y);
      //   display->drawLine(cube_pt[2].x,cube_pt[2].y,cube_pt[3].x,cube_pt[3].y);
      //   display->drawLine(cube_pt[3].x,cube_pt[3].y,cube_pt[0].x,cube_pt[0].y);

      // }
    
      
    cube.render();
    cube2.render();
    triangle.render();

    //display->drawLine(cube_pt[4].x,cube_pt[4].y,cube_pt[5].x,cube_pt[5].y);
    //display->drawLine(cube_pt[5].x,cube_pt[5].y,cube_pt[6].x,cube_pt[6].y);
    //display->drawLine(cube_pt[6].x,cube_pt[6].y,cube_pt[7].x,cube_pt[7].y);
    //display->drawLine(cube_pt[7].x,cube_pt[7].y,cube_pt[4].x,cube_pt[4].y);

    //display->drawLine(cube_pt[0].x,cube_pt[0].y,cube_pt[4].x,cube_pt[4].y);
    //display->drawLine(cube_pt[1].x,cube_pt[1].y,cube_pt[5].x,cube_pt[5].y);
    //display->drawLine(cube_pt[2].x,cube_pt[2].y,cube_pt[6].x,cube_pt[6].y);
    //display->drawLine(cube_pt[3].x,cube_pt[3].y,cube_pt[7].x,cube_pt[7].y);

    display->display();

    cube.inc_Angle(1);
    cube2.inc_Angle(1);
    triangle.inc_Angle(1);
    break;
    case 1:
      display->clear();

        XValue++;
      if (XValue <= 120)
      {
        graphValue[XValue] = sin(XValue*.2);
        
        if (XValue == 120)
        {
          newValueTicker.attach_ms(150,[]{
            newValue = true;
            Serial.println("prejent !");
          });
        } 
        
        
        
      }else
        {
          if (newValue)
          {
            newValue = false;
            for (size_t i = 0; i < 120; i++)
            {
              graphValue[i]= graphValue[i+1];
            }
            graphValue[120] = sin(XValue*.2);
          }
          
        }
      for (size_t i = 0; i < 120; i++)
        {
          display->setPixel(i,32+(graphValue[i]*30));
        }
      display->drawHorizontalLine(0,32,128);
      display->display();
      delay(100);
      break;
  default:
    State = 0;
    display->clear();
    display->drawString(0,0,String(State));
    display->display();

    break;
  }
  

  delay(50);
}