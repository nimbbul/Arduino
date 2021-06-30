
#include <OneWire.h>
#include "rgb_lcd.h"

rgb_lcd lcd; //new object
OneWire ds(3);  //new object pin 3

// set color lcd
const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

int temperature = 0; 


uint32_t myTimer_TEMP; //timer to temperature 1second
uint32_t myTimer_FLOW; //timer to flowe 1 second


volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input

void flow () // Interrupt function
{
   flow_frequency++; //couner to pulses from flowe sensor
}
void setup()
{
   pinMode(flowsensor, INPUT); //pin 2
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
  
   // set up the LCD's number of columns and rows:
   lcd.begin(16, 2);
   lcd.setRGB(colorR, colorG, colorB);
}

int detectTemperature(){

  byte data[2]; //2 byte 

  ds.reset(); //reset data from temp SENSOR
  ds.write(0xCC); //skip looking for address , we have 1 sensor 1 wire only
  ds.write(0x44);// measure the temperature only

  if (millis() - myTimer_TEMP >= 1000)//wait for 1 sec
  {
    myTimer_TEMP = millis();
    ds.reset(); //reset data from temp SENSOR
    ds.write(0xCC);
    ds.write(0xBE); //send data from sensor temp
    //data from sensor 2 byte
    data[0] = ds.read(); // READ AND WRITE data to low byte
    data[1] = ds.read(); //READ AND WRITE data to HIGH byte

    // get data from temp sensor
    temperature = (data[1] << 8) + data[0]; 
    temperature = temperature >> 4;
    Serial.println(temperature);
  }
}

void loop ()
{ 
   detectTemperature();
 
   // set the cursor to column 0, line 0

    lcd.setCursor(0, 0);
    lcd.print("TEMP:  ");
    lcd.print(temperature);
    lcd.print(" ");
    lcd.print((char)223);
    lcd.print("C");

    if (temperature > 25 ) //change collor of LCD
        {
        lcd.setRGB(64,0,0);
        }
    else 
        {
        lcd.setRGB(0,0,255);
        }
 
   // Every second, calculate and print litres/hour
   if(millis() - myTimer_FLOW >= 1000)
   {
     myTimer_FLOW = millis();// Updates Time
      // Pulse frequency (Hz) = 38Q, Q is flow rate in L/min. YF-S402B
      l_hour = (flow_frequency * 60 / 38); // (Pulse frequency x 60 min) / 38 Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/hour");
      lcd.setCursor(0, 1);
      lcd.print("FLOW:");
      lcd.print(l_hour);
      lcd.print(" L/hour ");  
   }
}
