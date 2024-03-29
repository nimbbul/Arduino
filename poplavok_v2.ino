
#include <SHT1x.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <microDS3231.h>
MicroDS3231 rtc;  //часы


#define dataPin  10
#define clockPin 11
SHT1x sht1x(dataPin, clockPin);

#define  humidity_relay_Pin 7 


#define  relay_pomp_in 5 // выкачивать из ведер
#define  relay_pomp_out 6 // выкачивать в ведро


#define lower_level_pin A0
#define upper_level_pin A1

int lower_level_Data = 0 ;
int upper_level_Data = 0 ;


int pompa_in_flag = 0;

int temp_c;
int humidity;
 

boolean relay_flag = false;
boolean A = false;
uint32_t tmr;
boolean flag = true;
boolean flag1 = true;
boolean flag2 = true;
boolean flag_LCD ;
uint32_t myTimer_tmp_humi;
uint32_t lCD_tmr;
uint32_t myTimer_level_sensor;

#define period2 40*1000L  //40 секунд
#define period3 60*5*1000L  // 5 минут
#define period1 60*2*1000L  // 2 min
//#define period1 60*1000L  // 1 минутa
//#define period3 60*120*1000L  // 2часа

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 12, 9, 8, 4);




void setup() {

 rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);

Serial.begin(9600);
Serial.println("ok");


  // проверка наличия модуля на линии i2c
  // вызов rtc.begin() не обязателен для работы
 if (!rtc.begin()) {
    Serial.println("DS3231 not found");
    for(;;);
  }

   if (rtc.lostPower()) {            // выполнится при сбросе батарейки
    Serial.println("lost power!");
    // тут можно однократно установить время == времени компиляции
  }
  
  
Serial.println(flag);//
Serial.println(flag_LCD);//  
  
Serial.print("Flag pompa :");Serial.println(pompa_in_flag);
 
  pinMode(dataPin, INPUT);
  pinMode(clockPin, INPUT);
 
  pinMode(relay_pomp_in, OUTPUT);
  pinMode(relay_pomp_out, OUTPUT);
  pinMode(humidity_relay_Pin, OUTPUT);

upper_level_Data = analogRead(upper_level_pin);
upper_level_Data = map(upper_level_Data, 0, 500, 0, 5);
upper_level_Data= constrain(upper_level_Data, 0, 5);


  if (upper_level_Data > 0 ){
    pompa_in_flag =1;
  }




Serial.println("-----------------------------");
Serial.print("Flag pompa :");Serial.println(pompa_in_flag);
 Serial.print("Down sensor :");Serial.println(lower_level_Data);
 Serial.print("Up sensor :");Serial.println(upper_level_Data);
 Serial.println("-----------------------------");
  Serial.println("-----------------------------");


}





