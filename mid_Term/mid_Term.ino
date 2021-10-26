
#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 //OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED dispaly width, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA,SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:   A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO: 2(SDA), 3 (SCL), ...
#define OLED_RESET  76// Rest pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0X3C ///< See datasheet for address; 0x3D for 128x64,
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//#define

#define LOGO_HEIGHT 16
#define LOGO_WIDTH  16


void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHAPVCC = generate display voltage from 3.3V Internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); //Dont proceed, loop forever
  }

  //Show initial display buffer contents on the screen --
  //the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); //Pause for 2 seconds

  //Clear the buffer
  display.clearDisplay();

  //Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  //Show the display buffer on the screen. You MUST call display() after
  //drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...


  testfillcircle();
  testdrawstyles();




  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);


}



void loop() {
}



void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(5);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  display.setCursor(7, 7);
  display.printf("Help", 47, 47);
  display.display();
  delay(1000);

display.clearDisplay();
display.setTextSize(5);
display.setTextColor(SSD1306_WHITE);
display.setRotation(0);
display.setCursor(35, 10);
display.printf("is", 47, 47);
display.display();
delay(1000);

display.clearDisplay();
display.setTextSize(5);
display.setTextColor(SSD1306_WHITE);
display.setRotation(0);
display.setCursor(35, 10);
display.printf("on", 47, 47);
display.display();
delay(1000);

display.clearDisplay();
display.setTextSize(5);
display.setTextColor(SSD1306_WHITE);
display.setRotation(0);
display.setCursor(20, 10);
display.printf("the", 47, 47);
display.display();
delay(1000);


display.clearDisplay();
display.setTextSize(5);
display.setTextColor(SSD1306_WHITE);
display.setRotation(0);
display.setCursor(20, 10);
display.printf("way", 47, 47);
display.display();
delay(1000);



}
void testfillcircle(void) {
  display.clearDisplay();

  for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(2);
  }

  delay(2000);

}
