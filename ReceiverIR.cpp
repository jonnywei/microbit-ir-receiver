#include "pxt.h"

namespace DFRobotIR { 
int ir_code = 0x00;
int ir_addr = 0x00;
int data;

int logic_value(){//Determine logical value "0" and "1" subfunctions
    uint32_t lasttime = system_timer_current_time_us();
    uint32_t nowtime;
    while(!uBit.io.P16.getDigitalValue());//Low wait
    nowtime = system_timer_current_time_us();
    if((nowtime - lasttime) > 400 && (nowtime - lasttime) < 700){//Low 560us
        while(uBit.io.P16.getDigitalValue());//Is high wait
        lasttime = system_timer_current_time_us();
        if((lasttime - nowtime)>400 && (lasttime - nowtime) < 700){//And then the high 560us
            return 0;
        }else if((lasttime - nowtime)>1500 && (lasttime - nowtime) < 1800){//Then the high level is 1.7ms
            return 1;
       }
    }
    return -1;
}

void pulse_deal(){
    int i;
    ir_addr=0x00;//clear
    for(i=0; i<16;i++ )
    {
      if(logic_value() == 1)
      {
        ir_addr |=(1<<i);
      }
    }
    //Parse the command instruction in the remote control code
    ir_code=0x00;//clear
    for(i=0; i<16;i++ )
    {
      if(logic_value() == 1)
      {
        ir_code |=(1<<i);
      }
    }

}

void remote_decode(void){
    data = 0x00;
    uint32_t lasttime = system_timer_current_time_us();
    uint32_t nowtime;
    while(uBit.io.P16.getDigitalValue()){//High level wait
        nowtime = system_timer_current_time_us();
        if((nowtime - lasttime) > 100000){//More than 100 ms indicates that no key is pressed at this time
            ir_code = 0xff00;
            return;
        }
    }
    //If the high level lasts for less than 100ms
    lasttime = system_timer_current_time_us();
    while(!uBit.io.P16.getDigitalValue());//Low wait
    nowtime = system_timer_current_time_us();
    if((nowtime - lasttime) < 10000 && (nowtime - lasttime) > 8000){//9ms
        while(uBit.io.P16.getDigitalValue());//High wait
        lasttime = system_timer_current_time_us();
        if((lasttime - nowtime) > 4000 && (lasttime - nowtime) < 5000){//4.5ms: The infrared protocol header is received and the data is newly sent. Start parsing logic 0 and 1
            pulse_deal();
            //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
            data = ir_code;
            return;//ir_code;
        }else if((lasttime - nowtime) > 2000 && (lasttime - nowtime) < 2500){//2.25ms,Indicates that the packet sent is the same as the previous packet
            while(!uBit.io.P16.getDigitalValue());//Low wait
            nowtime = system_timer_current_time_us();
            if((nowtime - lasttime) > 500 && (nowtime - lasttime) < 700){//560us
                //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
                data = ir_code;
                return;//ir_code;
            }
        }
    }
}

 //% 
    int irCode(){
    remote_decode();
    return data;
    }
}