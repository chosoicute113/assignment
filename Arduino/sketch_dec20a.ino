//////////////////// CONFIG//////////////////////////////////
#include <ESP8266WiFi.h>  //include wifi connect library for System
#include <SoftwareSerial.h> // include Software Serial library for System
#include <FirebaseArduino.h> //include firebase library for System
#include <ArduinoJson.h> //inclue Arduino Json library for System
#define FIREBASE_HOST "myapp-e3a2f-default-rtdb.firebaseio.com" //define FIREBASE DATABASE HOST
#define FIREBASE_DTB "fB8DvP0D81NxT9lA3msTzLte706gJpVDIt5AXxc9" //define FIREBASE SECRET CODE
#define WIFI_SSID "Greenwich-Guest" //define local Wi-Fi name
#define WIFI_PASSWORD "greenwichvn@123"// define local Wi-Fi password
#define D0 16 //define pin value for digital port D0
#define D1 5  //define pin value for digital port D1
#define D2 4  //define pin value for digital port D2
#define D3 0  //define pin value for digital port D3
#define D4 2  //define pin value for digital port D4
#define D5 14 //define pin value for digital port D5
#define D6 12 //define pin value for digital port D6
#define D7 13 //define pin value for digital port D7
#define D8 15 //define pin value for digital port D8
#define A0 17 //define pin value for digital port A0
///////////////////SETUP///////////////////////////////////
int PIR_SENSOR_PIN = D4; //set pin port for PIR sensor
int LIGHT_SENSOR_PIN = A0;//set pin port for Light sensor
int LED1_PIN = D1;//set pin port for LED 1
int LED2_PIN = D2;//set pin port for LED 2
int LED3_PIN = D3;//set pin port for LED 3
void setup() {
  // put your setup code here, to run once:
  pinMode(PIR_SENSOR_PIN, INPUT);//set pin mode for PIR Sensor
  pinMode(LIGHT_SENSOR_PIN, INPUT);//set pin mode for Light sensor
  pinMode(LED1_PIN, OUTPUT);// set pin mode for LED 1
  pinMode(LED2_PIN, OUTPUT);// set pin mode for LED 2
  pinMode(LED3_PIN, OUTPUT);// set pin mode for LED 3
  Serial.begin(9600);// set Serial output at 9600 baudrate

  connectWifi();// connect to wifi with defined WIFI name and password
  connectFirebase(); // connect to Firebase database with defined Firebase database host and code
}
////////////////////////// BEGIN ///////////////////////////////
void loop() {
  Firebase.setInt("PIR_SENSOR/PIR_VALUE",digitalRead(PIR_SENSOR_PIN)); //set value of PIR Sensor in database based on the value of sensor received in environment
  int light_sensor = map(analogRead(LIGHT_SENSOR_PIN),0,200,255,0); // set a map for light sensor value
  if(light_sensor < 0)
  {
    Firebase.setInt("LIGHT_SENSOR/LIGHT_VALUE",0); // if the light sensor receive < 0 value from environment, the value of Light sensor on database will be set to 0
  }
  else Firebase.setInt("LIGHT_SENSOR/LIGHT_VALUE",light_sensor); // if the light sensor receive > or = 0 value from environment, the value of light sensor on database will be set to the data it received
  

  int pir_Value = Firebase.getInt("PIR_SENSOR/PIR_VALUE"); // receive information from database and define it with int pir_Value
  int light_condition1 = Firebase.getInt("led_1/status"); // receive information from database and define it with int light_conditions1
  int light_condition2 = Firebase.getInt("led_2/status"); // receive information from database and define it with int light_conditions2
  int light_condition3 = Firebase.getInt("led_3/status"); // receive information from database and define it with int light_conditions3
  int light_value1 = Firebase.getString("led_1/value").toInt(); //receive information from database, convert it to int and define it with int light_value1
  int light_value2 = Firebase.getString("led_2/value").toInt(); //receive information from database, convert it to int and define it with int light_value2
  int light_value3 = Firebase.getString("led_3/value").toInt(); //receive information from database, convert it to int and define it with int light_value3
  if(pir_Value) //check for true value from pir_value
  {
    ledC(light_value1, light_condition1, LED1_PIN); // Control led 1 base on the value that this function received
    Serial.print("Light_value: "); 
    Serial.println(light_value1); // print light 1 value to console
    Serial.print("Light_condition: "); 
    Serial.println(light_condition1); // print light 1 status to console
    ledC(light_value2, light_condition2, LED2_PIN); // Control led 2 base on the value that this function received
    ledC(light_value3, light_condition3, LED3_PIN); // Control led 3 base on the value that this function received
  }
  else{
    noHuman(light_value1, light_condition1, LED1_PIN); // Control led 1 base on the value that this function received
    noHuman(light_value2, light_condition2, LED2_PIN); // Control led 2 base on the value that this function received
    noHuman(light_value2, light_condition2, LED2_PIN); // Control led 3 base on the value that this function received
  }
  delay(500);
  
}
////////////////////////// FUNCTION ////////////////////////////
void connectWifi(){
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // connect to local wifi with defined WI FI name and password in config
  Serial.println("");
  Serial.print("Connecting");

  //print to console " . " while Arduino board not connected to wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  //print to console Wifi name and its IP if Arduino board connected to wifi
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());


  
}
void connectFirebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_DTB);// connect to firebase realtime database with defined firebase database host and secrect code in config
  if(Firebase.failed()){
    Serial.println("Firebase connection failed"); //print to console the message if connection between Arduino board and database is fail
  }
  else{
    Serial.println("Firebase connection established"); //print to console the message if connection between Arduino board and database is success
  }
}
//////////////////////////////////////////////////////////////////////////////
void ledC(int light_sensor, int light_condition, int led_pin) //Collect conditions to control leds.
{

  if(light_condition == 1)
  {
    light(light_sensor,led_pin); // if led is turned on, set led light_level with brightness value (light_sensor) and set led pin port (led_pin)
  }
  else light(0,led_pin); // if led is turned off, set led light_level with value 0 and set led pin port (led_pin)
}
void light(int light_level, int led) //Control leds based on received data
  {
    analogWrite(led, light_level); // set brightness for a led pin port
  }
void noHuman(int light_sensor, int light_condition, int led_pin) //Collect conditions to control leds.
{ 
  if(light_condition == 1){
    for(int i = light_sensor; i > 0; i = i-1) //Turn led off if there are no movement in room but with light turned on
    {
      Serial.print("i = ");
      Serial.println(i);
      light(i,led_pin); 
      light_condition = 0;
    }
    } 
    else{
      light(0,led_pin); //Stay the value 0 in light status is already 0 and no any movement in room
    }
}
