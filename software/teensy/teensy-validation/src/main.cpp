#include <Arduino.h>
#include "constants.h"
#include <math.h>
#include <SPI.h>
#include "QuadEncoder.h"
#include "bridge.h"
// Encoders
QuadEncoder enc3(CHA_ENC3, PIN_ENC3_A, PIN_ENC3_B, 0);
QuadEncoder enc2(CHA_ENC2, PIN_ENC2_A, PIN_ENC2_B, 0);
QuadEncoder enc1(CHA_ENC1, PIN_ENC1_A, PIN_ENC1_B, 0);
#define PIN_OUT      28
//#define BFR_SIZE     32
#define STATE_IDLE   0
#define STATE_CHECK  1
#define STATE_DELAY  2
#define STATE_ACTIVE 3
#define SINE_FREQ    0.25

Bridge com;
IntervalTimer tim;
bool flg = 0;

volatile uint8_t bfr[BFR_SIZE];
int idx = 0;
bool rdy = false;

uint8_t state = STATE_IDLE;
uint32_t cnt_tim = 0;
void isr_tim()
{
  flg = 1;
}
void setup()
{
  //Serial.begin(12000000);
  com.begin(&Serial);
  pinMode(PIN_OUT, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  tim.begin(isr_tim, TS_TEENSY*1e6);
}
volatile float out = 0;
double t = 0.0;
uint32_t t_p = 0;
void loop()
{
  if(flg)
  {
    flg = 0;
    t+=TS_TEENSY;
    uint8_t cmd = com.read_command();
    if(cmd == CMD_STOP)
    {
      state = STATE_IDLE;
    }
    switch(state)
    {
      case STATE_IDLE:
      // IDLE > ---------------------------------------------------------------- //
        if(cmd == CMD_START)
        {
          state = STATE_DELAY;
        }
      // < IDLE ---------------------------------------------------------------- //
        break;
      case STATE_CHECK:
      // CHECK > --------------------------------------------------------------- //
        if(bfr[0] == START_CMD)
        {
          state = STATE_ACTIVE;
          cnt_tim = 0;
          t = 0;
        }else
        {
          state = STATE_IDLE;
          rdy = false;
        }
      // < CHECK --------------------------------------------------------------- //
        break;
      case STATE_DELAY:
      // DELAY > --------------------------------------------------------------- //
      cnt_tim++;
        if(cnt_tim == (uint32_t) (SIM_DELAY/TS_TEENSY))
        {
          cnt_tim = 0;
          state = STATE_ACTIVE;
          t_p = millis();
          Serial.flush();
        }
      // < DELAY --------------------------------------------------------------- //
        break;
      case STATE_ACTIVE:
      // ACTIVE > -------------------------------------------------------------- //
        cnt_tim++;
        //if(cnt_tim == (uint32_t) (TS_MATLAB/TS_TEENSY))
        if(com.data_valid())
        {
          digitalWrite(LED_BUILTIN, HIGH);
          com.write_encoder(com.read_data(),1.3,2.4);
        }else{
          digitalWrite(LED_BUILTIN, LOW);
          com.write_encoder((float) Serial.available(),0,0);
        }
        
        // if(true)
        // {
        //   //cnt_tim = 0;
        //   t = (millis()-t_p)*1e-3;
        //   Serial.print(millis(), DEC_PLCS);  //theta_1
        //   Serial.print("\n");

        //   // Serial.print(sin(2*M_PI*0.25*t), DEC_PLCS);  //theta_1
        //   // Serial.print(" ");
        //   // Serial.print((float)cnt_tim, DEC_PLCS); // theta_2
        //   // Serial.print(" ");
        //   // Serial.print(t, DEC_PLCS); // theta_3
        //   // Serial.print("\n");
        // }
      // < ACTIVE -------------------------------------------------------------- //
        break;
    }
  }
}

void serialEvent()
{
  com.rx_ready_isr();
}


// void serialEvent()
// {
//   if(!rdy)
//   {
//     while(Serial.available())
//     {
//       uint8_t b = Serial.read();
//       if(b == LINE_FEED)
//       {
//         rdy = true;
//         idx = 0;
//       }else
//       {
//         bfr[idx] = b;
//         idx++;
//       }
//     }
//   }
// }




