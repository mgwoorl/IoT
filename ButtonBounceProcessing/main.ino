#define BUTTON_PIN 2 
#define BOUNCE_TIME 50
#define PRESSED HIGH

volatile long int press_time = 0;
volatile bool pressed_candidate = false;
volatile long int hold_time = 0;
volatile int press_count = 0;
volatile bool report_to_user = false;
bool button_pressed = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), process_button_click, RISING);
}

void loop() {
  if (report_to_user) {
    Serial.println("Press candidate");
    report_to_user = false;
  }

  if (pressed_candidate) {
    if (millis() - press_time > BOUNCE_TIME) {
      if (digitalRead(BUTTON_PIN) == PRESSED && !button_pressed) {
        button_pressed = true;
        press_count++;
        
        Serial.print("Button pressed, count: ");
        Serial.println(press_count);
      }
    }
  }

  if (button_pressed && digitalRead(BUTTON_PIN) == LOW) {
    hold_time = millis() - press_time;

    Serial.print("Button released, hold time: ");
    Serial.print(hold_time);
    Serial.println(" ms");

    button_pressed = false;
    pressed_candidate = false;
  }
}

void process_button_click() {
  if (!pressed_candidate) {
    press_time = millis();
    pressed_candidate = true;
    report_to_user = true;
    hold_time = 0;
  }
}
