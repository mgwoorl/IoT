#define GREEN_LED_PIN 4 // зеленый 
#define YELLOW_LED_PIN 3 // желтый (синий)
#define RED_LED_PIN 2 // красный 

#define SET_AUTO_MODE 'a'
#define SET_MANUAL_MODE 'm'

#define SET_MANUAL_RED 'r'
#define SET_MANUAL_GREEN 'g'

#define STATE_AUTO 0
#define STATE_MANUAL 1

#define BLINK_INTERVAL 500

int state = STATE_AUTO;
long int previous_millis = 0;
int iteration = 0;

long int green_light = 10000; // зеленый светится (10 с)
long int green_blink = 2000; // зеленый мигает (2 с)
long int yellow_light = 1000; // желтый светится (1 с)
long int red_light = 7000; // красный светится (7 с)
long int yellow_for_manual = 3000; // перевод через желтый (синий)(3 с)

long int manual_duration = 60000; // 60 секунд

bool is_auto = true;
bool is_manual = false;
bool is_green = false;
bool is_red = false;

void setup() {
  Serial.begin(9600);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
}

void func_for_auto_mode() {
  long int current_millis = millis();
    
  // зеленый светится
  if (iteration == 0) {
    digitalWrite(GREEN_LED_PIN, HIGH);
        
    if (current_millis - previous_millis >= green_light) {
      previous_millis = current_millis;
      iteration++; // переходим к след. этапу
    }
  }
    
  // зеленый мигает
  else if (iteration == 1) {
    if ((current_millis / 500) % 2 == 0) {
      digitalWrite(GREEN_LED_PIN, HIGH);
    } 
            
    else {
      digitalWrite(GREEN_LED_PIN, LOW);
    }

    if (current_millis - previous_millis >= green_blink) {
      previous_millis = current_millis;
      digitalWrite(GREEN_LED_PIN, LOW);
      iteration++;
    }
  }
    
  // желтый светится
  else if (iteration == 2) {     
    digitalWrite(YELLOW_LED_PIN, HIGH);
        
    if (current_millis - previous_millis >= yellow_light) {
      previous_millis = current_millis;
      digitalWrite(YELLOW_LED_PIN, LOW);
      iteration++; // переходим к след. этапу
    }
  }
    
  // красный светится
  else if (iteration == 3) {
    digitalWrite(RED_LED_PIN, HIGH);
        
    if (current_millis - previous_millis >= red_light) {
      previous_millis = current_millis;
      digitalWrite(RED_LED_PIN, LOW);
      iteration++; // переходим к след. этапу
    }
  }
    
  // желтый снова светится
  else if (iteration == 4) {
    digitalWrite(YELLOW_LED_PIN, HIGH);
        
    if (current_millis - previous_millis >= yellow_light) {
      previous_millis = current_millis;
      digitalWrite(YELLOW_LED_PIN, LOW);
      iteration = 0; // возвращаемся к началу
    }
  }
}

void func_for_manual_mode() {
  long int start_manual = millis();
  turn_off_all_LEDs();

  if (manual_duration - (millis() - start_manual) > 0) { // если не прошло 60 секунд
    if (Serial.available() > 0) {
      char command_color = Serial.read();

      if (command_color == SET_MANUAL_RED) {
        is_green = false;
        is_red = true;

        turn_off_all_LEDs();
        digitalWrite(RED_LED_PIN, HIGH);
        Serial.print("RED");
      }

      else if (command_color == SET_MANUAL_GREEN) {
        is_red = false;
        is_green = true;

        turn_off_all_LEDs();
        digitalWrite(GREEN_LED_PIN, HIGH);
        Serial.print("GREEN");
      }

      else { // если 60 сек прошло
        turn_off_all_LEDs();
        digitalWrite(YELLOW_LED_PIN, HIGH);
        
        long int current_millis_yellow = millis();

        if (current_millis_yellow - previous_millis >= yellow_for_manual) {
          digitalWrite(YELLOW_LED_PIN, LOW);
          previous_millis = current_millis_yellow; 
        }

        is_manual = false;
        is_auto = true;
      }
    }
  }

  else {
    turn_off_all_LEDs();
    digitalWrite(YELLOW_LED_PIN, HIGH);
        
    long int current_millis_yellow = millis();
    
    if (current_millis_yellow - previous_millis >= yellow_for_manual) {
      digitalWrite(YELLOW_LED_PIN, LOW);
      previous_millis = current_millis_yellow;
    }

    is_manual = false;
    is_auto = true;
  }
}

void turn_off_all_LEDs() {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void update_state() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case SET_AUTO_MODE:
        state = STATE_AUTO;
        iteration = 0;
        is_manual = false;
        is_auto = true;
        break;

        case SET_MANUAL_MODE:
          state = STATE_MANUAL;
          is_auto = false;
          is_manual = true;
          break;
    }
  }
}

void loop() {
    update_state();

    if (is_auto) {
      func_for_auto_mode();
    }

    else if (is_manual) {
      func_for_manual_mode();
    }
}
