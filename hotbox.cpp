/* 1.8.2018 Hotbox Kyle Bowerman
* 0.1.0 Fuel Gauge from MAX17043_Simple
* 0.2.0 + ds18b20
* 0.2.3 ad temp as particle variables
* 0.2.4 interval timer
*/

#include "application.h"
 #include "lib/OneWire/OneWire.h"
 #include "lib/SparkDallas/spark-dallas-temperature.h"
 #include "lib/SparkFun_MAX17043_Particle_Library/firmware/SparkFunMAX17043.h"
 #include "lib/streaming/firmware/spark-streaming.h"
 #include "lib/SparkIntervalTimer/src/SparkIntervalTimer.h"
 #include "hotbox.h"

String myname = "unknown";

void handler(const char *topic, const char *data) {
		myname =  String(data);
}

void setup()  {
	delay(2000);
	Serial.begin(9600); // Start serial, to output debug data
	sensor.begin();  // start onewire ds182b20


	Particle.variable("voltage", voltage);
  	Particle.variable("soc", soc);
  	Particle.variable("alert", alert);
  	Particle.variable("file", FILENAME);
    Particle.variable("version", MYVERSION);
  	Particle.variable("devices",deviceCount);
    Particle.variable("temps",temps);
  	Particle.function("q", queryDevices);
    Particle.function("relayon", relayOn) ;
    Particle.function("relayoff", relayOff);
  	Particle.function("relay", relayFunc);
    Particle.function("reset", cloudRestFunction);

	lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge
  	lipo.quickStart(); // Quick start restarts the MAX17043 in hopes of getting a more accurate guess for the SOC.
    lipo.setThreshold(40); // Set alert threshold to 20%.

 //Need to set the device Index Array at startup
  deviceCount = getDeviceCount();
  queryDevices("auto");
	Particle.subscribe("particle/device/name", handler);
	Particle.publish("particle/device/name");

  //pins
   pinMode(D3, OUTPUT);
     pinMode(D4, OUTPUT);
     pinMode(D5, OUTPUT);
     pinMode(D6, OUTPUT);
     pinMode(D7, OUTPUT);  // built in LED

// this is not working
//  myTimer.begin(getTempHandler,500000, uSec);  // 10 Million = 10 seoncds

}

void loop() {

	voltage = lipo.getVoltage();
	soc = lipo.getSOC();
	alert = lipo.getAlert();
	delay(500);
}

// Functions -----------------------------

int getDeviceCount() {
  //sensor.begin();
  deviceCount = sensor.getDeviceCount();
  return deviceCount;
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++)
  {
          Serial.print(" ");
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

int queryDevices(String command) {

    if(command == "auto" || command == "") {
        for(int i=0; i < deviceCount; i++ ) { // sets and prints the device array
            sensor.getAddress(deviceIndexArray[i], i);
            Serial.print("Device Index ");
            Serial.print(i);
            printAddress(deviceIndexArray[i]);
            Serial.print("\n");
        }
        Serial.print("--------------------------------------\n");
        return deviceCount;
    }
		if(command == "t") {
       temps = "";  // reset the variable to empty
			sensor.requestTemperatures();

			for(int i=0; i < 5; i++ ) {
				deviceTemp[i] = sensor.getTempF(*deviceAddressArray[i]);
				if ( deviceTemp[i] > 0 ) {
        //  String thistemp = String(deviceTemp[i])
				  Serial << "Device " << deviceName[i] << " temp " <<  deviceTemp[i] << endl;
					Particle.publish(myname + "/" + deviceName[i]+"/temp", String(deviceTemp[i]));
          temps.concat(String(deviceTemp[i]).substring(0,4));
          temps.concat(" ");

					RGB.control(true);
					RGB.color(1,1,255);
					delay(1000);
					RGB.color(255,255,0);
					delay(1000);
					RGB.color(1,1,255);
					delay(1000);
					RGB.control(false);
					//System.sleep(10); this will make it sleep
				}

			}
			Serial.print("--------------------------------------\n");
			return deviceCount;
		}
    if( command == "now" )  return Time.now();
    if( command == "uptime" ) return millis()/1000;
    if( command == "freq" ) return System.ticksPerMicrosecond();

	}

int relayFunc(String command) {
  if(command == "on" ) {
    digitalWrite(relay, HIGH);
    Particle.publish(myname +"/relay", "API on");
    return 1;
  }
  if(command == "off") {
		digitalWrite(relay, LOW);
    Particle.publish(myname + "relay", "API off");
    return 0;
  }

}

int relayOn(String command) {
    Particle.publish(myname + "/relay/on",command);   //publish even to particle cloud
  //  request.path = String("/device/create?type=event&desc=relay%20ON&name=raptor&data=" + command );
    // http.get(request, response, headers);
      // Particle.publish("DEBUG",  request.path);
      // Particle.publish("mresponse",  response.body); //DEBUG


    if(command != "all") {
        digitalWrite(command.toInt(), HIGH);
        return 1;
    }
    if(command == "all") {
        for (int i = 0; i < 8; i++ ) {
            digitalWrite(i, HIGH);
        }
        return 1;
    }
    else return -1;
}

int relayOff(String command) {
    Particle.publish(myname + "/relay/off", command);
  //  request.path = String("/device/create?type=event&desc=relay%20Off&name=raptor&data=" + command );
  //   http.get(request, response, headers);


    if(command != "all") {
        digitalWrite(command.toInt(), LOW);
        return 1;
    }
    if(command == "all") {
        for (int i = 0; i < 8; i++ ) {
            digitalWrite(i, LOW);
        }
        return 1;

    }
    else return -1;
}

void getTempHandler(void) {
   queryDevices("t");
 }

int cloudRestFunction(String command) {
  System.reset();
  return 1;
}
