#include <Arduino.h>
#include "constants.h"
#include <math.h>
#include <SPI.h>
#include <dac.h>
#define PIN_OUT      28
#define BFR_SIZE     32
#define STATE_IDLE   0
#define STATE_CHECK  1
#define STATE_DELAY  2
#define STATE_ACTIVE 3
#define SINE_FREQ    0.25

IntervalTimer tim;
Dac dac;
bool flg = 0;

volatile uint8_t bfr[BFR_SIZE];
uint8_t idx = 0;
bool rdy = false;

uint8_t state = STATE_IDLE;
uint32_t cnt_tim = 0;
void isr_tim()
{
  flg = 1;
}
void setup()
{
  Serial.begin(12000000);
  pinMode(PIN_OUT, OUTPUT);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV);
  SPI.setDataMode(SPI_MODE);
  for(uint8_t i = 0; i < BFR_SIZE; i++)
  {
    bfr[i] = 0;
  }
  bfr[0] = 0x44; // Command
  bfr[1] = (uint8_t) ' ';
  bfr[2] = (uint8_t) '1';
  bfr[3] = (uint8_t) '.';
  bfr[4] = (uint8_t) '0';
  bfr[5] = (uint8_t) ' ';

  // Add five more floats separated by whitespace
  bfr[6]  = (uint8_t) '2';
  bfr[7]  = (uint8_t) '.';
  bfr[8]  = (uint8_t) '0';
  bfr[9]  = (uint8_t) ' ';

  bfr[10] = (uint8_t) '3';
  bfr[11] = (uint8_t) '.';
  bfr[12] = (uint8_t) '0';
  bfr[13] = (uint8_t) '\n';
  tim.begin(isr_tim, TS_TEENSY*1e6);
}
volatile float out = 0;
float t = 0.0;
uint32_t m_p = micros();
void loop()
{
  if(flg)
  {
    flg = 0;
    // 1) Check for faults in DAC
    if(dac.get_fault() != NO_FAULT)
    {
      // Reset dac
      dac.hw_reset();
      // Slight delay
      delayMicroseconds(1);
      // Rewrite config
      dac.write_config(DAC_CONF_RA, DAC_CONF_PV, DAC_CONF_IRO, DAC_CONF_ETS, DAC_CONF_B2C, DAC_CONF_OVR, DAC_CONF_CV);
      
    }
    // 2) Reset dac and add small timeout
    dac.hw_reset();
    dac.write_config(DAC_CONF_RA, DAC_CONF_PV, DAC_CONF_IRO, DAC_CONF_ETS, DAC_CONF_B2C, DAC_CONF_OVR, DAC_CONF_CV);
    // 3) check for fault again.

  }




  if(flg)
  {
    flg = 0;
    t+=TS_TEENSY;

    switch(state)
    {
      case STATE_IDLE:
      // IDLE > ---------------------------------------------------------------- //
        if(rdy)
        {
          state = STATE_CHECK;
        }
      // < IDLE ---------------------------------------------------------------- //
        break;
      case STATE_CHECK:
      // CHECK > --------------------------------------------------------------- //
        if(bfr[0] == START_CMD)
        {
          cnt_tim = 0;
          state = STATE_DELAY;
        }else
        {
          state = STATE_IDLE;
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
        }
      // < DELAY --------------------------------------------------------------- //
        break;
      case STATE_ACTIVE:
      // ACTIVE > -------------------------------------------------------------- //
        cnt_tim++;
        if(cnt_tim == (uint32_t) (TS_MATLAB/TS_TEENSY))
        {
          char s1[16];
          int i;
          for(i = 0; i < BFR_SIZE; i++)
          {
            if(((char) bfr[i]) == ' ')
            {
              break;
            }
            s1[i] = bfr[i];
          }
          uint32_t cmd = atoi(s1);
          float in = atof((char*)(bfr+i+1));
          // Measure
          Serial.print(cmd);
          Serial.print(" ");
          Serial.print(in, DEC_PLCS);  //theta_1
          Serial.print(" ");
          Serial.print(sin(2.0*M_PI*SINE_FREQ*t), DEC_PLCS); // omega_1
          Serial.print(" ");
          Serial.print(2.0, DEC_PLCS); // theta_2
          Serial.print(" ");
          Serial.print(3.0, DEC_PLCS);  //omega_2
          Serial.print(" ");
          Serial.print(4.0, DEC_PLCS); // theta_3
          Serial.print(" ");
          Serial.print(5.0, DEC_PLCS); // omega_3
          Serial.print("\n");
        }
      // < ACTIVE -------------------------------------------------------------- //
        break;
    }




    // // Check for available USB data and read it
    // if(!rdy)
    // {
    //   while(Serial.available())
    //   {
    //     uint8_t b = Serial.read();
    //     if(b == LINE_FEED)
    //     {
    //       for(uint8_t i = idx; i < BFR_SIZE; i++) // Zero out rest of buffer
    //       {
    //         bfr[i] = 0;
    //       }
    //       rdy = true;
    //       idx = 0;
    //     }else
    //     {
    //       bfr[idx] = b;
    //       idx++;
    //     }
    //   }
    // }else // Process available data
    // {
    //   if((char) bfr[0] == 'B') // Start command recieved
    //   {
    //     rdy = false;
    //     state = STATE_ON;
    //     cnt_tim = 0;
    //   }else if((char) bfr[0] == 'S') // Stop command recieved
    //   {
    //     rdy = false;
    //     state = STATE_IDLE;
    //   }else // Start command previously recieved, now data is flowing in
    //   {
    //     //state = STATE_COM;
    //   }
    //   if(state == STATE_ON)
    //   {
    //     cnt_tim++;
    //     if(cnt_tim == 3000) // Ensure that Simulink is ready
    //     {
    //       cnt_tim = 0;
    //       state = STATE_COM;
    //     }
    //   }
    //   if(state == STATE_COM)
    //   {
    //     char s1[16];
    //     int i;
    //     for(i = 0; i < BFR_SIZE; i++)
    //     {
    //       if(((char) bfr[i]) == ' ')
    //       {
    //         break;
    //       }
    //       s1[i] = bfr[i];
    //     }
    //     uint32_t cmd = atoi(s1);
    //     float in = atof((char*)(bfr+i+1)); //float in = atof((char*)bfr);
    //     Serial.print(cmd);
    //     Serial.print(" ");
    //     Serial.print(in, DEC_PLCS);  //theta_1
    //     Serial.print(" ");
    //     Serial.print(1.0, DEC_PLCS); // omega_1
    //     Serial.print(" ");
    //     Serial.print(2.0, DEC_PLCS); // theta_2
    //     Serial.print(" ");
    //     Serial.print(3.0, DEC_PLCS);  //omega_2
    //     Serial.print(" ");
    //     Serial.print(4.0, DEC_PLCS); // theta_3
    //     Serial.print(" ");
    //     Serial.print(5.0, DEC_PLCS); // omega_3
    //     Serial.print("\n");
    //     rdy = false;
    //   }
    // }
  }
}

