//by Little_Satan

#include <Simu_Remote_CC1101.h>
#include <EEPROM.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Arduino.h>
#define pulse 650

int RxPin = 2;
int TxPin = 6;
int hwbtn = 0;

//Tranmit
int repeat;
bool prog = 0;
uint8_t code[10];
uint8_t code_save[10];

//Receive
int check_duration;
bool wait = 0;
bool received = 0;
int crc;
int checkcrc;
static unsigned long resetTime = 0;
bool Receive = 0;
static int interruptPin = 0;
int bit_count=0;
bool last_bit;
bool decoded_bin[80];
uint8_t decoded_sig[10];
uint8_t decoded_dec[10];
uint8_t encrypted_dec[10];

///////////////////////////////////////////////////////////
void Simu::Init(void){
ELECHOUSE_cc1101.Init();
ELECHOUSE_cc1101.setMHZ(433.42);
}
///////////////////////////////////////////////////////////
void Simu::enableReceive(int pin){
RxPin=pin;
pinMode(RxPin,INPUT);
interruptPin = digitalPinToInterrupt(RxPin);
ELECHOUSE_cc1101.SetRx();
attachInterrupt(interruptPin, handleInterrupt, CHANGE);
}
///////////////////////////////////////////////////////////
void Simu::disableReceive(void){
detachInterrupt(interruptPin);
}
///////////////////////////////////////////////////////////
void Simu::enableTransmit(int pin){
TxPin=pin;
pinMode(TxPin,OUTPUT);
}
///////////////////////////////////////////////////////////
void Simu::add_remote(uint8_t rc0,uint8_t rc1,uint8_t btn_id,uint8_t rem_id1,uint8_t rem_id2,int rem,int btn){
if (btn > 3){btn=3;}
rem*=14;
btn*=3;
esp_eeprom_begin();
if (rem_id1 != EEPROM.read(rem+12) || rem_id2 != EEPROM.read(rem+13)){
EEPROM.write(rem+12,rem_id1);
EEPROM.write(rem+13,rem_id2);
for (int i=0; i<4; i++){
EEPROM.write(rem+(i*3),0);
EEPROM.write(rem+(i*3+1),0);
EEPROM.write(rem+(i*3+2),0);
}
esp_eeprom_commit();
}
if (btn_id != EEPROM.read(rem+btn+2)){
EEPROM.write(rem+btn,rc0);
EEPROM.write(rem+btn+1,rc1);
EEPROM.write(rem+btn+2,btn_id);
esp_eeprom_commit();
}
}
///////////////////////////////////////////////////////////
void Simu::prg_transmit(void){
if(prog == 0){
prog=1;
}else{
prog=0;  
}
}
///////////////////////////////////////////////////////////
void Simu::prg_transmit(int rem,int btn){
code[2] = 0;
code[3] = 0;
esp_eeprom_begin();
code[4] = EEPROM.read(rem+btn+2);
code[5] = EEPROM.read(rem+12);
code[6] = EEPROM.read(rem+13);
EEPROM.write(rem+btn,0);
EEPROM.write(rem+btn+1,1);
esp_eeprom_commit();
}
///////////////////////////////////////////////////////////
void Simu::transmit(int rem,int btn,int rep){
if (btn > 3){btn=3;}
hwbtn=0;
rem*=14;
btn*=3;
esp_eeprom_begin();
code[2] = EEPROM.read(rem+btn);
code[3] = EEPROM.read(rem+btn+1);
code[4] = EEPROM.read(rem+btn+2);
code[5] = EEPROM.read(rem+12);
code[6] = EEPROM.read(rem+13);

if (code[2] == 255 && code[3] == 255){
EEPROM.write(rem+btn+1,0);
EEPROM.write(rem+btn,0);
}
else if (code[3] == 255){
EEPROM.write(rem+btn,code[2]+1);
EEPROM.write(rem+btn+1,0);
}else{
EEPROM.write(rem+btn+1,code[3]+1);
}
esp_eeprom_commit();
if (prog == 1){rep=25; prg_transmit(rem,btn);}
transmit(code[2],code[3],code[4],code[5],code[6],rep);
}
///////////////////////////////////////////////////////////
void Simu::btn_transmit(int rem,int btn,int hwb){
if (btn > 3){btn=3;}
hwbtn=hwb;
rem*=14;
btn*=3;
esp_eeprom_begin();
code[2] = EEPROM.read(rem+btn);
code[3] = EEPROM.read(rem+btn+1);
code[4] = EEPROM.read(rem+btn+2);
code[5] = EEPROM.read(rem+12);
code[6] = EEPROM.read(rem+13);


if (code[2] == 255 && code[3] == 255){
EEPROM.write(rem+btn+1,0);
EEPROM.write(rem+btn,0);
}
else if (code[3] == 255){
EEPROM.write(rem+btn,code[2]+1);
EEPROM.write(rem+btn+1,0);
}else{
EEPROM.write(rem+btn+1,code[3]+1);
}
esp_eeprom_commit();
if (prog == 1){prg_transmit(rem,btn);}
transmit(code[2],code[3],code[4],code[5],code[6],0);
}
///////////////////////////////////////////////////////////
void Simu::transmit(uint8_t rc0,uint8_t rc1,uint8_t btn_id,uint8_t rem_id1,uint8_t rem_id2,int rep){
repeat=rep;

code_save[2] = rc0;
code_save[3] = rc1;
code_save[4] = btn_id;
code_save[5] = rem_id1;
code_save[6] = rem_id2;
code_save[7] = 8;
code_save[8] = 196;
code_save[9] = 25;

set_crc_encrypt();

if (prog == 1){code[0] = 163;}

ELECHOUSE_cc1101.SetTx();

transmit_init();
transmit_sync1();
transmit_code();

btn_repeat();

prog = 0;
hwbtn=0;

ELECHOUSE_cc1101.SetRx();
}
///////////////////////////////////////////////////////////
void Simu::btn_repeat(void){
if (hwbtn == 0){
for (int i=0; i<=repeat; i++){
transmit_sync2(i+1);
code[7] += 4;
code[9] += 13;
transmit_code();
}
}else{
int i=0;
transmit_sync2(i+1);
code[7] += 4;
code[9] += 13;
transmit_code();
i++;
while (digitalRead(hwbtn) == HIGH){
if (i<60){
transmit_sync2(i+1);
code[7] += 4;
code[9] += 13;
transmit_code();
i++;
}else{
ELECHOUSE_cc1101.SetRx();
}
}
}
}
///////////////////////////////////////////////////////////
void Simu::set_crc_encrypt(void){
int calc = 8;

while(code[7]!=code_save[8]){

for (int i=0; i<10; i++){
code[i] = code_save[i];
}
int crc = 0;
code[7] = calc;
code[0] = 163;
code[1] = 0;
for (int i=0; i<10; i++){
crc = crc ^ code[i] ^ (code[i] >> 4);
}
crc &= 0b1111;
code[0] += 1;
code[1] = crc;
code[1] += 240;

for (int i=1; i<10; i++){
code[i] ^= code[i-1];
}
if (calc==127 && code[7]!=code_save[8]){
calc = 128;
}
else if (code[7]!=code_save[8]){
calc+=17;
}
}
}
///////////////////////////////////////////////////////////
void Simu::transmit_init(void){
digitalWrite(TxPin, HIGH); // High
delayMicroseconds(6500);
digitalWrite(TxPin, LOW); // Low
delay(20);
}
///////////////////////////////////////////////////////////
void Simu::transmit_sync1(void){
for (int i = 0; i < 12; i++){
digitalWrite(TxPin, HIGH); // High
delayMicroseconds(pulse*4);
digitalWrite(TxPin,LOW); // Low
delayMicroseconds(pulse*4);
}
digitalWrite(TxPin, HIGH); // PIN 2 HIGH
delayMicroseconds(4800);
digitalWrite(TxPin, LOW); // PIN 2 LOW
delayMicroseconds(pulse);
}
///////////////////////////////////////////////////////////
void Simu::transmit_sync2(int rep){

if (((code[79*rep / 8] >> (7 - (79 % 8))) & 1) == 1){
digitalWrite(TxPin, LOW);
delayMicroseconds(pulse*3);
}else{
digitalWrite(TxPin, LOW);
delayMicroseconds(pulse*2);
}
for (int i = 0; i < 6; i++){
digitalWrite(TxPin, HIGH); // High
delayMicroseconds(pulse*4);
digitalWrite(TxPin,LOW); // Low
delayMicroseconds(pulse*4);
}
digitalWrite(TxPin,HIGH); // Low
delayMicroseconds(4800);
digitalWrite(TxPin, LOW); // PIN 2 LOW
delayMicroseconds(pulse);
}
///////////////////////////////////////////////////////////
void Simu::transmit_code(void){
for (int i = 0; i < 80; i++){
if (((code[i / 8] >> (7 - (i % 8))) & 1) == 1){
digitalWrite(TxPin, LOW);
delayMicroseconds(pulse);
digitalWrite(TxPin, HIGH);
delayMicroseconds(pulse);
}else{    
digitalWrite(TxPin, HIGH);
delayMicroseconds(pulse);
digitalWrite(TxPin, LOW);
delayMicroseconds(pulse);   
}
}
}
///////////////////////////////////////////////////////////
void Simu::esp_eeprom_begin(void){
#ifdef ESP32 || ESP8266
EEPROM.begin(512);
#endif
}
///////////////////////////////////////////////////////////
void Simu::esp_eeprom_commit(void){
#ifdef ESP32 || ESP8266
EEPROM.commit();
#endif
}
///////////////////////////////////////////////////////////
bool Simu::receive(void){
if (bit_count >= 80){
convert_bit();
decrypt();
check_crc();
}
if (received == 1){
return(1);
}else{
return(0);
}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Simu::convert_bit(void){
int c = 0;
for (int i = 0; i<10; i++){
int calc = 128;
for (int i2 = 0; i2<8; i2++){
if (decoded_bin[c] == 1){
decoded_sig[i] += calc;
}
calc /= 2;
c++;
}
}
for (int i = 0; i<10; i++){
decoded_dec[i] = decoded_sig[i];
}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Simu::decrypt(void){
encrypted_dec[0] = decoded_dec[0];
for (int i = 0; i < 10; i++){
decoded_sig[i] ^= decoded_sig[i+1];
}
for (int i = 1; i < 10; i++){
encrypted_dec[i] = decoded_sig[i-1];
}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Simu::check_crc(void){
crc = 0;
checkcrc = encrypted_dec[1];
encrypted_dec[0]=163;
encrypted_dec[1]=0;
for (int i = 0; i < 10; i++){
crc = crc ^ encrypted_dec[i] ^ (encrypted_dec[i] >> 4);
}
crc &= 0b1111;

encrypted_dec[0] += 1;
encrypted_dec[1] = checkcrc;

if (checkcrc-240 == crc){
bit_count=0;
received=1;
}else{
received=0;
clear_received();
}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int Simu::received_code(int addr, bool c){
if(c == 0){
return(decoded_dec[addr]);
}else{
return(encrypted_dec[addr]);
}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Simu::clear_received(void){
received=0;
bit_count=0;
last_bit=0;
Receive = 0;
wait = 0;
for (int i = 0; i<10; i++){
decoded_sig[i] = 0;
decoded_dec[i] = 0;
encrypted_dec[i] = 0;
}
attachInterrupt(interruptPin, handleInterrupt, CHANGE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Simu::handleInterrupt() {
static unsigned long lastTime = 0;
const long time = micros();
const unsigned int duration = time - lastTime;

if (millis()-resetTime > 100){
bit_count=0;
last_bit=0;
Receive = 0;
wait = 0;
}
if (duration > 100){
if (duration + check_duration < 28000 && duration + check_duration > 25000 && Receive == 0){
Receive = 1;
check_duration = 0;
bit_count = 0;
}
if (Receive == 1){
if (duration> 1000 && duration<1600){
if (last_bit == 1){
decoded_bin[bit_count] = 0;
last_bit = 0;
bit_count++;
}else{
decoded_bin[bit_count] = 1;
last_bit = 1;
bit_count++;
}
}
if (duration > 300 && duration < 900 && wait == 0){
decoded_bin[bit_count] = last_bit;
bit_count++;
wait = 1;
}
else if (duration > 300 && duration < 900 && wait == 1){
wait = 0;
}
}
if (bit_count >= 80){
detachInterrupt(interruptPin);
}
}
check_duration = duration;
lastTime = time;
resetTime = millis();
}


Simu simu;
