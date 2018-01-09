//defines
#define FILENAME "hotbox"
#define MYVERSION "0.2.a"
#define ONE_WIRE_BUS D2


//Globals
 double voltage = 0; // Variable to keep track of LiPo voltage
 double soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)
 bool alert; // Variable to keep track of whether alert has been triggered
 int deviceCount = 0; // Variable for Onewire devices


//Prototypes
 int getDeviceCount();

//Declarations
 OneWire oneWire(ONE_WIRE_BUS);
 DallasTemperature sensor(&oneWire);
