#include "mqttHandler.h"
#include "sensorHandler.h"
#include "fuzzyHandler.h"
#include <ESP32Servo.h>
#include <Arduino.h>

#define SERVO_PIN 26

MQTT mqtt;
FUZZY fuzzy;
IRsensor IRsensor1(27);
IRsensor IRsensor2(14);
IRsensor IRsensor3(12);
IRsensor IRsensor4(13);

Ping Ping1(15, 2);
Ping Ping2(4, 5);
Ping Ping3(18, 19);

Servo servo;
void gate();
float rata2_Us();

unsigned long millisMqtt = 0;
unsigned long millisFuzzy = 0;

void setup()
{
  Serial.begin(115200);

  mqtt.begin();
  fuzzy.begin();
  IRsensor1.begin();
  IRsensor2.begin();
  IRsensor3.begin();
  IRsensor4.begin();
  Ping1.begin();
  Ping2.begin();
  Ping3.begin();
  servo.attach(SERVO_PIN);
  servo.write(90);
}

void loop()
{
  // gate();

  if (millis() - millisFuzzy >= 1000)
  {
    millisFuzzy = millis();
    fuzzy.setInput(1, rata2_Us());
    fuzzy.fuzify();
  }

  mqtt.loop();
  if (millis() - millisMqtt >= 4000)
  {
    millisMqtt = millis();
    float output_fuzzy = fuzzy.defuzify();
    Serial.println(output_fuzzy);
    mqtt.publish(output_fuzzy);
  }
}

void gate()
{
  // Serial.printf("IR State1: %d\n", IRsensor1.getRead());
  // Serial.printf("IR State2: %d\n", IRsensor2.getRead());
  // Serial.printf("IR State3: %d\n", IRsensor3.getRead());
  // Serial.printf("IR State4: %d\n", IRsensor4.getRead());

  // IR detects
  if (!IRsensor1.getRead() || !IRsensor2.getRead() || !IRsensor3.getRead())
  {
    // Open then close lid
    servo.write(180);
    delay(6000);
    for (int i = 180; i > 1; i--)
    {
      servo.write(i);
      delay(10);
    }
  }
  else
  {
    servo.write(0);
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
  float jarak1 = Ping1.readDistanceCM();
  float jarak2 = Ping2.readDistanceCM();
  float jarak3 = Ping3.readDistanceCM();

  Serial.printf("US State1: %f\n", jarak1);
  Serial.printf("US State2: %f\n", jarak2);
  Serial.printf("US State3: %f\n", jarak3);

  float rata_rata = ((jarak1 + jarak2 + jarak3) / 3);
  Serial.printf("US Avg: %f\n\n", rata_rata);

  return rata_rata;
}