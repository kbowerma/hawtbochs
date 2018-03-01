
# HawtBochs
A Particle photon implementation to keep a coldframe from over heating in direct sun.

[![](http://res.cloudinary.com/bowerman/image/upload/c_scale,w_750/v1519847342/DSC_0078.png)](http://res.cloudinary.com/bowerman/image/upload/v1519847342/DSC_0078.png)


# API

## Particle Functions: fan
***method: setUniversalPWMFAN***

```    
usage:   fan n_value
```

* n:  fan number 1 or 2
* value: 0 - 100
*  value = 0 = set pwm value to zero and close valve (relay)
*  value = 1 = set pwm to zero but leave valve open




pin | Function | pin | Function
----| ------- | ----| -------
VIN| - | 3v3 | -
GND| - | RST | -
Tx| - | VBAT | -
Rx| - | GND | -
WKP| - | D7 | blue led
DAC| - | D6 | MOSFET 4 __pruple__/  e62
A5| Relay 2  | D5 | MOSFET 3 __blue__ / e61
A4| Relay 1  | D4 | MOSFET 2 __green__ / e60
A3| VoltageMeasure (59)  | D3 | MOSFET 1 __yellow__ / e59
A2| PWM Fan1  | D2 | 1 wire ds18b20
A1| -  | D1 | SCL TSL2561
A0| -  | D0 | SDA TSL2561


## IN
Valve A4 (moson 14)
Fan D4  (moson 4)


## OUT

Valve A5 (moson 15 )
Fan D3

## Lux Reading benchmarks
LUMU | TSL2561 | YN216 Bars
-----|------|
297 | 256 | 0
302 | 262 | 0
1120 | 1062 | 1
2028 | 1950 | 2
2985 | 2857 | 3
3926 | 3736 | 4
5740 | 5522 | 6
7480 | 7161 | 8
6837 | 6644 | 9
16570 | 15016 | Flashlight

## TSL2561 pinouts

clr | PORT | PIN | addr | addr
----|------|-----|------|------|
brn | SDA | D0 |f6 | e56
grey | SCL |D1 |f7 |d57

## Valve pinouts
 f |color| 14 | 15
---- |-----|------|
Open |Red | 46 | 35
Close | blue | 44 | 33
Ground |Yellow | 39 | 28
|  |A4 |A5
|  ***ctl pin*** | 39 | 28

## PWM Fan assignments


spring pos | PIN | Jumper pos | colorLed | System
------| ---------|---------|-----------|----|
f38 | TX | 65 | PURPLE | FAN1
f37 | RX | 64 | BROWN | FAN2
f36 | WKP | 63| GREY | unused


## Mosfet Board
This board is used like a relay board offering 4 channels (NOT USED)

![](https://i.ebayimg.com/images/g/0~sAAOSwaSZZ5iq-/s-l225.jpg)
