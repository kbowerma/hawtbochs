
//defines
#define FILENAME "hotbox"
#define MYVERSION "0.6.1_fan"
#define BUILD_DATE "2/26/2018"
#define ONE_WIRE_BUS D2
#define FAN1 TX
#define FAN2 RX
#define FAN1RELAY D3
#define FAN2RELAY D4
#define VALVE1RELAY 14 //A4
#define VALVE2RELAY 15 // A5

// PWM
//SystemCoreClock = 120MHz, period = 4000 @ 30Khz
#define PWM_FREQ 25000 // in Hertz (SET YOUR FREQUENCY)


//Globals
 double voltage = 0; // Variable to keep track of LiPo voltage
 double soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)
 bool alert; // Variable to keep track of whether alert has been triggered
 int deviceCount = 0; // Variable for Onewire devices
 String temps = "init";
 String myname = "unknown";
 double Vsource = 0;  // set Voltage for board input
 unsigned long ra_Interval = 300;  // intervall in seconds (ra = run at)
 unsigned long ra_lastTime = 0;
 uint32_t lux = 0; //TSL2561






//Prototypes
 int getDeviceCount();
 void printAddress(DeviceAddress deviceAddress);
 int queryDevices(String command);
// int relayFunc(String command);
 int moson(String command);
 int mosoff(String command);
 int mostoggle(String command);
 void getTempHandler(void);
 int cloudRestFunction(String command);
 void colorLed(int mydelay);
// void blinkLed(void);
 void displayTLS2561SensorDetails(void);
 void configureTSL2516Sensor(void);
 int setPWMFAN1(String command);
 int setPWMFAN2(String command);
 int setUniversalPWMFAN(String command);





//Declarations
 OneWire oneWire(ONE_WIRE_BUS);
 DallasTemperature sensor(&oneWire);
 TCPClient client;  //thingspeak
 Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
// IntervalTimer myTimer;


DeviceAddress deviceIndexArray[5];  // Dyanamic array,   I think I only use it for the print
 DeviceAddress ta1 = { 0x28, 0xB6, 0x98, 0x58, 0x06, 0x00, 0x00, 0x37 };
 DeviceAddress tb1 = { 0x28, 0xED, 0x33, 0xDD, 0x06, 0x00, 0x00, 0xFA };
 DeviceAddress td1 = { 0x28, 0xC8, 0xA0, 0xDE, 0x06, 0x00, 0x00, 0xD6 };
 DeviceAddress te1 = { 0x28, 0xFA, 0xD1, 0x3A, 0x06, 0x00, 0x00, 0xB2 };
 DeviceAddress tc1 = { 0x28, 0xF5, 0x39, 0xDD, 0x06, 0x00, 0x00, 0xBD };
 DeviceAddress tf1 = { 0x28, 0xED, 0x48, 0xE3, 0x02, 0x00, 0x00, 0x24 };
 DeviceAddress* deviceAddressArray[6] = {  &ta1, &tb1, &tc1, &td1, &te1, &tf1 };
 String deviceName[6] = {"a1","b1","c1","d1","e1", "f1"};
 double deviceTemp[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //initialize device tempartures
