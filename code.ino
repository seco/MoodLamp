// Library
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#define MOOD_NUM       7

// NeoPixel Macros
#define PIN            6  // Which pin on the Arduino is connected to the NeoPixels?
#define NUMPIXELS      1  // How many NeoPixels are attached to the Arduino?
// Color macros
#define OFF            0
#define RED            1
#define GREEN          2
#define BLUE           3

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// WiFi library
WiFiClient client;
IPAddress ip;

String reply;

// WiFi settings
const char* ssid = "wifi-name";
const char* password = "wifi-password";
const int httpPort = 80;
const char* host = "twitter.com";

// Time to sleep (in seconds)
const int sleepTimeS = 600; // 10 min

String search[] = {
  "GET /search.json?q=\"i+love+you\"+OR+\"i+love+her\"+OR+\"i+love+him\"+OR+\"all+my+love\"+OR+\"i'm+in+love\"+OR+\"i+really+love\"&rpp=30&result_type=recent",
  "GET /search.json?q=\"happiest\"+OR+\"so+happy\"+OR+\"so+excited\"+OR+\"i'm+happy\"+OR+\"woot\"+OR+\"w00t\"&rpp=30&result_type=recent",
  "GET /search.json?q=\"wow\"+OR+\"O_o\"+OR+\"can't+believe\"+OR+\"wtf\"+OR+\"unbelievable\"&rpp=30&result_type=recent",
  "GET /search.json?q=\"i+hate\"+OR+\"really+angry\"+OR+\"i+am+mad\"+OR+\"really+hate\"+OR+\"so+angry\"&rpp=30&result_type=recent",
  "GET /search.json?q=\"i+wish+i\"+OR+\"i'm+envious\"+OR+ \"i'm+jealous\"+OR+\"i+want+to+be\"+OR+\"why+can't+i\"+&rpp=30&result_type=recent",
  "GET /search.json?q=\"i'm+so+sad\"+OR+\"i'm+heartbroken\"+OR+\"i'm+so+upset\"+OR+\"i'm+depressed\"+OR+\"i+can't+stop+crying\"&rpp=30&result_type=recent",
  "GET /search.json?q=\"i'm+so+scared\"+OR+\"i'm+really+scared\"+OR+\"i'm+terrified\"+OR+\"i'm+really+afraid\"+OR+\"so+scared+i\"&rpp=30&result_type=recent",
}

const char* moodNames = {
 "love",
 "joy",
 "surprise",
 "anger",
 "envy",
 "sadness",
 "fear",
};

void led(int);

void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.

  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  // Print the IP address
  ip = WiFi.localIP();
}

void loop()
{
  // Use WiFiClient class to create TCP connections
  while (!client.connect(host, httpPort))
  {
    delay(500);
  }

  for(int i = 0; i < MOOD_NUM; i++)
  {
    // This will send the request to the server
    client.print(search[i] + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    
    delay(20);
    
    while (client.available()) 
    {
      reply = client.readStringUntil('\r');
    }
  }

  ESP.deepSleep(sleepTimeS * 1000000);
}

void led(int color) {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for(int i=0;i<NUMPIXELS;i++){
    
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    switch(color)
    {
      // color map - green red blue
      case RED:
        pixels.setPixelColor(i, pixels.Color(0,255,0));
      break;

      case GREEN:
        pixels.setPixelColor(i, pixels.Color(255,0,0));
      break;

      case BLUE:
        pixels.setPixelColor(i, pixels.Color(0,0,255));
      break;

      case OFF:
        pixels.setPixelColor(i, pixels.Color(0,0,0));
      break;
    }

    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}
