#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int seed = 7155;
int n_functions = 2;

int potentiometer_pin = A0;
int button_forward_pin = 5;
int button_backward_pin = 6;

typedef void (*module_function)();
module_function functions[] = {&potentiometer_puzzle,
                               &switch_puzzle};

int screen_width = 20; // columns
int screen_height = 4; // rows
char hyphenation_hint = '|';

int buttondelay = 400; // delay length after button press to prevent doubleclick accidents

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
  pinMode(button_forward_pin, INPUT);
  pinMode(button_backward_pin, INPUT);
  lcd.begin(20,4);
  lcd.backlight();
}

void loop() {
  //potentiometer_puzzle();
  switch_puzzle();
}

void potentiometer_puzzle() {
  int range_min = 1;
  int range_max = 50;
  int target = random(range_min, range_max);
  String ohje = "Aseta P1 arvoon ";
  ohje += target;
  ohje += " ja paina B1 pohjaan.";
  display_message(ohje);
  
  target = random(range_min, range_max);
  ohje = "Pida B1 painettuna, aseta P1 arvoon ";
  ohje += target;
  ohje += " ja paasta B1 irti. ";
  display_message(ohje);
}

void switch_puzzle() {
  bool targets[] = { false, false, false, false };
  for(int i = 0; i < 4; i++) {
    targets[i] = random(0,2) == 0;
  }
  
  String ohje = "Aseta V1, V2, V3 ja V4 seuraaviin asentoihin:\n";
  for (int i = 0; i < 4; i++) {
    ohje += targets[i] ? "OFF " : "ON ";
  }
  ohje += "ja paina B1.";

  display_message(ohje);
}

void shuffle(module_function *funs, int len) {
  for (int i = 0; i < len - 1; i++) {
    long r = random(i, len);
    module_function tmp = funs[i];
    funs[i] = funs[r];
    funs[r] = tmp;
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
  while((*message_row).next) {
    rows++;
    message_row = (*message_row).next;
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
          if (top_row_index + i <= rows) {
            lcd.setCursor(0, i);
            lcd.print((*printed).text);
            printed = (*printed).next;
          }
      }
      refresh = false;
    }

    // move forward?
    if (digitalRead(button_forward_pin) == HIGH) {
      while(digitalRead(button_forward_pin) == HIGH){
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
      if (digitalRead(button_backward_pin) == HIGH) {
        while(digitalRead(button_backward_pin) == HIGH){
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

