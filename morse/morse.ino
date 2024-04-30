int isPrintMorseCode = 0;

int frequency = 500;
int dotDelay = 80;
int dashDelay = 130;

int spacebarDelay = 1000;


unsigned long pressStartTime = 0;
unsigned long pressEndTime = 0;
unsigned long pressDuration = 0;
unsigned long noPressStartTime = 0;

char lastWord[9];
int wordIndex = 0;
int isSpacebar = 1;

int btnState = 1;

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
      tone(11, frequency);
    pressEndTime = millis();
    pressDuration = pressEndTime - pressStartTime;

    if (pressDuration < 130) {
      if (isPrintMorseCode)
        Serial.print(".");
      lastWord[wordIndex] = '.';
      delay(dotDelay);
      noTone(11);
      wordIndex++;
    } else {
      if (isPrintMorseCode)
        Serial.print("-");
      lastWord[wordIndex] = '-';
      delay(dashDelay);
      noTone(11);
      wordIndex++;
    }
    isSpacebar = 0;
  } else {
    noPressStartTime = millis();
    while (digitalRead(A0) == 1) {
      if (Serial.available() > 0) {  // 시리얼 입력이 있는 경우
        char morse = Serial.read();  // 시리얼로부터 문자 읽기
        Serial.print(morse);

        if (morse == '.') {
          tone(11, frequency);
          delay(dotDelay);
          noTone(11);
        } else if (morse == '-') {
          tone(11, frequency);
          delay(dashDelay);
          noTone(11);
        } else if (morse == ' ') {
          tone(11, frequency);
          delay(dashDelay);
          noTone(11);
        }
      }


      if (millis() - noPressStartTime > 200) {
        decodeMorse(lastWord);
        memset(lastWord, '\0', sizeof(lastWord));
        wordIndex = 0;
      } 
      if (millis() - noPressStartTime > spacebarDelay && isSpacebar == 0) {
        Serial.print(" ");
        isSpacebar = 1;
      }
    }
  }

  // 디바운스 효과를 위한 딜레이
  delay(50);
}

void decodeMorse(char* morse) {
  // 알파벳, 숫자, 특수 문자 모스 부호 매핑
  const char* morseCodes[] = {
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--..",  // Z
    "dummy",
    "-----",  // 0
    ".----",  // 1
    "..---",  // 2
    "...--",  // 3
    "....-",  // 4
    ".....",  // 5
    "-....",  // 6
    "--...",  // 7
    "---..",  // 8
    "----.",  // 9
    "dummy",
    ".-.-.-",    // .
    "--..--",    // ,
    "..--..",    // ?
    ".----.",    // '
    "-.-.--",    // !
    "-..-.",     // /
    "---...",    // :
    "-.-.-.",    // ;
    ".--.-.",    // =
    ".-.-",      // +
    "-....-",    // -
    ".-..-.",    // "
    ".--.-.",    // @
    "-.--.",     // (
    "-.--.-",    // )
    "..--.-",    // _
    "-.-.-",     // Starting Signal
    ".-.-.",     // End of message
    "-...-",     // BT 줄바꿈
    "........",  // 영문
    "...-."      // 국문
  };

  // 매핑된 모스 부호와 일치하는 문자 찾기
  const char* characters[] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "0123456789",
    ".,?'!/:;=+-a@()_bcdef "
  };

  // 일치하는 문자 출력
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 56; j++) {
      if (strcmp(morse, morseCodes[j]) == 0) {
        if (characters[i][j] == 'a')  // "
          Serial.print('"');
        else if (characters[i][j] == 'b')  // Starting Signal
          Serial.print("Starting Signal");
        else if (characters[i][j] == 'c')  // End of message
          Serial.print("End of message");
        else if (characters[i][j] == 'd')  // 줄바꿈
          Serial.println("\n");
        else if (characters[i][j] == 'e') {  // 영문
          Serial.print("English");
        } else if (characters[i][j] == 'f') {  // 국문
          Serial.print("국문");
        } else
          Serial.print(characters[i][j]);
        return;
      }
    }
  }
}
