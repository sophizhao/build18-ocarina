#include <WiFi.h>
#include <WebServer.h>
#include "Adafruit_Soundboard.h"

// Choose any two pins that can be used with SoftwareSerial to RX & TX
#define SFX_TX 0
#define SFX_RX 1

// Connect to the RST pin on the Sound Board
#define SFX_RST 15

// WiFi credentials
const char* ssid = "CMU-DEVICE";

// Initialize variables for note and song
String note = "";  // Start with an empty note
String song = "";  // Song will hold concatenated notes
bool justChanged = false;
String html_image = "";
String songbook[13] = {
  "564564", // Zelda's Lullaby - LURLUR
  "354354", // Saria's Song - DRLDRL
  "645645", // Epona's Song - ULRULR
  "536536", // Sun Song - RDURDU
  "236236", // Storm Song - ADUADU
  "523523", // Song Of Time - RADRAD
  "536536", // Prelude of Light - URURLU
  "264545", // Minuet Of The Forest - AULRLR
  "32325353", // Bolero Of Fire - DADARDRD
  "13554", // Serenade Of Water - ADRRL
  "4552453", // Nocturne Of Shadow - LRRALRD
  "232532",  // Requiem Of Spirit - ADARDA
 "6633454512"};
String htmls[12] = {"https://i.pinimg.com/736x/71/14/f0/7114f0c406e390f385d0f31a42fc8df0.jpg","https://www.zeldadungeon.net/wiki/images/4/41/Ocarina-Sarias-Song.jpg",
"https://static.wikia.nocookie.net/zelda_gamepedia_en/images/a/a1/OoT3D_Epona_Model.png", "https://pixnio.com/free-images/2017/10/17/2017-10-17-06-04-14.jpg","https://www.zeldadungeon.net/wiki/images/2/2f/Ocarina-Song-of-Storms.jpg",
"https://static.wikia.nocookie.net/zelda_gamepedia_en/images/6/60/TP_Door_of_Time_Model.png/","https://static.wikia.nocookie.net/zelda/images/a/a1/Pedestal_of_Time_%28Ocarina_of_Time%29.png/",
"https://static.wixstatic.com/media/a078d7_a39b1862053f401688b2390badd6715b~mv2.png/v1/fill/w_560,h_336,al_c,lg_1,q_85,enc_auto/a078d7_a39b1862053f401688b2390badd6715b~mv2.png","https://static.wixstatic.com/media/a078d7_972c5ae9f3ec4c64adabf3edad2c8b47~mv2.jpg/v1/fill/w_1110,h_695,al_c,q_85,usm_0.66_1.00_0.01,enc_auto/a078d7_972c5ae9f3ec4c64adabf3edad2c8b47~mv2.jpg",
"https://i.pinimg.com/736x/ee/07/03/ee07036ab32566199c22e22ee414658a.jpg","https://static.wixstatic.com/media/a078d7_601f235182404ac1af75c081ee645abe~mv2.jpg/v1/fill/w_1024,h_800,al_c,q_85,enc_auto/a078d7_601f235182404ac1af75c081ee645abe~mv2.jpg",
"https://static.wixstatic.com/media/a078d7_5d374882821c403cba7390b63a1eb604~mv2.jpg/v1/fill/w_1110,h_566,al_c,q_85,usm_0.66_1.00_0.01,enc_auto/a078d7_5d374882821c403cba7390b63a1eb604~mv2.jpg"};

bool linkMode = false;
bool konami = false;

String lastN(String input, int n)
{
  return input.substring(input.length() - n);
}

