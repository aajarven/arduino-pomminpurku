#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int seed = 71551;
int n_functions = 2;

int potentiometer_pin = A0;
int button_forward_pin;

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
//  module_function *f = functions;
//  shuffle(functions, n_functions);
//  for (int i = 0; i < n_functions; i++) {
//    functions[i](seed);
//  }

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
  while(true) {
    if(digitalRead(button_forward_pin) == HIGH) break;
    delay(50);
  }
  target = random(range_min, range_max);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pida B1 painettuna,");
  lcd.setCursor(0,1);
  lcd.print("aseta P1 arvoon ");
  lcd.print(target);
  lcd.setCursor(0,2);
  lcd.print("ja paasta B1 irti.");
  while(true) {
    if(digitalRead(button_forward_pin) == HIGH) break;
    delay(50);
  }
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
    lcd.print(targets[i] ? "ON " : " OFF");
  }
  lcd.setCursor(0,3);
  lcd.print("ja paina B1");
  while(true) {
    if(digitalRead(button_forward_pin) == HIGH) break;
    delay(50);
  }
}

void shuffle(module_function *funs, int len) {
  for (int i = 0; i < len - 1; i++) {
    long r = random(i, len);
    module_function tmp = funs[i];
    funs[i] = funs[r];
    funs[r] = tmp;
  }
}

