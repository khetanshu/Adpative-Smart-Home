/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

//Wifi Configuration
char* ssid1="ktcmobile";
char* pwd="ktcpark1217";

//webservice URL
String URL="http://khetanshupi:1880/system?id=";

//Lights Configurations
int LIGHT1_GPIO=5;
int LIGHT2_GPIO=4;



void setup() {

    USE_SERIAL.begin(115200);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    //USE_SERIAL.printf("New SSID = %s...\n", ssid1);
    WiFiMulti.addAP(ssid1,pwd);
    pinMode(LIGHT1_GPIO, OUTPUT);  
    pinMode(LIGHT2_GPIO, OUTPUT);
}

void loop() {
   int LIGHT1CurrentStatus=-1;
   int LIGHT2CurrentStatus=-1;
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        LIGHT1CurrentStatus=getStatusAndUpdate("1",LIGHT1_GPIO,LIGHT1CurrentStatus );
        LIGHT2CurrentStatus=getStatusAndUpdate("2",LIGHT2_GPIO,LIGHT2CurrentStatus );
    }
    //delay(300);
}

int getStatusAndUpdate(String id,int LIGHT_GPIO, int LIGHTCurrentStatus){
        HTTPClient http;
        //USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        http.begin(URL+id); //HTTP
        //USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                int LIGHTNewStatus = payload.toInt();
                USE_SERIAL.printf("ID=%d New status for= %d old Status =%d\n",id.toInt(),LIGHTNewStatus,LIGHTCurrentStatus);
                if(LIGHTCurrentStatus=-1  || LIGHTNewStatus !=  LIGHTCurrentStatus){
                    LIGHTCurrentStatus=LIGHTNewStatus;
                    digitalWrite(LIGHT_GPIO, LIGHTNewStatus);
                    USE_SERIAL.printf("\nUpdated = to %d",LIGHTNewStatus);
                }
            } 
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
        return LIGHTCurrentStatus;
}

