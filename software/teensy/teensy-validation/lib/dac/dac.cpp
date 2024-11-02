#include "dac.h"

Dac::Dac()
{
  
}

void Dac::begin(SPIClass *spi_port, uint8_t dev_type, uint8_t pin_nsync, uint8_t pin_nldac, uint8_t pin_nreset, uint8_t pin_nclear, uint8_t pin_nalert)
{
  this->_spi_port = spi_port;
  this->_spi_port->begin();
  this->_spi_port->setClockDivider(SPI_CLOCK_DIV);
  this->_spi_port->setDataMode(SPI_MODE);
  this->_config_valid = 0;
  this->_dev_type = dev_type;
  if(dev_type != AD5721 || dev_type != AD5721R || dev_type != AD5761 || dev_type != AD5761R)
  {
    this->_error = ERR_INVALID_DEVICE;
  }
  this->_pin_nsync = pin_nsync;
  this->_pin_nldac = pin_nldac;
  this->_pin_nreset = pin_nreset;
  this->_pin_nclear = pin_nclear;
  this->_pin_nalert = pin_nalert;
}
// Encodes control register paramaters into a 16 bit word
uint16_t Dac::encode_ctrl()
{  
  return (this->_ra & 0x0007) | ((this->_pv << 3) & 0x0018) | ((this->_iro << 5) & 0x0020) | ((this->_ets << 6) & 0x0040) | ((this->_b2c << 7) & 0x0080) | ((this->_ovr << 8) & 0x0100) | ((this->_cv << 9) & 0x0600);        
}
// Reads / writes the device. Pin #LSYNC is strobed low during the read
void Dac::transfer()
{
  digitalWrite(this->_pin_nsync, LOW); 
  delayMicroseconds(DEL_NSYNC_US); // Small delay
  this->_bfr[0] = this->_spi_port->transfer(this->_bfr[0]);
  this->_bfr[1] = this->_spi_port->transfer(this->_bfr[1]);
  this->_bfr[2] = this->_spi_port->transfer(this->_bfr[2]);
  delayMicroseconds(DEL_NSYNC_US); // Small delay
  digitalWrite(this->_pin_nsync, HIGH);
  delayMicroseconds(DEL_NSYNC_US); // Small delay
}
// Issues a control register readback command.
// Function will return false if first byte recieved does not match
bool Dac::read_config()
{
  // Readback control register
  // 1) Send readback control register command
  this->_bfr[0] = CMD_R_CTRL_REG; // Readback control register command
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x00; // Dummy value
  this->transfer();
  // 2) Read value of control register
  this->_bfr[0] = 0x00; // NOP command
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x00; // Dummy value
  this->transfer();
  if(this->_bfr[0] != CMD_R_CTRL_REG){ // First byte (command) recieved must be same as send
    this->_config_valid = 0;
    this->_error = ERR_COMMAND_RECIEVED;
    return false;
  }
  this->_config_valid = 1;
  this->_ra = this->_bfr[2] & 0x7;
  this->_pv = this->_bfr[2] >> 3 & 0x3;
  this->_iro = this->_bfr[2] >> 5 & 0x1;
  this->_ets = this->_bfr[2] >> 6 & 0x1;
  this->_b2c = this->_bfr[2] >> 7 & 0x1;
  this->_ovr = this->_bfr[1] & 0x1;
  this->_cv = this->_bfr[1] >> 1 & 0x3;
  this->_bo = (this->_bfr[1] >> 3) & 0x1;
  this->_sc = (this->_bfr[1] >> 4) & 0x1;
  return true;
}

// Sets the output voltage to settings stored in CV[1:0] by strobing pin #CLEAR
void Dac::hw_clear_output()
{
  digitalWrite(this->_pin_nclear, LOW);
  delayMicroseconds(DEL_NSYNC_US);
  digitalWrite(this->_pin_nclear, HIGH);
}
// Puts device into power-up reset state by strobing #RESET pin
void Dac::hw_reset()
{
  digitalWrite(this->_pin_nreset, LOW);
  delayMicroseconds(DEL_NSYNC_US);
  digitalWrite(this->_pin_nreset, HIGH);
  this->_error = NO_ERR;
  this->_config_valid = 0;
}
// Sets the output voltage to settings stored in CV[1:0] by writing command 0111
void Dac::sw_clear_output()
{
  this->_bfr[0] = CMD_SW_DATA_RST;
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x00; // Dummy value
  this->transfer();
}
// Puts the device into power-up reset state by writing command 1111
void Dac::sw_reset()
{
  this->_bfr[0] = CMD_SW_FULL_RST;
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x00; // Dummy value
  this->transfer();
  this->_error = NO_ERR;
  this->_config_valid = 0;
}

// Disable's daisy chain feature
// NOTE: This disables the SDO pin completely, therefore no readback will be possible.
//       To re-enable the daisy chain feature and readback functionality, a full power cycle
//       is needed thus only a disable function exists. Daisy chain feature is on enabled
//       default.
void Dac::disable_daisy_chain()
{
  this->_bfr[0] = CMD_SET_RESET_DAISY_CHAIN;
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x01; // Dummy value
  this->transfer();
}

