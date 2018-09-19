void executeAT(String command, String endKeyString = "OK", int timeOut = 500){
  String input = "";
  Serial.println(command);
  //Serial.println("executing: " + command);
  while (true){
    if (Serial.available()){
      //Serial.print(":");
      char in = Serial.read();
      input += in;
      //Serial.write(in);
      //TODO: add timeout, error handling (perhaps send the command again?)
      if(input.indexOf(endKeyString) > -1 || input.indexOf("ERROR") > -1){
        while (Serial.available()){
          char in = Serial.read();
          input += in;
          //Serial.write(in);
        }
        break;
      }
    }else{
      delay(10);
      timeOut--;
      if (timeOut <= 0){
        break;
      }
    }
  }
  //Serial.println("done executing: " + command);
  //Serial.println();
}

String readAT(String command, String readKeyString, String endKeyString = "OK", int timeOut = 500){
  String input = "";
  String output = "";
  Serial.println(command);
  //Serial.println("reading: " + command);
  while (true){
    if (Serial.available()){
      char in = Serial.read();
      input += in;
      //Serial.write(in);
      //TODO: add timeout, error handling (perhaps send the command again?)
      if(input.indexOf(readKeyString) > -1){
        while (Serial.available()){
          char in = Serial.read();
          input += in;
          //Serial.write(in);
          if(in == '\n'){
            while (Serial.available()){
              char in = Serial.read();
              input += in;
              //Serial.write(in);
              if(in == '\n'){
                break;
              }
              output += in;
            }
            break;
          }
        }
      }
      if(input.indexOf(endKeyString) > -1 || input.indexOf("ERROR") > -1){
        while (Serial.available()){
          char in = Serial.read();
          input += in;
          //Serial.write(in);
        }
        break;
      }
    //No Signal
    }else{
      delay(10);
      timeOut--;
      if (timeOut <= 0){
        break;
      }
    }
  }
  //Serial.println("done reading: " + command);
  //Serial.println();
  return output;
}

void gsmInit(){
  executeAT("AT+SAPBR=3,1,\"APN\",\"internet.simobil.si\""); //AT+SAPBR=3,1,"APN","websi"
  executeAT("AT+SAPBR=1,1");                   //AT+SAPBR=1,1
  executeAT("AT+HTTPINIT");                    //AT+HTTPINIT
}

/*String getSIM() {
  return readAT("AT+CCID","AT+CCID");
}*/

String sendData(int id, String token, int fogLevel, float temperature, int humidity, int pressure){
  //?id=1&token=vegaFog456&megla=50&temperatura=23&vlaga=36&pritisk=1002
  //"http://g2a.mojvegovc.si/megla/api/update.php?id=" + id + "&token=" + token + "&megla=" + fogLevel + "&temperatura=" + temperature + "&vlaga=" + humidity + "&pritisk=" + pressure
  executeAT("AT+HTTPPARA=\"CID\",1"); //AT+HTTPPARA="CID",1
  //http://g2a.mojvegovc.si/megla/api/update.php?id=1&token=vegaFog456&megla=50&temperatura=23&vlaga=36&pritisk=1002
  String url = "http://g2a.mojvegovc.si/megla/api/update.php";
  url += "?id=";
  url += id;
  url += "&token=";
  url += token;
  url += "&megla=";
  url += fogLevel;
  url += "&temperatura=";
  url += temperature;
  url += "&vlaga=";
  url += humidity;
  url += "&pritisk=";
  url += pressure;
  executeAT("AT+HTTPPARA=\"URL\",\"" + url + "\"", "http");
  //AT+HTTPPARA="URL","http://g2a.mojvegovc.si/megla/api/update.php?id=1&token=vegaFog456&megla=50&temperatura=23&vlaga=36&pritisk=1002"
  
  executeAT("AT+HTTPACTION=0","+HTTPACTION:"); //AT+HTTPACTION=0
  return readAT("AT+HTTPREAD=0,1", "+HTTPREAD:"); //AT+HTTPREAD=0,1
}

