#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void setup_telegram(){
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  // send message to telegram on startup
  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

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

    if (text == "/reset") {
      String welcome = "Hallo, " + from_name + ".\n";
      welcome += "Silahkan atur jarak ketinggian air\n\n";
      bot.sendMessage(CHAT_ID, welcome, "");
//      maxWater = distance;
      maxWater = write_storage(distance);
      String set_distance = "Ketinggian air sudah diatur : " + String(distance) + ".\n";
      bot.sendMessage(CHAT_ID, set_distance, "");
    }
    
    if (text == "/info") {
      String information = "Ketinggian air maksimal : " + String(maxWater) + ".\n";
      information += "Ketinggian air saat ini : " + String(distance) + ".\n";
      information += "Baterai voltase : " + String(analogRead(voltagePin)) + "V.\n";
      String selenoidStatus;
      if(selenoid_status){
        selenoidStatus = "On";
      }else{
        selenoidStatus = "Off";
      }
      information += "Selenoid status : " + selenoidStatus + ".\n";
      bot.sendMessage(CHAT_ID, information, "");
    }
  }
}

void loop_telegram(){
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
