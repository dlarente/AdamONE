#include <Arduino.h>

unsigned long currentMillis;
unsigned long previousMillis = 0;
const unsigned long interval = 1; //10 mSec tick

unsigned char blink_tmr = 100;
unsigned char ad_blink_tmr = 110;
boolean toggle = 0;
boolean ad_toggle = 0;
unsigned int ad_value = 0;
unsigned char ad_tmr = 60;
unsigned int ad_map_value = 0;
int i = 0;
unsigned char serial_tmr = 50;
char inSerial[15];
unsigned char led_state = 0xFF;

void setup() {
  // put your setup code here, to run once:

  for (i = 2; i <= 13; i++) //intialize all digital outputs except 0 and 1 as they are used for UART
  {
    pinMode(i, OUTPUT);
  }

  //cannot place this in a loop due to the define the way it is
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  //needed for serial monitor and bluetooth hc05
  Serial.begin(9600);
  Serial.println("Setup Complete");
}

void loop()
{
  // put your main code here, to run repeatedly:

  //crude way to have a 1 mSec time for events
  currentMillis = millis();
  if ( currentMillis - previousMillis >= interval )
  {
    //need to get a new value to evaluate on the next loop
    previousMillis = currentMillis;

    //decrement all timers
    blink_tmr--;
    ad_tmr--;
    serial_tmr--;
    ad_blink_tmr--;
  }

  if (ad_tmr == 0)
  {
    ad_tmr = 500; //check every 500 mSec
    ad_value = analogRead(A5);
    ad_map_value = map(ad_value, 0, 1023, 0, 255);
    Serial.println(ad_map_value, DEC);
    analogWrite(3, ad_map_value);
  }

  if (ad_blink_tmr == 0)
  {
    ad_blink_tmr = 25; //check every 25 mSec
    ad_toggle = !ad_toggle;

    digitalWrite(A0, ad_toggle);
    digitalWrite(A1, ad_toggle);
    digitalWrite(A2, ad_toggle);
    digitalWrite(A3, ad_toggle);
  }

  if (blink_tmr == 0)
  {
    blink_tmr = 1000; //blink at a 1 second rate
    toggle = !toggle;

    for (i = 2; i <= 13; i++)
    {
      if (i != 3)
      {
        digitalWrite(i, toggle);
      }
    }
  }

  if (serial_tmr == 0)
  {
    serial_tmr = 60;
    if (Serial.available() > 0)
    {
      while (Serial.available() > 0)
      {
        inSerial[i] = Serial.read();
        i++;
      }
      inSerial[i] = '\0';
      Check_Protocol(inSerial);
      i = 0;
    }
  }
}

void Check_Protocol(char inStr[])
{
  int index = 0;

  if (!strcmp(inStr, "c"))
  {
    Serial.println(" ");
    Serial.println("www.adamelectronics.com");
    Serial.println("(248) 583-2000");
    Serial.println("dlarente@adamelectronics.net");

    for (index = 0; index < 15; index++)
    {
      inStr[index] = 0;
    }
    index = 0;
  }
  else
  {
    for (index = 0; index < 15; index++)
    {
      inStr[index] = 0;
    }
  }
}
