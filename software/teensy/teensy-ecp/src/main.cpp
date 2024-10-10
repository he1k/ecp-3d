#include <Arduino.h>
#include <SPI.h>
#include "constants.h"
#include "QuadEncoder.h"
#include "dac.h"
#include "bridge.h"
// QuadEncoder enc3(1, 0, 1, 0);
QuadEncoder enc3(CHA_ENC3, PIN_ENC3_A, PIN_ENC3_B, 0);
// QuadEncoder enc2(2, 3, 2, 0);
QuadEncoder enc2(CHA_ENC2, PIN_ENC2_A, PIN_ENC2_B, 0);
//QuadEncoder enc1(3, 8, 7, 0);
QuadEncoder enc1(CHA_ENC1, PIN_ENC1_A, PIN_ENC1_B, 0);

IntervalTimer tim;

Dac dac;

Bridge com;

bool res;
uint8_t flg = 0;
uint8_t ra = RA_PM_10;
uint8_t pv = PV_MID_SCALE;
uint8_t iro = IRO_ENABLED;
uint8_t ets = ETS_ENABLED;
uint8_t b2c = B2C_BINARY;
uint8_t ovr = OVR_DISABLED;
uint8_t cv = CV_MID_SCALE;
uint8_t ddc = DDC_ENABLED;

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

void isr_tim()
{
  flg = 1;
}

void ramp()
{
  dac.write_output(0);
  delay(1);
  dac.write_output(1024);
  delay(1);
  dac.write_output(2048);
  delay(1);
  dac.write_output(3072);
  delay(1);
  dac.write_output(4095);
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
  res = dac.init_device(ra, pv, iro, ets, b2c, ovr, cv, ddc);
  // delay(2000);
  // dac.print_config();
  // delay(100);
  // dac.read_config();
  // dac.print_config();
  // delay(1000);
  // dac.read_config();
  // dac.print_config();

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

  // Init timer
  tim.begin(isr_tim, CTRL_TS*1e6);
}
#define LOOP_DELAY 500
uint16_t out = 0;
float f = 0;
#define STATE_OFF 0
#define STATE_ON 1
uint8_t state = STATE_OFF;
uint32_t cnt = 0;
uint32_t m_p = 0;
uint32_t t_p = 0;
uint8_t set = 0;
void loop()
{
  if(flg)
  {
    flg = 0;
    cnt++;
    switch(com.read_command())
    {
      case CMD_NONE:
        break;
      case CMD_START:
        t_p = micros();
        state = STATE_ON;
        break;
      case CMD_STOP:
        set = 0;
        state = STATE_OFF;
        break;
    }
    if(state == STATE_OFF)
    {
      dac.sw_clear_output();
    }else if(state == STATE_ON)
    {
      if(com.data_valid())
      {
        set = 1;
        digitalWrite(PIN_DEBUG, HIGH);
        f = com.read_data();
        if(f > TORQUE_LIM)
        {
          f = TORQUE_LIM;
        }else if(f < -TORQUE_LIM)
        {
          f = -TORQUE_LIM;
        }
        int32_t u = 2048 + f*2048.0/TORQUE_MAX;
        if(u > 4095)
        {
          u = 4095;
        }else if(u < 0)
        {
          u = 0;
        }
        //dac.write_output((uint16_t)u);
        uint32_t m = micros();
        //com.write_encoder(((float)enc1.read())*C2D,((float)enc2.read())*C2D,((float)enc3.read())*C2D);
        // com.write_encoder(f,m-m_p,(float)Serial.available());
        //Serial.print(f,6);
        //Serial.print("\n");
        m_p = m;
      }
      if(set){
        com.write_encoder(f,0.0,(float)Serial.available());
      }
    }
  }
}


void serialEvent()
{
  com.rx_ready();
}