#include <ArduinoHardware.h>

#include <ros.h>
#include <rov_diy/rov_msg.h>

ros::NodeHandle  nh;

void _blnk(int _count)
{
  for (int _i=0; _i< _count; _i++)
  {
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
    
    if (_i > 15) break;
  }
}

void _msgCB( const rov_diy::rov_msg& cmd_msg){
  int _c = cmd_msg.msg_int;
  _blnk(_c);
}

ros::Subscriber<rov_diy::rov_msg> sub("chat_costom", &_msgCB );

void setup()s
{ 
  pinMode(13, OUTPUT);
  nh.getHardware()->setBaud(230400);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{  
  nh.spinOnce();
  delay(1);
}
