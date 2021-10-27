#include <OneButton.h>
#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>
#include <Adafruit_BME280.h>
#define SCREEN_WIDTH 128 //OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED dispaly width, in pixels
#define OLED_RESET  76// Rest pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0X3C ///< See datasheet for address; 0x3D for 128x64,
#define LOGO_HEIGHT 16
#define LOGO_WIDTH  16
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;

bool BmeStatus;
bool buttonState;
bool buttonStateTwo;
bool isStarted;
const int ButtonPIN = 23;
const int ButtonPinTwo = 22;
const byte BmeAddress = 0x76;
const char degree = 0xF8 ; // Decimal 248 = 0 xF8
float tempC;
float tempF;
int status;

OneButton button1 (ButtonPIN , false);
OneButton buttonTwo (ButtonPinTwo, false);

void setup() {
  Serial.begin(9600);
  button1.attachClick(click);
  button1.setClickTicks(250);
  button1.setPressTicks(4000);
  buttonTwo.attachClick(clickTwo);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); //Dont proceed, loop forever
  }

  BmeStatus = bme.begin (BmeAddress);
  if (BmeStatus == false) {
    Serial.printf("BME280 at address 0x%02 X failed tostart", BmeAddress);
  }

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Ethernet.begin(mac);
  delay(200);
  printIP();
  Serial.printf("LinkStatus: %i  \n", Ethernet.linkStatus());
  display.display();
  delay(2000);

  display.clearDisplay();

  testfillcircle();

  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}

void loop() {

  tempC = bme.readTemperature();
  tempF = (tempC * 9.0 / 5.0) + 32;
  testtempdegree();

  button1.tick();
  buttonTwo.tick();
  if (buttonState) {
    testfamilycall();
    buttonState = false;
    int randoCool = random(40000, 60000);

    for (int i = 1; i <= 255; i += 10) {
      setHue(5, true, randoCool , i, 255); //loop the rainbow
      delay (100);
    }
  }

  if (buttonStateTwo) {
    testemergstyles();
    buttonStateTwo = false;
    int randoWarm = random(100, 10000);

    for (int i = 1; i <= 255; i += 10) {
      setHue(5, true, randoWarm  , i, 255); //loop the rainbow
      delay (100);
    }
  }

  if (isStarted) {
    setHue(5, false, 0, 0, 0);
    delay(1000);
    isStarted = false;
  }
}

void testtempdegree(void) {

  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  display.setCursor(7, 9);
  display.printf("%0.1f%c", tempF, degree);
  display.display();
}

void testemergstyles(void) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  display.setCursor(10, 7);
  display.printf("Dialed", 47, 47);
  display.display();
  delay(800);

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  display.setCursor(35, 10);
  display.printf("911", 47, 47);
  display.display();
  delay(800);
}

void testfamilycall(void) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  display.setCursor(10, 7);
  display.printf("Dialed", 47, 47);
  display.display();
  delay(800);

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  display.setCursor(10, 7);
  display.printf("Family", 47, 47);
  display.display();
  delay(800);
}

//  display.clearDisplay();
//  display.setTextSize(5);
//  display.setTextColor(SSD1306_WHITE);
//  display.setRotation(0);
//  display.setCursor(20, 10);
//  display.printf("way", 47, 47);
//  display.display();
//  delay(800);
//}

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

void click() {
  isStarted = true;
  buttonState = true;
  Serial.println("click!!");
}
void clickTwo() {
  isStarted = true;
  buttonStateTwo = true;
  Serial.println("but2");
}
void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
    Serial.printf("%i.", Ethernet.localIP()[thisByte]);
  }
  Serial.printf("%i\n", Ethernet.localIP()[3]);
}
