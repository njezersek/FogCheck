#include <DHT.h> //https://github.com/adafruit/Adafruit_Sensor
#include <DHT_U.h>
#include <TimerOne.h>
#include "arduinoFFT.h"
#include <Adafruit_BMP183.h>
#include <SoftwareSerial.h>
#include <FastLED.h>

//   ## PINOUT ##
// used pins: 2,6,7,8,9,10,11,12,15
// DHT_PIN
#define DHT_PIN 2
// LED PINS
#define LED_PIN 6
#define LED_TRANZISTOR_PIN 5
// GSM PINS
#define GSM_RX_PIN 7
#define GSM_TX_PIN 8
// BMP 183 PINS
#define BMP183_CS  9
#define BMP183_SDI 10
#define BMP183_SDO 11
#define BMP183_CLK 12
// LASER PIN
#define LASER_PIN 4
// PHOTORESISTOR PIN
#define PHOTORESISTOR_PIN A0

//   ## INSTANCIATIN & CONFIG ##
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

Adafruit_BMP183 bmp = Adafruit_BMP183(BMP183_CLK, BMP183_SDO, BMP183_SDI, BMP183_CS);

//gsm modul
SoftwareSerial gsm(GSM_RX_PIN,GSM_TX_PIN); // RX, TX

//led trak
#define NUM_LEDS   60
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];
int ledIndex = 0;

const long intervalMeritve = 2000;  //ms
const long intervalAnimacije = 2000;  //ms
const long intervalLaserja = 1;  //ms

#define ID 19 //id merilne postaje
#define TOKEN "vegaFog456" //geslo za povezavo s streznikom
#define MIN_FOG_VAL 0    //kalibracija
#define MAX_FOG_VAL 200  //

//   ## statusne spremenljivke ##
float  megla = 0.0;
float  temp  = 0.0;
float  vlaga = 0.0;
float  tlak  = 0.0;
double rosa    = 0.0;
bool laser = false;
bool opozorilo = false;

//   ## SETUP ##
void setup(){
  delay(3000); // power-up safety delay
  //led trak
  FastLED.addLeds<WS2813, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  //serial debug
  Serial.begin(115200);
  Serial.println("test");
  //laser
  pinMode(LASER_PIN, OUTPUT);
  Timer1.initialize(intervalLaserja*1000);
  Timer1.attachInterrupt(toggleLaser);

  //gsm
  gsm.begin(9600);
  //gsmInit();
}

void loop(){
  if(millis() % intervalMeritve == 0){
    megla = izmeriMeglo();
    temp = izmeriTemperaturo();
    vlaga = izmeriVlago();
    tlak = izmeriTlak();
    rosa = izracunTockeRosisca();
    
    Serial.print("megla: "); 
    Serial.println(megla);
  
    Serial.print("temperatura: ");
    Serial.println(temp);
      
    Serial.print("vlaga: ");
    Serial.println(vlaga);
  
    Serial.print("tlak: ");
    Serial.println(tlak);
    
    Serial.print("rosa: ");
    Serial.println(rosa);

    /*Serial.println("--posiljanje na streznik");

    String response = sendData(ID, TOKEN, megla, temp, vlaga, tlak);
    Serial.println(" odgovor: "+response);
    if(response == "OK MEGLA")opozorilo = true;
    else opozorilo = false;*/
    
    Serial.println("------------");
  }
  if(millis() % intervalAnimacije){
    ledTrakOpozorilo();
  }
}
