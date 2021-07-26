#include "U8glib.h"
#include <HX711.h>
#include <motor.h>
#define power 10 //震动马达
#define BUZZER 8//喇叭针脚

#define NOTE_D0 -1 
#define NOTE_D1 294 
#define NOTE_D2 330 
#define NOTE_D3 350 
#define NOTE_D4 393 
#define NOTE_D5 441 
#define NOTE_D6 495 
#define NOTE_D7 556 

#define NOTE_DL1 147 
#define NOTE_DL2 165 
#define NOTE_DL3 175 
#define NOTE_DL4 196 
#define NOTE_DL5 221 
#define NOTE_DL6 248 
#define NOTE_DL7 278 

#define NOTE_DH1 589 
#define NOTE_DH2 661 
#define NOTE_DH3 700 
#define NOTE_DH4 786 
#define NOTE_DH5 882 
#define NOTE_DH6 990 
#define NOTE_DH7 112  
//以上部分是定义是把每个音符和频率值对应起来，其实不用打这么多，但是都打上了，后面可以随意编写

#define WHOLE 1 
#define HALF 0.5 
#define QUARTER 0.25 
#define EIGHTH 0.25 
#define SIXTEENTH 0.625  
//这部分是用英文对应了拍子，这样后面也比较好看

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//配置OLED屏幕针脚

int base = 0, ms = 0, s = 0, mi = 51, hr = 23, week = 2;//进行初始化时间
int nMi = 52, nHr = 23;//闹钟时间
double tem = 0;//温度
double Weight = 0;
void tem_read(){//读取当前温度
  tem = (double)analogRead(A0) * (5 / 10.23);//读取并计算温度
}



int tune[] =  
{  
 NOTE_D0,NOTE_D0,NOTE_D0,NOTE_D6,NOTE_D7,NOTE_DH1,NOTE_D7,NOTE_DH1,NOTE_DH3,NOTE_D7,NOTE_D7,NOTE_D7,NOTE_D3,NOTE_D3,
 NOTE_D6,NOTE_D5,NOTE_D6,NOTE_DH1,NOTE_D5,NOTE_D5,NOTE_D5,NOTE_D3,NOTE_D4,NOTE_D3,NOTE_D4,NOTE_DH1,
 NOTE_D3,NOTE_D3,NOTE_D0,NOTE_DH1,NOTE_DH1,NOTE_DH1,NOTE_D7,NOTE_D4,NOTE_D4,NOTE_D7,NOTE_D7,NOTE_D7,NOTE_D0,NOTE_D6,NOTE_D7,
 NOTE_DH1,NOTE_D7,NOTE_DH1,NOTE_DH3,NOTE_D7,NOTE_D7,NOTE_D7,NOTE_D3,NOTE_D3,NOTE_D6,NOTE_D5,NOTE_D6,NOTE_DH1,
 NOTE_D5,NOTE_D5,NOTE_D5,NOTE_D2,NOTE_D3,NOTE_D4,NOTE_DH1,NOTE_D7,NOTE_D7,NOTE_DH1,NOTE_DH1,NOTE_DH2,NOTE_DH2,NOTE_DH3,NOTE_DH1,NOTE_DH1,NOTE_DH1,
 NOTE_DH1,NOTE_D7,NOTE_D6,NOTE_D6,NOTE_D7,NOTE_D5,NOTE_D6,NOTE_D6,NOTE_D6,NOTE_DH1,NOTE_DH2,NOTE_DH3,NOTE_DH2,NOTE_DH3,NOTE_DH5,
 NOTE_DH2,NOTE_DH2,NOTE_DH2,NOTE_D5,NOTE_D5,NOTE_DH1,NOTE_D7,NOTE_DH1,NOTE_DH3,NOTE_DH3,NOTE_DH3,NOTE_DH3,NOTE_DH3,
 NOTE_D6,NOTE_D7,NOTE_DH1,NOTE_D7,NOTE_DH2,NOTE_DH2,NOTE_DH1,NOTE_D5,NOTE_D5,NOTE_D5,NOTE_DH4,NOTE_DH3,NOTE_DH2,NOTE_DH1,
 NOTE_DH3,NOTE_DH3,NOTE_DH3,NOTE_DH3,NOTE_DH6,NOTE_DH6,NOTE_DH5,NOTE_DH5,NOTE_DH3,NOTE_DH2,NOTE_DH1,NOTE_DH1,NOTE_D0,NOTE_DH1,
 NOTE_DH2,NOTE_DH1,NOTE_DH2,NOTE_DH2,NOTE_DH5,NOTE_DH3,NOTE_DH3,NOTE_DH3,NOTE_DH3,NOTE_DH6,NOTE_DH6,NOTE_DH5,NOTE_DH5,
 NOTE_DH3,NOTE_DH2,NOTE_DH1,NOTE_DH1,NOTE_D0,NOTE_DH1,NOTE_DH2,NOTE_DH1,NOTE_DH2,NOTE_DH2,NOTE_D7,NOTE_D6,NOTE_D6,NOTE_D6,NOTE_D6,NOTE_D7
};//这部分就是整首曲子的音符部分，用了一个序列定义为tune，整数

