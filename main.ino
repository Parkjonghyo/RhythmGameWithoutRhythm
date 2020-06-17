#include <OLED_I2C.h> //myOLED 라이브러리 참조
OLED  myOLED(SDA, SCL, 8);

// 2개의 아날로그 핀과 (X, Y축 움직임), 2개의 디지털 핀 (O,X 버튼) 정의

const int VERT = 0; // analog
const int HORIZ = 1; // analog
const int OBUTTON = 6; //OBUTTON
const int XBUTTON = 7; //XBUTTON

//최고점수 기록용 전역변수
int BestScore = 0;

extern uint8_t SmallFont[]; //폰트를 6x8로
extern uint8_t MediumNumbers[];
extern uint8_t TinyFont[];

//사용자 지정 함수
int check(); //입력 확인 함수
int PJ(int,int,int); //타이밍에 맞췄는지 확인하는 함수(판정)
int GameStart(); //게임 시작판
int GameBoard(int); //게임 실행판
void END(int); //게임 result
void cute(); //꾸미기..?

void setup()
{
  pinMode(OBUTTON,INPUT); // obutton 읽기모드
  pinMode(XBUTTON,INPUT); // xbutton 읽기모드
  digitalWrite(OBUTTON,HIGH); //obutton값을 HIGH로
  digitalWrite(XBUTTON,HIGH); //xbutton값을 HIGH로
  myOLED.begin(); //OLED를 초기화
  myOLED.setFont(SmallFont); //OLED폰트를 6x8로
  randomSeed(analogRead(0)); //랜덤
}

void loop()
{
  END(GameBoard(GameStart())); //게임판 먼저 실행 후 리턴값으로 END를 실행 END가 리턴되면 다시 스타트판으로 간다.
}

int GameStart(){
  delay(500); //연속입력 제거
  while(1){
    myOLED.update(); 
    myOLED.clrScr();
    myOLED.print("This is Game",CENTER,0);
    myOLED.print("6keys!!!",CENTER,8);
    cute();
    myOLED.print("PRESS BUTTON",CENTER,50);
    myOLED.setFont(TinyFont);
    myOLED.print("O HARD     X EASY",CENTER,58);
    myOLED.setFont(SmallFont);
    if(digitalRead(OBUTTON) == LOW){ 
      return 1;
    }
    else if(digitalRead(XBUTTON) == LOW){
      return 0;
    }
  }
}

//끝난 함수
void END(int cnt){
  delay(500);
  if(cnt>BestScore){
    BestScore=cnt;
  }
  while(1){
    myOLED.update(); 
    myOLED.clrScr();
    myOLED.print("Oh.. It's END..",CENTER,0);
    myOLED.print("Your Score",CENTER,8);
    myOLED.setFont(MediumNumbers);
    myOLED.printNumI(cnt, CENTER,16); //printNumI로 변수 값 출력
    myOLED.setFont(SmallFont);
    myOLED.print("Best Score",CENTER,32);
    myOLED.setFont(MediumNumbers);
    myOLED.printNumI(BestScore, CENTER,40);
    myOLED.setFont(SmallFont);
    myOLED.print("Press Any Button",CENTER,56);
    if(digitalRead(OBUTTON) == LOW || digitalRead(XBUTTON) == LOW){
      return;
    }//리턴으로 빠져나감
  }
}

//판정 체크 함수
int PJ(int a,int CK,int i){
  if(i<=12 && CK == a){
       return 129; 
      }//앞의 1/13정도에서만 입력되도록 만들고 맞았다면 돌아가도록
      else if(i<0){
       return -999;
      }//틀렸다면 리턴값 -999
    return i; //12이하가 아닐때 바로 i를 돌려준다.
}

//큐트해짐
void cute(){
    myOLED.print("*v*",LEFT,16);
    myOLED.print(">v<",RIGHT,16);
    myOLED.print("OWO",LEFT,30);
    myOLED.print("START!!",CENTER,30);
    myOLED.print("^v^",RIGHT,30);
    myOLED.print("$_$",LEFT,44);
    myOLED.print(";o;",RIGHT,44);
}//print만 엄청나게

//입력 체크 함수
int check(){
    static int vertical, horizontal,OB, XB, CK = 9; //여러번 선언할 필요 없음으로 static으로 선언
    horizontal = analogRead(HORIZ); //가로 입력
    vertical = analogRead(VERT); //세로 입력
    OB = digitalRead(OBUTTON); //obutton입력
    XB = digitalRead(XBUTTON); //xbutton입력
    if(horizontal == 1023) { //왼쪽
      CK = 0;
    }
    else if(vertical == 0) {//위쪽
      CK = 1;
    }
    else if(horizontal == 0) {//오른쪽
      CK= 2;
    }
    else if(vertical == 1023) {//아래쪽
      CK = 3;
    }
    else if(OB == LOW) {//Obutton 클릭
      CK = 4;
    }
    else if(XB == LOW) {//Xbutton 클릭
      CK = 5;
    }
    else {//입력이 없다면 9
      CK = 9;
    }
    return CK; //그리고 CK를 반환한다.
};

int GameBoard(int difi){
  int y,CK,cnt=-1; //카운트와 CK, y는 게임 한판에 한번씩 해야하므로 static 없이, 하지만 while문에서 벗어나게
  int spd = 100; //이하동문
  while(1){
  y = random(0, 6)*11;//랜덤함수 사용
  myOLED.update(); 
  myOLED.clrScr();
  if(spd<250){
    if(difi ==0){
     spd++; //최대속도를 250으로 점점 빨라지게 
    }
    else{
      spd=spd+2;
    }
  }
  cnt++;
  for (int i=128; i>=-6; i=i-(spd*0.025))//내려가는 속도 조정하는 곳
  {
    CK = check();//체크함수에서 받아온 CK 를 CK에 넣는다.
    switch(y){
      case 0: //0입력 == 왼쪽
        myOLED.print("<", i, y);
        myOLED.update(); 
        myOLED.clrScr();
        i=PJ(0,CK,i);
      break;
      case 11: //11입력 == 위쪽
        myOLED.print("^", i, y);
        myOLED.update(); 
        myOLED.clrScr();
        i=PJ(1,CK,i);
      break;
      case 22: //22입력 == 오른쪽
        myOLED.print(">", i, y);
        myOLED.update(); 
        myOLED.clrScr();
        i=PJ(2,CK,i);
      break;
      case 33: //33입력 == 아래쪽
        myOLED.print("v", i, y);
        myOLED.update(); 
        myOLED.clrScr();
        i=PJ(3,CK,i);
      break;
      case 44: //44입력 == Obutton
        myOLED.print("0", i, y);
        myOLED.update(); 
        myOLED.clrScr();
        i=PJ(4,CK,i);
      break;
      case 55: //55입력 == Xbutton
        myOLED.print("X", i, y);
        myOLED.update(); 
        myOLED.clrScr();
        i=PJ(5,CK,i);
      break;
    }
    if(i==-999){
        return cnt;//-999일때 리턴을 시켜서 END함수의 재료로
      }else if(i==129){
        break; //129일시 브레이크해서 while문으로 돌아가게
      }
    }
  }
}
