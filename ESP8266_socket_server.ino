#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);


int showSerial = false;

static const char PROGMEM INDEX_HTML[] = R"rawliteral(


<!DOCTYPE html>
<html>

<head>
<script>


var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
 connection.onopen = function () {  connection.send('Connect ' + new Date()); };
 connection.onerror = function (error) {    console.log('WebSocket Error ', error);};
 connection.onmessage = function (e) {  console.log('Server: ', e.data);};

</script>
 
</head>

<body>

<button id="FAN" onclick="butts(this)"> fan a on  </button>
<button id="FAF" onclick="butts(this)"> fan a off </button>
<button id="FBN" onclick="butts(this)"> fan b on  </button>
<button id="FBF" onclick="butts(this)"> fan b off </button>

</body>

<script>

function butts(b){
  connection.send(b.id);
  }

</script>

</html>



)rawliteral";



void setup() {
    Serial.begin(115200);



    
    //USE_SERIAL.begin(921600);
    //USE_SERIAL.begin(115200);

    //USE_SERIAL.setDebugOutput(true);

if(showSerial){
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
}
    
    for(uint8_t t = 4; t > 0; t--) {
        //USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

  WiFiMulti.addAP("WiFiSSID", "PASSWORD");



    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
        if(showSerial){
          //USE_SERIAL.println ( "" );
        }
    }

    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    if(MDNS.begin("esp8266") && showSerial) {
        USE_SERIAL.println("MDNS responder started");
        USE_SERIAL.print ( "IP address: " );
        USE_SERIAL.println ( WiFi.localIP() );
        USE_SERIAL.println("ssid: ");
        USE_SERIAL.println(WiFi.SSID());
    }

    // handle index
    server.on("/", []() {
    server.send_P(200, "text/html", INDEX_HTML);
    });

    server.begin();

    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

   

}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:

        //code here runs on websocket disconnect
            
        if(showSerial){
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
        }
            Serial.print(0);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            if(showSerial){
            //USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            }
            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            //USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);


      if(payload[0] == 'F' && payload[1] == 'A' && payload[2] == 'N'){
        Serial.println("Fan A ON");
          
        }
      if(payload[0] == 'F' && payload[1] == 'A' && payload[2] == 'F'){
        Serial.println("Fan A OFF");
          
        }
      if(payload[0] == 'F' && payload[1] == 'B' && payload[2] == 'N'){
        Serial.println("Fan B ON");
          
        }
      if(payload[0] == 'F' && payload[1] == 'B' && payload[2] == 'F'){
        Serial.println("Fan B OFF");
          
        }

       break; //end of switch
      }
      
            
} //webSocketEvent


void loop() {
    webSocket.loop();
    server.handleClient();

      }