// Initializes device with parameters provided in function arguments
bool Dac::init_device(uint8_t ra, uint8_t pv, uint8_t iro, uint8_t ets, uint8_t b2c, uint8_t ovr, uint8_t cv, uint8_t ddc)
{
  // Issue control register write with supplied configuration (includes software reset before)
  this->write_config(ra, pv, iro, ets, b2c, ovr, cv);

  // First readback config register
  if(!this->read_config()) // Check if command send matches the one recieved
  {
    return false;
  }
  // Then check if written equals read
  if(!is_params_equal(ra, pv, iro, ets, b2c, ovr, cv))
  {
    this->_error = ERR_PARAM_MISMATCH;
    return false;
  }
  if(this->_bo || this->_sc){
    this->_error = ERR_BROWNOUT_OR_SHORT; // Pretty critical
    return false;
  }

  // Daisy chain feature
  // TODO: Disable this in final version. In case of alert use hw / sw reset to determine fault
  if(ddc & 0xFE){ // ddc is 1 bit wide
    this->_error = ERR_INVALID_PARAM;
    return false;
  }
  this->_ddc = ddc;
  if(this->_ddc == DDC_DISABLED)
  {
    this->disable_daisy_chain();
  }
  return true;
}

// Debug function that returns the contents of the ith byte of the SPI buffer 
uint8_t Dac::get_buffer(uint8_t i)
{
  return this->_bfr[i];
}
// Prints the current configuration
// NOTE: Also useful to check if the config is actually loaded into the DAC
void Dac::print_config()
{
  Serial.println("|     AD5721 config:    |");
  Serial.println("|-----------------------|");
  if(this->_config_valid)
  {
    Serial.print("-> RA [2:0] = "); Serial.println(this->_ra,BIN);
    Serial.print("-> PV [1:0] = "); Serial.println(this->_pv,BIN);
    Serial.print("-> IRO      = "); Serial.println(this->_iro,BIN);
    Serial.print("-> ETS      = "); Serial.println(this->_ets,BIN);
    Serial.print("-> B2C      = "); Serial.println(this->_b2c,BIN);
    Serial.print("-> OVR      = "); Serial.println(this->_ovr,BIN);
    Serial.print("-> CV [1:0] = "); Serial.println(this->_cv,BIN);
    Serial.print("-> BO       = "); Serial.println(this->_bo,BIN);
    Serial.print("-> SC       = "); Serial.println(this->_sc,BIN);
  }else{
    Serial.println("! CONFIG IS INVALID !");
  }
  Serial.println("|-----------------------|");
}
// Writes both input and output register
// NOTE: The state of #LDAC is ignored for this command
void Dac::write_output(uint16_t in)
{
  this->_bfr[0] = CMD_W_DAC_REG;
  this->_bfr[1] = (in >> 4) & 0x00FF;
  this->_bfr[2] = (in & 0x000F) << 4;
  this->transfer();
} 

bool Dac::is_params_equal(uint8_t ra, uint8_t pv, uint8_t iro, uint8_t ets, uint8_t b2c, uint8_t ovr, uint8_t cv)
{
  if(this->_ra != ra){
    return false;
  }
  if(this->_pv != pv){
    return false;
  }
  if(this->_iro != iro){
    return false;
  }
  if(this->_ets != ets){
    return false;
  }
  if(this->_b2c != b2c){
    return false;
  }
  if(this->_ovr != ovr){
    return false;
  }
  if(this->_cv != cv){
    return false;
  }
  return true;
}


