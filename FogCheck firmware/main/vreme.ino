void bmeMeritve() {
    BME280::TempUnit tempUnit(BME280::TempUnit_Celcius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);

    bme.read(tlak, temp, vlaga, tempUnit, presUnit);
}

bool preveriMegla(){
  if(vlaga >= 90){
    return true;
  } else {
    return false;
  }
}

