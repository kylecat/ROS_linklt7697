/********************************************************************** 
 ** 由ROS端，經 To_7697 及 From_7697 兩個Topic交換資料
 ** Serial 會被占用，其他通訊需要用Serial1(P6,P7)或是SoftSerial來實現 
 ** cmd.msg(控制訊號)格式：
 *  string name // 預計要用來辨識 servo (未完成)
 *  int32 pin   // servo 腳位
 *  int32 angle // servo 轉動角度
 ** resp.msg(控制訊號)格式：
 *  string name // 預計要用來辨識 servo (未完成)
 *  int32 angle // 目前角度
 * 
 * Author：Kyle Liu
 * License: MIT
 * Reference：https://github.com/frankjoshua/rosserial_arduino_lib
 **********************************************************************/
 /*******************************
  * Servo 馬達相關函式庫及宣告
  *******************************/
#include <Servo.h>
Servo servo;

int _servoPin, _sevoAngle = 0; // 用全域變數放callback裡的資料(好像可以省略)

 /*******************************
  * ROS相關函式庫及宣告
  *******************************/
#include <ArduinoHardware.h>
#include <ros.h>
#include <linklt7697/cmd.h>
#include <linklt7697/resp.h>

ros::NodeHandle  nh;    // 建立Node Handle 這個實例，主要用來跟RaspberryPi上的Rosserial 建立連線

linklt7697::cmd _cmd;   // 用 _cmd 建立linklt7697::cmd這個struct

// Callback函數(留意放入的參數格式)
void servo_CB( const linklt7697::cmd& cmd_msg) 
{
  /***********************************************
   * 從cmd_msg這個struct中取得 pin 及 angle 資料
   * 驅動pin腳位上的servo motor
   * 順便切換一下usr的燈號
   **********************************************/
  _servoPin = cmd_msg.pin;
  _sevoAngle = cmd_msg.angle;
  servo.attach(_servoPin);  // attach it to pin
  servo.write(_sevoAngle); // set servo angle, should be from 0-180
  digitalWrite(LED_BUILTIN, HIGH - digitalRead(LED_BUILTIN)); //toggle led
}

// Callback函數OK了就建立一個 Subscriber 的實例(sub)，Topic為"To_7697"
ros::Subscriber<linklt7697::cmd> sub("To_7697", &servo_CB );

// Publisher 設定
linklt7697::resp _resp;                      // 用 _resp 建立linklt7697::resp這個struct
ros::Publisher chatter("From_7697", &_resp);    // 建立 Publisher 的實例(chatter)，Topic為"From_7697"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  nh.initNode();          // 初始化NodeHandle
  nh.advertise(chatter);   // 啟動Publish
  nh.subscribe(sub);       // 啟動Subscrible
}

void loop() {

  _resp.name = "HiROS";       // 把資料塞進linklt7697::resp的struct裡
  _resp.angle = _sevoAngle;    // 要把所有項目都塞資料才能回傳
  chatter.publish( &_resp );   // 發布資料

  nh.spinOnce();              // 應該是發布一次後停止
  delay(1000);

}
