float izmeriMeglo(){
  //konfiguracija fft megla senzorja
  const uint16_t samples = 64; //This value MUST ALWAYS be a power of 2
  const double samplingFrequency = 1000;
  unsigned int delayTime = 0;
  
  if(samplingFrequency<=1000)delayTime = 1000/samplingFrequency;
  else delayTime = 1000000/samplingFrequency;
  
  double vReal[samples];
  double vImag[samples];
  
  //kalibracija
  float minavg = MIN_FOG_VAL;
  float maxavg = MAX_FOG_VAL;

  for(uint16_t i =0;i<samples;i++){
    vReal[i] = double(analogRead(PHOTORESISTOR_PIN));
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
    if(samplingFrequency<=1000)
      delay(delayTime);
    else
      delayMicroseconds(delayTime);
  }

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);                  /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples);                    /* Compute magnitudes */
  /*for (uint16_t i = 0; i < (samples >> 1); i++){ // logiranje celotnega fft
    Serial.print(vReal[i], 4);
    Serial.print(",");
  }
  Serial.println();*/
  
  float avg = (vReal[29]+vReal[28]+vReal[27])/3;
  float percentage = floatMap(avg, minavg, maxavg, 100, 0);

  return percentage;
}

float floatMap(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
