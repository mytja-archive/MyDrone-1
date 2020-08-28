/* Tranmsitter code for the Arduino Radio control with PWM output
 * Install the NRF24 library to your IDE
 * Upload this code to the Arduino UNO, NANO, Pro mini (5V,16MHz)
 * Connect a NRF24 module to it:
 
    Module // Arduino UNO,NANO
    
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12

This code transmits 7 channels with data from pins A0, A1, A2, A3, D2 and D3
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL; //Remember that this code should be the same for the receiver

RF24 radio(9, 10);  //Set CE and CSN pins

// The sizeof this struct should not exceed 32 bytes
struct Data_to_be_sent {
  byte ch1; // X - Left Joystick
  byte ch2; // Y - Left Joystick
  byte ch3; // X - Right Joystick
  byte ch4; // Y - Right Joystick
  byte ch5; // LED
  byte ch6; // STOP
  // byte ch7; // Not in use!!!
};

//Create a variable with the structure above and name it sent_data
Data_to_be_sent sent_data;



void setup()
{
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe);

  //Reset each channel value
  sent_data.ch1 = 0;
  sent_data.ch2 = 0;
  sent_data.ch3 = 0;
  sent_data.ch4 = 0;
  sent_data.ch5 = 0;
  sent_data.ch6 = 0;
  //sent_data.ch7 = 0;
}

/**************************************************/


void loop()
{
  /*If your channel is reversed, just swap 0 to 255 by 255 to 0 below
  EXAMPLE:
  Normal:    data.ch1 = map( analogRead(A0), 0, 1024, 0, 255);
  Reversed:  data.ch1 = map( analogRead(A0), 0, 1024, 255, 0);  */
  
  sent_data.ch1 = map( analogRead(A0), 0, 1024, -255, 255);
  sent_data.ch2 = map( analogRead(A1), 0, 1024, -255, 255);
  sent_data.ch3 = map( analogRead(A2), 0, 1024, -255, 255);
  sent_data.ch4 = map( analogRead(A3), 0, 1024, -255, 255);
  sent_data.ch5 = digitalRead(5);
  sent_data.ch6 = digitalRead(4);
  //sent_data.ch7 = map( analogRead(A4), 0, 1024, 0, 255);

  radio.write(&sent_data, sizeof(Data_to_be_sent));
}
