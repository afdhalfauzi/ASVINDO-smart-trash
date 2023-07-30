#include <Arduino.h>
#include <CTBot.h>

CTBot myBot;

struct TeleBot
{
    String token = "5682195681:AAFBHWTpMVypES1Z_gU8GE_py-iWDXvtc0U";
    int userID = 747199885;

    bool begin();
    bool sendMessage(String msg);
};

bool TeleBot::begin()
{
    while (!myBot.testConnection())
    {
        myBot.setTelegramToken(token);
        delay(500);
    }
    return true;
}

bool TeleBot::sendMessage(String msg)
{
    myBot.sendMessage(userID, msg);
    return true;
}