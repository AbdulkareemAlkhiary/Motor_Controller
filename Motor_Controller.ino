#include <TimerOne.h>
#include <LiquidCrystal.h>// import LCD library
LiquidCrystal LcdDriver(11, 9, 5, 6, 7, 8 ); // Defines for LCD
int Pin = 10;
int encoderPosition = 510; //declare encoder Position
unsigned long Timer; //Declare timer
unsigned long Timer2;


int ButtonState; // function that is to be called in loop to service
#define Idle 0
#define wait 1
#define low 2
int ButtonNextState(int input) //function that is to be called in loop to service
{
 switch (ButtonState) //compares values of variables to values specified in case statements
 {
 case Idle: //checks the value of the variable and statement runs if found
 if (input == LOW) //input is low
 {
 Timer = millis(); //record time
 ButtonState = wait; //set ButtonState to wait
 }
 break; //exits the switch statement
 case wait: //checks the value of the variable and statement runs if found
 if (input == HIGH) //INPUT IS HIGH
 {
 ButtonState = Idle; //set Buttonstate to Idle
 }
 else if (millis() - Timer >= 5) {
 ButtonState = low; //set ButtonState to low
 return 1;
 }
 break; //exits the switch statement
 case low: //checks the value of the variable and statement runs if found
 if (input == HIGH) //input is high
 {
 ButtonState = Idle; //set ButtonState to Idle
 }
 break; //exits the switch statement
 }
 return 0;
}
void MonitorA() // Interrupt service routines
{
 if ((digitalRead(2) == digitalRead(3)) && encoderPosition > 0)
//compare if input A equal to input B and don't go below zero
 {
 encoderPosition--; //decrement numbers
 }
 else if (encoderPosition < 1023)
 {
 encoderPosition++; //increment numbers
 }
}
void MonitorB()
{
 if ((digitalRead(2) == digitalRead(3)) && encoderPosition < 1023)
//compare if input A equal to input B and don't go above 1023
 {
 encoderPosition++; //increment numbers


 }
 else if (encoderPosition > 0)
 {
 encoderPosition--; //decrement numbers
 }
}
void setup()
{
 Timer2 = millis();
 Timer = millis(); //TIMER
 Timer1.initialize(1000); //set timer1 for 500 microseconds
 Timer1.pwm( 10 , 510 , 1000 );
 pinMode(2, INPUT); //sets pin as input
 pinMode(3, INPUT); //sets pin as input
 pinMode(4, INPUT); //sets pin as input
 pinMode(12, INPUT); //sets pin as input
 LcdDriver.begin(16, 2); // Set up the LCD's number of columns and rows
 LcdDriver.clear(); //clear the display
 LcdDriver.setCursor(0, 0);
 attachInterrupt( digitalPinToInterrupt(2), MonitorA, CHANGE);
//translate the actual digital pin to the specific interrupt number
 attachInterrupt( digitalPinToInterrupt(3), MonitorB, CHANGE);
//translate the actual digital pin to the specific interrupt number
 Serial.begin(9600);
 ButtonState = Idle; //set ButtonState to Idle
}
void loop() {
 if (millis() - Timer >= 10) //10 Millis has passed
 {
 Timer1.setPwmDuty(10, encoderPosition); //update duty cycle
 Timer += 10;
 }
 if (ButtonNextState(digitalRead(4)))
 {
 encoderPosition -= 1; //decrement
 }
if (millis() - Timer2 >= 100)
 {
 float rpmValue = MeasureRpm();// set rpmValue as MeasureRpm()
 if (rpmValue >= 100) {
 LcdDriver.setCursor(0, 1);
 LcdDriver.clear();
 LcdDriver.print(rpmValue);// print rpm value in the LCD
 Serial.println(encoderPosition);
 Serial.println(rpmValue);
 }
 Timer2 += 100;
 }
}
unsigned long RpmTime; //declare rpmtime
float MeasureRpm() //declare MeasureRpm as float
{


 float Rpm = 0;
 while (digitalRead(12) == LOW) {
 RpmTime = micros(); //set rpm time to micros
 }
 while (digitalRead(12) == HIGH) {
 }
 while (digitalRead(12) == LOW);
 RpmTime = micros() - RpmTime;// update rpm time
 Rpm = 6e7 / (8 * RpmTime);//calculate rpm
 return Rpm;
}