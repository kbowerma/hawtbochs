/* 1.8.2018 Hotbox Kyle Bowerman
* 0.1.0 Fuel Gauge from MAX17043_Simple
* 0.2.0 + ds18b20
* 0.2.3 ad temp as particle variables
* 0.2.4 interval timer
* 0.2.5 change relay words to mosfet words.
* 0.3.0 added voltage divider on A3 to measure Vsource
* 0.3.1 Remoed intervall timer becuase it has too many restrictions
* 0.4.0 Added basic interval logic, and mostoggle, and logs on interval
* 0.4.2  add fanson,fansoff,open,close
* 0.5.1  adds thingspeak support and tries to fix not send when 32 degrees
* 0.5.2 TSL2516 basic understaning
* 0.5.3  change voltage divder to .199
* 0.5.4 Add more thingspeak calls, had to refactor retun outside of if
*       Moving around pub sub name,   and making thingspeak keys channels dynamic.
* 0.6.1 switches to a universal fan function
*/

#include "application.h"
 #include "lib/OneWire/OneWire.h"
 #include "lib/SparkDallas/spark-dallas-temperature.h"
 #include "lib/SparkFun_MAX17043_Particle_Library/firmware/SparkFunMAX17043.h"
 #include "lib/streaming/firmware/spark-streaming.h"
 #include "lib/thingspeak-particle/firmware/ThingSpeak.h"
 #include "lib/Adafruit_TSL2561/firmware/Adafruit_TSL2561_U.h"
 #include "bowerman.h"
 #include "hotbox.h"


void handler(const char *topic, const char *data) {
		myname =  String(data);
    Particle.publish(myname + "/system_version", System.version().c_str(), 60, PRIVATE);
    Particle.publish(myname + "/build_date", BUILD_DATE, 60, PRIVATE);
    if (myname == "liberty") {
      myWriteAPIKey = libertyWriteAPIKey;
      myChannelNumber = libertyChannelNumber;
    }
    if (myname == "LakeCreek") {
      myWriteAPIKey = LakeCreekWriteAPIKey;
      myChannelNumber = LakeCreekChannelNumber;
    }
    IPAddress myIP = WiFi.localIP();
    String ipStr = String(myIP[0])+"."+String(myIP[1])+"."+String(myIP[2])+"."+String(myIP[3]);
    Particle.publish(myname + "/LocalIP", ipStr, 60,PRIVATE);
    String myVersion = System.version().c_str();
    delay(2000);
    Particle.publish(myname + "/Version", myVersion, 60,PRIVATE);
    Particle.publish(myname + "/rssi", String( WiFi.RSSI()), 60, PRIVATE);
    Particle.publish(myname + "/SSID", String( WiFi.SSID()), 60, PRIVATE);
}


void setup()  {
	delay(2000);
	Serial.begin(9600); // Start serial, to output debug data
	sensor.begin();  // start onewire ds182b20

  // Partcile Variables and functions
	Particle.variable("voltage", voltage);
  	Particle.variable("soc", soc);
  	Particle.variable("alert", alert);
  	Particle.variable("file", FILENAME);
    Particle.variable("version", MYVERSION);
    Particle.variable("build_date", BUILD_DATE);
  	Particle.variable("devices",deviceCount);
    Particle.variable("temps",temps);
    Particle.variable("Vsource", Vsource);
    Particle.variable("lux", lux);
  	Particle.function("q", queryDevices);
    Particle.function("moson", moson) ;
    Particle.function("mosoff", mosoff);
    Particle.function("mostoggle",mostoggle);
    Particle.function("reset", cloudRestFunction);
    Particle.function("fan1", setPWMFAN1);
    Particle.function("fan2", setPWMFAN2);
    Particle.function("fan", setUniversalPWMFAN);

	lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge
    lipo.quickStart(); // Quick start restarts the MAX17043 in hopes of getting a more accurate guess for the SOC.
    lipo.setThreshold(40); // Set alert threshold to 20%.
    deviceCount = getDeviceCount();
    queryDevices("auto");
    Particle.publish("particle/device/name");
	  Particle.subscribe("particle/device/name", handler);

    // Particle.publish(myname + "/system_version", System.version().c_str(), 60, PRIVATE);
    // Particle.publish(myname + "/build_date", BUILD_DATE, 60, PRIVATE);

  //pins
  //int PWMFAN = A2;
  pinMode(D3, OUTPUT);
     pinMode(D4, OUTPUT);
     pinMode(D5, OUTPUT);
     pinMode(D6, OUTPUT);
     pinMode(D7, OUTPUT);  // built in LED
     pinMode(A4, OUTPUT);
     pinMode(A5, OUTPUT);
     pinMode(A3,INPUT_PULLDOWN);
     pinMode(FAN1, OUTPUT);  //PWM FAN 1
     pinMode(FAN2, OUTPUT);  //PWM FAN 2
     analogWrite(FAN1, 0);
     analogWrite(FAN2, 0);

  ThingSpeak.begin(client);


  if(!tsl.begin())
    {
      /* There was a problem detecting the ADXL345 ... check your connections */
      Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
      while(1);
    }
    displayTLS2561SensorDetails(); // Display some basic information on this sensor
    configureTSL2516Sensor(); // Setup the sensor gain and integration time

}

