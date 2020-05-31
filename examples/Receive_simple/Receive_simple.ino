#include<Simu_Remote_CC1101.h>

void setup(){
Serial.begin(115200);  
simu.Init();                //Initialize CC1101 and set frequency to 433.42MHz.
simu.enableReceive(2);      //configure the receive pin and switch on receive.
}

void loop(){
if (simu.receive() == 1){   //checks whether something has been received

Serial.println("Decrypted Code:");
for (int i=0; i<10; i++){
Serial.print(simu.received_code(i,1)); //(addr,1 = decrypted_data) outputs the decrypted data in the serial monitor.
Serial.print(" ");
}
Serial.println("\n");

simu.clear_received();    //reset and get ready to receive new ones.
}

}
