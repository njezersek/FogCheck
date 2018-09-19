float izmeriTemperaturo(){
  return dht.readTemperature(); 
}

float izmeriVlago(){
  return dht.readHumidity(); 
}

float izmeriTlak(){
  return bmp.getPressure(); 
}

float izracunTockeRosisca() {
  float  rosa_k1 = 0.0;
  float  rosa_k2 = 0.0;
  if (temp < 0) {
    rosa_k1 = 17.966;
    rosa_k2 = 247.15;
  } else {
    rosa_k1 = 17.368;
    rosa_k2 = 238.88;
  }

  float rosa = (rosa_k2 * (log(vlaga / 100) + (rosa_k1 * temp) / (rosa_k2 + temp))) / (rosa_k1 - (log(vlaga / 100) + (rosa_k1 * temp) / (rosa_k2 + temp)));
  return rosa;
}

bool preveriMegla(){
  if(vlaga >= 90){
    return true;
  } else {
    return false;
  }
}