void loop() {

	voltage = lipo.getVoltage();
	soc = lipo.getSOC();
	alert = lipo.getAlert();
  // voltage divider  8.2 / (8.2 + 33 ) == .199
  Vsource =  ( analogRead(A3) * 3.3 /  (0.199 * 4095 )) ;

  //Interval Logic see:  https://community.particle.io/t/millis-and-rollover-tutorial/20429
  //TODO add this to a runAt handler Function
  unsigned long now = millis();
  if ( now - ra_lastTime >= 1000*ra_Interval ) {
    queryDevices("t");
    queryDevices("v");
    queryDevices("lux");
    Particle.publish(myname + "/battery/voltage",String(voltage));
    Particle.publish(myname + "/battery/soc",String(soc));
  //  Particle.publish(myname + "/lux/",String(lux));
    Particle.publish(myname + "/loop",String(millis()/1000));
    ra_lastTime = now;
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  }

  sensors_event_t event;
  tsl.getEvent(&event);
  lux = event.light;
	// delay(500);
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
    int myreturn = 0;
    if(command == "auto" || command == "") {
        for(int i=0; i < deviceCount; i++ ) { // sets and prints the device array
            sensor.getAddress(deviceIndexArray[i], i);
            Serial.print("Device Index ");
            Serial.print(i);
            printAddress(deviceIndexArray[i]);
            Serial.print("\n");
        }
        Serial.print("--------------------------------------\n");
        myreturn = deviceCount;
    }
		if(command == "t") {
       temps = "";  // reset the variable to empty
			sensor.requestTemperatures();

			for(int i=0; i < 6; i++ ) {
				deviceTemp[i] = sensor.getTempF(*deviceAddressArray[i]);
				if ( deviceTemp[i] > 0 && deviceTemp[i] != 32) {
        //  String thistemp = String(deviceTemp[i])
				  Serial << "Device " << deviceName[i] << " temp " <<  deviceTemp[i] << endl;
					Particle.publish(myname + "/" + deviceName[i]+"/temp", String(deviceTemp[i]));
          temps.concat(String(deviceTemp[i]).substring(0,4));
          temps.concat(" ");
          colorLed(1000);
          ThingSpeak.setField(i+1,String(deviceTemp[i]));
				}


			}
			Serial.print("--------------------------------------\n");
      //if (temps != "") ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
			myreturn = deviceCount;

		}
    if( command == "now" )  myreturn = Time.now();
    if( command == "uptime" ) myreturn = millis()/1000;
    if( command == "freq" ) myreturn = System.ticksPerMicrosecond();
    if( command == "v" )  {
       // Vsource = analogRead(A3);
      //Particle.publish(myname + "/" + deviceName[i]+"/temp", String(deviceTemp[i]));
      // Particle.publish(myname + "/Vin",String((Vsource*3.3/4095)/.20));
      Particle.publish(myname + "/Vs",String(Vsource));
      ThingSpeak.setField(8,String(Vsource));
    //  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      myreturn = int(Vsource*10);
    }
    if (command == "open") {
      moson("14"); //in vavle
      delay(3000);
      moson("15"); //out vavle
      myreturn = 1;
    }
    if (command == "close") {
      mosoff("14"); //in Fan
      delay(3000);
      mosoff("15"); //out fan
      myreturn = 1;
    }
    if (command == "fanson") {
      moson("4"); //in Fan
      delay(3000);
      moson("3"); //out fan
      myreturn = 1;
    }
    if (command == "fansoff" ){
      mosoff("4"); //in Fan
      delay(3000);
      mosoff("3"); //out fan
      myreturn = 1;
    }
    if (command == "lux") {
      Particle.publish(myname + "/lux/",String(lux));
      ThingSpeak.setField(7,String(lux));
      myreturn = lux;
    }
  //ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  return myreturn;

	}
