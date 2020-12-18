#include <FirebaseESP8266.h>
#include <FirebaseJson.h>

#include <DHT.h>
#include <ESP8266WiFi.h>


#define FIREBASE_AUTH "XXXXXXXXXXXXXXX"
#define FIREBASE_HOST "XXXXXXXXXXXXXXX"
#define WIFI_SSID "CXXXXX"
#define WIFI_PASS "*****"

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

FirebaseData firebaseData;

FirebaseJson json;

void setup(){
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("connecting to wifi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.println("connected!");
  Serial.println("ip address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}

void sensorUpdate(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if(isnan(h) || isnan(t) || isnan(f) ){
    Serial.println(F("failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.println(h);
  Serial.print(F("% temperature: "));
  Serial.println(t);
  Serial.print(F("C  ,"));
  Serial.print(f);
  Serial.println(F("F  "));

  if(Firebase.setFloat(firebaseData, "Suhu", t)){
    Serial.println("Passed");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  if (Firebase.setFloat(firebaseData, "Kelembaban", h))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  
}

void loop(){
  sensorUpdate();
}
