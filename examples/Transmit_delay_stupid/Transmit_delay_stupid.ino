#include <Simu_Remote_CC1101.h>


int Rollingcode0 = 0;
int Rollingcode1 = 0;
int ButtonID = 60;
int RemoteID1 = 160; 
int RemoteID2 = 40;
int Repeat = 0;


void setup(){
Serial.begin(115200);
simu.Init();
simu.enableTransmit(6);

}

void loop(){

if (Rollingcode1 == 255 && Rollingcode0 == 255){
Rollingcode0=0;
Rollingcode1=0;
}
else if(Rollingcode1 == 255){
Rollingcode0++;
Rollingcode1=0;
}
simu.transmit(Rollingcode0,Rollingcode1,ButtonID,RemoteID1,RemoteID2,Repeat);
Rollingcode1++;
delay(5000);

}