float duration[]= 
{  
  1,1,1,0.5,0.5,     1+0.5,0.5,1,1,     1,1,1,0.5,0.5,
  1+0.5,0.5,1,1,     1,1,1,1,          1+0.5,0.5,1,1, 
  1,1,0.5,0.5,0.5,0.5,    1+0.5,0.5,1,1,     1,1,1,0.5,0.5,
  1+0.5,0.5,1,1,    1,1,1,0.5,0.5,     1+0.5,0.5,1,1,
  1,1,1,0.5,0.5,    1,0.5,0.25,0.25,0.25,0.5,    0.5,0.5,0.5,0.25,0.5,1,
  0.5,0.5,0.5,0.5,1,1,    1,1,1,0.5,0.5,    1+0.5,0.5,1,1,
  1,1,1,0.5,0.5,    1.5,0.5,1,1,    1,1,1,1,
  0.5,0.5,1,1,0.5,0.5,    1.5,0.25,0.5,1,    1,1,1,1,
  1,1,1,1,    1,1,1,1,    0.5,0.5,1,1,0.5,0.5,
  1,0.5,0.5,1,1,    1,1,1,1,    1,1,1,1,
  0.5,0.5,1,1,0.5,0.5,    1,0.5,0.25,0.5,1,    1,1,1,0.5,0.5
};//这部分是整首曲子的节拍部分，也定义个序列duration，浮点（数组的个数和前面音符的个数是一样的，一一对应么）

int length = sizeof(tune)/sizeof(tune[0]);//这里用了一个sizeof函数， 可以查出tone序列里有多少个音符;
void alarm_music() {
  while(hr == nHr && mi == nMi) {//播放闹钟1分钟
      Serial.println("RUN!");
      
     digitalWrite(power,HIGH);//传感器震动
     for(int x=0;x<length;x++)//循环音符的次数
  {  
    tone(BUZZER,tune[x]);//此函数依次播放tune序列里的数组，即每个 音符

    delay(400*duration[x]);//每个音符持续的时间，即节拍duration，是调整时间的越大，曲子速度越慢，越小曲子速度越快，自己掌握吧

    noTone(BUZZER);//停止当前音符，进入下一音符
    Weight = - Get_Weight();  //计算放在传感器上的重物重量
    Serial.print(Weight,2); //串口显示重量
  Serial.print("g\n");  //显示单位
      if(Weight <= 20){
        digitalWrite(power,LOW);
        break;
      }
  } 
    delay(5000);//等待5秒后，循环重新开始
  
  }
}

void time_update() {
  ms = millis() - base;
  if (ms > 999) {
    ms = 0;
    base = millis() - 61; //用于校正误差,每过1s提前61ms
    if (++s > 59) {
      s = 0;
      base = millis() + 8; //用于校正误差,每过1min延迟8ms
      if (++mi > 59) {
        mi = 0;
        base = millis() - 38; //用于校正误差,每过1hour提前38ms
        if (++hr > 23) {
          hr = 0;
          base = millis() - 8; //用于校正误差,每过1day提前8ms
          if (++week > 6) week = 0;
        }
      }
    }
  }
}

//读取按键状态，不按为0，短按为1，长按为2
int key_read(int KEY){
  //按键一端接VCC，一端接IO口。按键不按是0，按下是1
  if(digitalRead(KEY) == 1){
    delay(1000);
    if(digitalRead(KEY) == 1){
      Serial.println("LongPush!");
      return 2;
    }
    else{
      Serial.println("ShortPush!");
      return 1;
    }
  }
  else{
    Serial.println("NoPush!");
    return 0;
  }
}

void draw() {
  //按照秒数绘制宽4个像素的进度条
  for (int x = 0; x < 2 * s; x++)
    for (int y = 0; y < 4; y++)
      u8g.drawPixel(x + 5, y + 3);

  u8g.setFont(u8g_font_gdb30n);//设置gdb30像素数字类字体
  
    if (hr > 9) {//如果数字只有一位就添加0在前面
      u8g.setPrintPos( 8, 41);
    } else {
      u8g.drawStr(10, 41, "0");
      u8g.setPrintPos( 32, 41);
    }
    u8g.print(hr); 

  if (s % 2 == 0)//冒号在秒数为偶数时才绘制，达到冒号闪烁的效果
    u8g.drawStr( 56, 37, ":");

  if (mi > 9) {//如果数字只有一位就添加0在前面
    u8g.setPrintPos( 70, 41);
  } else {
    u8g.drawStr(70, 41, "0");
    u8g.setPrintPos( 94, 41);
  }
  u8g.print(mi);

  u8g.setFont(u8g_font_gdb14r);//设置gdb14像素常用类字体
  switch (week) {//绘制星期数到屏幕
    case 0: u8g.drawStr(6, 61, "Sun"); break;
    case 1: u8g.drawStr(6, 61, "Mon"); break;
    case 2: u8g.drawStr(6, 61, "Tue"); break;
    case 3: u8g.drawStr(6, 61, "Wed"); break;
    case 4: u8g.drawStr(6, 61, "Thu"); break;
    case 5: u8g.drawStr(6, 61, "Fri"); break;
    default: u8g.drawStr(6, 61, "Sat");
  }

  u8g.setPrintPos( 56, 61);//绘制温度示数到屏幕
  u8g.print(tem);
  u8g.drawStr(111, 61, "C");
}
void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(power, OUTPUT);
  //pinMode(SET_KEY,OUTPUT);
  //pinMode(CANCEL_KEY,OUTPUT);
  //pinMode(LAST_KEY,OUTPUT);
  //pinMode(NEXT_KEY,OUTPUT);
  Serial.begin(9600);
  Serial.print("Welcome to use!\n");
  Init_motor();
  Init_Hx711();
  Get_Maopi();
  u8g.setRot180();//旋转屏幕180度
  base = millis();//初始化基准时间

}

void loop() {
  Weight = - Get_Weight();  //计算放在传感器上的重物重量
  tem_read();//温度读取
  time_update();//更新时间
  //while(hr == nHr && mi == nMi && Trigger == 1)
    alarm_music();//闹铃函数
  //key_read();
  u8g.firstPage(); do draw(); while (u8g.nextPage());//刷新屏幕
  Serial.print(Weight,2); //串口显示重量
  Serial.print("g\n");  //显示单位
}

