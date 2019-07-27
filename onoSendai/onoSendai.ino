#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> 
#include <LiquidCrystal_I2C.h>


#include "index.h" //Our HTML webpage contents

// Constant for lcd size
#define LCDWIDTH 16
#define LCDHEIGHT 2

//SSID and Password for ESP8266
const char* ssid = "PartyHat";
const char* password = "" ;
const String s = MAIN_page; //Read HTML page

//Global Variables
int scrollingRow = LCDHEIGHT / 2;
int pinnedRow =  LCDHEIGHT / 2 - 1;
int scrollingSpeed = 400;
String ipAddress;


ESP8266WebServer server(80); //Server on port 80
LiquidCrystal_I2C lcd(0x27, 16, 2);

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  if ( server.hasArg ("userInput")){
    handlePostForm();
    Serial.println("new path worked");
    }
  else {
    server.send(200, "text/html", s); //Send web page
    }
}

void handlePostForm(){
  String userInput = server.arg("userInput");
  Serial.println("Text received. Contents: ");
  Serial.println(userInput); 
  lcd.clear();
  lcd.setCursor(0,0);
  //lcd.print("Text received. Contents: ");
  //lcd.setCursor(0,1);
  server.send(200, "text/html", s);
  pinAndScrollText(ipAddress, pinnedRow, userInput, scrollingRow, scrollingSpeed); 
  
}
void handleNotFound() {
  handleRoot();
}

/* 
 This routine is from Goet.  They posted it on the arduino forum at the following link:
  https://forum.arduino.cc/index.php?topic=216486.0  
   This procedure pins a given text in the center of a desired row while scrolling from right to left another given text on another desired row.
    Parameters:
    const String &pinnedText: pinned String
    int pinnedRow: desired row for pinned String
    const String &scrollingText: scrolling String
    int scrollingRow: desired row for scrolling String
    int v = scrolling speed expressed in milliseconds
*/
void pinAndScrollText(const String &pinnedText, int pinnedRow, const String &scrollingText, int scrollingRow, int v) {
  if (pinnedRow == scrollingRow || pinnedRow < 0 || scrollingRow < 0 || pinnedRow >= LCDHEIGHT || scrollingRow >= LCDHEIGHT || pinnedText.length() > LCDWIDTH || v < 0) {
    lcd.clear();
    lcd.print("Error");
    while (1);
  }
  int l = pinnedText.length();
  lcd.setCursor(l % 2 == 0 ? LCDWIDTH / 2 - (l / 2) : LCDWIDTH / 2 - (l / 2) - 1, pinnedRow);
  lcd.print(pinnedText);
  int x = LCDWIDTH;
  int n = scrollingText.length() + x;
  int i = 0;
  int j = 0;
  while (n > 0) {
    if (x > 0) {
      x--;
    }
    lcd.setCursor(x, scrollingRow);
    if (n > LCDWIDTH) {
      j++;
      i = (j > LCDWIDTH) ? i + 1 : 0;
      lcd.print(scrollingText.substring(i, j));
    } else {
      i = i > 0 ? i + 1 : 0;
      if (n == scrollingText.length()) {
        i++;
      }
      lcd.print(scrollingText.substring(i, j));
      lcd.setCursor(n - 1, scrollingRow);
      lcd.print(' ');
    }
    n--;
    if (n > 0) {
      delay(v);
    }
  }
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);
  Serial.println("Setting UP...");
  //WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
 
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  ipAddress = myIP.toString();
  Serial.print("HotSpt IP:");
  Serial.println(myIP);
  
  
  server.on("/", handleRoot);      //Which routine to handle at root location
  //server.on("/postForm", handlePostForm); //Handle a form that's posted

  //start DNS
  if (!MDNS.begin("powermeter")) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");


  server.begin();                  //Start server
  Serial.println("HTTP server started");


//============= set up LCD ====================================
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(myIP);
}  

//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
 
}
