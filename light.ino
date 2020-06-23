#include "Button2.h";
#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
    #include <avr/power.h>
#endif

#define NUMPIXELS  12   // WS2812灯珠数量
#define WSPIN      0    // WS2812连接引脚
#define BTNPIN     2    // TTP223触摸按键连接引脚

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);
Button2 button = Button2(BTNPIN);
bool PowerMark = false, RainbowMark = false, pixelMark = false;
static byte i = 0;
byte Brightness = 150;

struct WSColor{
    byte R;
    byte G;
    byte B;
}nowColor;
struct WSColor initColor[8] = {
    {//暖黄
        255,125,15,
    },
    {//雪白
        255,250,250,
    },
    {//猩红
        220,20,60
    },
    {//金色
        255,215,0,
    },
    {//草绿色
        173,255,47,
    },
    {//青色
        64,224,208,
    },
    {//蓝色
        30,120,255,
    },
    {//暗紫罗兰
        148,0,211,
    }
};

void setup(){
    pixels.begin();

    button.setClickHandler(click);
    button.setLongClickHandler(longClick);
    button.setDoubleClickHandler(doubleClick);
    button.setTripleClickHandler(tripleClick);

    pixels.setBrightness(0);
    pixels.show();
}

void loop(){
    button.loop();
    if(PowerMark && RainbowMark){
        static int j=0;

        pixels.setBrightness(Brightness);
        for(byte i=0; i<pixels.numPixels(); i++) {
            pixels.setPixelColor( i, Wheel(( i + j ) & 255 ));
        }
        pixels.show();

        j++;
        if(j>255){
            j=0;
        }
        delay(10);
    }
    else if(PowerMark && pixelMark){
        if ( !(i < 8 && i >= 0) ) {
            i = 0;
        }
        nowColor = initColor[i];
        pixels.setBrightness(Brightness);
        for(int i = 0; i < NUMPIXELS; i++){
            pixels.setPixelColor(i, nowColor.R, nowColor.G, nowColor.B);
        }
        pixels.show(); 
        pixelMark = false;
    }
}

void click(Button2& btn) {
    changeColor();
}
void doubleClick(Button2& btn) {
    changeBrightness();
}
void tripleClick(Button2& btn) {
    changeMode();
}
void longClick(Button2& btn) {
    int time = btn.wasPressedFor();
    changePower(time);
}

void changeColor() {
    if(PowerMark){
        if(!RainbowMark){
            i++;
        }
        RainbowMark = false;
        pixelMark = true;
    }
}
void changeBrightness() {
    if(PowerMark){
        if(Brightness < 50){
            Brightness = 50;
        }
        else if(Brightness < 100){
            Brightness = 100;
        }
        else if(Brightness < 150){
            Brightness = 150;
        }
        else if(Brightness < 200){
            Brightness = 200;
        }
        else if(Brightness < 255){
            Brightness = 255;
        }
        else{
            Brightness = 10;
        }
        pixels.setBrightness(Brightness);
        pixels.show();
    }
}
void changeMode() {
    if(PowerMark){
        RainbowMark = !RainbowMark;
    }
}
void changePower(int time) {
    if (time > 500) {
        PowerMark = !PowerMark;
        if(!PowerMark){
            RainbowMark = false;
            pixelMark = false;
            pixels.setBrightness(0);
            pixels.show();
        }
        else if(PowerMark){
            pixelMark = true;
        }
    }
}

uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if(WheelPos < 170) {
        WheelPos -= 85;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else{
        WheelPos -= 170;
        return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}
