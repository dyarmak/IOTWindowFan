char buf[80];

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
}




void setup() {
    Serial.begin(115200);
}

void loop() {
    if (readline(Serial.read(), buf, 80) > 0) {
      if (strcmp(buf, "Fan A ON")  == 0){ 
        Serial.println("Turn on Fan A");
        
     }else if(strcmp(buf, "Fan B ON") == 0){
         Serial.println("Turn on Fan B");
         
     }else if(strcmp(buf, "FanA OFF") == 0){
         Serial.println("Turn off Fan A");
         
     }else if(strcmp(buf, "FanB OFF") == 0){
         Serial.println("Turn off Fan B");
         
     }else{
//        Serial.print("You entered: >");
//        Serial.print(buf);
//        Serial.println("<");
    }
    memset(buf,0,sizeof(buf));
    }


}
