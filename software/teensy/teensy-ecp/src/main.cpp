#include <Arduino.h>
#include "constants.h"
#include "serial_commands.h"
#include "QuadEncoder.h"
QuadEncoder enc3(1, 0, 1, 0);
QuadEncoder enc2(2, 2, 3, 0);
QuadEncoder enc1(3, 7, 8, 0);
IntervalTimer tim;
bool flag = false;
bool active = false;
uint8_t bfr[BFR_SIZE];
uint8_t idx_bfr = 0;
bool full_bfr = false;
bool cmd_rdy = false;
void tim_isr()
{
  flag = true;
}
void float_to_bytes(float *f, uint8_t *b)
{
  memcpy(b, f, 4);
}
void tx_float(uint8_t *b)
{
  Serial.print(b[3], HEX);
  if(!(b[2] & 0xf0)){
    Serial.print(0);
  }
  Serial.print(b[2], HEX);
  if(!(b[1] & 0xf0)){
    Serial.print(0);
  }
  Serial.print(b[1], HEX);
  if(!(b[0] & 0xf0)){
    Serial.print(0);
  }
  Serial.print(b[0], HEX);
}
void setup()
{
  Serial.begin(BAUD_RATE);
  for(int i = 0; i < BFR_SIZE; i++)
  {
    bfr[i] = 0;
  }
  idx_bfr = 0;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  enc1.setInitConfig();
  //enc1.EncConfig.filterCount = 5;
  //enc1.EncConfig.filterSamplePeriod = 255;
  enc1.init();
  enc2.setInitConfig();
  //enc2.EncConfig.filterCount = 5;
  //enc2.EncConfig.filterSamplePeriod = 255;
  enc2.init();
  enc3.setInitConfig();
  //enc3.EncConfig.filterCount = 5;
  //enc3.EncConfig.filterSamplePeriod = 10;
  enc3.init();


  tim.begin(tim_isr, SAMPLE_TIME*1e6);
}
uint8_t cnt = 0;
float f_cnt = 0.0;
void loop()
{
  //Serial.print("enc1: "); Serial.print(enc1.read()); Serial.print("\t");
  //Serial.print("enc2: "); Serial.print(enc2.read()); Serial.print("\t");
  //Serial.print("enc3: "); Serial.print(enc3.read()); Serial.println("\t");
  //delay(100);
  //Serial.write(1);
  //Serial.write(LINE_FEED);
  
  // if(false)
  // {
  //   //Serial.write(bfr[0]);
  //   //Serial.write(bfr[1]);  
  //   switch(bfr[0])
  //   {
  //     case CMD_START:
  //       if(bfr[1] == LINE_FEED) // Correctly received command
  //       {
  //         Serial.write(CMD_START_RX);
  //         Serial.write(LINE_FEED);
  //         active = true;
  //       }else
  //       {
  //         Serial.write(CMD_ERROR_TERMINATION);
  //         Serial.write(LINE_FEED);
  //       }
  //       break;
  //     case CMD_STOP:
  //       if(bfr[1] == LINE_FEED) // Correctly received command
  //       {
  //         Serial.write(CMD_STOP_RX);
  //         Serial.write(LINE_FEED);
  //         active = false;
  //       }else
  //       {
  //         Serial.write(CMD_ERROR_TERMINATION);
  //         Serial.write(LINE_FEED);
  //       }
  //       break;
  //   }
  //   cmd_rdy = false;
  //   idx_bfr = 0;
  // }
  // if(cmd_rdy)
  // {
  //   tx_float(bfr);
  //   cmd_rdy = false;
  //   idx_bfr = 0;
  // }
  if(flag)// && active)
  {
    flag = false;
    uint8_t b[4];
    //f_cnt =  3.3155519962310791015625f;
    //f_cnt =  3.3155901432037353515625f;// 3476608.25f;
    f_cnt = ((float)enc1.read())*360.0f/16000.0f;
    float_to_bytes(&f_cnt,b);
    tx_float(b);
    Serial.print(" ");
    f_cnt = ((float)enc2.read())*360.0f/16000.0f;
    float_to_bytes(&f_cnt,b);
    tx_float(b);
    Serial.print(" ");
    f_cnt = ((float)enc3.read())*360.0f/16000.0f;
    float_to_bytes(&f_cnt,b);
    tx_float(b);
    Serial.print("\n");
    digitalWrite(LED_BUILTIN, HIGH);
  }else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void serialEvent()
{
  if(Serial.available())
  {
    if(idx_bfr < BFR_SIZE)
    {
      uint8_t b = Serial.read();
      if(b == LINE_FEED)
      {
        //Serial.print(idx_bfr, HEX);
        for(int i = 0; i < idx_bfr; i++)
        {
          Serial.print(bfr[i],HEX);
        }
        Serial.print("\n");
        digitalWrite(LED_BUILTIN, HIGH);
        idx_bfr = 0;
      }else
      {
        if(b >= 0x30 && b <= 0x39)
        {
          b -= 0x30;
        }else if(b >= 0x41 && b <= 0x46) // Using upper case letters for digits > 9 - this is done when constants are written in dec
        {
          b = (b & 0x0F) + 0x9;
        }else if(b >= 0x61 && b <= 0x66) // Using lower case letters for digits > 9 - this is done when constants are written in hex
        {
          b = (b & 0x0F) + 0x9;
          //b -= (0x061+0x0A);
        }
        bfr[idx_bfr] = b;
        idx_bfr++;
      }
      // if(idx_bfr == 4)
      // {
        
      // }else
      // {
      //   bfr[idx_bfr] = b;
      //   idx_bfr++;
      // }
    }
  }
}

/*
void serialEvent()
{
  if(Serial.available()) // Maybe change to while loop instead
  {
    if(!cmd_rdy && idx_bfr >= BFR_SIZE )
    {
      full_bfr = true;
    }else if(!cmd_rdy && idx_bfr < BFR_SIZE)
    {
      uint8_t b = Serial.read();
      bfr[idx_bfr] = b;
      idx_bfr++;
      if(b == LINE_FEED)
      {
        cmd_rdy = true;
      }
    }
  }
}
*/