void loop() {

  Serial.print(rtc.getHours());
  Serial.print(":");
  Serial.print(rtc.getMinutes());
  Serial.print(":");
  Serial.print(rtc.getSeconds());
  Serial.print(" ");

  Serial.println();
 

lower_level_Data = analogRead(lower_level_pin);
upper_level_Data = analogRead(upper_level_pin);
lower_level_Data = map(lower_level_Data, 0, 500, 0, 5);
upper_level_Data = map(upper_level_Data, 0, 500, 0, 5);
lower_level_Data = constrain(lower_level_Data, 0, 5);
upper_level_Data= constrain(upper_level_Data, 0, 5);


// Serial.print("Down sensor :");Serial.println(lower_level_Data);
// Serial.print("Up sensor :");Serial.println(upper_level_Data);
// Serial.println("-----------------------------");





if (millis() - myTimer_level_sensor >= 3000) {
 myTimer_level_sensor = millis();   

 if (lower_level_Data == 0 && upper_level_Data == 0 && pompa_in_flag == 0) {//бак пустой,выключаем насос
    Serial.println("бак пустой выключем насос"); //
    Serial.print("Down sensor :");Serial.println(lower_level_Data);
 Serial.print("Up sensor :");Serial.println(upper_level_Data);
 Serial.println("-----------------------------");

   // digitalWrite(led_Pin, HIGH);
    digitalWrite(relay_pomp_in, LOW);//выключаем насос
    pompa_in_flag = 1;//true; //сохраняем флаг
   A=true;
   
 } 
        
 if  (lower_level_Data > 0 && upper_level_Data > 0  && pompa_in_flag == 1) {//бак заполнcен включаем насос
          Serial.println("бак заполнен включаем насос"); //
          Serial.print("Down sensor :");Serial.println(lower_level_Data);
 Serial.print("Up sensor  :");Serial.println(upper_level_Data);
 Serial.println("-----------------------------");

         // digitalWrite(led_Pin, LOW);
          digitalWrite(relay_pomp_in, HIGH); //включаем насос
          pompa_in_flag = 0;//false;
         // pompa_out_flag = true; //включаем насос на впрыск
         A=false;
         
 }    
 }   
//  Serial.println("----------88888----------");
//if ((rtc.getHours()>6)&&(rtc.getHours()<0))
//{

 if (rtc.getHours()>=18) {
     if ( rtc.getHours()<=23 && rtc.getMinutes()<=59)
  
 {
   if (millis() - tmr >= (flag ? period1 : period2)) { //включаем насос на вкачивание period1=3min period2=30sec
    tmr = millis();
   // flag = !flag;
    digitalWrite(relay_pomp_out, flag);
    //digitalWrite(led_Pin, flag);
    Serial.println("-----------------------------");
     Serial.println("насос на впрыск");
      Serial.print("Flag: "); Serial.println(flag);
     flag_LCD = flag;
     Serial.print("Flag_LCD: "); Serial.println(flag_LCD);
  
Serial.println("-----------------------------");
    
  
     flag = !flag;
   }
 } 
  }

 if (rtc.getHours()>=0) {
     if ( rtc.getHours()<=11 && rtc.getMinutes()<=59)
  
 {


   if (millis() - tmr >= (flag1 ? period1 : period2)) { //включаем насос на вкачивание period1=3min period2=40sec
    tmr = millis();
   // flag = !flag;
    digitalWrite(relay_pomp_out, flag1);
    //digitalWrite(led_Pin, flag);
    Serial.println("-----------------------------");
     Serial.println("насос на впрыск");
      Serial.print("Flag: "); Serial.println(flag);
     flag_LCD = flag1;
     Serial.print("Flag_LCD: "); Serial.println(flag_LCD);
  
Serial.println("-----------------------------");
    
  
     flag1 = !flag1;
   }
 } 
  }


  if (rtc.getHours()>=12) 
    {  
     
     if (rtc.getHours()<=17 && rtc.getMinutes()<=59){   
    
   if (millis() - tmr >= (flag2 ? period3 : period2)) { //включаем насос на вкачивание period3=7min period2=40sec
    tmr = millis();
   // flag = !flag;
    digitalWrite(relay_pomp_out, flag2);
    //digitalWrite(led_Pin, flag);
    Serial.println("-----------------------------");
     Serial.println("насос на впрыск");
      Serial.print("Flag: "); Serial.println(flag);
     flag_LCD = flag2;
     Serial.print("Flag_LCD: "); Serial.println(flag_LCD);
  
Serial.println("-----------------------------");
    
  
     flag2 = !flag2;
   }
 } 
  }


  
//}
//if  ((rtc.getHours()>=0 )&&(rtc.getHours()<=6))
//{
//  
//   if (millis() - tmr >= (flag ? period3 : period2)) { //включаем насос на вкачивание period3=10мин period2=20sec
//    tmr = millis();
//   // flag = !flag;
//    digitalWrite(relay_pomp_out, flag);
//    //digitalWrite(led_Pin, flag);
//    Serial.println("-----------------------------");
//     Serial.println("насос на впрыск");
//      Serial.print("Flag: "); Serial.println(flag);
//     flag_LCD = flag;
//     Serial.print("Flag_LCD: "); Serial.println(flag_LCD);
//  
//Serial.println("-----------------------------");
//    
//  
//     flag = !flag;
//    
//   }
//  
//}
  
 if (millis() - myTimer_tmp_humi >= 20000) {
  myTimer_tmp_humi = millis();  
  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
 // temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
 // Print the values to the serial port
 Serial.println("-----------------------------");
  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C / ");
 // Serial.print(temp_f, DEC);
 Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  
    if(humidity >= 55) {

       digitalWrite(humidity_relay_Pin, HIGH);
        Serial.print("humidity >= 55");

    }

    else

    {
       digitalWrite(humidity_relay_Pin, LOW);
        Serial.println("humidity <= 55 "); 
        Serial.println("-----------------------------");
       

      }
     
 }
   


  if (millis() - lCD_tmr >= 2000) {
lCD_tmr = millis();      
      
   display.begin();
  display.clearDisplay();
  display.setContrast(50);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Humidity:"); 
   display.setCursor(52, 0);
   display.println(rtc.getHours());
  //display.println(humidity);
   display.setCursor(65, 0);
  display.println("H");
  

   if (A==1){
  display.setCursor(0, 10);
  display.println("Pump off");
   }
   else {
   display.setCursor(0, 10);
  display.println("Pump on");
   }
   
   display.setCursor(0, 20);
   display.println("Uper:");
   display.setCursor(30, 20);
  display.println(upper_level_Data);
  display.setCursor(40, 20);
  display.println("Down:");
   display.setCursor(70, 20);
  display.println(lower_level_Data);
   display.setCursor(0, 30);
  display.println("Pump In:");
  display.setCursor(50, 30);
   display.println(flag_LCD);
    display.setCursor(0, 40);
   display.println("Temp:");
   display.setCursor(30, 40);
    display.println(temp_c, DEC);
     display.setCursor(44, 40);
    display.println("C");
    //display.setCursor(0, 50);
   // display.println("Clock:");
   // display.setCursor(30, 50);
//  display.println(rtc.getHours());
    
    
  display.display(); 
         
  
  }  
      
    } 
 
