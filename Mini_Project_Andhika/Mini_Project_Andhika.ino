//Andhika Dwiki (Mini Project_Integrated IoT Monitoring System & Mobile Programming)
//2 Input(PIR Sensor & DHT Sensor) & 2 Output (Lamp & Relay/Lamp2) & 1 Camera

#include <WiFi.h>                                                // esp32 library
#include <DHT.h>  //library dht as sensor for temperature and humidity
#include <FirebaseESP32.h>                          // firebase library
#define FIREBASE_HOST "https://loginfirebase-a4b8b-default-rtdb.firebaseio.com/" // the project name address from firebase id
#define FIREBASE_AUTH "mtNsaPA7M0qr0UbgZ2UHJUrsd1R3ETO1df93mJk6" // the secret key generated from firebase

#define WIFI_SSID "Rizky 129"                        // input home or public wifi name
#define WIFI_PASSWORD "88889999"                    //password of wifi ssid
                               
#define DHTPIN 27                 // Digital pin connected to DHT11
#define DHTTYPE DHT11             // Initialize dht type as DHT 11
                                     
DHT dht(DHTPIN, DHTTYPE);  

String fireStatus = "";      // led status received from firebase
                                               
const int pirPin = 26;
int led = 25;                                                               
int led2 = 19;                                                               
String informasi; //take data/status

void setup() {
  dht.begin();   
  Serial.begin(9600);

  delay(1000);
                
  pinMode(17, OUTPUT);
  pinMode(27, OUTPUT);                
  pinMode(14, OUTPUT);                
  pinMode(pirPin, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi

  Serial.print("Connecting to ");

  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {

    Serial.print(".");

    delay(500);

  }

  Serial.println();

  Serial.print("Connected to ");

  Serial.println(WIFI_SSID);

  Serial.print("IP Address is : ");

  Serial.println(WiFi.localIP());                                                      //print local IP address

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase

  Firebase.setString("LED_STATUS", "OFF");                                          //send initial string of led status

}

void loop() {
  int motionValue = digitalRead(pirPin); //if sensor detect motion, lamp will be turn on

if (motionValue == HIGH) {
    // Motion detected, turn on the LED
    digitalWrite(led, HIGH);
    Serial.println("Motion Detected!");
    informasi = "Motion Detected !";
    delay(1000); // Wait for 1 second to avoid rapid triggering
  } 
  
  else {
    // No motion detected, turn off the LED
    digitalWrite(led, LOW);
    Serial.println("No Motion");
    informasi = "Motion Undetected !";
  }

  String Motion = String(informasi); 
  Firebase.pushString("/test/nilaiSensor", Motion);  

  float h = dht.readHumidity();                                 // Read Humidity
  float t = dht.readTemperature();

  if(isnan(h) || isnan(t)){
    Serial.println("Failed to read form dht sensor");
    return;
  } 
  Serial.print("Humidity: ");  
  Serial.print(h);
  Serial.println("%\t");

  Serial.print("Temperature: ");  
  Serial.print(t);  
  Serial.println("°C ");

  String HumidityString = String(h);
  String TemperatureString = String(t);

  Firebase.setString("Humidity", HumidityString);
  Firebase.setString("Temperature", TemperatureString);
 

  Firebase.setFloat("Humidity:", h);
  Firebase.setFloat("Temperature:", t);

//  handle error
  Serial.println("Temperature and Humidity Data Sept Successfully ");
  delay(1000);

fireStatus = Firebase.getString("LED_STATUS");

if (Firebase.failed()) 
    {
 
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
  if (fireStatus == "ON") {                         // compare the input of led status received from firebase

    Serial.println("Led Turned ON");                 
                                                        // make output led ON
    digitalWrite(17, HIGH);                                                         // make output led ON

  }


  else if (fireStatus == "OFF") {              // compare the input of led status received from firebase

    Serial.println("Led Turned OFF");

                                                           // make output led ON
    digitalWrite(17, LOW);                                                         // make output led ON

  }

 
}
