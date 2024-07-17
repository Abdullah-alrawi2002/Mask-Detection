#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "ssid";
const char* password = "pass";

#define ECHO_PIN 5 
#define TRIG_PIN 4
#define SERVO_PIN 2
#define IR_SENSOR_PIN 0
// Define pump if needed
// #define PUMP_PIN 14

long duration; 
int distance;
Servo myservo;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  myservo.attach(SERVO_PIN);
  myservo.write(0);

  pinMode(IR_SENSOR_PIN, INPUT);
  // pinMode(PUMP_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  server.begin();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  int irsVal = digitalRead(IR_SENSOR_PIN);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; 
  
  if (distance < 45) {
    Serial.println("H");
    delay(2000);
  }

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while(!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/M") != -1) {
    myservo.write(90);
    delay(1000);
    myservo.write(0);
  } else if (request.indexOf("/P") != -1) {
    Serial.println("D");
    delay(30000);
    if (irsVal == 0) {
      Serial.println("S");
    }
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML><html><center><h1 style='background-color:DodgerBlue;'>ATI SYSTEMS</h1></center></html>");
}
