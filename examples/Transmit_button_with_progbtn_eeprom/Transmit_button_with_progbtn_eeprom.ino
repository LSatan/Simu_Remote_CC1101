#include <Simu_Remote_CC1101.h>

// push button attached to pin from + 5V
// for Esp8266/Esp32 +3,3V
// 10K resistor attached to pin from ground

int Led = 9;      //Led pin 9
int Button0 = 3;  //Button0 pin 3
int Button1 = 4;  //Button1 pin 4
int Button2 = 5;  //Button2 pin 5
int Button3 = 7;  //Button3 pin 7
int ProgBtn = 8;  //Programming button pin 7

bool prg = 0;
bool mode = 0;

static unsigned long led_timer;

void setup(){
Serial.begin(115200);

pinMode(Button0,INPUT);
pinMode(Button1,INPUT);
pinMode(Button2,INPUT);
pinMode(Button3,INPUT);
pinMode(ProgBtn,INPUT);
pinMode(Led,OUTPUT);

simu.Init();
simu.enableTransmit(6);

//add a Remote to eeprom for 4 buttons.
//The variables are only set at the first start.
//By changing Remote Id1 or Id2, the data in the eeprom are overwritten.
//Every button has its own rolling code counter and ID.
//Remote ID1 and ID2 must be the same for every remote control.
//If you want to use different remote IDs, you have to use a different remote number.
//An eeprom of 512 bytes of memory can store up to 36 remote controls with 4 buttons each.
simu.add_remote(0,0,60,160,40,0,0); // add the 1th button (Begin of Rolling code 0 = 0, Begin of Rolling code 1 = 0, Button ID, Remote ID1 = 60, Remote_ID2, Store to place(Remote number) = 0, Button number = 0)
simu.add_remote(0,0,61,160,40,0,1); // add the 2th button to remote 0. 
simu.add_remote(0,0,62,160,40,0,2); // add the 3th button to remote 0.
simu.add_remote(0,0,63,160,40,0,3); // add the 4th button to remote 0.
}

void loop(){

if (digitalRead(Button0) == HIGH){
digitalWrite(Led, HIGH);
simu.btn_transmit(0,0,Button0);// (Transmit stored remote 0, Button 0, Hardware Button0)
prg=0;
mode=0;
}
else if (digitalRead(Button1) == HIGH){
digitalWrite(Led, HIGH);
simu.btn_transmit(0,1,Button1);// (Transmit stored remote 0, Button 1, Hardware Button1)
prg=0;
mode=0;
}
else if (digitalRead(Button2) == HIGH){
digitalWrite(Led, HIGH);
simu.btn_transmit(0,2,Button2);// (Transmit stored remote 0, Button 2, Hardware Button2)
prg=0;
mode=0;
}
else if (digitalRead(Button3) == HIGH){
digitalWrite(Led, HIGH);
simu.btn_transmit(0,3,Button3);// (Transmit stored remote 0, Button 3, Hardware Button3)
prg=0;
mode=0;
}
else if (digitalRead(ProgBtn) == HIGH && prg == 0 && mode == 0){
prg=1;
mode=1;
simu.prg_transmit(); // Switches to programming mode.
}
else if (digitalRead(ProgBtn) == HIGH && prg == 1 && mode == 0){
prg=0;
mode=1;
simu.prg_transmit(); //Switches to normal mode.
}
else if (digitalRead(ProgBtn) == LOW && mode == 1){
mode=0;
}
if (prg == 1){
if (millis()-led_timer < 200){
digitalWrite(Led, LOW);
}
else if (millis()-led_timer < 400){
digitalWrite(Led, HIGH);
}
else if (millis()-led_timer > 400){
led_timer = millis();
}
}else{
digitalWrite(Led, LOW);
}
}
