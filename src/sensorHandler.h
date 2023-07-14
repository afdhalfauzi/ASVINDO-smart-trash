#pragma once
#include <Arduino.h>

struct IRsensor
{
    int pinSensor;

    IRsensor(int pinSensor_)
    {
        pinSensor = pinSensor_;
    } // constructor

    bool begin();
    bool getRead();
};

bool IRsensor::begin()
{
    pinMode(IRsensor::pinSensor, INPUT);
    return true;
}

bool IRsensor::getRead()
{
    return digitalRead(IRsensor::pinSensor);
}

///////////////////////////////// ultrasonic /////////////////////////////////

struct Ping
{
    int ECHOpin;
    int TRIGpin;
    float distance;

    Ping(int ECHOpin_, int TRIGpin_)
    {
        ECHOpin = ECHOpin_;
        TRIGpin = TRIGpin_;

    } // constructor

    bool begin();
    float readDistanceCM();
};

bool Ping::begin()
{
    pinMode(Ping::ECHOpin, INPUT);
    pinMode(Ping::TRIGpin, OUTPUT);
    return true;
}

float Ping::readDistanceCM()
{
    digitalWrite(TRIGpin, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGpin, LOW);
    int duration = pulseIn(ECHOpin, HIGH);
    return duration * 0.034 / 2;
}
