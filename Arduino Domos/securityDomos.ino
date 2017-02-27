/*
 Project Title: Home Security System
 Author: Stefano Carone
 Class: 5^ F
 Term: Summer 2017
 Started Date: 16/11/2016
 Due date: 
 Version: 1.0
 Revision Date: 06/02/2017
*/

/////////////////////////////////////////////////////////////////
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Password.h> //http://www.arduino.cc/playground/uploads/Code/Password.zip
#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip

//Password
int passwd_pos = 10;  // the postition of the password 
Password password = Password( "1234" );
Password active = Password( "0" );

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Four columns

char keys[ROWS][COLS] = // Define the Keymap
{
{'1', '2', '3'},
{'4', '5', '6'},
{'7', '8', '9'},
{'*', '0', '#'}
};

byte rowPins[ROWS] = {23,25,27,29};     //connect to the row pinouts of the keypad
byte colPins[COLS] = {22,24,26};     //connect to the column pinouts of the keypad

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Assignign arduino pins to LCD display module


//constants for LEDs, inputs and outputs
int pirPin1 = 42; //53
int pirPin2 = 41; //51
int pirPin3 = 40; //49
int switchPin1 = 38;
int switchPin2 = 36;
int switchPin3 = 34;
int speakerPin = 6;
int motorPin1 = 7;
int motorPin2 = 8;
int motorPin3 = 9;
int motorPin4 = 10;

int alarmStatus = 0;
int alarmActive = 0;
int invalidCount = 0;

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  //Adding time
  Wire.begin();
  displayCodeEntryScreen();

  //setup and turn off both LEDs
  pinMode(speakerPin, OUTPUT);
  
  pinMode(pirPin1, INPUT);  //Angolo destro alto
  pinMode(pirPin2, INPUT);  //Angolo destro basso
  pinMode(pirPin3, INPUT); //Angolo sinistro
  pinMode(switchPin1, INPUT); //Porta
  pinMode(switchPin2, INPUT); //Finestra destra
  pinMode(switchPin3, INPUT); //Finestra sinistra
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  digitalWrite(speakerPin, LOW);

  keypad.addEventListener(keypadEvent);
}

boolean Stato0 = false;
boolean Stato1 = false;
boolean Stato2 = false;
boolean Stato3 = false;
boolean Dblock = true;
boolean Ablock = true;

void loop(){
int zone = 0;
char input;

  keypad.getKey();
  if (Serial.available() > 0) {
    input = Serial.read();
    if (input == 'a' && alarmActive == 1){
      for (int i = 0; i<5; i++){
        delay(10);
       orario();
    }
    }
    else if (input == 'b' && alarmActive == 1){
      for (int i = 0; i<6; i++){
        delay(10);
       antiOrario();
    }
    }
    else if (input == 'c' && alarmActive == 1){
      for (int i = 0; i<4; i++){
        delay(10);
       antiOrario();
    }
    }
    else if (input == 'A' && alarmActive == 1){
      for (int i = 0; i<5; i++){
        delay(10);
       antiOrario();
    }
    }
    else if (input == 'B' && alarmActive == 1){
      for (int i = 0; i<6; i++){
        delay(10);
       orario();
    }
    }
    else if (input == 'C' && alarmActive == 1){
      for (int i = 0; i<4; i++){
        delay(10);
       orario();
    }
    }
    
  }
if (alarmActive == 1){
  Dblock = true;
  if (Ablock == true){
       Serial.println("0");
       delay(500);
      if (Serial.available() > 0) {
        input = Serial.read();
        if (input == 'h'){
              Ablock = false;
            }
        }
       }
    if (Stato0 == false && (digitalRead(pirPin1) == HIGH || digitalRead(switchPin2) == HIGH))
    {
      int cnt = 0;
      while(cnt < 3){
        if (digitalRead(pirPin1) == HIGH){
          cnt++;
        }
        else{
          break;
        }
      }
      zone = 1;
      alarmTriggered(zone);
    }
    
    else if (Stato1 == false && (digitalRead(pirPin2) == HIGH || digitalRead(switchPin1) == HIGH))
     {
       zone = 2;
       alarmTriggered(zone);
     }
    else if (Stato2 == false && (digitalRead(pirPin3) == HIGH || digitalRead(switchPin3) == HIGH)){
      zone = 3;
      alarmTriggered(zone);
    }
    if (Stato3 == false && invalidCount > 3){
      invalidCount = 0;
      zone = 4;
      alarmTriggered(zone);
    }
    if (Stato0 == true && Stato1 == true && Stato2 == true){
      Stato0 = false;
      Stato1 = false;
      Stato2 = false; 
    }
   }
else{
   Ablock = true;
   if (Dblock == true){
      Serial.println("1");
      delay(500);
      if (Serial.available() > 0) {
        input = Serial.read();
        if (input == 'h'){
              Dblock = false;
           }
        }
       }
}
}

