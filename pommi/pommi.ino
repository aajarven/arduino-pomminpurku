int seed = 71551;
int n_functions = 7;
int potentiometer_pin = A0;
typedef bool (*module_function)(int);
module_function functions[] = {&moduuli1,
                               &moduuli2,
                               &moduuli3,
                               &moduuli4,
                               &moduuli5,
                               &moduuli6,
                               &moduuli7};
int led_pins[] = {2, 3, 4, 5, 6, 7, 8};

void setup() {
  randomSeed(seed);
  for (int pin = 8; pin > 1; pin--) {
    pinMode(pin, OUTPUT);
  }
  pinMode(potentiometer_pin, INPUT);
  
}

void loop() {
  int potentiometer_value = analogRead(potentiometer_pin);
  light_leds(ceil(potentiometer_value/1023.0 * 7.0), led_pins, 7);

}

bool moduuli1(int seed) {
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
}

bool moduuli2(int seed) {
  digitalWrite(3, HIGH);
  delay(500);
  digitalWrite(3, LOW);
}

bool moduuli3(int seed) {
  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(4, LOW);
}

bool moduuli4(int seed) {
  digitalWrite(5, HIGH);
  delay(500);
  digitalWrite(5, LOW);
}

bool moduuli5(int seed) {
  digitalWrite(6, HIGH);
  delay(500);
  digitalWrite(6, LOW);
}

bool moduuli6(int seed) {
  digitalWrite(7, HIGH);
  delay(500);
  digitalWrite(7, LOW);
}

bool moduuli7(int seed) {
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

void light_leds(int n, int *pins, int n_pins){
  for(int i=0; i < n_pins; i++){
    digitalWrite(pins[i], LOW);
  }
  for(int i=0; i < n; i++){
    digitalWrite(pins[i], HIGH);
  }
}

