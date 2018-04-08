#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int seed = 7155;
int n_functions = 2;

int potentiometer_pin = A0;
int button_forward_pin = 5;

typedef void (*module_function)();
module_function functions[] = {&potentiometer_puzzle,
                               &switch_puzzle};

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

void setup() {
  randomSeed(seed);
  pinMode(button_forward_pin, INPUT);
  lcd.begin(20,4);
  lcd.backlight();
}

void loop() {
  potentiometer_puzzle();
  switch_puzzle();
}

void potentiometer_puzzle() {
  int range_min = 1;
  int range_max = 50;
  int target = random(range_min, range_max);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aseta P1 arvoon ");
  lcd.print(target);
  lcd.setCursor(0,1);
  lcd.print("ja paina B1 pohjaan");

  wait_for_button_press(button_forward_pin);
  
  target = random(range_min, range_max);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pida B1 painettuna,");
  lcd.setCursor(0,1);
  lcd.print("aseta P1 arvoon ");
  lcd.print(target);
  lcd.setCursor(0,2);
  lcd.print("ja paasta B1 irti.");
  
  wait_for_button_press(button_forward_pin);
}

void switch_puzzle() {
  bool targets[] = { false, false, false, false };
  for(int i = 0; i < 4; i++) {
    targets[i] = random(0,2) == 0;
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aseta V1, V2, V3, V4");
  lcd.setCursor(0,1);
  lcd.print("asentoihin ");
  lcd.setCursor(0,2);
  for (int i = 0; i < 4; i++) {
    lcd.print(targets[i] ? "OFF " : "ON ");
  }
  lcd.setCursor(0,3);
  lcd.print("ja paina B1");
  
  wait_for_button_press(button_forward_pin);
}

void shuffle(module_function *funs, int len) {
  for (int i = 0; i < len - 1; i++) {
    long r = random(i, len);
    module_function tmp = funs[i];
    funs[i] = funs[r];
    funs[r] = tmp;
  }
}

void wait_for_button_press(int pin){
  /**
   * Waits until button has been pressed and released
   */
    // wait for button press
  while(digitalRead(button_forward_pin) != HIGH) {
    delay(50);
  }

  // wait for button release
  while(digitalRead(button_forward_pin) != LOW) {
    delay(50);
  }
}

