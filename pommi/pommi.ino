#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int seed = 7155;
int n_functions = 7;

// final pins
int potentiometer_pin = A0;
int button1_pin = 5;

// temporary pins
int *switch_pins[] = {10, 11, 12, 13};

typedef bool (*module_function)();
module_function functions[] = {&potentiometer_puzzle,
                               &switch_puzzle,
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
  pinMode(button1_pin, INPUT);
  lcd.begin(20,4);
  lcd.backlight();

  // temporary settings
  for (int pin = 0; pin < 4; pin++){
    pinMode(switch_pins[pin], INPUT);
  }
}

void loop() {
//  module_function *f = functions;
//  shuffle(functions, n_functions);
//  for (int i = 0; i < n_functions; i++) {
//    functions[i](seed);
//  }

  if (potentiometer_puzzle()){
    lcd.clear();
    lcd.print("potikkapuzzle ok");
  } else {
    lcd.clear();
    lcd.print("potikkapuzzle epaonnistui");
  }
  delay(5000);

  if (switch_puzzle()){
    lcd.clear();
    lcd.print("switchit ok");
  } else {
    lcd.clear();
    lcd.print("switchit ei ok");
  }
  delay(5000);
  
}

bool potentiometer_puzzle() {
  /**
   * Requires the user to adjust the potentiometer to the given value,
   * press the button down and adjust the potentiometer to a new value
   * while holding the button down. If the button is pressed or released
   * on a wrong value, the puzzle is failed, otherwise successful.
   */
  int range_min = 1;
  int range_max = 50;
  int target = random(range_min, range_max);
  int potentiometer_value;
  
  while(true){
    potentiometer_value = map(analogRead(potentiometer_pin), 0, 1023, range_min - 1, range_max);
    display_potentiometer(target, potentiometer_value);
    
    if (digitalRead(button1_pin) == HIGH){
      if (potentiometer_value == target){
        target = random(range_min, range_max);
        
        while(digitalRead(button1_pin) == HIGH){
          potentiometer_value = map(analogRead(potentiometer_pin), 0, 1023, range_min - 1, range_max);
          display_potentiometer(target, potentiometer_value);
          delay(200);
        }
        
        if (potentiometer_value == target){
          return true;
        } else {
          return false;
        }
        
      } else {
        return false;
      }
    }
    
    delay(200);
  }
}

void display_potentiometer(int target, int value){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(target);
    lcd.setCursor(0,1);
    lcd.print(value);
}

bool switch_puzzle() {
  /**
   * Shows the correct order 
   */
  bool target_states[] = {false, false, false, false};
  bool switch_states[] = {false, false, false, false};
  for (int i = 0; i < 4; i++) {
    target_states[i] = random(0,2) == 1;
  }
  while(true) {
    update_switches(switch_states);
    lcd.clear();
    for (int i=0; i<4; i++){
      lcd.print(target_states[i]);
      lcd.print(" ");
    }
    if (digitalRead(button1_pin) == HIGH) {
      for (int i = 0; i < 4; i++) {
        if (target_states[i] != switch_states[i]){
          return false;
        }
      }
      return true;
    }
    delay(200);
  }
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

void update_switches(bool *states){
  for(int i = 0; i<4; i++){
    states[i] = digitalRead(switch_pins[i]) == HIGH;
  }
}

