#include <WiFi.h>
#include <DHT.h>
#include "AdafruitIO_WiFi.h"
#define DHTPIN 13
#define DHTTYPE DHT11
#define lm 12
#define ldr 14
#define somoi 33
#define rain 2
#define pumpin 18
// Wi-Fi credentials
#define WIFI_SSID "WIFI_NAME"
#define WIFI_PASS "WIFI_PASSWORD"

// Adafruit IO credentials
#define IO_USERNAME  "IO_USERNAME"
#define IO_KEY       "IO_NAME"
long last=millis();

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
DHT dht(DHTPIN, DHTTYPE);

// Feed name
AdafruitIO_Feed *airtem = io.feed("pbel.airtemp");
AdafruitIO_Feed *airmoi = io.feed("pbel.airhumid");
AdafruitIO_Feed *soitem = io.feed("pbel.soiltemp");
AdafruitIO_Feed *soimoi = io.feed("pbel.soilmoist");
AdafruitIO_Feed *light = io.feed("pbel.ambilight");
AdafruitIO_Feed *rai = io.feed("pbel.rainamou");
AdafruitIO_Feed *pum = io.feed("cont.pump");
void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("DHT11 Sensor Ready!");
  Serial.println("Connecting to Adafruit IO");
  io.connect();
  pum->onMessage(handleMessage);
  while (io.status() < AIO_CONNECTED) {
    Serial.println("                -------\n\
               /       \\ \n\
              / O   O   \\ \n\
             |   <      | \n\
              \\  .___,  / \n\
               \\_______/ \n\
                \\    / \n\
           ______)  (_____ \n\
          /               \\");
    delay(200);
    Serial.println("                -------\n\
               /       \\ \n\
              /  O   O  \\ \n\
             |     <    | \n\
              \\   .___, / \n\
               \\_______/ \n\
                \\    / \n\
           ______)  (_____ \n\
          /               \\");
    delay(200);
    Serial.println("                -------\n\
               /       \\ \n\
              /   O   O \\ \n\
             |      >   | \n\
              \\    .___ / \n\
               \\_______/ \n\
                \\    / \n\
           ______)  (_____ \n\
          /               \\");
    delay(200);
    Serial.println("                -------\n\
               /       \\ \n\
              /   -   - \\ \n\
             |      >   | \n\
              \\    .___ / \n\
               \\_______/ \n\
                \\    / \n\
           ______)  (_____ \n\
          /               \\");
    delay(200);
    Serial.println("                -------\n\
               /       \\ \n\
              /  o   o  \\ \n\
             |     >    | \n\
              \\   .___, / \n\
               \\_______/ \n\
                \\    / \n\
           ______)  (_____ \n\
          /               \\");
    delay(200);
  }

  Serial.println("\nConnected to Adafruit IO!");
  pum->get();
}

void loop() {
  io.run();
  if(millis()-last>60000){
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
    Serial.print("AIO status: ");
    Serial.println(io.statusText());
    last=millis();
    int lmval = analogRead(lm);
    float vol = lmval * (3.3 / 4095.0);
    float tem = vol * 100.0;
    Serial.print("Temperature: ");
    Serial.print(tem);
    Serial.println("°C");

    // Send data to the feed
    soitem->save(tem);
    float airhumi = dht.readHumidity();
    airmoi->save(airhumi);  
    float airtemp = dht.readTemperature();
    airtem->save(airtemp);
    int raini=analogRead(rain);
    rai->save(raini);
    int soil=analogRead(somoi);
    soimoi->save(soil);
    int ligh=analogRead(ldr);
    light->save(ligh);
  }
  delay(10);
}
void handleMessage(AdafruitIO_Data *data) {
  int pin=data->toInt();
  digitalWrite(pumpin,pin);
  Serial.print("pump = ");
  Serial.println(pin);
}