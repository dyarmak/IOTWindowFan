// --- *** IOT Window Fan Control Sketch for Arduino *** --- ///
/* 
 *  This sketch will control the fan state from: 
 *  a temperature difference (inside vs out)
 *  OR
 *  a manual button press
 *  OR
 *  a website via an ESP8266
 *  
  *  available values for mode / switch 
  *  0 = default or temp mode
  *  1 = physical button mode
  *  2 = websockets mode
 *  
 * After a mode change, the time is logged, and once an interval has elapsed, return to default / mode = 0
 */

/*  Outside temp sensor is a DS18B20 communicating via OneWire on pin 8
 *  See DS18bTempFunction.ino for DS18B20SetupFunction() and tempRead();
 */

// ------Constants------ //
// Input pin assignments for the buttons
  const int butFanTop = 2;   //pin 2 is top button, used to turn ON top fans 
  const int butFanBot = 3;   //pin 3 is middle button, used to turn ON bottom fans
  const int butOff    = 4;   //pin 4 is bottom button, turns OFF all fans
 // temperature set point
  const int setTemp = 25; 
 // Output pin assignments for fans
  const int fanTop = 7;          //pin 7 in output for top Fan array
  const int fanBot = 6;         //pin 6 is output for bottom Fan array
  const int fanBox = 5;         // pin 5 is output for electronics box Fan

// ------Variables------ //
// timing variables used for timing
unsigned long currentMillis = 0;              // stores the value of millis() in each iteration of loop()
unsigned long previousTempRead = 0;           // store the value of millis last time temp was read
unsigned long previousButtonMillis = 0;       // stores value of millis last time a button was pressed
unsigned long previousWebsocketsMillis = 0;    // stores value of millis last time a websockets was pressed

unsigned long  halfMinInterval = 30000;   //60,000 ms = 1 min
unsigned long  tenMinInterval = 600000;  // 600,000 ms = 10 min
unsigned long  hourInterval   = 3600000; // 3,600,000 ms = 1 hour


// mode variable for switch 
  int mode = 0;

// current outside temp variable
  float outCelsius = 0;

// button state variables
  int stateTop = 0;
  int stateBot = 0;
  int stateOff = 0;

// Character array for for Serial.read()
char buf[80];
//Function for reading text from Serial

int readline(int readch, char *buffer, int len) {
    static int pos = 0;
    int rpos;

    if (readch > 0) {
        switch (readch) {
            case '\r': // Ignore CR
                break;
            case '\n': // Return on new-line
                rpos = pos;
                pos = 0;  // Reset position index ready for next time
                return rpos;
            default:
                if (pos < len-2) {
                    buffer[pos++] = readch;
                    buffer[pos] = 0;
                }
        }
    }
    return 0;
} //End of readline

void setup() {
//Set button pins as inputs
  pinMode(butFanTop, INPUT);
  pinMode(butFanBot, INPUT);
  pinMode(butOff, INPUT); 
//Set fan pins as outputs
  pinMode(fanTop, OUTPUT);
  pinMode(fanBot, OUTPUT);

//initialize serial com port  
  Serial.begin(115200);
//initialize DS18b20 temp probe
  DS18B20SetupFunction();
  delay(250);
//read the temp before starting loop
  tempRead();
  currentMillis = millis();
  previousTempRead = currentMillis;
}

void loop() {
  
  // **--- TO DO ---** //
  /* - interface with ESP8266 over serial Pin 0 and Pin 1
   * - Websocket controls
   * - send temp data and fan states to ESP.
   * - data logging
   * - add Nokia or other display?
   */
  
//store current millis 
  currentMillis = millis();

//check buttons and store in stateXxx variable
  buttonRead();


// ----- ***** WebSockets ***** ----- //

//check Serial for websocket input
// if(Serial data){
//   readWebSocket();
//   mode = 2;
//   previousWebsocketsMillis = currentMillis; //set websocket timer
//



// ** mode switch statement ** //
  switch(mode){
    case 0:    //Default - Temp mode
/* need to add compare outside to inside temp once I have the BME280 sensor and have done some testing
 * if(inCelsius > outCelsius && inCelsius > setTemp)
 * something like the above
 */ 
       if(outCelsius > setTemp){
        digitalWrite(fanTop,HIGH);
        digitalWrite(fanBot,HIGH);
      }else if(outCelsius < (setTemp-2) ){
        digitalWrite(fanTop,LOW);
        digitalWrite(fanBot,LOW);
      }
      break;
    case 1:
        if(stateTop == HIGH){
          digitalWrite(fanTop, stateTop);
          stateTop = LOW;
        }else if(stateBot == HIGH){
          digitalWrite(fanBot, stateBot);
          stateBot = LOW;
        }else if(stateOff == HIGH){
          digitalWrite(fanTop, LOW);
          digitalWrite(fanBot, LOW);
        }
//buton mode timer
    if( (currentMillis - previousButtonMillis) > hourInterval){
        //set back to default/temp mode and turn off fans
        mode = 0;
        digitalWrite(fanTop, LOW);
        digitalWrite(fanBot, LOW);
      }

      break;
    case 2: //Change fan states according to ESP input
    
    if( (currentMillis - previousWebsocketsMillis) > hourInterval){
        //set back to default/temp mode and turn off fans
        mode = 0;
        digitalWrite(fanTop, LOW);
        digitalWrite(fanBot, LOW);
      }
      break;
    }

// ------ Temp read and Log------ //
// read temp and logData once per minute
     if( (currentMillis - previousTempRead) > halfMinInterval ){
       tempRead();
       //logData();
       //displayTemp();
       //sendTempToESP();
       previousTempRead = currentMillis;  
       }


  // **ADD** Send temp to ESP

  
  // **ADD** Display Temp on display

}

void serialEvent(){
    if (readline(Serial.read(), buf, 80) > 0) {
      if (strcmp(buf, "Fan A ON")  == 0){ 
        Serial.println("Turn on Fan A");
        digitalWrite(fanTop, HIGH);
        mode = 2;
        previousWebsocketsMillis = currentMillis;
     }else if(strcmp(buf, "Fan B ON") == 0){
         Serial.println("Turn on Fan B");
         digitalWrite(fanBot, HIGH);
         mode = 2;
         previousWebsocketsMillis = currentMillis;
     }else if(strcmp(buf, "Fan A OFF") == 0){
         Serial.println("Turn off Fan A");
         digitalWrite(fanTop, LOW);
         mode = 2;
         previousWebsocketsMillis = currentMillis;
     }else if(strcmp(buf, "Fan B OFF") == 0){
         Serial.println("Turn off Fan B");
         digitalWrite(fanBot, LOW);
         mode = 2;
         previousWebsocketsMillis = currentMillis;
     }else{
//        Serial.print("You entered: >");
//        Serial.print(buf);
//        Serial.println("<");
    }
    memset(buf,0,sizeof(buf));
    }
  
  }//End SerialEvent()




