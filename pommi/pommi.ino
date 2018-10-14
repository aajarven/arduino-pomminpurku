#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define N_DIGITAL_PINS 14
#define N_ANALOG_PINS 6
#define ANALOG_TOLERANCE 10

int seed = 7155;
int n_functions = 7;

int digital_pin_values[N_DIGITAL_PINS];
int analog_pin_values[N_ANALOG_PINS];

// final pins
int potentiometer_pin = A0;
int button1_pin = 5;
int button2_pin = 6;

// temporary pins
int *switch_pins[] = {10, 11, 12, 13};

int screen_width = 20; // columns
int screen_height = 4; // rows
char hyphenation_hint = '|';

int buttondelay = 400; // delay length after button press to prevent doubleclick accidents

typedef bool (*module_function)();
module_function functions[] = {&potentiometer_puzzle,
                               &switch_puzzle,
                               &moduuli3,
                               &moduuli4,
                               &moduuli5,
                               &moduuli6,
                               &moduuli7
                              };

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

struct TextRow {
  /**
   * Used to store instructions and other text splitted to individual rows.
   * Resembles a node in a linked list. Contains a pointer to the text row.
   * For the first and last rows the previous_row and next_row (respectively)
   * are NULL.
   */
   
    TextRow* previous;
    TextRow* next;
    char* text;
};

void setup() {
  Serial.begin(9600);
  
  randomSeed(seed);
  for (int pin = 8; pin > 1; pin--) {
    pinMode(pin, OUTPUT);
  }
  pinMode(potentiometer_pin, INPUT);
  pinMode(button1_pin, INPUT);
  lcd.begin(20, 4);
  lcd.backlight();

  // temporary settings
  for (int pin = 0; pin < 4; pin++) {
    pinMode(switch_pins[pin], INPUT);
  }
}

void loop() {
  //  module_function *f = functions;
  //  shuffle(functions, n_functions);
  //  for (int i = 0; i < n_functions; i++) {
  //    functions[i](seed);
  //  }

  display_message("Jukolan talo, ete|lai|ses|sa Hameessa, "
    "seisoo eraan maen poh|joi|sel|la rin|teel|la, liki "
    "Toukolan kylaa. Sen la|hei|sin ym|pa|ris|to on kivinen "
    "tanner, mutta alempana alkaa pellot, joissa, ennenkuin "
    "talo oli havioon mennyt, aaltoili terainen vilja.");
  read_digital_values(digital_pin_values);
  read_analog_values(analog_pin_values);

  if (potentiometer_puzzle()) {
    lcd.clear();
    lcd.print("potikkapuzzle ok");
  } else {
    lcd.clear();
    lcd.print("potikkapuzzle epaonnistui");
  }
  delay(5000);

  read_digital_values(digital_pin_values);
  read_analog_values(analog_pin_values);

  if (switch_puzzle()) {
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
     Requires the user to adjust the potentiometer to the given value,
     press the button down and adjust the potentiometer to a new value
     while holding the button down. If the button is pressed or released
     on a wrong value, the puzzle is failed, otherwise successful.
  */
  int range_min = 1;
  int range_max = 50;
  int target = random(range_min, range_max);
  int potentiometer_value;
  bool checked_digitals[] = {false, false, false, false, false, false, false, false, false, false, true, true, true, true};
  bool checked_analogs[] = {false, false, false, false, false};

  while (true) {
    potentiometer_value = map(analogRead(potentiometer_pin), 0, 1023, range_min - 1, range_max);
    display_potentiometer(target, potentiometer_value);

    if (digitalRead(button1_pin) == HIGH) {
      if (potentiometer_value == target) {
        target = random(range_min, range_max);

        while (digitalRead(button1_pin) == HIGH) {
          potentiometer_value = map(analogRead(potentiometer_pin), 0, 1023, range_min - 1, range_max);
          display_potentiometer(target, potentiometer_value);

          delay(200);
          if (!check_changes(checked_digitals, checked_analogs)) {
            return false;
          }
        }

        if (potentiometer_value == target) {
          return true;
        } else {
          return false;
        }

      } else {
        return false;
      }
    }

    if (!check_changes(checked_digitals, checked_analogs)) {
      return false;
    }
    delay(200);
  }
}

void display_potentiometer(int target, int value) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(target);
  lcd.setCursor(0, 1);
  lcd.print(value);
}

