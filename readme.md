

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
