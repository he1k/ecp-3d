#include <Arduino.h>
#include <SPI.h>
#include "constants.h"
#include "QuadEncoder.h"
#include "dac.h"
#include "bridge.h"
#include <EEPROM.h>
// Encoders
QuadEncoder enc3(CHA_ENC3, PIN_ENC3_A, PIN_ENC3_B, 0);
QuadEncoder enc2(CHA_ENC2, PIN_ENC2_A, PIN_ENC2_B, 0);
QuadEncoder enc1(CHA_ENC1, PIN_ENC1_A, PIN_ENC1_B, 0);
// Timer
IntervalTimer tim;
// DAC
Dac dac;
// USB
Bridge com;
// 
bool res;
// Timer flag
bool flg = 0;
// Timer interrupt service routine function
void isr_tim()
{
  flg = 1;
}
float f = 0;
#define STATE_OFF 0
#define STATE_ON 1
#define STATE_DELAY 2
uint8_t state = 0;
uint8_t led_green = 0;
uint8_t led_blue = 0;
uint8_t led_red = 0;
uint32_t t_p = 0;
uint32_t t_worst = 0;
uint8_t new_worst = 0;
char stat = 'N';
float u[2];
float y[2];
void set_leds(uint8_t y, uint8_t b, uint8_t r)
{
  digitalWrite(PIN_LED_GREEN, y);
  digitalWrite(PIN_LED_BLUE, b);
  digitalWrite(PIN_LED_RED, r);
}
uint32_t read_flash(uint16_t addr)
{
  return EEPROM.read(addr) | (EEPROM.read(addr+1) << 8) | (EEPROM.read(addr+2) << 16) | (EEPROM.read(addr+3) << 24);
}
void write_flash(uint16_t addr,uint32_t data)
{
  EEPROM.write(addr,data &           0x000000FF);
  EEPROM.write(addr+1,(data >> 8) &  0x000000FF);
  EEPROM.write(addr+2,(data >> 16) & 0x000000FF);
  EEPROM.write(addr+3,(data >> 24) & 0x000000FF);
}
void setup()
{
  // Init communication
  com.begin(&Serial);
  
  pinMode(PIN_DEBUG, OUTPUT);
  digitalWrite(PIN_DEBUG, LOW);

  // Init DAC
  dac.begin(&SPI, AD5721R, PIN_NSYNC, PIN_NLDAC, PIN_NRESET, PIN_NCLEAR, PIN_NALERT);
  pinMode(PIN_NRESET, OUTPUT);
  pinMode(PIN_NALERT, INPUT);
  pinMode(PIN_NLDAC, OUTPUT);
  pinMode(PIN_NCLEAR, OUTPUT);
  pinMode(PIN_NSYNC, OUTPUT);
  digitalWrite(PIN_NRESET, HIGH);
  digitalWrite(PIN_NCLEAR, HIGH);
  digitalWrite(PIN_NSYNC, HIGH);
  digitalWrite(PIN_NLDAC, LOW);
  res = dac.init_device(DAC_CONF_RA, DAC_CONF_PV, DAC_CONF_IRO, DAC_CONF_ETS, DAC_CONF_B2C, DAC_CONF_OVR, DAC_CONF_CV, DAC_CONF_DDC);
  // Init encoders
  enc1.setInitConfig();
  // enc1.EncConfig.filterCount = ENC_FILTER_N;
  // enc1.EncConfig.filterSamplePeriod = ENC_FILTER_T_US;
  enc1.init();
  enc2.setInitConfig();
  // enc2.EncConfig.filterCount = ENC_FILTER_N;
  // enc2.EncConfig.filterSamplePeriod = ENC_FILTER_T_US;
  enc2.init();
  enc3.setInitConfig();
  // enc3.EncConfig.filterCount = ENC_FILTER_N;
  // enc3.EncConfig.filterSamplePeriod = ENC_FILTER_T_US;
  enc3.init();
  enc1.write(0);
  enc2.write(0);
  enc3.write(0);

  // Debug
  led_green = 0;
  led_blue = 0;
  led_red = 0;
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  set_leds(led_green,led_blue,led_red);
  

  f = 0.0;
  u[0] = 0.0f;
  u[1] = 0.0f;
  y[0] = 0.0f;
  y[1] = 0.0f;
  new_worst = 0;
  t_worst = read_flash(ADDR_WORST_CASE);
  // delay(2000);
  // Serial.print("Worst case (us) = ");
  // Serial.println(t_worst);
  // Init timer
  tim.begin(isr_tim, CTRL_TS*1e6);
}
#define START_CMD_M 12
#define STOP_CMD_M 42
#define INIT_CMD_M 13
#define ACTIVE_CMD_M 14
void loop()
{
  // 1) Process data
  if(flg)
  {
    uint32_t t1 = micros();
    flg = 0;
    if(com.data_valid())
    {
      com.read(); // Read data
      if(com._cmd == START_CMD_M) // Start cmd
      {
        led_green = 1;
        if(com._data == 3.14f) // Start data
        {
          led_blue = 1;
        }
        state = 1;
      }
      if(com._cmd == STOP_CMD_M) // Stop cmd
      {
        led_green = 0;
        if(com._data == 2.72f) // Start data
        {
          led_blue = 0;
          led_red = 0;
          state = 0;
        }
      }
      if(com._cmd == INIT_CMD_M) // Init cmd
      {
        led_red = 0;
        state = 2;
      }
      if(com._cmd == ACTIVE_CMD_M) // Init cmd
      {
        led_red = 1;
        state = 3;
        u[1] = u[0];
        u[0] = com._data;
      }
    }
    //set_leds(led_green,led_blue,led_red);
    switch(state)
    {
      case 0: // OFF
        if(new_worst)
        {
          new_worst = 0;
          write_flash(ADDR_WORST_CASE, t_worst);
        }
        break;
      case 1: // Start signal recieved, wait time
        break;
      case 2: // Init command recieved, waiting for RT sync
        break;
      case 3: // Now active
        uint32_t t = micros();
        y[1] = y[0];
        y[0] = sin(2*M_PI*t*1e-6);
        com.write(2,sin(2*M_PI*t*1e-6),u[0],u[1]);
        t_p = t;
        break;
    }
    t1 = t1 - micros(); // Calculate delay
    if(t1 > t_worst)
    {
      new_worst = 1;
      t_worst = t1;
    }
  }
}


void serialEvent()
{
  com.rx_ready();
}

