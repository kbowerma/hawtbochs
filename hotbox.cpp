#include "application.h"
#include "lib/OneWire/OneWire.h"
#include "lib/SparkDallas/spark-dallas-temperature.h"
#include "lib/SparkFun_MAX17043_Particle_Library/firmware/SparkFunMAX17043.h"
#include "hotbox.h"



void setup()
{
	Serial.begin(9600); // Start serial, to output debug data

//FuelGague
	Particle.variable("voltage", voltage);
  	Particle.variable("soc", soc);
  	Particle.variable("alert", alert);
  	Particle.variable("file", FILENAME);
    Particle.variable("version", MYVERSION);
  	// To read the values from a browser, go to:
  	// http://api.particle.io/v1/devices/{DEVICE_ID}/{VARIABLE}?access_token={ACCESS_TOKEN}

	lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge
  	lipo.quickStart(); // Quick start restarts the MAX17043 in hopes of getting a more accurate guess for the SOC.
    lipo.setThreshold(20); // Set alert threshold to 20%.

}

void loop()
{

	voltage = lipo.getVoltage();

	soc = lipo.getSOC();

	alert = lipo.getAlert();

	// Those variables will update to the Spark Cloud, but we'll also print them
	// locally over serial for debugging:
	Serial.print("Lucky Voltage: ");
	Serial.print(voltage);  // Print the battery voltage
	Serial.println(" V");

	Serial.print("Lucky Alert: ");
	Serial.println(alert);

	Serial.print("Lucky Percentage: ");
	Serial.print(soc); // Print the battery state of charge
	Serial.println(" %");
	Serial.println();

	delay(500);
}