int moson(String command) {
    Particle.publish(myname + "/relay/on",command);   //publish even to particle cloud
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
int mosoff(String command) {
    Particle.publish(myname + "/relay/off", command);
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
int mostoggle(String command) {
   Particle.publish(myname + "/relay/toggle", command);
   digitalWrite(command.toInt(),!digitalRead(command.toInt() ));
   return 1;
}
void getTempHandler(void) {
   queryDevices("t");
 }
int cloudRestFunction(String command) {
  System.reset();
  return 1;
}
void colorLed(int mydelay) {
  RGB.control(true);
  RGB.color(1,1,255);
  delay(mydelay);
  RGB.color(255,255,0);
  delay(mydelay);
  RGB.color(1,1,255);
  delay(mydelay);
  RGB.control(false);
}
void configureTSL2516Sensor(void) {
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */

  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}
void displayTLS2561SensorDetails(void) {
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
int setPWMFAN1(String command) {
  analogWrite(FAN1, command.toInt());
  delay(1000);
  //return analogRead(WKP);
  return command.toInt();
}
int setPWMFAN2(String command) {
  int fanpercent = map(command.toInt(),0,100,0,255);
  if ( fanpercent > 0 ) digitalWrite(FAN2RELAY, HIGH);
  analogWrite(FAN2, fanpercent);
  if ( fanpercent == 0 ) digitalWrite(FAN2RELAY, LOW);
  return fanpercent;
}
int setUniversalPWMFAN(String command) {
  //int myfan = command.charAt(0);  //49 for 1; 50 for 2
  int myfan, fanrelay, myvalue, valverelay, fanum = 0;
  int myfanvalue = command.substring(2).toInt();
   myvalue = map(myfanvalue,0,100,0,255);

  if ( command.charAt(0) == 49 ) {
     fanum = 1;
     myfan = FAN1;
     fanrelay = FAN1RELAY;
     valverelay = VALVE1RELAY;
  }
  if ( command.charAt(0) == 50 ) {
     fanum = 2;
     myfan = FAN2;
     fanrelay = FAN2RELAY;
     valverelay = VALVE2RELAY;
  }
  // turn off fan and close vavle
  if (myfanvalue == 0 ) {
    analogWrite(myfan, myvalue, 1000);  //speed frequency to 100 (50HZ default)
    digitalWrite(fanrelay, LOW);  // shutoff fan
    digitalWrite(valverelay, LOW);  // close vavle
    return 100 * myfan + myvalue;
  }
  // turn off fan and open valve
  if (myfanvalue == 1 ) {
    analogWrite(myfan, 0, 1000);  // 1 runs the fan still
    digitalWrite(valverelay, HIGH);  // open vavle
    digitalWrite(fanrelay, LOW);  // shutoff fan
    //return 100 * myfan + myvalue;
    return myvalue;
  }
  // open the vavle and set fan
  if (myfanvalue > 1 ) {
    analogWrite(myfan, myvalue, 1000);
    digitalWrite(fanrelay, HIGH);  // TURN on fan
    digitalWrite(valverelay, HIGH);  // open vavle
    return 100 * myfan + myvalue;
  }



   return -1;

  /*
  int fanpercent = map(command.toInt(),0,100,0,255);
  if ( fanpercent > 0 ) digitalWrite(FAN2RELAY, HIGH);
  analogWrite(FAN2, fanpercent);
  if ( fanpercent == 0 ) digitalWrite(FAN2RELAY, LOW);
  return fanpercent;
  */
}
