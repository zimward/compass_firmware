//Definitions of I2C registers of the MCP23017 and MAG2771 chips
#ifndef _regs_h
#define _regs_h

#define MCP_IODIRA 0x0
#define MCP_IODIRB 0x1
#define MCP_GPIOA 0x12
#define MCP_GPIOB 0x13

//MAG3110 Registers
#define MAG_DR_STATUS 0x00
#define MAG_OUT_X_MSB 0x01
#define MAG_OUT_X_LSB 0x02
#define MAG_OUT_Y_MSB 0x03
#define MAG_OUT_Y_LSB 0x04
#define MAG_OUT_Z_MSB 0x05
#define MAG_OUT_Z_LSB 0x06
#define MAG_WHO_AM_I  0x07
#define MAG_SYSMOD    0x08
#define MAG_OFF_X_MSB 0x09
#define MAG_OFF_X_LSB 0x0A
#define MAG_OFF_Y_MSB 0x0B
#define MAG_OFF_Y_LSB 0x0C
#define MAG_OFF_Z_MSB 0x0D
#define MAG_OFF_Z_LSB 0x0E
#define MAG_DIE_TEMP  0x0F
#define MAG_CTRL_REG1 0x10
#define MAG_CTRL_REG2 0x11

//MAG3110 Data Ready Status bits for DR_STATUS Register
#define ZYXOW 7 // 1=Overwrite has occured in x,y or z axis; 0 = no overwrite
#define ZOW 6
#define YOW 5
#define XOW 4
#define ZYXDR 3 // 1= x,y or z axis ready; 0=no data ready
#define ZDR 2
#define YDR 1
#define XDR 0

//MAG3110 CTRL_REG1 bits
#define DR2 7 //Data Rate selection DR[2:0]
#define DR1 6
#define DR0 5
#define OS1 4 //Oversample ratio OS[1:0]
#define OS0 3
#define FR  2 //Fast Read 1 = Fast Read on:8 bit values are read; 0 = Full 16-bit read
#define TM  1 //Trigger immediate mesurement 0 = Normal operation 1 = Trigger Measurement if in standby chip will return to standby after measurement
#define AC  0 //0=Standby mode 1 = Active mode

//MAG3110 CTRL_REG2 bits
#define AUTO_MRST_EN 7 //Automatic Magnetic Sensor Reset (write only) 0 = disabled 1= enabled
#define RAW 5 // Data output correction 0 = normal mode:data values are correced 1= raw mode (factory calibration is always applied)
#define Mag_RST 4 //Magnetic Sensor reset (one-shot) 0 = reset cyle not active 1 = Reset cycle initiate

#endif

