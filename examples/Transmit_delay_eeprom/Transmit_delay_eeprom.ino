#include <Simu_Remote_CC1101.h>

void setup(){
Serial.begin(115200);
simu.Init();
simu.enableTransmit(6);

//add a Remote to eeprom for 4 buttons.
//The variables are only set at the first start.
//By changing Remote Id1 or Id2, the data in the eeprom are overwritten.
//Every button has its own rolling code counter and ID.
//Remote ID1 and ID2 must be the same for every remote control.
//If you want to use different remote IDs, you have to use a different remote number.
//An eeprom of 512 bytes of memory can store up to 36 remote controls with 4 buttons each.
simu.add_remote(0,0,60,160,40,0,0); // add the 1th button to remote 0. (Begin of Rolling code0 = 0, Begin of Rolling code1 = 0, Button ID = 60, Remote ID1 = 160, Remote_ID2 = 40, Store to place(Remote number) = 0, Button number = 0)
simu.add_remote(0,0,61,160,40,0,1); // add the 2th button to remote 0. (Begin of Rolling code0 = 0, Begin of Rolling code1 = 0, Button ID = 61, Remote ID1 = 160, Remote_ID2 = 40, Store to place(Remote number) = 0, Button number = 1)
simu.add_remote(0,0,62,160,40,0,2); // add the 3th button to remote 0. (Begin of Rolling code0 = 0, Begin of Rolling code1 = 0, Button ID = 62, Remote ID1 = 160, Remote_ID2 = 40, Store to place(Remote number) = 0, Button number = 2)
simu.add_remote(0,0,63,160,40,0,3); // add the 4th button to remote 0. (Begin of Rolling code0 = 0, Begin of Rolling code1 = 0, Button ID = 63, Remote ID1 = 160, Remote_ID2 = 40, Store to place(Remote number) = 0, Button number = 3)
}

void loop(){


simu.transmit(0,0,0);// (Transmit stored remote 0, Button 0, 0 Repeats)
delay(10000);
simu.transmit(0,1,0);// (Transmit stored remote 0, Button 1, 0 Repeats)
delay(10000);
simu.transmit(0,2,0);// (Transmit stored remote 0, Button 2, 0 Repeats)
delay(10000);
simu.transmit(0,3,0);// (Transmit stored remote 0, Button 3, 0 Repeats)
delay(10000);


}
