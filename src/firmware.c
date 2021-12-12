#define F_CPU 1000000UL //CPU Freq 1MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <TinyWireM.h>
#include <EEPROM.h>
#include <math.h>
#include "registers.h"
#include "electrodes.h"


#define EEPR_ADDR_CAL 0
#define EEPR_ADDR_OFF_X 1
#define EEPR_ADDR_OFF_Y 3
#define EEPR_ADDR_OFF_Z 5

//I2C functions
//OP = 1 READ; OP = 0 Write
constexpr uint8_t calcAddr(uint8_t baseAddr,uint8_t chipAddr){
  baseAddr |= chipAddr;
  return baseAddr;
}
constexpr uint8_t calcAddr(uint8_t chipAddr){
  return calcAddr(0x20,chipAddr);
}

void sendCmd(uint8_t addr,uint8_t reg, uint8_t value){
  TinyWireM.beginTransmission(addr);
  TinyWireM.write(reg);
  TinyWireM.write(value);
  TinyWireM.endTransmission();
}

//Reads 8-bit from specified register
uint8_t readCmd(uint8_t addr,uint8_t reg){
  TinyWireM.beginTransmission(addr);
  TinyWireM.write(reg);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(addr,1);
  return TinyWireM.read();
}

//MAG functions

inline void initMAG(){
  uint8_t addr=calcAddr(0xE,0x0);
  sendCmd(addr,MAG_CTRL_REG2,(1<<AUTO_MRST_EN));
  sendCmd(addr,MAG_CTRL_REG1,(1<<OS0)|(1<<AC)); //Set Sample Freq to 40 Hz with 32x OSR; Set chip into active mode
  //Offsets
  int16_t x;
  int16_t y;
  int16_t z;
  x=0;
  y=0;
  z=0;
  //Set the offsets of the MAG
  sendCmd(addr,MAG_OFF_X_MSB,x>>8);
  sendCmd(addr,MAG_OFF_X_LSB,x);
  
  sendCmd(addr,MAG_OFF_Y_MSB,y>>8);
  sendCmd(addr,MAG_OFF_Y_LSB,y);
  
  sendCmd(addr,MAG_OFF_Z_MSB,z>>8);
  sendCmd(addr,MAG_OFF_Z_LSB,z); 
}

void waitDR(){
  uint8_t addr = calcAddr(0xE,0x0);
  uint8_t dr=0;
  while((dr&((1<<XDR)|(1<<ZDR)|(1<<YDR)))!=((1<<XDR)|(1<<ZDR)|(1<<YDR))) {
    dr = readCmd(addr,MAG_DR_STATUS);    
    _delay_ms(5);
  }
}
/*
  Returns array of magnetic axies values X,Y,Z
*/
void readMAG(uint16_t* dat){
    uint8_t addr = calcAddr(0xE,0x0);
    waitDR();
    dat[0]=readCmd(addr,MAG_OUT_X_MSB)<<8;
    dat[0]|=readCmd(addr,MAG_OUT_X_LSB);
    dat[1]=readCmd(addr,MAG_OUT_Y_MSB)<<8;
    dat[1]|=readCmd(addr,MAG_OUT_Y_LSB);
    dat[2]=readCmd(addr,MAG_OUT_Z_MSB)<<8;
    dat[2]|=readCmd(addr,MAG_OUT_Z_LSB);
  //  dat[0]+=611;
  //  dat[1]+=501;
  //  dat[2]-=1722;
}

//MCP functions

inline void initMCP(){
  uint8_t addr = calcAddr(0);
  uint8_t addr2 = calcAddr(1);
  
  sendCmd(addr,MCP_IODIRA,0x0);
  sendCmd(addr,MCP_IODIRB,0x0);

  sendCmd(addr2,MCP_IODIRA,0x0);
  sendCmd(addr2,MCP_IODIRB,0x0);
}

void setMCPout(uint16_t m0,uint16_t m1){
  uint8_t addr = calcAddr(0);
  uint8_t addr2 = calcAddr(1);
  sendCmd(addr,MCP_GPIOA,m0>>8);
  sendCmd(addr,MCP_GPIOB,m0);
  
  sendCmd(addr2,MCP_GPIOA,m1>>8);
  sendCmd(addr2,MCP_GPIOB,m1);
}

void setMCPout(uint32_t value){
  setMCPout(value,value>>16);  
}

//Magnetometer calibration function
inline void calibr(){

}

inline void init(){
  cli();
  GIMSK |= (1<<PCIE);//Enable interrupts
  PCMSK |= (1<<PCINT4); //Set PB4 as interrupt trigger
  //PORTB |= (1<<PB4);  //enable pullup (setting output bit but leaving it as input)  
  
  /* Setup PWM modulators */
  //       PWM1A EN | Connect Pins to PWM   | Set Freq. Devider to 1/4 (265kHz eg. 1kHz PWM out)
  TCCR1 = (1<<PWM1A)|(1<<COM1A1)|(1<<COM1A0)|(1<<CS10)|(1<<CS11);//|(1<<CS12);
  OCR1A = 192;  // Sensory feedback Duty-Cyle = 25% (~250µS pulse Width)
  /* Setup of PWM modulators END */
  
  //Set PB1+PB3 as out
  DDRB |=(1<<PB1)|(1<<PB3)|(1<<PB4);
  _delay_ms(1);     //waiting while PB3 (mcp reset) is low
  PORTB |= (1<<PB3);//stop reseting mcp
  sei();
  TinyWireM.begin();
  initMAG();
  initMCP();
  if(!EEPROM.read(EEPR_ADDR_CAL)){
    calibr();
  }
}


int main(void){
  init();
  while(1){
    setMCPout(0xFFFF);
    PORTB |= 1 << PB4;
    _delay_ms(100);
    setMCPout(0x0);
    PORTB ^= ~(1<<PB4);
    _delay_ms(100);
  }
 // byte degres = 0;
 /* while(1){
      setMCPout(1<<getElectrode(degres++));
      degres%=360;
      _delay_ms(400);
  }*/
/*  while(1){
    int16_t *mag_field=readMAG();
    float heading = atan2(mag_field[0],mag_field[1]); // atan(x,y) -> north clockwise 
    uint16_t degr = (heading > 0 ? heading : (2*PI + heading)) * 360 / (2*PI);
    degr %= 360;
    byte electrode = getElectrode(degr);
    setMCPout(1<<electrode);
  }*/
  return 0;
}

//Interrupt handler
ISR(PCINT0_vect){
//  EEPROM.update(EEPR_ADDR_CAL,false); //Set calibrated byte to false
}
