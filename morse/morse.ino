int btnState = 1;

unsigned long pressStartTime = 0;
unsigned long pressEndTime = 0;
unsigned long pressDuration = 0;

unsigned long pressStartTime2 = 0;
unsigned long pressEndTime2 = 0;
unsigned long pressDuration2 = 0;
char lastWord = ' ';

void setup() {
  pinMode(A0, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  btnState = digitalRead(A0);

  if (btnState == 0) {
    pressStartTime = millis();
    while (digitalRead(A0) == 0)
      tone(11, 500, 55);
    pressEndTime = millis();
    pressDuration = pressEndTime - pressStartTime;

    if (pressDuration < 100) {
      Serial.print(".");
      lastWord = '.';
    } else {
      Serial.print("-");
      lastWord = '-';
    }
  } else {
    pressStartTime2 = millis();
    while (digitalRead(A0) == 1){
      if (Serial.available() > 0) {
        char morse = Serial.read();
        Serial.print(morse);

        if (morse == '.') {
          tone(11, 500, 55);
          delay(100);
          noTone(11);
        } else if (morse == '-') {
          tone(11, 500, 200);
          delay(230);
          noTone(11);
        } else if (morse == ' ') {
          delay(100);
        }
      }
    }
    pressEndTime2 = millis();
    pressDuration2 = pressEndTime2 - pressStartTime2;
    if (pressDuration2 > 200) {
      lastWord = ' ';
      Serial.print(" ");
    }
  }
  
  delay(50);
}
