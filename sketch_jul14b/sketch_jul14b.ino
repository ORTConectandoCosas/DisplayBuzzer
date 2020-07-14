/********************************/
// include the library code
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
/**********************************************************/

static const uint8_t pD0   = 16;
static const uint8_t pD1   = 5;
static const uint8_t pD2   = 4;
static const uint8_t pD3   = 0;
static const uint8_t pD4   = 2;
static const uint8_t pD5   = 14;
static const uint8_t pD6   = 12;
static const uint8_t pD7   = 13;
static const uint8_t pD8   = 15;
static const uint8_t pD9   = 3;
static const uint8_t pD10  = 1;

int lcdChars = 16;
int lcdLines = 1;
int tim = 250;

int soundBuzzer = false;
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, lcdChars, lcdLines); // set the LCD address to 0x27 for a 16 chars and 1 line display
/*********************************************************/
void setup()
{
  Wire.begin(pD2, pD1); 
  lcd.init();// initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.home();
 
}
/*********************************************************/
void loop() 
{
  lcd.home(); // set the cursor to home
  lcd.print("                LIMPIEZA SOLICITADA!"); //print message
  for (int i = 0; i < 36; i++)
  {
    if(soundBuzzer){
      tone(pD3, 3000, 500);
    }
    lcd.scrollDisplayLeft();
    //Scrolls the contents of the display one space to the left.// Print a message to the LCD.
    delay(tim); //wait for 250 microseconds
    soundBuzzer = !soundBuzzer;
  }
  lcd.clear(); //Clears the LCD screen and positions the cursor in the upper-left  corner.
}
/************************************************************/
