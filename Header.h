#define DoINFO // comment out to disable INFORMATION debug
#define DoBUG // comment out to disable Serial
#define DoOTA // comment out to disable OTA 192.168.0.120
#define DoSEND // comment out to disable Sending to ThingSpeak and ThingsBoard
// for ThingSpeak
unsigned long myChannelNumber = 700467; // Test // entre your channel's Number $$Original 700467 
const char * myWriteAPIKey = "BIDVZY1NGF76ZWK8"; // Test // enter your channel's Write API Key $$Original "BIDVZY1NGF76ZWK8"

#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "src/WiFiManager/WiFiManager.h"
#include "src/TimeAlarms/TimeAlarms.h"
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ThingSpeak.h>
#include <ThingsBoard.h>


WiFiClient wifiClient;
WiFiClient speakClient;
ThingsBoard tb(wifiClient);

// Internal Temperature sensor setup
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

boolean elementState = LOW, pumpState = LOW, holiday;

void chooseAlarm();
void tbRPC();
  
class Tasks{
public:
  AlarmID_t alarmID[255];

  void buildAlarm();
  void ElementOn();
  void ElementInput(int _ItemOff, int _IelementPeriod);
  void ElementOff();
  void clearAlarms();
  void alarmWeekday(int _hours, int _min, int _temp, int _dur);
  void alarmWeekend(int _hours, int _min, int _temp, int _dur);
  timeDayOfWeek_t getDOW(String _dow);
  String splitString(String data, char separator, int index);
  int getTimeTB(String _input, int _wHalf);
};

class FileSystem{
public:
  void buildSPIFFS();
  void saveJSON(StaticJsonDocument<2048> doc, String _path );
  void readJSON(bool printatt);
};

class Device{
public:
  String Time[10], Day[10], targetFwUrl, targetFwVer;
  int Temp[255], Dur[255];

  void updateSettings(StaticJsonDocument<2048> doc);
  void receiveAttribute(String _payload);
  void printSettings();

};

class Sensor{
public:
  Sensor();
  void resetVariables();
  void getValues();
  void controlActuators();
  void controlPump();
  float readADC(byte pin);
  double analogCalibration(double adcRaw);
  float ThermistorGlass100k(float ADCvalue, float Rdiv);
  float solarV(float ADCvalue, float Rup, float Rlow);
  float ldrLux10k(float ADCvalue, float Rdiv);
  void unknown();
  
private:
  const int luxLimit = 100; // Lux treashold when element will turn ON
  const byte nn = 20; // 20, Averaging - number of iterations in forLoop
  byte l = 0, m = 0; // used for collection and sending timing
  boolean lastpumpState  = LOW;
  // main veriables
  float adcA0 = 0, adcA1 = 0, adcA2 = 0, adcA3 = 0, adcA4 = 0, adcA5 = 0, adcA6 = 0, adcA7 = 0, adcA8 = 0, adcA9 = 0;
  float tempA0 = 0, tempA1 = 0, tempA2 = 0, tempA3 = 0, tempA4 = 0, tempA5 = 0, tempA6 = 0, tempA7 = 0, luxA8 = 0;
  float voltA9 = 0, intTemp = 0, hours = 0; // pumpState=0,
  int ADCA0 = 0, ADCA3 = 0, ADCA4 = 0, ADCA6 = 0;
  int luxPrevious = 0;
  int rssi;
  unsigned long for_millis = 0, send_millis = 0;
  
  float uee=0;
};

class Updater{
public:
  void getUpdate(String url);
  void cbOtaUpdate(t_httpUpdate_return ret);
};

class WFManager{
public:
  String token;
  int tankSize;
  int counter = 0;
  
  bool checkClientKeys();
  void buildWiFi(bool _autoconnect = true);
  void saveClientKeys();
  void readClientKeys();
};

FileSystem FileSystem;
Updater Updater;
Tasks Task;
Device Device;
Sensor Sensor;
WFManager WFManager;