bool switch_puzzle() {
  /**
     Shows the correct on/off statuses of the switches on display and after the
     button has been pressed checks if the user has set them correctly. Returns
     true if all switches are ok, otherwise false.
  */
  bool target_states[] = {false, false, false, false};
  bool switch_states[] = {false, false, false, false};
  bool checked_digitals[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false};
  bool checked_analogs[] = {true, false, false, false, false};

  lcd.clear();
  for (int i = 0; i < 4; i++) {
    target_states[i] = random(0, 2) == 1;
    lcd.print(target_states[i]);
    lcd.print(" ");
  }

  while (true) {
    update_switches(switch_states);
    if (digitalRead(button1_pin) == HIGH) {
      for (int i = 0; i < 4; i++) {
        if (target_states[i] != switch_states[i]) {
          return false;
        }
      }
      return true;
    }
    if (!check_changes(checked_digitals, checked_analogs)) {
      return false;
    }
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

void update_switches(bool *states) {
  for (int i = 0; i < 4; i++) {
    states[i] = digitalRead(switch_pins[i]) == HIGH;
  }
}

bool check_changes(bool *checked_digitals, bool *checked_analogs) {
  int current_digital_values[N_DIGITAL_PINS];
  read_digital_values(current_digital_values);
  for (int i = 0; i < N_DIGITAL_PINS; i++) {
    if (checked_digitals[i]) {
      if (current_digital_values[i] != digital_pin_values[i]) {
        return false;
      }
    }
  }

  int current_analog_values[N_ANALOG_PINS];
  read_analog_values(current_analog_values);
  for (int i = 0; i < N_ANALOG_PINS; i++) {
    if (checked_analogs[i]) {
      if (abs(current_analog_values[i] - analog_pin_values[i]) > ANALOG_TOLERANCE) {
        return false;
      }
    }
  }

  return true;
}

void read_digital_values(int pin_values[N_DIGITAL_PINS]) {
  for (int i = 0; i < N_DIGITAL_PINS; i++) {
    pin_values[i] = digitalRead(i);
  }
}

void read_analog_values(int pin_values[N_ANALOG_PINS]) {
  for (int i = 0; i < N_ANALOG_PINS; i++) {
    pin_values[i] = analogRead(i);
  }
}

struct TextRow* split_message(String message, int row_length) {
  /**
   * Splits the given string to pieces with a maximum length of the given row_length.
   * Splits are allowed at whitespace characters and dashes and additional positions
   * where hyphenation is allowed can be specified by inserting a hyphenation hint
   * (see variable hyphenation_hint) into the text. If a word is longer than the row,
   * a row is filled and a new line added.
   * 
   * Returns the first row of text as an TextRow.
   */

   struct TextRow* first_row = new TextRow;
   (*first_row).previous = NULL;
   (*first_row).next = NULL;
   (*first_row).text = NULL;

   int char_index = 0;
   struct TextRow* row = first_row;
   
   while (char_index < message.length()) {
    char* text = malloc(sizeof(char) * (row_length + 1));
    int last_break = 0;
    int row_index = 0;
    bool requires_hyphenation = true;
    
    while (row_index < row_length){
      char c = message.charAt(char_index);
      
      if (c == '\n') {
        text[row_index] = '\0';
        char_index++;
        last_break = row_index;
        requires_hyphenation = false;
        break;
        
      } else if (c == '\0') {
        text[row_index] = c;
        requires_hyphenation = false;
        last_break = row_index;
        char_index++;
        break;
        
      } else if (c == ' ') {
        if (row_index != 0) {
          text[row_index] = c;
          last_break = row_index;
          row_index++;
          requires_hyphenation = false;
        }
        char_index++;
        
      } else if (c == hyphenation_hint) {
        char_index++;
        last_break = row_index;
        requires_hyphenation = true;
        
      } else {
        text[row_index] = c;
        char_index++;
        row_index++;
      }
    }

    /* ensure that the row is properly terminated at an allowed position (if possible),
     * add hyphenation (if needed) and set the char index so that the next row starts
     * after the last allowed line terminator
     */
    if (last_break == 0) {
      text[row_length-1] = '-';
      text[row_length] = '\0';
      char_index--;
    } else {
      if (requires_hyphenation) {
        text[last_break] = '-';
      }
      text[last_break + 1] = '\0';
      char_index -= (row_index - last_break);
    }

    (*row).text = text;

    if (char_index < message.length()){
      struct TextRow* next = new TextRow;
      (*next).previous = row;
      (*next).next = NULL;
      (*row).next = next;
      row = next;
    }
   }

   return first_row;
}

void display_message(String message) {
  struct TextRow *first_row = split_message(message, screen_width);

  // count rows
  int rows = 1;
  struct TextRow *message_row = first_row;
  Serial.println((*message_row).text);
  while((*message_row).next) {
    rows++;
    message_row = (*message_row).next;
    Serial.println((*message_row).text);
  }

  // show the text
  int top_row_index = 0;
  struct TextRow *top_row = first_row;
  bool refresh = true;
  bool show_text = true;
  
  while (show_text){
    
    if (refresh){
      struct TextRow *printed = top_row;
      lcd.clear();
      for (int i=0; i<screen_height; i++){
          if (top_row_index + i < rows) {
            lcd.setCursor(0, i);
            lcd.print((*printed).text);
            printed = (*printed).next;
          }
      }
      refresh = false;
    }

    // move forward?
    if (digitalRead(button1_pin) == HIGH) {
      while(digitalRead(button1_pin) == HIGH){
        delay(buttondelay);
      }
      
      refresh = true;
      
      for (int i=0; i<screen_height; i++){
        top_row = (*top_row).next;
        top_row_index++;
        
        if (!(*top_row).next){
          if (i < screen_height - 1){ // running out of rows before a screenful
            show_text = false;
            break;
          }
        }
      }
      
      delay(buttondelay);
    }

    // move backward?
    if (top_row_index > 0) {
      if (digitalRead(button2_pin) == HIGH) {
        while(digitalRead(button2_pin) == HIGH){
          delay(buttondelay);
        }
        for (int i=0; i<screen_height; i++){
          top_row = (*top_row).previous;
          top_row_index--;
          refresh = true;
          
          if (!(*top_row).previous){
            break;
          }
        }
        delay(buttondelay);
      }
    }
  }

  // free the memory
  freeTexts(first_row);
}

void freeTexts(struct TextRow *message){
  struct TextRow *current_row = message;
  while ((*current_row).next){
    struct TextRow *next_row = (*current_row).next;
    free((*current_row).text);
    free(current_row);
    current_row = next_row;
  }
  free((*current_row).text);
  free(current_row);
}

