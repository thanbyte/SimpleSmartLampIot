#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "";
const char* password = "";

#define BOTtoken ""
#define CHAT_ID ""

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int RED_LED = 27;
const int YELLOW_LED = 26;
const int GREEN_LED = 25;

bool KeadaanRed = LOW;
bool KeadaanYellow = LOW;
bool KeadaanGreen = LOW;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Selamat Data Lampu IOT (Than), " + from_name + ".\n";
      welcome += "Gunakan Command Yang Sesuai Dengan Kegunaan.\n\n";
      welcome += "/on (Menyalakan Sistem) \n";
      welcome += "/off (Mematikan  Sistem) \n";
      welcome += "/merahon & /merahoff (Menyalakan & Mematikan  Sistem) \n";
      welcome += "/kuningon & /kuningoff (Menyalakan & Mematikan  Sistem) \n";
      welcome += "/hijauon & /hijauoff (Menyalakan & Mematikan  Sistem) \n";
      welcome += "/keadaan (Mengambil Data Keadaan Sistem) \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/on") {
      bot.sendMessage(chat_id, "Menyalakan Sistem, " + from_name , "");
      KeadaanRed = HIGH;
      KeadaanYellow = HIGH;
      KeadaanGreen = HIGH;
    }
    
    if (text == "/off") {
      bot.sendMessage(chat_id, "Mematikan Sistem, " + from_name , "");
      KeadaanRed = LOW;
      KeadaanYellow = LOW;
      KeadaanGreen = LOW;
    }

    if (text == "/merahon") {
      bot.sendMessage(chat_id, "Menyalakan Lampu Merah, " + from_name , "");
      KeadaanRed = HIGH;
    }

    if (text == "/merahoff") {
      bot.sendMessage(chat_id, "Mematikan Lampu Merah, " + from_name , "");
      KeadaanRed = LOW;
    }

    if (text == "/kuningon") {
      bot.sendMessage(chat_id, "Menyalakan Lampu Kuning, " + from_name , "");
      KeadaanYellow = HIGH;
    }

    if (text == "/kuningoff") {
      bot.sendMessage(chat_id, "Mematikan Lampu Kuning, " + from_name , "");
      KeadaanYellow = LOW;
    }

    if (text == "/hijauon") {
      bot.sendMessage(chat_id, "Menyalakan Lampu Hijau, " + from_name , "");
      KeadaanGreen = HIGH;
    }

        if (text == "/hijauoff") {
      bot.sendMessage(chat_id, "Mematikan Lampu Hijau, " + from_name , "");
      KeadaanGreen = LOW;
    }
    
    if (text == "/keadaan") {
      if (KeadaanRed == HIGH & KeadaanYellow == HIGH & KeadaanGreen == HIGH){
        bot.sendMessage(chat_id, "Lampu Menyala Semua", "");
      }else if(KeadaanRed == LOW & KeadaanYellow == HIGH & KeadaanGreen == HIGH){
        bot.sendMessage(chat_id, "Merah Mati Lainnya Nyala", "");
      }else if(KeadaanRed == LOW & KeadaanYellow == LOW & KeadaanGreen == HIGH){
        bot.sendMessage(chat_id, "Merah Dan Kuning Mati Lainnya Nyala", "");
      }else if(KeadaanRed == HIGH & KeadaanYellow == LOW & KeadaanGreen == HIGH){
        bot.sendMessage(chat_id, "Hijau Dan Kuning Mati Lainnya Nyala", "");
      }else if(KeadaanRed == LOW & KeadaanYellow == HIGH & KeadaanGreen == LOW){
        bot.sendMessage(chat_id, "Merah Dan Hijau Mati Lainnya Nyala", "");
      }
      else{
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, KeadaanRed);
  digitalWrite(YELLOW_LED, KeadaanYellow);
  digitalWrite(GREEN_LED, KeadaanGreen);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      digitalWrite(RED_LED, KeadaanRed);
      digitalWrite(YELLOW_LED, KeadaanYellow);
      digitalWrite(GREEN_LED, KeadaanGreen);
    }
    lastTimeBotRan = millis();
  }
}