void serialEvent()
{
  if(!rdy)
  {
    while(Serial.available())
    {
      uint8_t b = Serial.read();
      if(b == LINE_FEED)
      {
        for(uint8_t i = idx; i < BFR_SIZE; i++) // Zero out rest of buffer
        {
          bfr[i] = 0;
        }
        rdy = true;
        idx = 0;
      }else
      {
        bfr[idx] = b;
        idx++;
      }
    }
  }
}

// uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
// void loop()
// {
//   t1 = micros();
//   digitalWrite(PIN_OUT, LOW);
//   t1 = micros() - t1;
//   t2 = micros();
//   digitalWrite(PIN_OUT, HIGH);
//   t2 = micros() - t2;
//   t3 = micros();
//   digitalWriteFast(PIN_OUT, LOW);
//   t3 = micros() - t3;
//   t4 = micros();
//   digitalWriteFast(PIN_OUT, HIGH);
//   t4 = micros() - t4;
//   delay(250);
//   Serial.print("t1 (low)        = "); Serial.print(t1); Serial.print("\t");
//   Serial.print("t1 (high)       = "); Serial.print(t2); Serial.print("\t");
//   Serial.print("t1 (low, fast)  = "); Serial.print(t3); Serial.print("\t");
//   Serial.print("t1 (high, fast) = "); Serial.println(t4);
//   delay(500);
// }






