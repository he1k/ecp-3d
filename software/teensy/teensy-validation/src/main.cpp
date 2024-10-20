#include <Arduino.h>
#define PIN_OUT 28
#define BFR_SIZE 32
#define LINE_FEED 0xA
#define DEC_PLCS 6
#define START_CMD 'B'
#define STOP_CMD 'S'
#define STATE_IDLE 0
#define STATE_CHECK 1
#define STATE_DELAY 2
#define STATE_ACTIVE 3
#define SIM_DELAY 3.0
#define TS_TEENSY 0.001
#define TS_MATLAB 0.004
IntervalTimer tim;

bool flg = 0;

uint8_t bfr[BFR_SIZE];
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
  for(uint8_t i = 0; i < BFR_SIZE; i++)
  {
    bfr[i] = 0;
  }
  tim.begin(isr_tim, TS_TEENSY*1e6);
  while(1)
  {
    Serial.print(TS_MATLAB,6); Serial.print(" / "); Serial.print(TS_TEENSY,6); Serial.print(" = "); Serial.println((uint32_t) (TS_MATLAB/TS_TEENSY));
    delay(500);
  }
}
volatile float m[3] = {0,0,0}; // Measurements: m[0] -> m(k-2), m[1] -> m(k-1), m[2] -> m(k)
volatile float u[3] = {0,0,0}; // Output:       u[0] -> u(k-2), u[1] -> u(k-1), u[2] -> u(k)
volatile float out = 0;
float t = 0.0;

void loop()
{
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
          // Measure
          Serial.print(cmd);
          Serial.print(cmd);
          Serial.print(" ");
          Serial.print(in, DEC_PLCS);  //theta_1
          Serial.print(" ");
          Serial.print(1.0, DEC_PLCS); // omega_1
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






