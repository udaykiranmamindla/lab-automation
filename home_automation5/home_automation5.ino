#define BLYNK_TEMPLATE_ID "TMPL3rHf6U2i3"
#define BLYNK_DEVICE_NAME "Lab Automation 2"
#define BLYNK_AUTH_TOKEN "zfCMt9IIeUhhKmPMK0Au1E7IDm8Tfi9l"
#define BLYNK_PRINT Serial

#include<BlynkSimpleEsp8266.h>
#include<ESP8266WiFi.h>

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;
WiFiClient client;

const char* ssid  = "HITAM_S18";
const char* password  = "hitam@S18";

int sensor = D5  ; // Digital pin D5
//int sensor1 = D6;
int state = 0;
//int state1 = 0;
int relay = D0;
int relay2 = D3;


unsigned long previousmillis = 0;
const long interval = 30000;

int relaystat = LOW;
int relay2stat = LOW;
int automode = 0;

BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
}


void setup() {
  Serial.begin(9600);

  pinMode(sensor, INPUT);   // declare sensor as input
  //pinMode(sensor1, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(relay2, OUTPUT);

 Blynk.begin(auth, ssid, password);
  WiFi.mode(WIFI_STA);
  Serial.print("connected to ssid");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}
void loop() {
  Blynk.run();
  if (!automode) {
    Serial.println("automode");
    unsigned long currentmillis = millis();
    if (currentmillis - previousmillis >= interval) {
      previousmillis = currentmillis;
      //place the code to be executed for every one minute
    }
      sense();
      turnon();
      turnoff();
      
    }
}

void sense() {
    state = digitalRead(sensor);
    //state1 = digitalRead(sensor1);
}

void turnon() {
  if (state == HIGH) {
    Serial.println("Motion detected!");
    digitalWrite(relay, LOW);
    digitalWrite(relay2, LOW);

    Blynk.virtualWrite(V2, HIGH);
    Blynk.virtualWrite(V1, HIGH);
    delay(3000);
  }
}

void turnoff() {
  if (state == LOW) {
    Serial.println("Motion absent!");
    digitalWrite(relay, HIGH);
    digitalWrite(relay2, HIGH);
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V1, LOW);
  }
}

BLYNK_WRITE(V8) {
  automode = param.asInt();
}

BLYNK_WRITE(V2) {
  Serial.println("manualmode-relay");
  if (automode) {
    relaystat = param.asInt();
    digitalWrite(relay, !relaystat);
    Serial.println("relaytog");
  }
}

BLYNK_WRITE(V1) {
  Serial.println("manualmode-relay2");
  if (automode) {
    relay2stat = param.asInt();
    digitalWrite(relay2, !relay2stat);
    Serial.println("relay2tog");
  }
}
