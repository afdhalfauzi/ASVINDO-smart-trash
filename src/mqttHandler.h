#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoMqttClient.h>

WiFiClient espClient;
MqttClient client(espClient);

struct MQTT
{
    const char *ssid = "hotspot1";
    const char *password = "hotspothh";
    // const char *ssid = "BMZimages";
    // const char *password = "bennamazarina";
    const char *broker = "test.mosquitto.org";
    int port = 1883;
    const char *topic = "afd/test";

    bool begin();
    bool publish(int msg);
    bool loop();
};

bool MQTT::begin()
{
    Serial.print("Connecting to WiFI");
    WiFi.begin(ssid, password);
    int timeout_counter = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
        timeout_counter++;
        if (timeout_counter >= 10)
        {
            ESP.restart();
        }
    }
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (!client.connect(broker, port))
    {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(client.connectError());

        while (1)
            ;
    }
    Serial.println("You're connected to the MQTT broker!");
    Serial.println();
    return true;
}

bool MQTT::publish(int msg)
{
    client.beginMessage(topic);
    client.print(msg);
    client.endMessage();
    return true;
}

bool MQTT::loop()
{
    client.poll();
    return true;
}