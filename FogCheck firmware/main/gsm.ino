void executeAT(String command, String endKeyString = "OK"){
  String input = "";
  gsm.println(command);
  //Serial.println("executing: " + command);
  while(true){
    if (gsm.available()){
      //Serial.print(":");
      char in = gsm.read();
      input += in;
      //Serial.write(in);
      //TODO: add timeout, error handling (perhaps send the command again?)
      if(input.indexOf(endKeyString) > -1 || input.indexOf("ERROR") > -1){
        while (gsm.available()){
          char in = gsm.read();
          input += in;
          //Serial.write(in);
        }
        break;
      }
    }
  }
  //Serial.println("done executing: " + command);
  //Serial.println();
}

String readAT(String command, String readKeyString, String endKeyString = "OK"){
  String input = "";
  String output = "";
  gsm.println(command);
  //Serial.println("reading: " + command);
  while (true){
    if (gsm.available()){
      char in = gsm.read();
      input += in;
      //Serial.write(in);
      //TODO: add timeout, error handling (perhaps send the command again?)
      if(input.indexOf(readKeyString) > -1){
        while (gsm.available()){
          char in = gsm.read();
          input += in;
          //Serial.write(in);
          if(in == '\n'){
            while (gsm.available()){
              char in = gsm.read();
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
        while (gsm.available()){
          char in = gsm.read();
          input += in;
          //Serial.write(in);
        }
        break;
      }
    }
  }
  //Serial.println("done reading: " + command);
  //Serial.println();
  return output;
}

void gsmInit(){
  executeAT("AT+SAPBR=3,1,\"APN\",\"websi\"");
  executeAT("AT+SAPBR=1,1");
  executeAT("AT+HTTPINIT");
}

String sendData(int id, String token, float fogLevel, float temperature, float humidity, float pressure){
  //?id=1&token=vegaFog456&megla=50&temperatura=23&vlaga=36&pritisk=1002
  //"http://g2a.mojvegovc.si/megla/api/update.php?id=" + id + "&token=" + token + "&megla=" + fogLevel + "&temperatura=" + temperature + "&vlaga=" + humidity + "&pritisk=" + pressure
  executeAT("AT+HTTPPARA=\"CID\",1");
  
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
  
  executeAT("AT+HTTPACTION=0","+HTTPACTION:");
  String response = readAT("AT+HTTPREAD=0,1", "+HTTPREAD:");
  //Serial.println("response from server ("+url+"): " + response);
  return response;
}
