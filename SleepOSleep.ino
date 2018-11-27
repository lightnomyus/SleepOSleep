#include <DS3231.h>
#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
#define P2 10 // Button hour
#define P3 9 // Button minutes
#define P4 8 // SWITCH Alarm
#define LED1 11 // PWM LED
#define LED2 12

//objects
DS3231  rtc(SDA, SCL);
Time t;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

//************Variables**************//
int alarmHours =0;
int alarmMinutes = 0;  // Holds the current alarm time
int isAlarmOn =0;

void setup() { 
 lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
 pinMode(P2,INPUT_PULLUP);
 pinMode(P3,INPUT_PULLUP);
 pinMode(P4,INPUT_PULLUP);
 pinMode(LED1,OUTPUT);
 pinMode(LED2,OUTPUT);
 Serial.begin(9600);
 rtc.begin(); // Initialize the rtc object
}
void loop() { 
 //lcd.clear();
 t = rtc.getTime();
  //alarm hours setting
  if( digitalRead(P2)== LOW ){
      Serial.println("Hour");
    if(alarmHours==23){
      alarmHours=0;
      delay(200);
    } else {
      alarmHours=alarmHours+1;
      delay(200);
    }
  }

  //alarm minutes setting
  if( digitalRead(P3)== LOW ){
    Serial.println("Minutes");
    if (alarmMinutes==59){
      alarmMinutes=0;
      delay(200);
    } else {
      alarmMinutes=alarmMinutes+1;
      delay(200);
    }
  }

  //switch for Alarm to ON/OFF
  if( digitalRead(P4)==LOW ){
    Serial.println("Alarm");
    if( isAlarmOn==0 ){
      isAlarmOn = 1;
      delay(200);
    } else {
      isAlarmOn = 0;
      Serial.println("0");
      delay(200);
    }
  }
  
 printTime();
 printAlarm();
 printStatus();
 makeAlarm();
 chooseLED();
}

void chooseLED(){
  if ( isAlarmOn==1 ){
     if( ledbefore(15)==1 ){
        analogWrite(LED1, 64);
     }
     if( ledbefore(10)==1 ){
        analogWrite(LED1, 128);
     }
     if( ledbefore(5)==1 ){
        analogWrite(LED1, 191);
     }
     if( ( t.hour == alarmHours ) && ( t.min == alarmMinutes ) ){
        analogWrite(LED1, 255);
     }
  } else {
     analogWrite(LED1, 25);
  }
}

void printTime(){
  if(t.hour <= 9){
    lcd.setCursor(0,0);
    lcd.print("0");
    lcd.print(t.hour, DEC);
  } else{
    lcd.setCursor(0,0);
    lcd.print(t.hour, DEC);
  }
 lcd.setCursor(2,0);
 lcd.print(":");
 if(t.min <= 9){
  lcd.setCursor(3,0);
  lcd.print("0");
  lcd.print(t.min, DEC);
 } else{
  lcd.setCursor(3,0);
  lcd.print(t.min, DEC);
 }
 lcd.setCursor(5,0);
 lcd.print("/");
 lcd.setCursor(6,0);
 lcd.print(rtc.getDateStr());
}

void printStatus(){
  lcd.setCursor(0,1);
  if( isAlarmOn == 0 ) {
    lcd.print("Alarm OFF");
  } else if ( isAlarmOn == 1 ){
    lcd.print("Alarm ON ");
  } else{
    lcd.print("XX");
  }
}

void printAlarm(){
  lcd.setCursor(10,1);
  if (alarmHours <= 9){
    lcd.print("0");
  }
  lcd.print(alarmHours);
  lcd.print(":");
  if (alarmMinutes <= 9){
    lcd.print("0");
  }
  lcd.print(alarmMinutes); 
}

//fungsi makeAlarm, return 1 klo waktu skrng== waktu alarm disetel dan alarm = ON, 0 klo ga
void makeAlarm(){
  if( isAlarmOn == 1 ){
    if( ( t.hour == alarmHours ) && ( t.min == alarmMinutes ) ) {
       digitalWrite(LED2,HIGH);
       Serial.write("1");
       //return 1;
    } else {
      digitalWrite(LED2,LOW);
      //return 0;
    }
  } else{
    digitalWrite(LED2,LOW);
    //return 0;
  }
}

int ledbefore( int minBeforeAlarm ){
  //return 0 klo minBeforeAlarm != yg dinginkan, return 1 klo minBeforeAlarm = yg diinginkan
  int selisih;
  if( isAlarmOn==1 )
  {
    if( alarmMinutes < minBeforeAlarm )//bikin case klo set alarm <15 time now
    {
      selisih = alarmMinutes + 60 - t.min;
    } else
    {
      selisih = alarmMinutes - t.min;
    }
  }
  
  if( selisih == minBeforeAlarm )
  {
    return 1;
  } else{
    return 0;
  }
}

void preAlarm( int durationMin, int pinForLED, int ledIntensity )//durasi dlm meni, pin utk LED, dan intensitasnya brp (max=100% = 255)
{
  int totalTime = durationMin*60;//dalam satuan detik
  for( int i = 0; i<=totalTime; i++ ){
    analogWrite(pinForLED, ledIntensity);
    delay(1000);
  }
}
