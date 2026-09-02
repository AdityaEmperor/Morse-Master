#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27 , 16 , 2);

#define BLYNK_TEMPLATE_ID "TMPL36iIaRoJi"
#define BLYNK_TEMPLATE_NAME "arthritis"
#define BLYNK_AUTH_TOKEN "mPB2PsOrNPwewFK94b8JB8-a2VJlFnoG"
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT DebugSerial


#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#include <BlynkSimpleStream.h>


char auth[] = "mPB2PsOrNPwewFK94b8JB8-a2VJlFnoG";


void setup () {

  pinMode(13,OUTPUT);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  // Debug console
  DebugSerial.begin(9600);

  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  

  
}

char msg[16] = {' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' '} ;

char ch[]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

int index = 0 , i = 0;

int whichbuttonpress() {

  if(digitalRead(8)==LOW && digitalRead(9)==HIGH && digitalRead(10)==HIGH && digitalRead(11)==HIGH && digitalRead(12)==HIGH)
  {

    delay(200);
    return 5;
  }

  else if(digitalRead(9)==LOW && digitalRead(8)==HIGH && digitalRead(10)==HIGH && digitalRead(11)==HIGH && digitalRead(12)==HIGH)
  {
    delay(200);
    return 4;
  }

  else if(digitalRead(10)==LOW && digitalRead(8)==HIGH && digitalRead(9)==HIGH && digitalRead(11)==HIGH && digitalRead(12)==HIGH)
  {
    
    delay(200);
    return 3;
  }

  else if(digitalRead(11)==LOW && digitalRead(8)==HIGH && digitalRead(10)==HIGH && digitalRead(9)==HIGH && digitalRead(12)==HIGH)
  {
    
    delay(200);
    return 2;
  }

  else if(digitalRead(12)==LOW && digitalRead(8)==HIGH && digitalRead(10)==HIGH && digitalRead(11)==HIGH && digitalRead(9)==HIGH)
  {
    
    delay(200);
    return 1;
  }

  else 
  {
   
    return 0;
  }
  
    return 0;
  
    }

int k = 0;
void inputmsg(){
  for(k ; k != 5 ;) {
      Blynk.run();

        k = 0;

        if(msg[index] != ' ') {
          
          lcd.print(msg[index]);
        }

        else { 
          
          lcd.print(" ");
          
        }

        
          k=whichbuttonpress();
          
        
        
        switch(k) {

            case 1 : 
            if(index != 0) index=index-1;
            else index = 15;
            break;

            case 4 :
            if(index != 15) index=index+1;
            else index = 0;
            break;

            case 2 :
            if (i != 0) i=i-1;
            else i = 27;
            msg[index] = ch[i];
            break;

            case 3 :
            if (i != 27) i=i+1;
            else i = 0;
            msg[index] = ch[i];
            break;

            default : 
                      lcd.clear();
                      lcd.setCursor(0,0);
                      for(int i=0;i<16;i++){
                        if(msg[i] == ' ')
                        lcd.print("_");
                        else lcd.print(msg[i]);
                      }
                      delay(100);
           }
    }
}

char morse_map[26][6] = {
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--.."   // Z
    };


short int unit = 250;

void blink(char p) {
  if(p == '.'){
    digitalWrite(13,HIGH);
    Blynk.virtualWrite(V1,255);
    delay(unit);
    digitalWrite(13,LOW);
    Blynk.virtualWrite(V1,0);
    delay(unit);
  }
  
  else if(p=='-') {
    digitalWrite(13,HIGH);
    Blynk.virtualWrite(V1,255);
    delay(unit*3);
    digitalWrite(13,LOW);
    Blynk.virtualWrite(V1,0);
    delay(unit);
  }
}


void displaymsg() {
  Blynk.run();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);

  int limit ;
  for(int y=15 ; y>=0 ; y--) {
    if(msg[y] == ' ' && msg[y-1] != ' ') {
      limit = y-1;
      break;
    }
  }

  for(int j=0 ; j <= limit ; j++) {
        
    if(msg[j] >= 'a' && msg[j] <= 'z') {
      
      
      int m = msg[j] - 97;
      
        for(int n=0 ; n < sizeof(morse_map[m])/sizeof(morse_map[m][0]) ; n++){
            blink(morse_map[m][n]) ;
        }
      
      delay(2*unit); // 3 unit paus between each character
      
    }
    
        else if(msg[j] == ' ') 
        {
          delay(4*unit); // 7 unit pause for space , 3 already done , 1 for end of character , 3 in betwwn characters (1 of which was done by end of character) so only 4 remain
        }
    
        
    }
  
}

void loop() {
  inputmsg();
  Blynk.run();
  Blynk.virtualWrite(V2,msg);
  Blynk.virtualWrite(V2,"\n");
  displaymsg();
  delay(5000);
  
}
