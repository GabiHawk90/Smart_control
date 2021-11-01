#include <OneButton.h>
#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>
#include <wemo.h>
#include <Ultrasonic.h> // include Seeed Studio ultrasonic ranger library
#include <Adafruit_BME280.h>

//oled setup
#define SCREEN_WIDTH 128 //OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED dispaly width, in pixels
#define OLED_RESET  4// Rest pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0X3C ///< See datasheet for address; 0x3D for 128x64,

//OBJECTS
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;
const int ULTRAPIN = 2;
Ultrasonic ultraSon (ULTRAPIN);
const int BUTTONPIN = 23;
const int BUTTONPINTWO = 22;
OneButton button1 (BUTTONPIN , false);
OneButton buttonTwo (BUTTONPINTWO, false);

const byte BMEADDRESS = 0x76;
bool BmeStatus;
bool isStarted;
bool buttonState;
bool buttonStateTwo;
bool isLampOn;
bool isFanOn;

const char DEGREE = 0xF8 ; // Decimal 248 = 0 xF8
float tempC;
float tempF;




void setup() {
  Serial.begin(9600);

  startObjects();

  //button set up
  button1.attachClick(click);
  buttonTwo.attachClick(clickTwo);
  button1.setClickTicks(500);
  buttonTwo.setClickTicks(500);

  showFillCircle();
}

void loop() {
  long rangeInInches;

  rangeInInches = ultraSon.MeasureInInches();
//  Serial.printf("%i inch\n", rangeInInches);

  setWemos(rangeInInches);

  tempC = bme.readTemperature();
  tempF = (tempC * 9.0 / 5.0) + 32;
  showTempDegree();

  button1.tick();
  buttonTwo.tick();
  if (buttonState) {
    showFamilyCall();
    buttonState = false;
    int randoCool = random(40000, 60000);
    setHues(randoCool, true);
  }
  if (buttonStateTwo) {
    showEmergCall();
    buttonStateTwo = false;
    int randoWarm = random(100, 10000);
    setHues(randoWarm, true);
  }
  //turn hues off
  if (isStarted) {
    setHues(0, false);

    delay(1000);
    isStarted = false;
  }
}

void showTempDegree(void) {

  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  display.setCursor(7, 9);
  display.printf("%0.1f%c", tempF, DEGREE);
  display.display();
}

void showEmergCall(void) {
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
void showFamilyCall(void) {
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
void showFillCircle(void) {
  display.clearDisplay();

  for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(2);
  }
  delay(2000);
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
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
void startObjects() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); //Dont proceed, loop forever
  }

  BmeStatus = bme.begin (BMEADDRESS);
  if (BmeStatus == false) {
    Serial.printf("BME280 at address 0x%02 X failed tostart", BMEADDRESS);
  }

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Ethernet.begin(mac);
  delay(200);
  printIP();
  Serial.printf("LinkStatus: %i  \n", Ethernet.linkStatus());

}
void setHues(int color, bool hueOn) {
  if (hueOn) {
    for (int i = 0; i <= 255; i += 10) {
      for (int h = 1; h <= 6; h++) {
        setHue(h, true, color , i, 255);
        delay (50);
      }
    }
  }
  else {
    for (int h = 1; h <= 6; h++) {
      setHue(h, false, 0, 0, 0);
    }
  }
}
void setWemos(int rangeInInches) {
  if (rangeInInches <= 20) {//when room is empty fan
    if (!isLampOn) {
      switchOFF(3);
      switchON(0);
      isLampOn = true;
      isFanOn = false;
    }
  }
  else {
    if (!isFanOn) {
      switchON(3);
      switchOFF(0);
      isFanOn = true;
      isLampOn = false;
    }
  }
}
