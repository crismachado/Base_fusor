//VacuuMeter V.0.9
//This program queries MKS 901P and 999 series pressure transducers about pressure,
//either in torr or pascal, and displays the result on small Oled display.
// 27-09-2017 Finn Hammer


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
String sensorValue;
String pressureUnit;
String unitBuffer;
String unit;
const byte ledPin = 13;
const byte interruptPin3 = 3;
const byte interruptPin2 = 2;
int val = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (initializing the display)
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin3, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin3), pascalISP, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), fuspeakISP, FALLING);
}


void loop() {
  if (unit == "Pascal") {
    pascal();
  }

  if (unit == "torr") {
    torr();
  }
if (unit == "mbar") {
    mbar();
  }
  else if (unit == "Fuspeak") {
    fuspeak();
  }

  display.display();
}


void pascalISP() {
  val = digitalRead (2);
  if (val == LOW) {
    unit = "torr";
    Serial.print ("@253U!TORR;FF");
    
  }
  else   {
    unit = "Pascal";
  }
  Serial.print ("@253U!PASCAL;FF");
}


void fuspeakISP() {
    val = digitalRead (3);
  if (val == LOW) {
    unit = "mbar";
    Serial.print ("@253U!MBAR;FF");
    
  }
  else
  unit = "Fuspeak";
  Serial.print ("@253U!TORR;FF");
}

void torr () {
  Serial.print ("@254PR3?;FF");
  sensorValue = Serial.readString();
  sensorValue.remove(0, 7); //Remove the first 7 caracters from @253ACK7.61E+2;FF to get 7.39E+2;FF
  sensorValue.remove(7);  //Remove caracters from the 7th, to get 7.61E+2}
  pressureUnit = "torr";
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Foreline Pressure");
  display.setTextSize(2);
  display.setCursor(0, 9);
  display.print(sensorValue);
  display.setTextSize(1);
  display.setCursor(85, 25);
  display.print(pressureUnit);
}

void mbar () {
  Serial.print ("@254PR3?;FF");
  sensorValue = Serial.readString();
  sensorValue.remove(0, 7); //Remove the first 7 caracters from @253ACK7.61E+2;FF to get 7.39E+2;FF
  sensorValue.remove(7);  //Remove caracters from the 7th, to get 7.61E+2}
  pressureUnit = "mbar";
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Foreline in Mbar");
  display.setTextSize(2);
  display.setCursor(0, 9);
  display.print(sensorValue);
  display.setTextSize(1);
  display.setCursor(85, 25);
  display.print(pressureUnit);
}
void pascal () {
  Serial.print ("@254PR3?;FF");
  sensorValue = Serial.readString();
  sensorValue.remove(0, 7); //Remove the first 7 caracters from @253ACK7.61E+2;FF to get 7.39E+2;FF
  sensorValue.remove(7);  //Remove caracters from the 7th, to get 7.61E+2}
  pressureUnit = "Pascal";
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Foreline Pressure");
  display.setTextSize(2);
  display.setCursor(0, 9);
  display.print(sensorValue);
  display.setTextSize(1);
  display.setCursor(85, 25);
  display.print(pressureUnit);
}




void fuspeak () {
  Serial.print ("@254PR3?;FF");
  sensorValue = Serial.readString();
  sensorValue.remove(0, 7); //Remove the first 7 caracters from @253ACK7.61E+2;FF to get 7.39E+2;FF
  sensorValue.remove(7);  //Remove caracters from the 7th, to get 7.61E+2

  unitBuffer = sensorValue;
  unitBuffer.remove (0, 5);
  if (unitBuffer == "+2")  {
    pressureUnit = "torr";
  }
  else  if (unitBuffer == "+1") {
    pressureUnit = "torr";
  }
  else  if (unitBuffer == "+0") {
    pressureUnit = "torr";
  }
  else  if (unitBuffer == "-1") {
    pressureUnit = "micron";
    sensorValue.remove (1, 1);
    sensorValue.remove (3);
  }
  else  if (unitBuffer == "-2") {
    pressureUnit = "micron";
    sensorValue.remove (1, 1);
    sensorValue.remove (2);
  }

  else  if (unitBuffer == "-3") {
    pressureUnit = "micron";
    sensorValue.remove (1, 1);
    sensorValue.remove (1);
  }
  else  if (unitBuffer == "-4") {
    pressureUnit = "millitorr";
  }
  else  if (unitBuffer == "-5") {
    pressureUnit = "millitorr";
  }
  else  if (unitBuffer == "-6") {
    pressureUnit = "millitorr";

  }
  else pressureUnit = "endelse";


  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Foreline Pressure");
  display.setTextSize(2);
  display.setCursor(0, 9);
  display.print(sensorValue);
  display.setTextSize(1);
  display.setCursor(85, 25);
  display.print(pressureUnit);
}




