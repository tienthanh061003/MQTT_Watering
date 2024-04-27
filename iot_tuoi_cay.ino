//khai bao thu vien
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define wifiLed    D0   //D0

//khai bao thong tin wifi va cac ket noi voi mqtt
const char* ssid = "Thanh"; //WiFI Name
#define password "0932983766" //WiFi Password

const char* mqttServer = "projectiottest1.cloud.shiftr.io";
const char* mqttUserName = "projectiottest1"; // MQTT username
const char* mqttPwd = "12ASD"; // MQTT password
const char* clientID = "Esp1"; // client id

//khai bao cac bien de chua cac publish/ subscribe

const char *SUB_D = "Dirt_Data";
const char *PUB_T = "temperature";
const char *PUB_H = "huminity";
const char *PUB_D = "data_dirt";


// khai bao bien de tinh toan
float temp;
int aNumber;
float hum;
int value1, real_value;
int percent_true;
float percent;
unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 2000;        // Interval at which to publish sensor readings
//khai bao cac du lieu can cho sensor DHT11( do am va nhiet do)
#define DHTPIN 5  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//cac lenh thu tuc de ket noi voi mqtt
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (80)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
 delay(10);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void reconnect() {
 while (!client.connected()) {
 if (client.connect(clientID, mqttUserName, mqttPwd)) {
 Serial.println("MQTT connected");
      // ... and resubscribe
      client.subscribe(SUB_D);
      
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


//cac cau lenh de subscribe data
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (strstr(topic,SUB_D))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    aNumber = atof((char *)payload);
    Serial.println("\n");
    
}}

void setup() {
pinMode(16,OUTPUT);
digitalWrite(16, LOW);
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
   client.setServer(mqttServer, 1883);
   client.setCallback(callback);
}

void loop() {


unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    t=a+poi


    
    // New DHT sensor readings
    hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
      temp = dht.readTemperature();
      //hai dong ke la cau lenh publish len mqtt
      client.publish(PUB_T, String(temp).c_str(), true);
      client.publish(PUB_H, String(hum).c_str(), true);
      // in du lieu len Serial Mornitor
      Serial.printf("Temperature: %.2f \n", temp);
      Serial.printf("Humidity: %.2f \n", hum);
      
//doc cam bien do am dat
    for(int i=0;i<=9;i++){
    real_value+=analogRead(A0);
  }
  value1=real_value/10;
  float percent = map(value1, 0, 1023, 0, 100);    // Set giá thang giá trị đầu và giá trị cuối để đưa giá trị về thang từ 0-100. 
                                                
  percent=100-percent;                            // Tính giá trị phần trăm thực. Chuyển điện thế từ 3.3V ( khô ) thành 3.3V ( ẩm )
  //Serial.print("Dirt Moister: ");
  Serial.printf("Soil Moister: %.2f %", percent);
  Serial.print("\n");
  real_value=0;
  //dua du lieu len mqtt
client.publish(PUB_D, String(percent).c_str(), true);

if ( aNumber >= percent ) {
      digitalWrite(16, HIGH);  // Turn the pump on
      Serial.println("Pump on \n");
      delay(2000);
    } 
    else if (aNumber <= percent) {
      digitalWrite(16, LOW);  // Turn the pump off
      Serial.println("Pump off");
      Serial.println("\n");
    }    
  
//cac dong nay cung la thu tuc
  
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  else{
    digitalWrite(wifiLed, LOW);
  }
  client.loop();
}
}
