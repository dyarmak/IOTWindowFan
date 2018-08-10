
//this function reads the states of all the buttons and stores them in the stateXxx variables

void buttonRead(){

stateTop = digitalRead(butFanTop);
stateBot = digitalRead(butFanBot);
stateOff = digitalRead(butOff);
if(stateTop == HIGH || stateBot == HIGH || stateOff == HIGH){
  mode = 1;
  previousButtonMillis = currentMillis;
  }
}
