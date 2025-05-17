/*
 *
 *    This sketch is for the WiFi joystick
 *
 *    ----------
 *    Copyright (C) 2022 - PRESENT  Zhengyu Peng
 *    Website: https://zpeng.me
 *
 *    `                      `
 *    -:.                  -#:
 *    -//:.              -###:
 *    -////:.          -#####:
 *    -/:.://:.      -###++##:
 *    ..   `://:-  -###+. :##:
 *           `:/+####+.   :##:
 *    .::::::::/+###.     :##:
 *    .////-----+##:    `:###:
 *     `-//:.   :##:  `:###/.
 *       `-//:. :##:`:###/.
 *         `-//:+######/.
 *           `-/+####/.
 *             `+##+.
 *              :##:
 *              :##:
 *              :##:
 *              :##:
 *              :##:
 *               .+:
 *
 */

#include <WiFi.h>
#include <WiFiUdp.h>

#include <Adafruit_NeoPixel.h>

// GPIO pin number for the LED
#define PIN_RGB 8

// GPIO pin number for the joystick
#define PIN_X 34
#define PIN_Y 35
#define PIN_SW 4


// WiFi parameters
const char *ssid = "smartyrobot_cathy";
const char *password = "smartyrobot_cathy";
boolean connected = false;

// UDP
WiFiUDP udp;
const char *udpAddress = "192.168.4.1";
const int udpPort = 1234;

// PWM properties
const int pwmFreq = 5000;
const int pwmResolution = 8;

// joystick values
int valX = 0;
int valY = 0;
int valSw = 0;

// old joystick values
int valX_old = 0;
int valY_old = 0;
int valSw_old = 0;

Adafruit_NeoPixel rgbLed(1, PIN_RGB, NEO_GRB + NEO_KHZ800);
struct RGB {
    uint8_t r, g, b;
};

constexpr RGB COLOR_OFF   = {0, 0, 0};
constexpr RGB COLOR_RED   = {255, 0, 0};
constexpr RGB COLOR_GREEN   = {0, 255, 0};
constexpr RGB COLOR_BLUE   = {0, 0, 255};


void setColor(const RGB& color) {
    rgbLed.setPixelColor(0, rgbLed.Color(color.r, color.g, color.b));
    rgbLed.show();
}

void setup()
{
    // initilize hardware serial:
    Serial.begin(115200);
    delay(10);

    rgbLed.begin(); 
    rgbLed.show(); 

    // // configure joystick pin mode
    // pinMode(PIN_Y, ANALOG);
    // pinMode(PIN_X, ANALOG);

    // pinMode(PIN_SW, INPUT_PULLUP);

    // // configure LED PWM functionalitites
    // pinMode(PIN_POWER, OUTPUT);
    // pinMode(PIN_CONNECTED, OUTPUT);

    // ledcAttach(PIN_POWER, pwmFreq, pwmResolution);
    // ledcAttach(PIN_CONNECTED, pwmFreq, pwmResolution);

    // // blink LEDs
    // ledcWrite(PIN_POWER, 2);
    // ledcWrite(PIN_CONNECTED, 2);
    // delay(1000);

    // ledcWrite(PIN_CONNECTED, 0);

    // connect to the WiFi network
    connectToWiFi(ssid, password);

    setColor(COLOR_GREEN);
}



void loop()
{
//     valX = analogRead(PIN_X);
//     valY = analogRead(PIN_Y);
//     valSw = digitalRead(PIN_SW);

//     int temp_valX = floor(valX/64);
//     int temp_valY = floor(valY/64);

//     int temp_valX_old = floor(valX_old/64);
//     int temp_valY_old = floor(valY_old/64);


//     // only send UDP when the joystick is moved
//     if ((temp_valX != temp_valX_old) || (temp_valY != temp_valY_old) || (valSw != valSw_old))
//     {
//         valX_old = valX;
//         valY_old = valY;
//         valSw_old = valSw;
//         if (connected)
//         {
//             // ledcWrite(PIN_CONNECTED, 2);
//             // send joystick values to the UDP server
//             udp.beginPacket(udpAddress, udpPort);
//             udp.printf("X%d:Y%d:S%d:", valX, valY, valSw);
// //            udp.printf("X%d:Y%d:S%d:", temp_valX, temp_valY, valSw);
//             udp.endPacket();
//             // ledcWrite(PIN_CONNECTED, 0);
//         }
//     }

    delay(10);
}

void connectToWiFi(const char *ssid, const char *pwd)
{
    Serial.println("Connecting to WiFi network: " + String(ssid));

    // delete old config
    WiFi.disconnect(true);
    // register event handler
    WiFi.onEvent(WiFiEvent);

    // initiate connection
    WiFi.begin(ssid, pwd);

    Serial.println("Waiting for WIFI connection...");
}

// WiFi event handler
void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        // when connected set
        Serial.print("WiFi connected! IP address: ");
        Serial.println(WiFi.localIP());
        // initializes the UDP state
        // this initializes the transfer buffer
        udp.begin(WiFi.localIP(), udpPort);
        connected = true;
        // ledcWrite(PIN_YELLOW, 0);
        ledcWrite(PIN_CONNECTED, 2);
        
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        connected = false;
        ledcWrite(PIN_CONNECTED, 0);
        // ledcWrite(PIN_YELLOW, 2);
        // ledcWrite(PIN_POWER, 0);
        break;
    default:
        break;
    }
}
