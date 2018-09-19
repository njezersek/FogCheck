void toggleLaser(){
  if(laser)laser = false;
  else laser = true;
  digitalWrite(LASER_PIN, laser);
}
