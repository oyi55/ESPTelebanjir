#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>


char ssid[] = "BUROOK";   
char password[] = "bancetkali"; 

  
const unsigned int TRIG_PIN=D4;
const unsigned int ECHO_PIN=D3;
int Buffer = 0;

#define BOTtoken "6915933653:AAHJYXLL_qFluJ_ihs_LFX9ENxGAAIr56rk"  
#define idAnda "1578154023"



WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


long bufKirimPesan;

int Bot_mtbs = 1500; 
long Bot_lasttime;  
int cm = 0;
String txtBuff = "";


int kirimPesan = 120000; // sesuaikan


int maxCm = 5; // sesuaikan (pembacaan maksimal air)
int minCm = 14; // sesuaikan (pembacaan minimal air)


int alarmHabis = 10; // sesuaikan (pesan air habis)
int alarmPenuh = 80; // sesuaikan (pesan air penuh)


int tinggiAir = 0;


bool flag1 = false;
bool flag2 = false;
bool debug = true;

void handleNewMessages(int numNewMessages) {

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    
    Serial.println(text);
    txtBuff = text;
  }
// masukan perintah disini


 if (txtBuff == "Cok"){
   bot.sendMessage("1578154023","Nih ! ketinggian Air Sekarang: "+ String(tinggiAir)  +  " Cm" , "");

 }
}

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
 
 
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  client.setFingerprint("F2 AD 29 9C 34 48 DD 8D F4 CF 52 32 F6 57 33 68 2E 81 C1 90");


  Serial.println("");
  Serial.println("Wifi Terhubung");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
 // bot.sendMessage(idAnda, "Online");
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
 
}

void loop() {


if (millis() > Bot_lasttime + Bot_mtbs) //1500
      {
        
        
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while(numNewMessages)
          {
            Serial.println("Ada Pesan masuk");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
          }


        // atur tampilan lcd sesuka kalian
      
      bacaJarak(10,2); // rata rata dan jeda ms
      

      Bot_lasttime = millis();
      
        
      
      }
 




  
  if (tinggiAir <= alarmHabis && flag1 == false){
    flag1 = true;
    flag2 = false;
   
    bot.sendMessage("1578154023","Pemberitahuan gan, Air Habis : "  + String(tinggiAir)  +  " Cm", "");
    

  }

  if (tinggiAir >= alarmPenuh && flag2 == false){
    flag2 = true;
    flag1 = false;
  
    bot.sendMessage("1578154023","Pemberitahuan gan, Air Penuh : "  + String(tinggiAir)  +  " Cm", "");
    
  }

kirimPesanOtomatis(kirimPesan);


}



int x = 0;
void  bacaJarak(int sample,int Delay){
 
 
 for (int x = 0; x < sample; x ++){
 
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  

 const unsigned long duration= pulseIn(ECHO_PIN, HIGH);
 int distance= duration/29/2;
 Buffer = Buffer + distance;
 delay(Delay);

 
  }
  
//Serial.println (Buffer/sample);
cm = Buffer/sample;



tinggiAir = minCm - cm;

if (debug == true){
 
Serial.print ("Reading : ");
Serial.println (cm);
 
}

Buffer = 0;



}


void kirimPesanOtomatis(long x){
if (millis() > bufKirimPesan + x)
{

    bot.sendMessage("1578154023","PESAN OTOMATIS , Ketinggian Airnya segini gan : "  + String(tinggiAir)  +  " Cm", "");

    bufKirimPesan = millis();
      
}

  
}