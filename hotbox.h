
//defines
#define FILENAME "hotbox"
#define MYVERSION "0.2.3/1.22.18"
#define ONE_WIRE_BUS D2


//Globals
 double voltage = 0; // Variable to keep track of LiPo voltage
 double soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)
 bool alert; // Variable to keep track of whether alert has been triggered
 int deviceCount = 0; // Variable for Onewire devices
 String temps = "init";
// PINS
 int relay = D3;


//Prototypes
 int getDeviceCount();
 void printAddress(DeviceAddress deviceAddress);
 int queryDevices(String command);
 int relayFunc(String command);
 int relayOn(String command);
 int relayOff(String command);

//Declarations
 OneWire oneWire(ONE_WIRE_BUS);
 DallasTemperature sensor(&oneWire);
 

DeviceAddress deviceIndexArray[5];  // Dyanamic array,   I think I only use it for the print
 DeviceAddress ta1 = { 0x28, 0xB6, 0x98, 0x58, 0x06, 0x00, 0x00, 0x37 };
 DeviceAddress tb1 = { 0x28, 0xED, 0x33, 0xDD, 0x06, 0x00, 0x00, 0xFA };
 DeviceAddress td1 = { 0x28, 0xC8, 0xA0, 0xDE, 0x06, 0x00, 0x00, 0xD6 };
 DeviceAddress te1 = { 0x28, 0xFA, 0xD1, 0x3A, 0x06, 0x00, 0x00, 0xB2 };
 DeviceAddress tc1 = { 0x28, 0xF5, 0x39, 0xDD, 0x06, 0x00, 0x00, 0xBD };
 DeviceAddress* deviceAddressArray[5] = {  &ta1, &tb1, &tc1, &td1, &te1 };
 String deviceName[5] = {"a1","b1","c1","td1","e1"};
 double deviceTemp[5] = {0.0, 0.0, 0.0, 0.0, 0.0}; //initialize device tempartures
