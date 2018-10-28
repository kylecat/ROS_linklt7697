#ifndef _ROS_rov_diy_rov_msg_h
#define _ROS_rov_diy_rov_msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rov_diy
{

  class rov_msg : public ros::Msg
  {
    public:
      typedef const char* _msg_str_type;
      _msg_str_type msg_str;
      typedef int32_t _msg_int_type;
      _msg_int_type msg_int;
      typedef float _msg_double_type;
      _msg_double_type msg_double;

    rov_msg():
      msg_str(""),
      msg_int(0),
      msg_double(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_msg_str = strlen(this->msg_str);
      varToArr(outbuffer + offset, length_msg_str);
      offset += 4;
      memcpy(outbuffer + offset, this->msg_str, length_msg_str);
      offset += length_msg_str;
      union {
        int32_t real;
        uint32_t base;
      } u_msg_int;
      u_msg_int.real = this->msg_int;
      *(outbuffer + offset + 0) = (u_msg_int.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_msg_int.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_msg_int.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_msg_int.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->msg_int);
      offset += serializeAvrFloat64(outbuffer + offset, this->msg_double);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_msg_str;
      arrToVar(length_msg_str, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_msg_str; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_msg_str-1]=0;
      this->msg_str = (char *)(inbuffer + offset-1);
      offset += length_msg_str;
      union {
        int32_t real;
        uint32_t base;
      } u_msg_int;
      u_msg_int.base = 0;
      u_msg_int.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_msg_int.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_msg_int.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_msg_int.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->msg_int = u_msg_int.real;
      offset += sizeof(this->msg_int);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->msg_double));
     return offset;
    }

    const char * getType(){ return "rov_diy/rov_msg"; };
    const char * getMD5(){ return "fa4179b65e751a4bb16d19c95f60ec8f"; };

  };

}
#endif