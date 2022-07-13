/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/
//Im using Wemos D1 R1 - set this settings when compiling
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <dht.h>

// Update these with values suitable for your network.

const char* ssid = "JioFiber-24";
const char* password = "PASSWORD";  


const char* mqtt_server = "m12.cloudmqtt.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
#define CONTROL_LIGHTS 14

#define DHT11_PIN 12
dht DHT;



void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  digitalWrite(CONTROL_LIGHTS, HIGH); // turn off the Control lights to begin with, HIGH is off !!
  Serial.println ("Turning control lights off ");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    Serial.println("Setting CONTROL_LIGHTS Pin 14 to turn Lights ON,in function callback");
    
    digitalWrite(CONTROL_LIGHTS, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // it is actually HIGH
    // 
  } else {
    Serial.println("Setting CONTROL_LIGHTS Pin 14 to turn Lights OFF, in function callback");
    digitalWrite(CONTROL_LIGHTS, HIGH);  // Turn the LED off by making the voltage HIGH
    // this is low, lights off.
  }

}

void reconnect() {
  // Loop until we're reconnected
  int flag = 0;
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "CREDENTIALS_FROM_BROKER", "PASSWORD_FROMBROKER" )) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      if ( flag ) {
         flag =0;
         Serial.println("sending a 1");
         client.publish("outTopic", "1");
      } else {
        Serial.println("sending a 0");
        client.publish("outTopic", "0");
      }
      // ... and resubscribe
      client.subscribe("inLightsTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED OFF to indicate that the connection is not done.

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED OFF - to indicate connection not done yet.
  
  // this is pin 14 for turning lights on and off.
  pinMode (CONTROL_LIGHTS, OUTPUT);
  

  Serial.begin(115200);
  Serial.println ("turning CONTROL LIGHTS/Pin 14 off i.e HIGH ");
  setup_wifi();
  client.setServer(mqtt_server, 19757);
  client.setCallback(callback);
}

void loop() {

  char publish_str [20];
  
  if (!client.connected()) {
    digitalWrite(BUILTIN_LED, HIGH);   // Got disconnected, so turn the wifi indicator off.  
    reconnect();
  }
  client.loop();
  digitalWrite(BUILTIN_LED, LOW);   // Got connected, enable wifi indicator on.


  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  int temp = DHT.temperature;
  int humd = DHT.humidity;
  sprintf ( publish_str, "%d", temp);
  client.publish("/WhiteHouse/C1-702/temperature", publish_str);

  sprintf ( publish_str, "%d", humd);
  client.publish("/WhiteHouse/C1-702/humidity", publish_str);


  //delay(1000);
    
    //snprintf (msg, 75, "hello world #%ld", value);
    //Serial.print("Publish message: ");
    //Serial.println(msg);
    //client.publish("outTopic", msg);

    // 2nd phase of code where it sends a 0 or a 1
    
    /* if ( value ) {
         Serial.println("sending a 1");
         client.publish("outTopic", "1");
         digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
         digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED on (Note that LOW is the voltage level

         value=0;
    
      } else {
        Serial.println("sending a 0");
        client.publish("outTopic", "0");
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level


        value=1;
      }
      */

  }
}
