#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define DHTPIN 4

DHT dht(DHTPIN,DHT11);

char ssid[] = "Sai Sachith";     // your network SSID (name)
char password[] = "123saisachith"; // your network key

#define BOTtoken "2097535350:AAGkApUs-WSz7g6knE1D1xqTYwE6UHRKenM" // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 200;
unsigned long lastTimeBotRan;

int LED1stat=0;
int LED2stat=0;

void handleNewMessages(int numNewMessages) 
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/LED1ON") {
      digitalWrite(5,HIGH);
      LED1stat= 1;
      bot.sendMessage(chat_id, "LED1 is HIGH", "");
    }

    if (text == "/LED1OFF") {
      LED1stat = 0;
      digitalWrite(5, LOW);
      bot.sendMessage(chat_id, "LED1 is LOW", "");
    }

    if (text == "/LED2ON") {
      digitalWrite(2,HIGH);
      LED2stat = 1;
      bot.sendMessage(chat_id, "LED2 is HIGH", "");
    }

    if (text == "/LED2OFF") {
      LED2stat = 0;
      digitalWrite(2, LOW);
      bot.sendMessage(chat_id, "LED2 is LOW", "");
    }

    if(text =="/TEMP")
    {
      Serial.print(temperature);
      bot.sendMessage(chat_id, "The temperature is: ",String(temperature));
    }
    if(text =="/HUMIDITY")
    {
      Serial.print(humidity);
      bot.sendMessage(chat_id,"The humidity is: ",String(humidity));
    }

    if (text == "/start") {
      String welcome = "ESP32 controller, " + from_name + ".\n";
      welcome += "/LED1ON : to switch-on the LED1\n";
      welcome += "/LED1OFF : to switch-off the LED1\n";
      welcome += "/LED2ON : to switch-on the LED2\n";
      welcome += "/LED2OFF : to switch-off the LED2\n";
      welcome += "/TEMP : to display the temperature\n";
      welcome += "/HUMIDITY: to display the humidity";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup() {
  Serial.begin(115200);

  client.setInsecure();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);

  delay(10);

  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);

}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}
