

## Mosfet Board
This board is used like a relay board offering 4 channels
![](https://images-na.ssl-images-amazon.com/images/I/81XHFEECRBL._SL1500_.jpg)

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
A2| -  | D2 | 1 wire ds18b20
A1| -  | D1 |
A0| -  | D0 |


## IN
Valve A4 (moson 14)
Fan D4  (moson 4)


## OUT

Valve A5 (moson 15 )
Fan D3

## Lux Reading
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
