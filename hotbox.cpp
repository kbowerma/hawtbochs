/* 1.8.2018 Hotbox Kyle Bowerman
* 0.1.0 Fuel Gauge from MAX17043_Simple
* 0.2.0 + ds18b20
*
*/

#include "application.h"
#include "lib/OneWire/OneWire.h"
#include "lib/SparkDallas/spark-dallas-temperature.h"
#include "lib/SparkFun_MAX17043_Particle_Library/firmware/SparkFunMAX17043.h"
#include "hotbox.h"



void setup()
{
	delay(2000);
	Serial.begin(9600); // Start serial, to output debug data
	sensor.begin();  // start onewire ds182b20

//FuelGague
	Particle.variable("voltage", voltage);
  	Particle.variable("soc", soc);
  	Particle.variable("alert", alert);
  	Particle.variable("file", FILENAME);
    Particle.variable("version", MYVERSION);
		Particle.variable("devices",deviceCount);
  	// To read the values from a browser, go to:
  	// http://api.particle.io/v1/devices/{DEVICE_ID}/{VARIABLE}?access_token={ACCESS_TOKEN}
	lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge
  	lipo.quickStart(); // Quick start restarts the MAX17043 in hopes of getting a more accurate guess for the SOC.
    lipo.setThreshold(20); // Set alert threshold to 20%.
	deviceCount =  getDeviceCount(); //Need to set the device Index Array at startup

}

void loop()
{

	voltage = lipo.getVoltage();
	soc = lipo.getSOC();
	alert = lipo.getAlert();



	delay(500);
}

int getDeviceCount() {
  //sensor.begin();
  deviceCount = sensor.getDeviceCount();
  return deviceCount;
}
