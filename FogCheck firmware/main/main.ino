#include <TimerOne.h>
#include <arduinoFFT.h>
#include <FastLED.h>
#include <BME280I2C.h>
#include <Wire.h>


#define LED_TRANZISTOR_PIN  6
#define LED_PIN             7
#define LASER_PIN           8
#define PHOTORESISTOR_PIN   A0
#define BME_SDA_PIN         A4
#define BME_SCL_PIN         A5

// PODATKI ZA SERVER
#define ID 1                   //id merilne postaje
#define TOKEN "vegaFog456"      //geslo za povezavo s streznikom
#define MIN_FOG_VAL 0           //kalibracija
#define MAX_FOG_VAL 200 

#define INTERVAL_MERITVE   5000   //ms
#define INTERVAL_ANIMACIJE 100    //ms
#define INTERVAL_LASERJA   1      //ms 


//   ## INSTANCIATING & CONFIG ##
arduinoFFT FFT = arduinoFFT();
BME280I2C bme;

//led trak
#define NUM_LEDS   60
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];
int ledIndex = 0;

//   ## statusne spremenljivke ##
float megla = 0.0;
float temp  = 0.0;
float vlaga = 0.0;
float tlak  = 0.0;
bool laser = false;
bool opozorilo = false;


void setup(){
  delay(15000); // power-up safety delay
  
  //led trak
  FastLED.addLeds<WS2813, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  //laser
  pinMode(LASER_PIN, OUTPUT);
  pinMode(LED_TRANZISTOR_PIN, OUTPUT);

  // Laser interrupt
  Timer1.initialize(INTERVAL_LASERJA * 1000);
  Timer1.attachInterrupt(toggleLaser);

  // BME sensor
  Wire.begin();
  while(!bme.begin()){
    delay(1000);
  }

  //gsm
  Serial.begin(9600);
  gsmInit();
}

void loop(){
  if(millis() % INTERVAL_MERITVE == 0){
    bmeMeritve();
    megla = izmeriMeglo();

    String response = sendData(ID, TOKEN, megla, temp, vlaga, tlak);

    if(response.charAt(0) == '1'){
      opozorilo = true;  
    }
    else{
      opozorilo = false;  
    }
    /*
    if(megla > 50){
      opozorilo = true;
    }
    else {
      opozorilo = false;
    }*/
    //opozorilo = (response == "MEGLA") ? true : false; 
    opozorilo ? digitalWrite(LED_TRANZISTOR_PIN, HIGH) : digitalWrite(LED_TRANZISTOR_PIN, LOW);
  }
   
  if(millis() % INTERVAL_ANIMACIJE == 0){
    ledTrakOpozorilo();
  }
}