bool compareSong(int i, String song){
  if (song.length() >= 10){
    String temp = lastN(song, 10);
    if(temp.equals(songbook[i])){ 
      Serial.println("konami true");
      konami = true; 
      return true;
    }
  }
  if (song.length() >= 8){
    String temp = lastN(song, 8);
    if(temp.equals(songbook[i])){ 
      html_image = htmls[i]; 
      return true;
    }
  }
  if (song.length() >= 6){
    String temp = lastN(song, 6);
    Serial.println(temp);
    if(temp.equals(songbook[i])){ 
      html_image = htmls[i]; 
      Serial.print(html_image);
      return true;
    }
  }
  if (song.length() >= 5){
    String temp = lastN(song, 5);
    if(temp.equals(songbook[i])){ 
      html_image = htmls[i]; 
      return true;
    }
  } 
  return false;
}

// Pin assignments
// const int notePin2 = 2;  // Pin 2 will represent note "2"
// const int notePin3 = 3;  // Pin 3 will represent note "3"

// Create a WebServer object on port 80
WebServer server(80);

// Function to update note based on the state of pins 2 and 3
// void updateNote() {
//   // Read the state of pin 2 and pin 3
//   bool pin2State = digitalRead(2);
//   bool pin3State = digitalRead(3);

//   // Set note based on which pin is LOW


//   // If note is set, add it to the song
//   if (note != "") {
//     song += note + " ";
//   }
// }

// Function to generate the HTML page
String getPageHTML() {
  String html = "<html><head>";
  
  // Adding some basic styles to center content
  html += "<style>";
  html += "body {";
  html += "  background-image: url(https://cdn.mos.cms.futurecdn.net/avbK7Yb8hm3wzeTY3hBjyj.jpg);";
  html += "  background-repeat: no-repeat;";
  html += "  background-size: cover;";
  html += "  text-align: center;"; // Center all text
  html += "  font-family: Arial, sans-serif;"; // Font styling
  html += "  color: black;"; // Text color
  html += "  padding: 0;";
  html += "  margin: 0;";
  html += "  height: 100vh;"; // Make the body take the full viewport height
  html += "}";
  
  html += "h1 {";
  html += "  font-size: 3em;";
  html += "  margin-top: 20px;";
  html += "  text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5);"; // Optional: Add shadow to the header
  html += "}";
  
  html += "p {";
  html += "  font-size: 1.5em;";
  html += "  margin: 10px 0;";
  html += "}";
  
  html += "#note, #song {";
  html += "  font-weight: bold;";
  html += "  font-size: 1.5em;";
  html += "}";
  
  html += "#html_image {";
  html += "  max-width: 90%;"; // Ensure the image doesn't stretch too much
  html += "  max-height: 400px;"; // Limit the image height
  html += "  margin-top: 20px;";
  html += "}";

  html += "</style></head>";
  
  html += "<body>";
  html += "<h1>Ocarina of Time</h1>";
  html += "<p>Current Note: <b id='note'>" + note + "</b></p>";
  html += "<p>Song: <b id='song'>" + song + "</b></p>";
  html += "<img id='html_image' src=\"" + html_image + "\">";

  // JavaScript to refresh both note and song every 1 second (1000ms)
  html += "<script>";
  html += "function refreshValues() {";
  html += "  fetch('/getValues').then(response => response.json()).then(data => {";
  html += "    document.getElementById('note').innerText = data.note;";
  html += "    document.getElementById('song').innerText = data.song;";
  html += "    document.getElementById('html_image').src = data.html_image;";
  html += "  });";
  html += "}";

  // Periodic updates every 1 second (1000ms)
  html += "setInterval(refreshValues, 500);";  // 1000ms = 1 second

  html += "</script>";
  html += "</body></html>";
  
  return html;
}


// Endpoint to fetch the current note and the song
String getValues() {

  // Create a JSON-like response with both values
  String response = "{\"note\":\"" + note + "\",";
  response += "\"song\":\"" + song + "\",";
  response += "\"html_image\":\"" + html_image + "\"}";

  return response;
}

Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);


