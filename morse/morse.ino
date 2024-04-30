int btnState = 1;
int isKorean = 0;

unsigned long pressStartTime = 0;
unsigned long pressEndTime = 0;
unsigned long pressDuration = 0;

unsigned long pressStartTime2 = 0;
unsigned long pressEndTime2 = 0;
unsigned long pressDuration2 = 0;

char lastWord[8]; // 모스 부호를 저장할 배열
int wordIndex = 0; // 배열 인덱스

void setup() {
  pinMode(A0, INPUT_PULLUP); // A0 핀을 내부 풀업 저항 사용하여 입력으로 설정
  pinMode(11, OUTPUT); // 11 핀을 출력으로 설정 (tone 출력을 위해)
  Serial.begin(9600); // 시리얼 통신 시작
}

void loop() {
  btnState = digitalRead(A0); // 버튼 상태 읽기

  if (btnState == 0) { // 버튼이 눌린 상태인 경우
    pressStartTime = millis(); // 버튼이 처음으로 눌린 시간 기록
    while (digitalRead(A0) == 0){}
      // tone(11, 500, 55);
    pressEndTime = millis(); // 버튼이 떨어진 시간 기록
    pressDuration = pressEndTime - pressStartTime; // 버튼이 눌린 시간 계산

    if (pressDuration < 100) {
      // Serial.print(".");
      lastWord[wordIndex] = '.'; // 모스 부호 저장
      // tone으로 소리 재생
      tone(11, 500, 55);
      // 소리 재생 시간
      delay(100);
      // 소리 중지
      noTone(11);
      wordIndex++;
    } else {
      // Serial.print("-");
      lastWord[wordIndex] = '-'; // 모스 부호 저장
      // tone으로 소리 재생
      tone(11, 500, 200);
      // 소리 재생 시간
      delay(230);
      // 소리 중지
      noTone(11);
      wordIndex++;
    }
  } else {
    pressStartTime2 = millis();
    while (digitalRead(A0) == 1){
      if (Serial.available() > 0) { // 시리얼 입력이 있는 경우
        char morse = Serial.read(); // 시리얼로부터 문자 읽기
        Serial.print(morse);

        if (morse == '.') { // 입력이 "."인 경우
          // tone으로 소리 재생
          tone(11, 500, 55);
          // 소리 재생 시간
          delay(100);
          // 소리 중지
          noTone(11);
        } else if (morse == '-') { // 입력이 "-"인 경우
          // tone으로 소리 재생
          tone(11, 500, 200);
          // 소리 재생 시간
          delay(230);
          // 소리 중지
          noTone(11);
        } else if (morse == ' ') {
          delay(100);
          lastWord[wordIndex] = ' '; // 띄어쓰기 저장
          wordIndex++;
        }
      }
      
      
      if(millis() - pressStartTime2 > 200) {
      // 모스 부호 해독 후 출력
      decodeMorse(lastWord);
      Serial.print(lastWord);
      memset(lastWord, '\0', sizeof(lastWord));
      wordIndex = 0;
      }
    }
    pressEndTime2 = millis(); // 버튼이 떨어진 시간 기록
    pressDuration2 = pressEndTime2 - pressStartTime2; // 버튼이 눌린 시간 계산
    if (pressDuration2 > 150) {
      Serial.print(" ");
    }
  }

  // 디바운스 효과를 위한 딜레이
  delay(50);
}

void decodeMorse(char* morse) {
  // 알파벳, 숫자, 특수 문자 모스 부호 매핑
  const char* morseCodes[] = {
    ".-",      // A
    "-...",    // B
    "-.-.",    // C
    "-..",     // D
    ".",       // E
    "..-.",    // F
    "--.",     // G
    "....",    // H
    "..",      // I
    ".---",    // J
    "-.-",     // K
    ".-..",    // L
    "--",      // M
    "-.",      // N
    "---",     // O
    ".--.",    // P
    "--.-",    // Q
    ".-.",     // R
    "...",     // S
    "-",       // T
    "..-",     // U
    "...-",    // V
    ".--",     // W
    "-..-",    // X
    "-.--",    // Y
    "--..",    // Z
    "dummy",
    "-----",   // 0
    ".----",   // 1
    "..---",   // 2
    "...--",   // 3
    "....-",   // 4
    ".....",   // 5
    "-....",   // 6
    "--...",   // 7
    "---..",   // 8
    "----.",   // 9
    "dummy",
    ".-.-.-",  // .
    "--..--",  // ,
    "..--..",  // ?
    ".----.",  // '
    "-.-.--",  // !
    "-..-.",   // /
    "---...",  // :
    "-.-.-.",  // ;
    ".--.-.",  // =
    ".-.-",    // +
    "-....-",  // -
    ".-..-.",  // "
    ".--.-.",  // @
    "-.--.",   // (
    "-.--.-",  // )
    "..--.-",  // _
    "-.-.-",   // Starting Signal
    ".-.-.",   // End of message
    "-...-",   // BT 줄바꿈
    "........",// 영문
    "...-."    // 국문
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
        if (characters[i][j] == 'a')        // "
          Serial.print('"');
        else if (characters[i][j] == 'b')   // Starting Signal
          Serial.print("End of message");
        else if (characters[i][j] == 'c')   // End of message
          Serial.print("End of message");
        else if (characters[i][j] == 'd')   // 줄바꿈
          Serial.println("");
        else if (characters[i][j] == 'e') {   // 영문
          Serial.print("English");
          isKorean = 0;
        }
        else if (characters[i][j] == 'f') {   // 국문
          Serial.print("국문");
          isKorean = 1;
        }
        else
          Serial.print(characters[i][j]);
        return;
      }
    }
  }
}


