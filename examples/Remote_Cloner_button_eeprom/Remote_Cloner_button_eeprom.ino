#include <Simu_Remote_CC1101.h>

// push button attached to pin from + 5V
// for Esp8266/Esp32 +3,3V
// 10K resistor attached to pin from ground

int RX_Pin = 2; // RX GDO2 on Pin2
int TX_Pin = 6; // TX GDO0 on Pin6

int Button[5] = {
3,  //Transmit BTN0 on Pin3
4,  //Transmit BTN1 on Pin4
5,  //Transmit BTN2 on Pin5
7,  //Transmit BTN3 on Pin7
8   //Mode change BTN4 on Pin8
};

int Led = 9; // Led on Pin9

bool mode=0;
bool Switch=0;

bool receive_state = 0;

int Time_LED = 0;
static unsigned long LED_Timer;

void setup(){
Serial.begin(115200);
pinMode(Led,OUTPUT);
for (int i = 0; i<5; i++){
pinMode(Button[i],INPUT);
}
simu.Init();
simu.enableTransmit(TX_Pin);
}

void loop(){
if (digitalRead(Button[4]) == HIGH && Switch == 0 && mode == 0){
mode = 1;
Switch = 1;
}
if (digitalRead(Button[4]) == HIGH && Switch == 0 && mode == 1){
mode = 0;
Switch = 1;
}
if (digitalRead(Button[4]) == LOW && Switch == 1){
Switch = 0;
}

if (mode == 1){

if (receive_state == 0){
simu.enableReceive(RX_Pin);
receive_state = 1;
}
if (simu.receive() == 1){
int r[5];
int z=0;
for (int i = 0; i<4; i++){
if (digitalRead(Button[i]) == HIGH){
for (int c = 2; c<7; c++){
r[z]=simu.received_code(c,1);
z++;
}
simu.add_remote(r[0],r[1]+1,r[2],r[3],r[4],i,0);
simu.clear_received();
for (int l=0; l < 10; l++){
digitalWrite(Led, HIGH);
delay(100);
digitalWrite(Led, LOW);
delay(100);
}
}
}
Time_LED = 500;
}else{
Time_LED = 2000;
}

}else{

if (receive_state == 1){
simu.disableReceive();
simu.clear_received();
receive_state = 1;
Time_LED = 0;
}

for (int i = 0; i<4; i++){
if (digitalRead(Button[i]) == HIGH){
digitalWrite(Led, HIGH);
simu.btn_transmit(i,0,Button[i]);
}
}
digitalWrite(Led, LOW);
}

if (Time_LED != 0){
if (millis()-LED_Timer < Time_LED/2){
digitalWrite(Led,LOW);
}
else if (millis()-LED_Timer > Time_LED/2){
digitalWrite(Led,HIGH);
}
if (millis()-LED_Timer >= Time_LED){
LED_Timer = millis();
}
}
}