void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Initialize pin modes for notePin2 and notePin3
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(SFX_RST, INPUT);

  Serial.println("Adafruit Sound Board!");

  // softwareserial at 9600 baud
  Serial1.begin(9600);
  // can also do Serial1.begin(9600)

  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1)
      ;
  }
  Serial.println("SFX board found");

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Handle root ("/") route for the main page
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getPageHTML());
  });

  // Handle /getValues route to return both note and song
  server.on("/getValues", HTTP_GET, []() {
    String response = getValues();
    server.send(200, "application/json", response);  // Send JSON response
  });

  // Start the server
  server.begin();
}

void loop() {
  // Continuously handle incoming requests
  if (digitalRead(2) == LOW) {
    uint8_t num = 1;
    note = "1";
    if (!justChanged) {
      song += note;
      if(linkMode){
        num += 8;
      }
      sfx.playTrack(num);
      for (int i = 0; i < 13; i++) { 
        if (compareSong(i,song)){
          song = "";
        }
      } 
    }
    justChanged = true;
    server.handleClient();
  } else if (digitalRead(3) == LOW) {
    uint8_t num = 2;
    note = "2";
    if (!justChanged) {
      song += note;
      if(linkMode){
        num += 8;
      }
      sfx.playTrack(num);
      for (int i = 0; i < 13; i++) { 
        if (compareSong(i,song)){
          song = "";
        }
      } 
    }
    justChanged = true;
    server.handleClient();
  } else if (digitalRead(4) == LOW) {
    uint8_t num = 3;
    note = "3";
    if (!justChanged) {
      song += note;
      if(linkMode){
        num += 8;
      }
      sfx.playTrack(num);
      for (int i = 0; i < 13; i++) { 
        if (compareSong(i,song)){
          song = "";
        }
      } 
    }
    justChanged = true;
    server.handleClient();
  } else if (digitalRead(5) == LOW) {
    uint8_t num = 4;
    note = "4";
    if (!justChanged) {
      song += note;
      if(linkMode){
        num += 8;
      }
      sfx.playTrack(num);
      for (int i = 0; i < 13; i++) { 
        if (compareSong(i,song)){
          song = "";
        }
      } 
    }
    justChanged = true;
    server.handleClient();
  } else if (digitalRead(6) == LOW) {
    uint8_t num = 5;
    note = "5";
    if (!justChanged) {
      song += note;
      if(linkMode){
        num += 8;
      }
      sfx.playTrack(num);
      for (int i = 0; i < 13; i++) { 
        if (compareSong(i,song)){
          song = "";
        }
      } 
    }
    justChanged = true;
    server.handleClient();
  } else if (digitalRead(7) == LOW) {
    uint8_t num = 6;
    note = "6";
    if (!justChanged) {
      song += note;
      if(linkMode){
        num += 8;
      }
      sfx.playTrack(num);
      for (int i = 0; i < 13; i++) { 
        if (compareSong(i,song)){
          song = "";
        }
      } 
    }
    justChanged = true;
    server.handleClient();
  } else if (digitalRead(8) == LOW) {
    uint8_t num = 7;
    note = "7";
    if (!justChanged) {
      song += note;
      if(linkMode){
        num += 8;
      }
      sfx.playTrack(num);
      for (int i = 0; i < 13; i++) { 
        if (compareSong(i,song)){
          song = "";
        }
      } 
    }
    justChanged = true;
    server.handleClient();
  } else if (digitalRead(9) == LOW) {
    uint8_t num = 8;
    note = "8";
    if (!justChanged) {
      song += note;
      if(linkMode){
        num += 8;
      }
      sfx.playTrack(num);
      for (int i = 0; i < 13; i++) { 
        if (compareSong(i,song)){
          song = "";
        }
      } 
    }
    justChanged = true;
    server.handleClient();
  } else if (digitalRead(10) == LOW) {
    song += "S";
    if(konami) {
      if(linkMode) {
        linkMode = false;
      } else {
        Serial.println("Link mode");
        linkMode = true;
      }
    }
    justChanged = true;
    server.handleClient();
  } else {
    justChanged = false;
    server.handleClient();
  }
  delay(50);
}

