#include<Simu_Remote_CC1101.h>

void setup(){
Serial.begin(115200);  
simu.Init();                //Initialize CC1101 and set frequency to 433.42MHz.
simu.enableReceive(2);      //configure the receive pin and switch on receive.
}

void loop(){

if (simu.receive() == 1){   //checks whether something has been received

Serial.println("Received Code:");
for (int i=0; i<10; i++){
Serial.print(simu.received_code(i,0)); //(addr,0 = encrypted_data) outputs the received data in the serial monitor.
Serial.print(" ");
}
Serial.println();

Serial.println("Decrypted Code:");
for (int i=0; i<10; i++){
Serial.print(simu.received_code(i,1)); //(addr,1 = decrypted_data) outputs the decrypted data in the serial monitor.
Serial.print(" ");
}
Serial.println("\n");

Serial.print("1)Fixed for CRC:         ");
Serial.println(simu.received_code(0,1));
Serial.print("2)CRC:                   ");
Serial.println(simu.received_code(1,1));
Serial.print("3)Rollingcode counter 0: ");
Serial.println(simu.received_code(2,1));
Serial.print("4)Rollingcode counter 1: ");
Serial.println(simu.received_code(3,1));
Serial.print("5)Button ID:             ");
Serial.println(simu.received_code(4,1));
Serial.print("6)Remote ID 1:           ");
Serial.println(simu.received_code(5,1));
Serial.print("7)Remote ID 2:           ");
Serial.println(simu.received_code(6,1));
Serial.print("8)Auto generated:        ");
Serial.println(simu.received_code(7,1));
Serial.print("9)Fixed Simu ID:         ");
Serial.println(simu.received_code(8,1));
Serial.print("10)Fixed Simu ID:        ");
Serial.println(simu.received_code(9,1));
Serial.println("\n");

simu.clear_received();    //reset and get ready to receive new ones.
}

}
