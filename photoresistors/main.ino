#define PHOTO_SENSOR_PIN A4
#define PHOTO_SENSOR_PIN2 A0

void setup() {
  Serial.begin(9600);
}

void loop() {
    int photo_val = analogRead(PHOTO_SENSOR_PIN);
    int photo_val2 = analogRead(PHOTO_SENSOR_PIN2);
    Serial.print("Photo val1 = ");
    Serial.println(photo_val);
    delay(500);  
    Serial.print("Photo val2 = ");
    Serial.println(photo_val2);
    delay(500);
}
