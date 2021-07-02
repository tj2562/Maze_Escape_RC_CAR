#include <Servo.h>
#include <SoftwareSerial.h>
//#define BT_RXD 12
//#define BT_TXD 13
//SoftwareSerial bluetooth(BT_RXD, BT_TXD);

String receiveStr="";
String receiveStr2="";

int A_1A = 6; //왼쪽 모터
int A_1B = 10;

int B_1A = 9; //오른쪽 모터
int B_1B = 11;

int IR_Sensor = A0;
int trigPinL = 4;
int echoPinL = 3; //pwm
int trigPinR = 8; 
int echoPinR = 5; //pwm

int motorASpeed =130;
int motorBSpeed = 130;

float FrontDist;
String C;
void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
//  bluetooth.begin(9600);

    
  pinMode(A_1A, OUTPUT);
  pinMode(A_1B, OUTPUT);
  pinMode(B_1A, OUTPUT);
  pinMode(B_1B, OUTPUT);

  pinMode(IR_Sensor,INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
   
  digitalWrite(A_1A, LOW);
  digitalWrite(A_1B, LOW);
  digitalWrite(B_1A, LOW);
  digitalWrite(B_1B, LOW);
}

void loop() {

 receiveStr=""; 
  while(bluetooth.available()){ 
    char myChar = (char)bluetooth.read(); 
    //mySerial int 값을 char 형식으로 변환
    receiveStr+=myChar; //수신되는 문자를 receiveStr에 모두 붙임 (1바이트씩 전송되는 것을 연결) 
    delay(50); //수신 문자열 끊김 방지
    Serial.print(receiveStr);
    if(receiveStr.length()>0) C = receiveStr;
  }
      if(C == "J") forward();
      else if(C== "{") back();
      else if(C == "Z") left();
      else if(C == "k") right();
      else if(C == "[") stop();
      else if(C == "K")
      autodrive();

}

void autodrive(){
        long leftDistance = 0;
        long rightDistance = 0;
        int RightDist = getStableDistanceCM(echoPinR, trigPinR);
        int LeftDist = getStableDistanceCM(echoPinL, trigPinL);
        int FrontDist = IR_Distance();
        Serial.print("정면"); Serial.println(FrontDist); 
        Serial.print("오른쪽");Serial.println(RightDist);
        Serial.print("왼쪽");Serial.println(LeftDist);
        if(FrontDist<8){
          back();  delay(300);
          stop(); delay(200);          
          if(LeftDist>RightDist) {
            left(); delay(200);
          }
          else {
            right(); delay(200);
          }
        }
        else{
           forward();  
           if(LeftDist<8 || LeftDist >  200) {
            right(); delay (100);
          }
          if(RightDist<8 || RightDist > 200){ 
            left(); delay(100);
          }
         }
}

void forward() {
  analogWrite(A_1A, motorASpeed);
  analogWrite(A_1B, 0);
  analogWrite(B_1A, motorBSpeed - 40);
  analogWrite(B_1B, 0);
}

void left() {
  analogWrite(A_1A, 0);
  analogWrite(A_1B, 0);
  analogWrite(B_1A, motorBSpeed);
  analogWrite(B_1B, 0);

}

void right() {
  analogWrite(A_1A, motorASpeed);
  analogWrite(A_1B, 0);
  analogWrite(B_1A, 0);
  analogWrite(B_1B, 0);

}

void back() {
  analogWrite(A_1A, 0);
  analogWrite(A_1B, motorASpeed);
  analogWrite(B_1A, 0);
  analogWrite(B_1B, motorBSpeed);
}

void stop() {
  analogWrite(A_1A, 0);
  analogWrite(A_1B, 0);
  analogWrite(B_1A, 0);
  analogWrite(B_1B, 0);
}

float IR_Distance(){
 int adc = analogRead(IR_Sensor);
 float volts=adc*5./1023.; // 전압
 float distance=12.08*pow(volts,-1.08); // 거리
 return distance * 2;
}

float getDistanceCM(int echoPin, int trigPin){
  digitalWrite(echoPin, LOW);
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  float distance = pulseIn(echoPin, HIGH) / 58;
  return distance;
}

float getStableDistanceCM(int echoPin, int trigPin){
  int CmSum = 0;
  for(int i = 0; i<8; i++){
    CmSum += getDistanceCM(echoPin, trigPin);
  }
  return CmSum/8;
}
