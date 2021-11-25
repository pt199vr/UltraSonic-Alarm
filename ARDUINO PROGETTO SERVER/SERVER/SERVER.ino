#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
 
// Wifi network station credentials
#define WIFI_SSID "MSI"
#define WIFI_PASSWORD "Ciccioaka"
WiFiServer server(4080);
IPAddress staticIP(192,168,15,100);
IPAddress gateway(192, 168, 15, 122);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(157, 27, 0, 1);

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "2063169754:AAEF2t-bmWsFk7q7qP5xi-CWX6LUKCXvxcU" 
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

//String for packet
String string = "";
bool check = true;
String chatid = "";
 
void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);
 
  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    if(chatid == "") chatid = chat_id;
    String text = bot.messages[i].text;
 
    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";
 
    if (text == "/start" && check)
    {
      String welcome = "Alarm is ON, requested by " + from_name + ".\n";
      welcome += "/sleep : to switch the alarm OFF\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
      check = false;
      chatid = chat_id;
    }
    if(text == "/sleep" && string != "sleep")
    {
        String sleep = "Alarm is now OFF, requested by " + from_name +".\nTo switch it on digit: /start";
        bot.sendMessage(chatid, sleep, "");
        string = "sleep";
        check = true;
    }

    if(string != "sleep" && string != "")
    {
        bot.sendMessage(chatid, string, "");
    }    
  }
}
 
 
void setup()
{
  Serial.begin(115200);
  
  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

 if (!WiFi.config(staticIP, gateway, subnet, dns)) {
    Serial.println("STA Failed to configure");
  }

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
 
  server.begin();  
}
 
void loop()
{
  WiFiClient client = server.available();
  String s = "";
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  if(client)
    {
      Serial.println("Client Connected");
    //READING FROM CLIENT
    while (client.available()>0) {
      char c = static_cast<char>(client.read());
      s += c;
    }
    Serial.println(s);
    bot.sendMessage(chatid, s, "");
  }
  handleNewMessages(numNewMessages);
  if(string != "sleep")string = "";
}
