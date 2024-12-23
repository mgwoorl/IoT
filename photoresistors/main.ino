#define PHOTO_RES1 A0
#define PHOTO_RES2 A1
#define LED_PIN1 2
#define LED_PIN2 3

unsigned long previousMillis = 0;
const long blink_interval = 200;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
}

void loop() {
  int photo_val1 = analogRead(PHOTO_RES1);
  int photo_val2 = analogRead(PHOTO_RES2);

  //Serial.print("Photo val1 = ");
  //Serial.println(photo_val1);
  //Serial.print("Photo val2 = ");
  //Serial.println(photo_val2);
  
  unsigned long currentMillis = millis();

  if (photo_val1 > photo_val2 + 50) {
    if (currentMillis - previousMillis >= blink_interval) {
      previousMillis = currentMillis;
      digitalWrite(LED_PIN1, !digitalRead(LED_PIN1));
      digitalWrite(LED_PIN2, LOW);
    }
  } 
  
  else if (photo_val2 > photo_val1 + 50) {
    if (currentMillis - previousMillis >= blink_interval) {
      previousMillis = currentMillis;
      digitalWrite(LED_PIN2, !digitalRead(LED_PIN2));
      digitalWrite(LED_PIN1, LOW);
    }
  } 
  
  else {
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, HIGH);
  }
}
