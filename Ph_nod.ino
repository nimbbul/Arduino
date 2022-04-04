#include <Adafruit_ADS1X15.h>
#include <OneWire.h>

int sensorValue = 0; 
unsigned long int avgValue; 
unsigned long int avgValue_ec; 
float b;
int buf[10],temp;
int buf_ec[10],temp_ec;

float temperature = 0.0; 
uint32_t myTimer_TEMP; //timer to temperature 1second


Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
OneWire ds(3);  //new object pin 8 temperature

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Hello!");

  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");

  // The ADC input range (or gain) can be changed via the following  65536
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
 //  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
 //  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
 //  ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
}


float detectTemperature(){

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
    temperature = ((data[1] << 8) | data[0]) * 0.0625;
    Serial.print("Temperature = ");
    Serial.println(temperature); Serial.println(" ");Serial.println("C ");
  }
}

void loop(void)
{
  int16_t adc0, adc1, adc2, adc3;
  float volts0, volts1, volts2, volts3;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
//  adc2 = ads.readADC_SingleEnded(2);
//  adc3 = ads.readADC_SingleEnded(3);

  volts0 = ads.computeVolts(adc0);
  volts1 = ads.computeVolts(adc1);
//  volts2 = ads.computeVolts(adc2);
//  volts3 = ads.computeVolts(adc3);

 for(int i=0;i<10;i++) 
 { 
  buf[i]=adc0; //analogRead(analogInPin);
  delay(10);
}
 for(int i=0;i<9;i++)
{
 for(int j=i+1;j<10;j++)
 {
  if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }

avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*0.1875/1000.0/6.0; // делим на 1000 потомучто в мили
 float phValue = -5.70 * volts0 + 21.338;
 float phValue_1 = -5.70 * pHVol + 21.338;
// float phValue = -5 * volts0 + 19.21;

//#########################################33

for(int i=0;i<10;i++) 
 { 
  buf_ec[i]=adc1; //analogRead(analogInPin);
  delay(10);
}
 for(int i=0;i<9;i++)
{
 for(int j=i+1;j<10;j++)
 {
  if(buf_ec[i]>buf_ec[j])
   {
    temp_ec=buf_ec[i];
    buf_ec[i]=buf_ec[j];
    buf_ec[j]=temp_ec;
   }
  }
 }

avgValue_ec=0;
 for(int i=2;i<8;i++)
 avgValue_ec+=buf_ec[i];

float ECVol=(float)avgValue_ec*0.1875/1000.0/6.0; // делим на 1000 потомучто в мили

 
 Serial.println("----------------------------PH-------------------------------");
  Serial.print("sensor = ");
 Serial.println(phValue);

 Serial.print("sensor 1 = ");
 Serial.println(phValue_1);
//  Serial.print("pHVol = ");
//  Serial.println(pHVol);
  Serial.println("-----------------------------------------------------------");
  Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0); Serial.println("V");
//  Serial.print(" my volts "); Serial.print(float((adc0*0.1875)/1000.0)); Serial.println("V");
    Serial.print(" my volts "); Serial.print(pHVol); Serial.println("V");

 Serial.println("---------------------------EC--------------------------------");    
  Serial.print("AIN1: "); Serial.print(adc1); Serial.print("  "); Serial.print(volts1); Serial.println("V");
  Serial.print("My EC volts = ");
 Serial.println(ECVol);
  
   Serial.println("-----------------------------------------------------------");
//  Serial.print("AIN2: "); Serial.print(adc2); Serial.print("  "); Serial.print(volts2); Serial.println("V");
//  Serial.print("AIN3: "); Serial.print(adc3); Serial.print("  "); Serial.print(volts3); Serial.println("V");
detectTemperature();
  delay(1000);
}
