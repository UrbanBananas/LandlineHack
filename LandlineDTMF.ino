#include <DTMF.h>
#include <string.h>

/*
  Phone Hack. Auto Pick up, DTMF detection, And Door Opening functions.

  Andrew Urbanas
  January 2015
 */

float n=128.0;
float sampling_rate=8926.0;
DTMF dtmf = DTMF(n,sampling_rate);


// the setup routine runs once when you press reset:
void setup() {
  pinMode(8, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(9, OUTPUT);
  Serial.begin(115200);
}

int nochar_count = 0;
float d_mags[8];

void loop() {
  char thischar = 0;
  char password[6] = {"69420"};
  char buffer[6];
  char zero[6] = {0, 0, 0, 0, 0, 0};
  int i=0;
  int currenttime = 0;
  int dumb=0;

  static double t_comp;
  double t_wait = 10000;
  t_comp = millis();

  if(analogRead(A0) < 300){
    digitalWrite(8, HIGH); //Pickup Phone
    while(1) {
      dtmf.sample(A1);
      dtmf.detect(d_mags,506);
      thischar = dtmf.button(d_mags,1800.);
      if(thischar == '#')
        break;
      if((thischar != 0) && (thischar != buffer[4])) {
        Serial.print(thischar); //Print current char for debug
        for(i=0;i<4;i++) {
           buffer[i] = buffer[i+1];
        }
        buffer[4] = thischar;
       }
      if ((millis() - t_comp) >= t_wait){ //10 sec timeout
        break;
      }

    }
    for(i=0;i<=5;i++)
      Serial.print(buffer[i]);
    buffer[5] = '\0';
    if (strcmp(buffer, password) == 0) {
      digitalWrite(9, HIGH);
    }
    delay(1000);
  }
  strcpy(buffer, zero);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}
