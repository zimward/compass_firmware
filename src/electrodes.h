#ifndef _ELECTRODES_H
#define _ELECTRODES_H

#define ELEC_1 0
#define ELEC_2 1
#define ELEC_3 4
#define ELEC_4 5
#define ELEC_5 6
#define ELEC_6 7
#define ELEC_7 8
#define ELEC_8 9
#define ELEC_9 10
#define ELEC_10 11
#define ELEC_11 12
#define ELEC_12 13
#define ELEC_13 14
#define ELEC_14 15
#define ELEC_15 16
#define ELEC_16 17
#define ELEC_17 18
#define ELEC_18 19
#define ELEC_19 20
#define ELEC_20 21
#define ELEC_21 22
#define ELEC_22 23
#define ELEC_23 24
#define ELEC_24 25
#define ELEC_25 26
#define ELEC_26 27

byte getElectrode(uint16_t i){
  byte e = 26-(i*26)/360;
  return e < 2 ? e : e+2;
}

#endif
