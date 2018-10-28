#include <ros.h>
#include <rov_diy/rov_msg.h>

//init node
ros::NodeHandle nh;

rov_diy::rov_msg _msg;

ros::Publisher chatter("arduino", &_msg);

void setup()
{
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(chatter);
}

void loop()
{
  _msg.msg_str="HiArduino";
  _msg.msg_int=int(1000);
  _msg.msg_double=double(0.001);

  chatter.publish( &_msg );
  nh.spinOnce();
  delay(1000);
}

