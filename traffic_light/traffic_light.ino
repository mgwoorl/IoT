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

void update_state() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        switch (command) {
            case SET_AUTO_MODE:
                state = STATE_AUTO;
                iteration = 0;
                break;

            case SET_MANUAL_MODE:
                state = STATE_MANUAL;
                break;
        }
    }
}

void loop() {
    func_for_auto_mode();
}
