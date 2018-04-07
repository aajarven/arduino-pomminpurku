#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int seed = 71551;
int n_functions = 7;

int potentiometer_pin = A0;

typedef bool (*module_function)();
module_function functions[] = {&potikka_puzzle,
                               &moduuli2,
                               &moduuli3,
                               &moduuli4,
                               &moduuli5,
                               &moduuli6,
                               &moduuli7};
int led_pins[] = {2, 3, 4, 5, 6, 7, 8};

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

void setup() {
  randomSeed(seed);
  for (int pin = 8; pin > 1; pin--) {
    pinMode(pin, OUTPUT);
  }
  pinMode(potentiometer_pin, INPUT);
  lcd.begin(20,4);
  lcd.backlight();
}

void loop() {
  if (potikka_puzzle()){
    lcd.clear();
    lcd.print("potikkapuzzle ok");
  }
  delay(5000);
}

bool potikka_puzzle() {
  int rounds = 0;
  int target = random(1, 50);
  int correct_cycles = 0;
  
  while(true){
    int potentiometer_value = map(analogRead(potentiometer_pin), 0, 1023, 0, 50);
    if (potentiometer_value == target) {
      correct_cycles++;
      if (correct_cycles > 5){
        rounds++;
        if (rounds > 1) {
          return true;
        } else {
          target = random(1, 50);
          correct_cycles = 0;
        }
      }
    } else {
      correct_cycles = 0;
    }
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(target);
    lcd.setCursor(0,1);
    lcd.print(potentiometer_value);
    delay(200);
  }
}

bool moduuli2() {
  digitalWrite(3, HIGH);
  delay(500);
  digitalWrite(3, LOW);
}

bool moduuli3() {
  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(4, LOW);
}

bool moduuli4() {
  digitalWrite(5, HIGH);
  delay(500);
  digitalWrite(5, LOW);
}

bool moduuli5() {
  digitalWrite(6, HIGH);
  delay(500);
  digitalWrite(6, LOW);
}

bool moduuli6() {
  digitalWrite(7, HIGH);
  delay(500);
  digitalWrite(7, LOW);
}

bool moduuli7() {
  digitalWrite(8, HIGH);
  delay(500);
  digitalWrite(8, LOW);
}

void shuffle(module_function *funs, int len) {
  for (int i = 0; i < len - 1; i++) {
    long r = random(i, len);
    module_function tmp = funs[i];
    funs[i] = funs[r];
    funs[r] = tmp;
  }
}

