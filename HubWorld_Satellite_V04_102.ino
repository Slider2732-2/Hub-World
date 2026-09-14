// Hub World Satellite 
// This one is 102 - change to suit

// Replies with full protocol to Hub 
// Animates character turning on/off a light - maybe tone for action
// If dark for over 10 seconds he goes to sleep (sleep mode from the Hub)
// Random movements such as arms moveets, legs.
// Dark mode triggers noise from hub of a ping and "Light = Dark!"




#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(128,64,&Wire,-1);


int personx = 80;
int persony = 30;
int eye1;
int eye2;
int looper; // general looping function


// Satellite specific
int lightLevel = 0;
int lampOn = 0;
int sun = 0;
int moon = 0;

const int LED = LED_BUILTIN;
const int SATELLITE_NUMBER = 102;

// Message reception
int messagePart = 0;
int receivedValues[4];

String incomingNumber = "";

int Hub = 0; // has the Hub been detected



void setup() 
{
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);

  pinMode(LED, OUTPUT);
  Serial.begin(115200);

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(10,10);
  display.print("Hub World");
  display.setTextSize(1); 
  display.setCursor(28,56);
  display.print("Satellite 102");

  display.display();

  delay(2000);

  display.clearDisplay();
}



void loop() 
{
    // ----  Start of basic screen
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
   

    display.setCursor(2,56);
    for(int a = 2; a <= 129; a++) {display.print("_");}

    leftdoor();
    rightdoor();
    display.display();

    // --- End of basic screen

    // display character person  
    personfront();
    eyes();
    mouth();
    display.display();

  



  // Check for incoming serial data
  while (Serial.available() > 0) {

    char incomingChar = Serial.read();

    // Newline means one number is complete
    if (incomingChar == '\n') {

      int value = incomingNumber.toInt();
      incomingNumber = "";

      // Waiting for the start marker
      if (messagePart == 0) {

        if (value == 999) {
          receivedValues[0] = value;
          messagePart = 1;
        }
      }

      // Receiving the remaining three fields
      else {

        receivedValues[messagePart] = value;
        messagePart++;

        // Four fields received
        if (messagePart == 4) {

          int satelliteNumber = receivedValues[1];
          int command = receivedValues[2];
          int specialCommand = receivedValues[3];

          // Is this message for me?
          if (satelliteNumber == SATELLITE_NUMBER) 
          {
            Hub = 1; // hub has been seen
            // Send acknowledgement
            digitalWrite(LED, HIGH);

            Serial.println(888);
            Serial.println(satelliteNumber);
            Serial.println(command);
            Serial.println(specialCommand);

            digitalWrite(LED, LOW);

            // Animate from left side to middle
            
            

            // print greeting on screen
            display.setCursor(30,12);
            display.print("HELLO!");
            display.display();

            delay(2000);

            // Move character into room
            for (looper = 105; looper > 100; looper--)
               {
                //looper = looper +1; // double speed walk to the right
                personx = looper;
                personfront();
                eyes();
                mouth();
                display.display();
                delay(1);
               } 
            
            display.setTextColor(SSD1306_BLACK);
            display.setCursor(30,12);
            display.print("HELLO!");
            display.display();
            display.setTextColor(SSD1306_WHITE);

          }

          // Ready for the next message
          messagePart = 0;
        }
      }
    }

    // Ignore carriage return
    else if (incomingChar != '\r') {
      incomingNumber += incomingChar;
    }
  }

  // Other Hub World code runs here
  lightLevel = analogRead(A0);
  if (Hub == 1) {Serial.println(lightLevel);} // send serial data to Hub
  delay(250); // not sampling constantly and sending serial data

  display.setCursor(0,0);
            display.print("Light = ");
            display.fillRect(48,0,30,10, SSD1306_BLACK);
            display.setCursor(48,0);
            display.print(lightLevel);
            display.display();


  drawLamp(); // draw the unlit lamp on screen

  // If light level is under 300 he should walk over and switch on the lamp
  if(lightLevel <=300 && lampOn == 0)
    {
      lampOn = 1;
      for (looper = 80; looper > 52; looper --)
          { personx = looper; looper --; personfront(); eyes(); mouth();}
      // Light switches on    
      display.fillRect(30, 38, 10, 15, SSD1306_WHITE);
      for (looper = 52; looper < 81; looper ++)
          { personx = looper; looper ++; personfront(); eyes(); mouth();}
    }

  // if light level greater than 300 he should walk over and switch off the lamp
  if(lightLevel >=400 && lampOn == 1) // bit of hysteresis
    {
      lampOn = 0;
      for (looper = 80; looper > 52; looper --)
          { personx = looper; looper --; personfront(); eyes(); mouth();}
      // Light switches off    
      display.fillRect(31, 39, 8, 13, SSD1306_BLACK);
      for (looper = 52; looper < 81; looper ++)
          { personx = looper; looper ++; personfront(); eyes(); mouth();}
    }

  // if light level greater than 800 draw a sun
  if (lightLevel > 800 && sun == 0)
    {
      sun = 1;
      display.fillCircle(115, 10, 10, SSD1306_WHITE);
      display.display();
      delay(3000); // temporary
    }

  if (lightLevel < 780 && sun == 1) // bit of hysteresis
    { 
      sun = 0;
      display.fillCircle(115, 10, 10, SSD1306_BLACK); 
      display.display();
    }  

   // if light level less than 200 draw a moon 
   if (lightLevel < 200 && moon == 0)
      {
        moon =1;
        display.fillCircle(115, 10, 10, SSD1306_WHITE);
        display.fillCircle(118, 7, 10, SSD1306_BLACK); // crescent moon
        display.display();
      }

    // if light level > 250 and moon is showing blank it
    if(lightLevel > 250 && moon == 1)
      {
        moon = 0;
        display.fillCircle(115, 10, 10, SSD1306_BLACK);
        display.display();
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
 display.display();
}

void eyes() // middle
{
 display.fillCircle(personx, persony, 4, SSD1306_WHITE); // head
 display.drawPixel(personx -2, persony -1, SSD1306_BLACK);
 display.drawPixel(personx +2, persony -1, SSD1306_BLACK);
 display.display();
}


void mouth() // straight across no emotion
{
 display.drawLine(personx -2, persony +2, personx +2, persony +2, SSD1306_BLACK);
 display.display(); 
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

void drawLamp()
{
  display.drawRect(30, 38, 10, 15, SSD1306_WHITE);
  display.drawLine(35, 54,35, 59, SSD1306_WHITE);
  display.fillRect(31, 59, 8, 6, SSD1306_WHITE);
}