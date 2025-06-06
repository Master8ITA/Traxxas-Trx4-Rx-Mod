#include <Servo.h>

#define THROTTLE_SIGNAL_IN 0 // INTERRUPT 0 = DIGITAL PIN 2 - use the interrupt number in attachInterrupt
#define THROTTLE_SIGNAL_IN_PIN 2 // INTERRUPT 0 = DIGITAL PIN 2 - use the PIN number in digitalRead

#define NEUTRAL_THROTTLE 1500 // this is the duration in microseconds of neutral throttle on an electric RC Car

volatile int nThrottleIn = NEUTRAL_THROTTLE; // volatile, we set this in the Interrupt and read it in loop so it must be declared volatile
volatile unsigned long ulStartPeriod = 0; // set in the interrupt
volatile boolean bNewThrottleSignal = false; // set in the interrupt and read in the loop
// we could use nThrottleIn = 0 in loop instead of a separate variable, but using bNewThrottleSignal to indicate we have a new signal
// is clearer for this first example

Servo posteriore;
Servo anteriore;
Servo riduttore;

void setup()
{
  // tell the Arduino we want the function calcInput to be called whenever INT0 (digital pin 2) changes from HIGH to LOW or LOW to HIGH
  // catching these changes will allow us to calculate how long the input pulse is
  attachInterrupt(THROTTLE_SIGNAL_IN,calcInput,CHANGE);
  
  posteriore.attach(9);
  anteriore.attach(10);
  riduttore.attach(11);

  Serial.begin(9600);
}

void loop()
{
 // if a new throttle signal has been measured, lets print the value to serial, if not our code could carry on with some other processing
 if(bNewThrottleSignal)
 {

   Serial.println(nThrottleIn); 

   // set this back to false when we have finished
   // with nThrottleIn, while true, calcInput will not update
   // nThrottleIn
   bNewThrottleSignal = false;
 }
traxxas();
 // other processing ...
}

void calcInput()
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(THROTTLE_SIGNAL_IN_PIN) == HIGH)
  {
    // get the time using micros - when our code gets really busy this will become inaccurate, but for the current application its
    // easy to understand and works very well
    ulStartPeriod = micros();
  }
  else
  {
    // if the pin is low, its the falling edge of the pulse so now we can calculate the pulse duration by subtracting the
    // start time ulStartPeriod from the current time returned by micros()
    if(ulStartPeriod && (bNewThrottleSignal == false))
    {
      nThrottleIn = (int)(micros() - ulStartPeriod);
      ulStartPeriod = 0;

      // tell loop we have a new signal on the throttle channel
      // we will not update nThrottleIn until loop sets
      // bNewThrottleSignal back to false
      bNewThrottleSignal = true;
    }
  }
}

void traxxas()
{
  
  int ch3 = map(nThrottleIn,1000,2000,-100,100);
  
  
  if(ch3 > -15 && ch3 < 15)
  {
    modo_1();
  }
  else if(ch3 > 20 && ch3 < 30)
  {
    modo_2();
  }
  else if(ch3 > 40 && ch3 < 60)
  {
	modo_3();
  }
  else if(ch3 > 70)
  {
	modo_4();
  }
  else if(ch3 < -20 && ch3 > -30)
  {
	modo_5();
  }
  else if(ch3 < -40 && ch3 > -60)
  {
	modo_6();
  }
  else if(ch3 < -70)
  {
	modo_7();
  }
}
  void modo_1()
  {
    posteriore.write(0);
    anteriore.write(0);
    riduttore.write(0);
  }
void modo_2()
{
  posteriore.write(180);
  anteriore.write(0);
  riduttore.write(0);
}

void modo_3()
{
    posteriore.write(180);
    anteriore.write(180);
    riduttore.write(0);
}

void modo_4()
{
    posteriore.write(180);
    anteriore.write(180);
    riduttore.write(180);
}

void modo_5()
{
    posteriore.write(0);
    anteriore.write(0);
    riduttore.write(180);
}

void modo_6()
{
    posteriore.write(180);
    anteriore.write(0);
    riduttore.write(180);
}

void modo_7()
{
    posteriore.write(180);
    anteriore.write(180);
    riduttore.write(180);
}