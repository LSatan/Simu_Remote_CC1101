# Simu Remote CC1101 v1.0
---------------------------------------------

Donations needed:
Somfy RTS "Old Style" (56bit) should be included in the library. With the same functions. The integration of existing libraries is out of the question since it should be emulated as 100% as the TSA + remote control.
For this purpose,  39.00 € is required for the corresponding remote control. I rely on donations for this purpose. In the following pool you can donate and see how much has already been raised. https://paypal.me/pools/c/8pFMW9lGp6

---------------------------------------------

Fully implemented. Simu Somfy RTS "New Style" (80bit).

Sending and receiving Simu:

Radio motor Hz / Hz02 / EHz /

Door control RSA /

Radio receiver SA Hz /

Garage door operator GM800 / GM1000

Micro radio receiver for lighting


Supposedly compatible with some Somfy devices. (Not tested)

Attention: The library may only be used for your own system. For example to check their security. You may make yourself 
vulnerable if you use it on other systems without permission! Use at your own risk!

This library can serve as an interface for your smart home. You can also build and program your own receiver for your 
existing remote controls.
Or create your own remote control.

---------------------------------------------
what is needed:
---------------------------------------------

Arduino based board or

Esp8266 based board or

Esp32 based board.

CC1101 433MHz version.

https://github.com/LSatan/SmartRC-CC1101-Driver-Lib

---------------------------------------------

The library is based on the findings of this article https://pushstack.wordpress.com/somfy-rts-protocol/
however, some things do not match for the above systems. Only the processing with xor for the code and the 
crc processing are correct.
In the following I will roughly explain how this system works.

Analysis of the log:
I have a roller door and I have 3 remote controls.
This allowed me to analyze and reproduce the signals exactly.

To keep it simple, the libary works with decimal numbers from 0 to 255 (8bit).
The remote control protocol consists of 10 8-bit codes.


The following description:

<img src="https://github.com/LSatan/Simu_Remote_CC1101/blob/master/img/simu_buttons.png"/>

<img src="https://github.com/LSatan/Simu_Remote_CC1101/blob/master/img/simu_progbtn.png"/>

<img src="https://github.com/LSatan/Simu_Remote_CC1101/blob/master/img/de_encryption_of_simu.png"/>

<img src="https://github.com/LSatan/Simu_Remote_CC1101/blob/master/img/data_repeat_simu.png"/>

---------------------------------------------
The receiving:
---------------------------------------------
I have perfectly matched the reception code to the above system.
The receiving is flawless, clean and perfect. Everything that is output has been crc checked.

The two first timings sent by the remote control randomly vary each time you press a button.
However, this always gives a total of 26500 microseconds.

example 1: timing 1 = 1800, timing 2 = 24700, total 26500 microseconds,

Example 2: timing 1 = 8000, timing 2 = 18500, total 26500 microseconds.

Example 3: timing 1 =  950, timing 2 = 25550, total 26500 microseconds.

An initialization sequence is then sent,
from timing 3 to timing 26, 24x approx 2500 microseconds.

followed by a signal indicating that the manchester code is now beginning.
timing 27 = approx. 4500 microseconds

The Manchester coded signal then follows.
timing high = approx. 1300 microseconds
timing low = approx. 650 microseconds

For the receiver to respond, the signal must be repeated at least 1x.
To do this, an initialization sequence is sent again, but only half as long
is like the first.
12x about 2500 microseconds.

again followed by followed by a signal indicating that the manchester code is now beginning..
timing = about 4500 microseconds.

Then the manchester signal was sent again but changed a little.
with each repetition, 4 are added to the 8th code section and 13 are added to the 10th code section.



---------------------------------------------
The transmitter:
---------------------------------------------

The send code corresponds to 99% of the above system and works perfectly.
In order for the receiver to react to the signal, it must be repeated at least once.
This happens automatically when repeat is set to 0.

The first two timings are fixed because they always work. Together they add up to 26500 microseconds.
Timing 1 = 20000 microseconds, timing 2 = 6500 microseconds, a total of 26500 microseconds.

Now the first initialization sequence follows 24x 2500 microseconds.
And the signal that the code starts 4500 microseconds

Now the manchester coded 10x 8bit code is sent.
The 10x 8bit code is provided with a crc checksum before sending, which makes up the 2nd 8bit signal.

Then the individual 8-bit codes are exchanged with each other with xor.
It is important that the last 3 encrypted codes, despite the crc, xor and rolling number
always stay the same.
The 7th encrypted 8-bit code is generated automatically.
This means that the signal remains the same at the end.

Now the 2nd initialization sequence follows 12x 2500 microseconds.
And the signal that the code starts 4500 micros.

With each repetition, the 8th and 10th encrypted 8bit codes are added.
8th 8bit code +4, 10th 8bit code +13.