// Checks if the parameters (bit fields) for the control register are valid (can be determined by size alone)
bool Dac::is_params_valid(uint8_t ra, uint8_t pv, uint8_t iro, uint8_t ets, uint8_t b2c, uint8_t ovr, uint8_t cv)
{
  if(ra & 0xF8){ // ra is 3 bits wide
    this->_error = ERR_INVALID_PARAM;
    return false;
  }
  if(pv & 0xFC){ // pv is 2 bits wide
    this->_error = ERR_INVALID_PARAM;
    return false;
  }
  if(iro & 0xFE){ // iro is 1 bits wide
    this->_error = ERR_INVALID_PARAM;
    return false;
  }
  if(ets & 0xFE){ // ets is 1 bit wide
    this->_error = ERR_INVALID_PARAM;
    return false;
  }
  if(b2c & 0xFE){ // b2c is 1 bit wide
    this->_error = ERR_INVALID_PARAM;
    return false;
  }
  if(ovr & 0xFE){ // ovr is 1 bit wide
    this->_error = ERR_INVALID_PARAM;
    return false;
  }
  if(cv & 0xFC){ // cv is 2 bits wide
    this->_error = ERR_INVALID_PARAM;
    return false;
  }
  return true;
}
// Writes a full software reset (required for reconfiguration of output range) followed by a write to the control register
// NOTE: Does not readback the control register for validation
bool Dac::write_config(uint8_t ra, uint8_t pv, uint8_t iro, uint8_t ets, uint8_t b2c, uint8_t ovr, uint8_t cv)
{
  // Check parameters
  if(!this->is_params_valid(ra, pv, iro, ets, b2c, ovr, cv)){
    return false;
  }
  this->_ra = ra;
  this->_pv = pv;
  if(this->_dev_type == AD5721 || this->_dev_type == AD5761)
  {
    this->_iro = 0; // No internal reference for this type. In this case DB5 is required to be 0
  }else
  {
    this->_iro = iro;
  }
  this->_ets = ets;
  this->_b2c = b2c;
  this->_ovr = ovr;
  this->_cv = cv;
  this->sw_reset();
  uint16_t tmp = this->encode_ctrl();
  this->_bfr[0] = CMD_W_CTRL_REG; 
  this->_bfr[1] = (tmp >> 8) & 0x00FF;
  this->_bfr[2] = tmp & 0x00FF;
  this->transfer();
  return true;
}
// Debug function that prints the contents of the SPI buffer
void Dac::print_buffer()
{
  Serial.println("Buffer contents:");
  Serial.print("buf[0] = "); Serial.print(this->get_buffer(0),HEX); Serial.print("\t");
  Serial.print("buf[1] = "); Serial.print(this->get_buffer(1),HEX); Serial.print("\t");
  Serial.print("buf[2] = "); Serial.println(this->get_buffer(2),HEX);
}
// Performs a readback of the input register
uint16_t Dac::read_input_reg()
{
  // 1) Send readback input register command
  this->_bfr[0] = CMD_R_INPUT_REG; // Readback input register command
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x00; // Dummy value
  this->transfer();
  // 2) Read value of register
  this->_bfr[0] = 0x00; // NOP command
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x00; // Dummy value
  this->transfer();
  if(this->_bfr[0] != CMD_R_INPUT_REG) // First byte (command) recieved must be same as send
  {
    this->_error = ERR_COMMAND_RECIEVED;
    return 0xFFFF;
  }
  uint16_t out = 0x0000;
  if(this->_dev_type == AD5761R || this->_dev_type == AD5761) // 16 bit register encoding
  {
    out = ((this->_bfr[1] << 8) & 0xFF00) | (this->_bfr[0] & 0x00FF); 
  }else                                                       // 12 bit register encoding
  {
    out = ((this->_bfr[1] << 4) & 0x0FF0) | ((this->_bfr[0] >> 4) & 0x000F);
  }
  return out;
}
// Performs a readback of the output register
uint16_t Dac::read_output_reg()
{
  // 1) Send readback output register command
  this->_bfr[0] = CMD_R_DAC_REG; // Readback output register command
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x00; // Dummy value
  this->transfer();
  // 2) Read value of register
  this->_bfr[0] = 0x00; // NOP command
  this->_bfr[1] = 0x00; // Dummy value
  this->_bfr[2] = 0x00; // Dummy value
  this->transfer();
  if(this->_bfr[0] != CMD_R_DAC_REG)
  {
    this->_error = ERR_COMMAND_RECIEVED;
    return 0xFFFF;
  }
  uint16_t out = 0x0000;
  if(this->_dev_type == AD5761R || this->_dev_type == AD5761) // 16 bit register encoding
  {
    out = ((this->_bfr[1] << 8) & 0xFF00) | (this->_bfr[0] & 0x00FF); 
  }else                                                       // 12 bit register encoding
  {
    out = ((this->_bfr[1] << 4) & 0x0FF0) | ((this->_bfr[0] >> 4) & 0x000F);
  }
  return out;
}

// Debug function that returns current error and resets the error status
uint8_t Dac::get_error()
{ 
  uint8_t error = this->_error;
  this->_error = NO_ERR;
  return error;
}

// Returns current DAC fault status
// NOTE: SC and BO bits are reset after a write to the control register.
uint8_t Dac::get_fault()
{
  uint8_t alert = !digitalRead(this->_pin_nalert);
  if(!alert) // No alert, no fault
  {
    return NO_FAULT;
  }
  if(this->_ddc == DDC_DISABLED) // In this case full reset is needed to reenable daisy chain feature
  {
    return FAULT_NO_READBACK;
  }
  // Some fault present, try to readback config register
  this->_error = NO_ERR; // Clear error
  this->read_config();   // Check serial communication
  if(this->_error == ERR_COMMAND_RECIEVED) // The serial communication is broken. Thus the digital circuit is turned off.
  {
    return FAULT_DIE_TEMP; // Could also be due to something different if ETS is not enabled
  }
  // Serial communication OK, then check for brownout or short circuit condition
  if(this->_sc && this->_bo)
  {
    return FAULT_BROWNOUT_AND_SHORT; 
  }
  if(this->_sc && !this->_bo)
  {
    return FAULT_SHORT;
  }
  if(!this->_sc && this->_bo)
  {
    return FAULT_BROWNOUT;
  }
  return NO_FAULT; // Dummy value
}

