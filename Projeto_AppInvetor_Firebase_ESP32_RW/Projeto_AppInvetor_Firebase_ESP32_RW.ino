
//------------------------------------------------------------//
//----------------Iot Home Automation Project-----------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
//----------------Developed by Wellington Dorta---------------//
//------------------------------------------------------------//


//============================================================//
//------------------------Libraries---------------------------//
//============================================================//

#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <addons/RTDBHelper.h>
#include <DHT.h>

//============================================================//
//------------------------Defines and classes------- ---------//
//============================================================//
#define WIFI_SSID "------------------"  // fill with your home WIFI ID                    
#define WIFI_PASSWORD "--------------"  // fill with your home WIFI PW      
#define DATABASE_URL "---------------"  // fill with your FIREBASE url
#define DATABASE_SECRET "------------"  // fill with your FIREBASE auth 
#define WifiLed 2
#define DHTPIN 4
#define DHTTYPE    DHT11 // if you have the DHT22 sensor, only replace the DHT11 to DHT22 and change the DELAY however you want

//============================================================//
//------------------------Objects-----------------------------//
//============================================================//

DHT dht(DHTPIN, DHTTYPE);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//============================================================//
//------------------------Variables---------------------------//
//============================================================//

unsigned long dataMillis = 0;
int count = 0;
//relays variables
int relay1 = 12;
int relay2 = 13;
int relay3 = 14;
int relay4 = 27;


String ledStatus = "";

void setup() {

  Serial.begin(115200); // serial defined as 9600 did not work
  dht.begin();

  pinMode(2, OUTPUT); // pin 2 ESP32 led defined as output
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // start wifi object
  
  Serial.print("Connecting to Wifi...");

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(2, HIGH); // ESP32 Led keep blinking when is trying to connect wifi
    delay(500);
    digitalWrite(2, LOW); // ESP32 Led keep blinking when is trying to connect wifi
    delay(500);
    Serial.print(".");
    delay(500);
  }

  printf("\n");
  Serial.println("Wifi Connected!"); 
  printf("\n");
  digitalWrite(2, HIGH); 
  Serial.println(WiFi.localIP().toString());


  Serial.println();

  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  Firebase.reconnectWiFi(true);

  Firebase.begin(&config, &auth);

  pinMode(relay1 , OUTPUT);
  pinMode(relay2 , OUTPUT);
  pinMode(relay3 , OUTPUT);
  pinMode(relay4 , OUTPUT);

}

void loop()
{
//============================================================//
//------------- Firebase DB Set and Get ----------------------//
//============================================================//
    homeAutomation(); // Start Home Automation Function
    sensorUpdate();  // Start Sensor Update Function
}

//============================================================//
//------------------- DHT sensor Function --------------------//
//============================================================//

void sensorUpdate(){
  // Reading temperature or humidity takes about 250 milliseconds! -> USING DHT22 sensor if you use DHT11 the recomendation is 2000 milliseconds
  delay(2000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C  ,"));
  Serial.print(f);
  Serial.println(F("°F  "));

  if (Firebase.RTDB.setFloat(&fbdo, "/Home_Automation/temperature", t))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.RTDB.setFloat(&fbdo, "/Home_Automation/humidity", h))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}

//============================================================//
//------------------- Home Automation Function --------------------//
//============================================================//
void homeAutomation()
{
    if (Firebase.RTDB.getString(&fbdo, "Home_Automation/SALA")) {
      ledStatus=fbdo.stringData();
    }
    if(ledStatus=="1") {
      digitalWrite(relay1,LOW);
      Serial.println(ledStatus);
    }
    else if (ledStatus=="0")
      digitalWrite(relay1,HIGH);
      Serial.println(ledStatus);
      delay(500);

    if (Firebase.RTDB.getString(&fbdo, "Home_Automation/COZINHA")) {
      ledStatus=fbdo.stringData();
    }
    if(ledStatus=="1") {
      digitalWrite(relay2,LOW);
      Serial.println(ledStatus);
    }
    else if (ledStatus=="0")
      digitalWrite(relay2,HIGH);
      Serial.println(ledStatus);
      delay(500);

    if (Firebase.RTDB.getString(&fbdo, "Home_Automation/QUARTO1")) {
      ledStatus=fbdo.stringData();
    }
    if(ledStatus=="1") {
      digitalWrite(relay3,LOW);
      Serial.println(ledStatus);
    }
    else if (ledStatus=="0")
      digitalWrite(relay3,HIGH);
      Serial.println(ledStatus);
      delay(500);

    if (Firebase.RTDB.getString(&fbdo, "Home_Automation/QUARTO2")) {
      ledStatus=fbdo.stringData();
    }
    if(ledStatus=="1") {
      digitalWrite(relay4,LOW);
      Serial.println(ledStatus);
    }
    else if (ledStatus=="0")
      digitalWrite(relay4,HIGH);
      Serial.println(ledStatus);
      delay(500);
}
