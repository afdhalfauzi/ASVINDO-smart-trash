#include "mqttHandler.h"
#include "sensorHandler.h"
#include "fuzzyHandler.h"
#include "notificationHandler.h"
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#define SERVO_PIN 5
#define BUZZER_PIN 27
#define LED_PIN 12

MQTT mqtt;
FUZZY fuzzy;
IRsensor InfraRed(34);
TeleBot teleBot;

Ping Ping1(19, 18);//(Echo, Trigger)
Ping Ping2(2, 4);
Ping Ping3(13, 15);

Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void gate();
void display();
float rata2_Us();

unsigned long millisMqtt = 0;
unsigned long millisFuzzy = 0;
float jarak1, jarak2, jarak3, avg, output_fuzzy;
bool isFull = true;

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  mqtt.begin();
  teleBot.begin();
  fuzzy.begin();
  InfraRed.begin();
  Ping1.begin();
  Ping2.begin();
  Ping3.begin();
  servo.attach(SERVO_PIN);
  servo.write(10);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  if (output_fuzzy > 75)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    if (!isFull)
    {
      teleBot.sendMessage("⚠ALERT⚠\nTempat sampah full!.\nPerlu dikosongkan agar dapat digunakan kembali");
    }
    isFull = true;
  }
  else
  {
    gate();
    digitalWrite(BUZZER_PIN, LOW);
    if (isFull)
    {
      avg = map(rata2_Us(), 20, 70, 0, 70);
      fuzzy.setInput(1, avg);
      fuzzy.fuzify();
      output_fuzzy = fuzzy.defuzify();
      teleBot.sendMessage("🚮INFO\nTempat sampah dapat digunakan kembali\nKapasitas saat ini:" + String(output_fuzzy, 0) + "%");
    }
    isFull = false;
  }

  if (millis() - millisFuzzy >= 1000)
  {
    millisFuzzy = millis();
    // avg = rata2_Us();
    avg = map(rata2_Us(), 20, 70, 0, 70);
    fuzzy.setInput(1, avg);
    fuzzy.fuzify();
    output_fuzzy = fuzzy.defuzify();
    Serial.println(output_fuzzy);
    display();
  }

  mqtt.loop();
  if (millis() - millisMqtt >= 4000)
  {
    millisMqtt = millis();
    mqtt.publish(output_fuzzy);
  }
}

void gate()
{
  Serial.printf("IR State1: %d\n", InfraRed.getRead());

  // IR detects
  if (!InfraRed.getRead())
  {
    // Open then close lid
    servo.write(180);
    delay(6000);
    for (int i = 180; i > 10; i--)
    {
      servo.write(i);
      delay(10);
    }
  }
  else
  {
    servo.write(10);
  }
}

float volume_sampah(int jarak)
{
  float panjang = 10.0;
  float lebar = 5.0;
  float volume = panjang * lebar * jarak;
  return volume;
}

float rata2_Us()
{
  jarak1 = Ping1.readDistanceCM();
  jarak2 = Ping2.readDistanceCM();
  jarak3 = Ping3.readDistanceCM();

  Serial.printf("US State1: %f\n", jarak1);
  Serial.printf("US State2: %f\n", jarak2);
  Serial.printf("US State3: %f\n", jarak3);

  float rata_rata = ((jarak1 + jarak2 + jarak3) / 3);
  Serial.printf("US Avg: %f\n\n", rata_rata);

  return rata_rata;
}

void display()
{
  lcd.setCursor(0, 0);
  lcd.print("1:");
  lcd.setCursor(5, 0);
  lcd.print("2:");
  lcd.setCursor(10, 0);
  lcd.print("3:");
  lcd.setCursor(2, 0);
  lcd.print(String(jarak1, 0) + " ");
  lcd.setCursor(7, 0);
  lcd.print(String(jarak2, 0) + " ");
  lcd.setCursor(12, 0);
  lcd.print(String(jarak3, 0) + " ");

  lcd.setCursor(0, 1);
  lcd.print("AVG:");
  lcd.setCursor(7, 1);
  lcd.print("FUZZY:");
  lcd.setCursor(4, 1);
  lcd.print(String(avg, 0) + " ");
  lcd.setCursor(13, 1);
  lcd.print(String(output_fuzzy, 0) + " ");
}