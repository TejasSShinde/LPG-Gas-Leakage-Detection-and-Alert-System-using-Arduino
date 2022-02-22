#include <WiFi.h>      
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h> 
#include <FirebaseESP32.h>
#include <FirebaseJson.h>
#include<HardwareSerial.h>
HardwareSerial gsm(1);

#define Relay1 16 
#define FIREBASE_HOST "registration-2862e-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "m9pTAOthyck8w48P4j1mcnXCcX0LkchLMdG2qgMt"
#define FIREBASEHOST "login-info-5f8dd-default-rtdb.firebaseio.com"
#define FIREBASEAUTH "7XGqnId0OtIqWjd6qQNIKOJCLqOqsHaREO02PBi1"

WiFiManager wifiManager;
String number="1",login="No",mac,user,val1;
int i,value,limit,relay[15],count=0,j,passval;
FirebaseData fbdo; 
FirebaseData fbdo1;       
void setup() {
    Serial.begin(115200);   
    wifiManager.autoConnect("LPG Gas Leakage Alert","password");
}
void loop() { 
   while(WiFi.status()!=WL_CONNECTED){
    Serial.print("Not Connected");
     wifiManager.autoConnect("LPG Gas Leakage Alert","password");
    }
   mac=WiFi.macAddress();
  Firebase.begin(FIREBASEHOST,FIREBASEAUTH);
  Firebase.reconnectWiFi(true);
  Firebase.getString(fbdo,mac+"/Username");
  user=fbdo.stringData();
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.getString(fbdo,user+"/login");
  login=fbdo.stringData();
   login.remove(0,2);
   i=login.length();
   login.remove(i-2,2);
   Serial.println(login);
  if(login=="Yes")
  { 
    while(Serial.available())
     {
        passval=Serial.read();
     }
      Serial.println(passval);
       if (Firebase.get(fbdo,user+"/relayno"))
        {
          FirebaseJson &json = fbdo.jsonObject();
        size_t len = json.iteratorBegin();
        j=len;
        String value = "",key;
        int type = 0;
        if(count==0)
        {
        for (size_t i = 0; i < len; i++)
        {
            json.iteratorGet(i, type, key, value);        
           limit=value.toInt();
           relay[i]=limit;
           pinMode(relay[i],OUTPUT);
        }
        }
        json.iteratorEnd();
        count=count+1;
    }
      if(passval==1)
      {
          SendMessage();
          for(int i=0;i<j;i++)
            {
              digitalWrite(relay[i],HIGH);
              Serial.println(relay[i]);
             }
       }
       if(Firebase.getString(fbdo,user+"/Phone Number"))
         {
            number=fbdo.stringData();
            number.remove(0,2);
            i=number.length();
            number.remove(i-2,2);
            Serial.println(number);
          }else{
            Serial.print("Error in getInt, ");
            Serial.println(fbdo.errorReason());
          }
    if (Firebase.get(fbdo,user+"/ElectricalComp"))
      {
          FirebaseJson &json = fbdo.jsonObject();
          size_t len = json.iteratorBegin();
          j=len;
          String key,value = "";
          int type = 0;
          for (size_t i = 0; i < len; i++)
            {
              json.iteratorGet(i, type, key, value);
              Serial.println(key); 
              if(Firebase.getString(fbdo1, user+"/ElectricalComp/"+key))
              {
                val1=fbdo1.stringData();
                Serial.println(val1);
               if(val1=="1" && passval==2)
                {
                digitalWrite(relay[i],LOW);
                Serial.println(val1);
                }
              if(val1=="0")
              {
                digitalWrite(relay[i],HIGH);
                Serial.println(val1);
              }
            }else{
         Serial.print("Error in getInt, ");
          Serial.println(fbdo1.errorReason());
           }
        }
        json.iteratorEnd();
    }
  }
  else
  {
    Serial.println("0");
  }
}

void SendMessage()
{
 gsm.begin(115200, SERIAL_8N1, 26, 27);
  gsm.println("AT+CMGF=1");
  delay(200);
  gsm.println("AT+CMGS=\""+number+"\""+"");
  delay(50);
   gsm.println("LPG Gas Leakage Alert.");
  delay(50);
  gsm.println((char)26);
  delay(50);
  Serial.println("AT+CMGS=\""+number+"\""+""); 
}