/////////////////////////  Functions  /////////////////////////////////
//take care of some special events
void alarmTriggered(int p){
  digitalWrite(speakerPin, HIGH);
  active.reset();
  password.reset();
  alarmStatus = 1;
  // alarmActive = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter PIN:");
  lcd.setCursor(0,1);
  if (p == 1)//Bedroom
  { 
    Serial.println("001");
    lcd.print("Motion in Bedroom");
    Stato0 = true;
    delay(1000);
  }
  else if(p == 2){ //Hall
    Serial.println("002");
    lcd.print("Motion in Hall");
    Stato1 = true;
    delay(1000);
  }
  else if(p == 3){ //Kitchen
    Serial.println("003");
    lcd.print("Motion in Kitchen");
    Stato2 = true;
    delay(1000);
  }
  else if(p == 4){ //Keypad
    Serial.println("004");
    lcd.print("Motion in Keypad");
    Stato3 = true;
    delay(1000);
  }
}                                                          

void displayCodeEntryScreen()    // Dispalying start screen for users to enter PIN
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter PIN:");
  lcd.setCursor(0,1);
  lcd.print("Security System");
}


void invalidCode()    // display meaasge when a invalid is entered
{
  if (alarmStatus == 0){
      invalidCount++;
  }
  active.reset();
  password.reset();
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("INVALID CODE");
  lcd.setCursor(3,1);
  lcd.print("TRY AGAIN!");
  delay(1500);
  displayCodeEntryScreen();
}

void activate()      // Activate the system if correct PIN entered and display message on the screen
{
    lcd.setCursor(0,0);
    lcd.print("SYSTEM ACTIVE!"); 
    alarmActive = 1;
    active.reset();
    delay(2000);
}

void deactivate()
{
  alarmStatus = 0;
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("SYSTEM");
  lcd.setCursor(1,0);
  lcd.print("DEACTIVATED!");
  digitalWrite(speakerPin, LOW);
  alarmActive = 0;
  Stato0 = false;
  Stato1 = false;
  Stato2 = false;
  Stato3 = false;
  password.reset();
  delay(5000);

  displayCodeEntryScreen();
}

void checkPassword(){                  // To check if PIN is corrected, if not, retry!
  if (password.evaluate() || active.evaluate())
  {  
    if(alarmActive == 0 && alarmStatus == 0)
    {
      activate();
    } 
    else if( alarmActive == 1 || alarmStatus == 1) {
      deactivate();
    }
  } 
  else {
    invalidCode();
  }
}

void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
  case PRESSED:
    if (passwd_pos - 15 >= 5) { 
      return ;
    }
    lcd.setCursor((passwd_pos++),0);
    switch (eKey){
    case '#':           //# is to validate password
      //Serial.println(eKey);    
      passwd_pos  = 10;
      checkPassword(); 
      break;
    case '*':         //* is to reset password attempt
      //Serial.println(eKey); 
      password.reset();
      active.reset();
      passwd_pos = 10;
      displayCodeEntryScreen();
   // TODO: clear the screen output 
      break;
    default:
      //Serial.println(eKey);
      if (alarmActive == 1){
          password.append(eKey);
      }
      else{
          active.append(eKey);
      }
      lcd.print("*");
    }
  }
}

void antiOrario(){
  int delayTime=50;
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(delayTime);
}

void orario(){
  int delayTime=50;
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(delayTime);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
}

