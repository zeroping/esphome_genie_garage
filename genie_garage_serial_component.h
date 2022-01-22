#include "esphome.h"

class GenieGarageSerial : public Component, public UARTDevice {
 public:
  GenieGarageSerial(UARTComponent *parent, BinarySensor *breakbeamSensorIn, BinarySensor *lampSensorIn, Cover *coverSensorIn, TextSensor* debugSensorIn) :
    UARTDevice(parent),
    breakbeamSensor(breakbeamSensorIn),
    lampSensor(lampSensorIn),
    coverSensor(coverSensorIn),
    debugSensor(debugSensorIn)
  {}

  BinarySensor *breakbeamSensor;
  BinarySensor *lampSensor;
  Cover* coverSensor;
  TextSensor* debugSensor;
  
  uint8_t fullclosed = 0xEC;
  String lastDebugText = "";
  
  void setup() override {
    // nothing to do here
  }
  void loop() override {
    uint8_t pktbuf[32];
    while (available()) {
      pktbuf[0] = read();
      //ESP_LOGD("Genie", "Ch: %02x", pktbuf[0] );
     
      if( pktbuf[0] == 0x01 ) {   //if first byte matches
 
        pktbuf[1] = read();
        int pktlen = pktbuf[1] + 1;
        if ( (pktlen  >= 2) 
          && (pktlen  <= 30)  
          && (available() >= pktlen-2) ) {  //if second byte matches and we have the whole packet
        
          String outPrint = "";
          outPrint += String(pktbuf[0] , HEX);
          outPrint += " " + String(pktbuf[1] , HEX);
          for(int i=2; i<pktlen; i++) //  read remainder
          {
            pktbuf[i] = read();
            outPrint += " " + String(pktbuf[i] , HEX);
          }
          // 01 0A 20 00 02 48 00 EC 00 00 60
          ESP_LOGD("Genie", "pkt %d:  %s ", pktlen, outPrint.c_str() );
          
          uint8_t checksum = 0;
          for(int i=0; i<(pktlen-1); i++) //  checksum all but last byte
          {
            checksum += pktbuf[i];
          }
          
          if( (checksum-1) == pktbuf[pktlen - 1]) // checksum checks out
          {
            if(pktbuf[2] == 0x20 && pktbuf[1] == 0x0A)
            {
              uint8_t open_amount = pktbuf[7];
              uint8_t status_flags_1 = pktbuf[4];
              uint8_t status_flags_2 = pktbuf[5];
              ESP_LOGI("Genie", "open_amount state %d flags %02x %02x ", open_amount, status_flags_1, status_flags_2);
              
              //update the breakbeam state
              if(pktbuf[5] & 0x04) { 
                breakbeamSensor->publish_state(true);
              } else {
                breakbeamSensor->publish_state(false);
              }
              
              //update the lamp state
              if(pktbuf[4] & 0x10) { 
                lampSensor->publish_state(true);
              } else {
                lampSensor->publish_state(false);
              }
              
              //update the cover operation
              if(pktbuf[4] & 0x01)
              {
                if(pktbuf[4] & 0x02)
                {
                  coverSensor->current_operation = COVER_OPERATION_OPENING;
                } else {
                  coverSensor->current_operation = COVER_OPERATION_CLOSING;
                }
              } else {
                coverSensor->current_operation = COVER_OPERATION_IDLE;
              }
// 
              //update the open percentage
              coverSensor->position = (fullclosed-open_amount) / float(fullclosed);
              coverSensor->publish_state( false );
              
              //update the debug info with the packet text
              if(!outPrint.equals(lastDebugText))
              {
                debugSensor->publish_state(outPrint.c_str());
                lastDebugText = outPrint;
              }
              
            }
            else
            {
              ESP_LOGD("Genie", "other packet %02d", pktbuf[2]);
            }
          } else {
            ESP_LOGW("Genie", "calculated checksum %02x != %02x ", checksum, pktbuf[pktlen]);
          }
          
        }
        else
        {
          ESP_LOGD("Genie", "missing some of packet %d %d", pktlen, available());
        }
      }
    }
  }
};

