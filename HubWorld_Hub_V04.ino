/*
Hub world
Slider2732_
August - September 2026

Introducing the Serial command protocol
SIDI

Arduino Nano V3 / SSD1306
2x 4.7K pullup resistors for I2C
Green LED on pin 10, red LED on pin 11

Version notes:
Initial screen setup
Initial person
Simple demo in this version, to check for satellites
Either left or right satellite check
Left is a simple handshake and stop animation
Right is walk through to other satellite 
Reserve top line for important messages/sensor data etc
Light level shown on top row

Next versions -
Use millis() not delay()
Use interrupts
*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(128,64,&Wire,-1);


int personx = 50;
int persony = 30;
int eye1;
int eye2;
int looper; // general looping function
int satellite1 = 0;
int satellite2 = 0;

int value = 0; // serial message value
unsigned long startTime;

int LowSoundPlayed = 0;// has a low sound played or not
int MidSoundPlayed = 0;
int HighSoundPlayed = 0;



void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC,0x3C);

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);


    pinMode(10, OUTPUT); // Green LED 330 ohm resistor
    pinMode(11, OUTPUT); // Red LED 330 ohm resistor
    
    // Audio output pin
    pinMode(9, OUTPUT);  // audio output pin
    
        display.clearDisplay();

        display.setTextColor(SSD1306_WHITE);

        display.setTextSize(2);
        display.setCursor(10,10);
        display.print("Hub World");
        display.setTextSize(1); 
        display.setCursor(28,56);
        display.print("Version 0.40");

        display.display();

        digitalWrite(11, HIGH); // Red LED showing LED section 'On'
        delay(500);
        digitalWrite(11, LOW);
        digitalWrite(10, HIGH); // Green LED 
        delay(500);
        digitalWrite(10, LOW);
        
        delay(2000);
    }




void loop()
{   
    if(satellite1 == 1) {satellite101();}
    if(satellite2 == 1) {satellite102();}



     // ----  Start of basic screen
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(2,56);
    for(int a = 2; a <= 129; a++) {display.print("_");}

    leftdoor();
    rightdoor();
    // --- End of basic screen

    // Person initial draw
    personfront();
    eyes();
    mouth();
    display.display();

    delay(2000);


display.display();
delay(2000);


    // Begin demo animations
    // Person left, knock on door, answer or no answer
    // Person right, knock on door, answer or no answer
    // Back to middle
    // Talk to user 
    // Lie down, snooze
    // Stop


    // character walk left
    for (looper = personx; looper > 20; looper--)
        {
         personx = looper;
         personfront();
         eyes();
        //mouth();
         display.display();
         delay(1);
        }
       

       //blank left arm, redraw, blank, redraw + sound
       display.drawLine(personx-6, persony+7, personx -8, persony +14, SSD1306_BLACK); // rid arm bug
       display.drawLine(personx-5, persony+7, personx -7, persony +14, SSD1306_BLACK); // blank left arm      
       display.display();
       display.drawLine(personx-5, persony+7, personx -10, persony, SSD1306_WHITE); //left arm
       display.display();
       display.setTextColor(SSD1306_WHITE);
       display.setCursor(30,12);
       display.print("KNOCK! KNOCK!");
       display.display();
       
       tone(9, 100, 50);
       delay(300);
       tone(9, 100, 50);
       delay(500);
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30,12);
       display.print("KNOCK! KNOCK!");
       display.display();
       display.setTextColor(SSD1306_WHITE);
       display.drawLine(personx-5, persony+7, personx -7, persony +14, SSD1306_BLACK); // orig left arm
       delay(1000);
       display.setTextColor(SSD1306_WHITE);
       display.display();
   


  // Send one complete four-part Serial message
  Serial.println(999);
  Serial.println(102);
  Serial.println(134);
  Serial.println(507);

unsigned long startTime = millis();
value = 0;

while (millis() - startTime < 1000)
{
    if (Serial.available())
    {
        String message = Serial.readStringUntil('\n');
        value = message.toInt();
        Serial.println(value);


        
        if (value == 888)
        {
            satellite2 = 1; // satellite has been found

            digitalWrite(10, HIGH); // Turn on green LED
            delay(1000);
            digitalWrite(10, LOW); // Turn off green LED

           display.setTextColor(SSD1306_WHITE);
           display.setCursor(30,12);
           display.print("HELLO!");
           display.display();
           display.fillRect(personx - 9, persony - 7, 19, 39, SSD1306_BLACK);// remove person
           display.drawLine(personx-5, persony+7, personx -10, persony, SSD1306_BLACK); //left arm
           display.display();

           delay(1000);
           display.setTextColor(SSD1306_BLACK);
           display.setCursor(30,12);
           display.print("HELLO!");
           display.display();
          
           satellite102();
           
          }
    } 
}

       

 if(value != 888)
      { 
       display.setTextColor(SSD1306_WHITE);
       display.setCursor(30,12);
       display.print("No Answer");
       display.display();
       unhappy();
       digitalWrite(11, HIGH); // Turn on red LED
       delay(1000);
       digitalWrite(11, LOW); // Turn off red LED
       delay(1000);
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30,12);
       display.print("No Answer");
       display.display();
       display.setTextColor(SSD1306_WHITE);
      }
    
  

   display.setTextColor(SSD1306_WHITE); 
   delay(100);


    // bug where 1 pixel is left near top of door
    display.drawLine(personx-5, persony+7, personx -10, persony, SSD1306_BLACK); //left arm
    display.display();


   // Move all of these demo functions to their own voids
   for (looper = personx; looper < 108; looper++)
{
    looper = looper +1; // double speed walk to the right
    personx = looper;
    personfront();
    eyes();
    //mouth();
    display.display();
    delay(1);
}

 
       display.drawLine(personx+4, persony+7, personx +6, persony +14, SSD1306_BLACK); // blank left arm      
       display.display();
       display.drawLine(personx+5, persony+7, personx +10, persony, SSD1306_WHITE); // Knock arm
       display.display();
       
       display.setTextColor(SSD1306_WHITE);
       display.setCursor(30,12);
       display.print("KNOCK! KNOCK!");
       display.display();
       
       tone(9, 100, 50);
       delay(300);
       tone(9, 100, 50);
       delay(500);
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30,12);
       display.print("KNOCK! KNOCK!");
       display.display();
       delay(1000);
       display.drawLine(personx+5, persony+7, personx +10, persony, SSD1306_BLACK); // Knock arm
       display.display();
       display.setTextColor(SSD1306_WHITE);

       personfront();
       eyes();
       display.display();


  // Send one complete four-part Serial message
  Serial.println(999);
  Serial.println(102);
  Serial.println(134);
  Serial.println(507);

startTime = millis();
value = 0;

while (millis() - startTime < 1000)
{
    if (Serial.available())
    {
        String message = Serial.readStringUntil('\n');
        value = message.toInt();
        Serial.println(value);

        if (value == 888)
        {
            display.fillRect(personx - 9, persony - 7, 19, 39, SSD1306_BLACK);
            display.display(); 

            digitalWrite(10, HIGH); // Turn on green LED
            delay(1000);
            digitalWrite(10, LOW); // Turn off green LED

            // Four acknowledgement flashes
            for (int i = 0; i < 4; i++)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                delay(500);
                digitalWrite(LED_BUILTIN, LOW);
                delay(500);
            }

          
            while (value == 888)
            {}
        }
    }
}

       

 if(value != 888)
      { 
       display.setTextColor(SSD1306_WHITE);
       display.setCursor(30,12);
       display.print("No Answer");
       display.display();
       unhappy();
       digitalWrite(11, HIGH); // Turn on red LED
       delay(1000);
       digitalWrite(11, LOW); // Turn off red LED
       delay(1000);
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30,12);
       display.print("No Answer");
       display.display();
       display.setTextColor(SSD1306_WHITE);
      }
    

// Back to middle
 for (looper = personx; looper >66; looper--)
{
    personx = looper;
    personfront();
    eyes();
    happy();
    display.display();
    delay(1);
}

delay(2000);

// normal mouth
    personfront();
    eyes();
    happy();
    display.display();

// Waiting message and eyes movement
for (looper =1; looper <5; looper++)
   {
    mouth();
    eyesLeft();
    for (int mutter = 1; mutter < 4; mutter++)
        {
        int mutterFreq = map(analogRead(A0), 0, 1023, 120, 250);
         tone(9, mutterFreq);
         delay(30);
         noTone(9);
         delay(20);
        }
    delay(500);
    mouth();
    eyes();
    for (int mutter = 1; mutter < 4; mutter++)
        {
         int mutterFreq = map(analogRead(A0), 0, 1023, 120, 250);
         tone(9, mutterFreq);
         delay(30);
         noTone(9);
         delay(20);
        }
   

    delay(2000); // standing waiting

    
   }


 display.fillRect(personx - 9, persony - 7, 19, 39, SSD1306_BLACK); // remove person


 // Snooze
 display.setTextColor(SSD1306_WHITE);
 display.setCursor(30,12);
 display.print("Sleep Time");
 display.display();
 snooze();
 delay(1000);

// print z's as end animation
while (satellite1 == 0 && satellite2 == 0) 
      {
       for(looper = 40; looper > 30; looper--)
          {
           display.setTextColor(SSD1306_WHITE); 
           display.setCursor(52, looper);
           display.print("z");
           display.display();
           delay(60);
           display.setTextColor(SSD1306_BLACK);
           display.setCursor(52, looper);
           display.print("z");
           display.display();
          }
        }

}



// Person shapes
void personfront()
{
 display.fillRect(personx - 9, persony - 7, 19, 39, SSD1306_BLACK);// remove person
 display.fillCircle(personx, persony, 4, SSD1306_WHITE); //head
 display.drawLine(personx,persony+4, personx, persony+2, SSD1306_WHITE); // neck
 display.drawRect(personx-4, persony+5, 8, 10, SSD1306_WHITE); // body height
 display.drawLine(personx-5, persony+7, personx -7, persony +14, SSD1306_WHITE); //left arm
 display.drawLine(personx+4, persony+7, personx +6, persony +14, SSD1306_WHITE); //right arm
 display.drawLine(personx-3, persony+14, personx -6, persony +30, SSD1306_WHITE); //left leg
 display.drawLine(personx+3, persony+14, personx +6, persony +30, SSD1306_WHITE); //right leg
}


void personleft()
{



}


void personright()
{



}


void eyes() // middle
{
 display.fillCircle(personx, persony, 4, SSD1306_WHITE); // head
 display.drawPixel(personx -2, persony -1, SSD1306_BLACK);
 display.drawPixel(personx +2, persony -1, SSD1306_BLACK);
 display.display();
}


void eyesLeft() // eyes left
{
 display.fillCircle(personx, persony, 4, SSD1306_WHITE); // head
 display.drawPixel(personx -3, persony -1, SSD1306_BLACK);
 display.drawPixel(personx +1, persony -1, SSD1306_BLACK);
 display.display();
}


void eyesRight()
{
 display.fillCircle(personx, persony, 4, SSD1306_WHITE); // head
 display.drawPixel(personx -1, persony -1, SSD1306_BLACK);
 display.drawPixel(personx +3, persony -1, SSD1306_BLACK);
 display.display();
}


void mouth() // straight across no emotion
{
 display.drawLine(personx -2, persony +2, personx +2, persony +2, SSD1306_BLACK);
 display.display(); 
}


void happy()
{
// Upturn of mouth 
  display.drawPixel(personx -2, persony-4, SSD1306_BLACK);
  display.drawPixel(personx -1, persony-3, SSD1306_BLACK);
  display.drawPixel(personx, persony+2, SSD1306_BLACK);
  display.drawPixel(personx +1, persony-3, SSD1306_BLACK);
  display.drawPixel(personx +2, persony-4, SSD1306_BLACK);
  display.display();
}


void unhappy()
{
  // downturn of mouth
  display.drawPixel(personx -2, persony+2, SSD1306_BLACK);
  display.drawPixel(personx -1, persony+1, SSD1306_BLACK);
  display.drawPixel(personx, persony, SSD1306_BLACK);
  display.drawPixel(personx +1, persony+1, SSD1306_BLACK);
  display.drawPixel(personx +2, persony+2, SSD1306_BLACK);
  display.display();
}


// Animations
void walkleft()
{

}


void walkright()
{

}


void personwave()
{

}


void leftdoor()
{
 //display.drawLine(1, 24, 1, 61, SSD1306_WHITE);
 display.fillRect(1,24,8,50, SSD1306_WHITE); 
}


void rightdoor()
{
 //display.drawLine(127, 24, 127, 61, SSD1306_WHITE);
 display.fillRect(119,24,8,50, SSD1306_WHITE);
}


void snooze()
{
  display.setCursor(2,60);
  display.drawLine(40, 50, 40, 63, SSD1306_WHITE); // left bedpost
  display.drawLine(80, 50, 80, 63, SSD1306_WHITE); // right bedpost
  display.drawLine(41, 58, 79, 58, SSD1306_WHITE); // bed middle
  display.fillCircle(48, 53, 4, SSD1306_WHITE); // character head
  display.drawLine(52,56, 75, 56, SSD1306_WHITE); // character body
  display.drawLine(76, 57, 76, 52, SSD1306_WHITE); // foot
  display.display();
}



void hello()
{
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30,12);
    display.print("HELLO!");
    happy();
    display.display();
    digitalWrite(10, HIGH); // Turn on green LED
    delay(1000);
    digitalWrite(10, LOW); // Turn off green LED
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(30,12);
    display.print("HELLO!");
    display.display();
    display.setTextColor(SSD1306_WHITE);
}




void satellite101()
{

}

void satellite102()
{
  while (satellite2 == 1)
  {

     if (Serial.available())
    {
        

        String message = Serial.readStringUntil('\n');
        value = message.toInt();
        Serial.println(value);
        display.fillRect(50, 0, 50, 12, SSD1306_BLACK);
        display.display();  
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.print("Light = ");
        display.setCursor(50, 0);
        display.print(value);
        display.display();
        
        if(LowSoundPlayed == 0  && value < 300) // lamp on for light sensor
          {
           LowSoundPlayed = 1; MidSoundPlayed = 0; HighSoundPlayed = 0;
           display.fillRect(30, 40, 60, 12, SSD1306_BLACK);
           display.display();  
           display.setTextColor(SSD1306_WHITE);
           display.setCursor(30,40);
           display.print("Low Light");
           display.display();

           tone(9, 100);
           delay(500);
           noTone(9);

          }

        if(MidSoundPlayed == 0  && value > 300 && value < 780) // medium levels for light sensor
          {
           LowSoundPlayed = 0; MidSoundPlayed = 1; HighSoundPlayed = 0;
           display.fillRect(30, 40, 60, 12, SSD1306_BLACK);
           display.display();  
           display.setTextColor(SSD1306_WHITE);
           display.setCursor(30,40);
           display.print("Mid Light");
           display.display();

           tone(9, 600);
           delay(500);
           noTone(9);
          }

        if(HighSoundPlayed == 0  && value > 800) // bright light for light sensor
          {
           LowSoundPlayed = 0; MidSoundPlayed = 0; HighSoundPlayed = 1;
           display.fillRect(30, 40, 60, 12, SSD1306_BLACK);
           display.display();  
           display.setTextColor(SSD1306_WHITE);
           display.setCursor(30,40);
           display.print("High Light");
           display.display();

           tone(9, 1000);
           delay(500);
           noTone(9);
          }



    }
        delay(50);
  }
}
  
