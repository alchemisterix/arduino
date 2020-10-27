//#include <LiquidCrystal.h>
#include <Thread.h>                                                               //threads support for timers routines blink/sleep
#include <ThreadController.h>                                                     //threads controller
#include <TimerOne.h>   
#include <TFT_HX8357.h>                                                           // Hardware-specific library


 
#define POT1_PIN    A0
#define POT2_PIN    A1
#define RL1_PIN     A8
#define RL2_PIN     A9
#define RL3_PIN     A10
#define BUTTON_PIN  0


ThreadController controll = ThreadController();                                   
Thread timer1Thread = Thread();                                                  
Thread timer2Thread = Thread();                                                  
TFT_HX8357 tft = TFT_HX8357();

int sec;
unsigned long prevMillis1 = 0;
int t1;
int t2;
int r1 = 0;
int r2 = 0;

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(RL1_PIN, OUTPUT);
  pinMode(RL2_PIN, OUTPUT);
  pinMode(RL3_PIN, OUTPUT);

  tft.begin();
    

  timer1Thread.onRun(timer1ThreadCallback);                                    
  timer1Thread.setInterval(1000);                                              
  controll.add(&timer1Thread);                                                  

  timer2Thread.onRun(timer2ThreadCallback);                                    
  timer2Thread.setInterval(1000);                                               
  controll.add(&timer2Thread);                                                  
  

  Timer1.initialize(20000);                                                      
  Timer1.attachInterrupt(timerCallback);                                        
  Timer1.start();                                                                
  timer1Thread.enabled = false;                                                  
  timer2Thread.enabled = false;                                                  

  t1 = analogRead(POT1_PIN)/17;
  t2 = analogRead(POT2_PIN)/34;

  testFillScreen();
}

void loop() {
    tft.setTextColor(TFT_YELLOW); 
    tft.drawString ("Timer1= ",55,25,1); 
    tft.drawString ("Timer2= ",55,45,1);
    tft.drawString ("sec ",175,25,1);
    tft.drawString ("sec ",175,45,1);
    tft.drawNumber(t1, 145, 25, 1);
    tft.setTextColor(TFT_BLACK);
    tft.drawNumber(t1, 145, 25, 1);
    tft.setTextColor(TFT_YELLOW); 
    tft.drawNumber(t2, 145, 45, 1);
    tft.setTextColor(TFT_BLACK);
    tft.drawNumber(t2, 145, 45, 1);
    
    if (t1==0 && timer1Thread.enabled==true){
      r1=1;
      timer1Thread.enabled = false;                                               
      digitalWrite (RL2_PIN,LOW);
      delay(2000);
      digitalWrite (RL3_PIN,HIGH);
      timer2Thread.enabled = true;                                                
    }
      
  

    if (t2==0 && timer2Thread.enabled  == true){
      r2=1;
      digitalWrite (RL3_PIN,LOW);
      digitalWrite (RL1_PIN,LOW);
      timer2Thread.enabled = false;
      tft.drawString ("Ready. Press start button",0,0,1);
    }
    
    if (digitalRead(BUTTON_PIN)==LOW) {
      delay(200);
      settingTimer();
    }
  


  
}

void settingTimer()
{
    timer1Thread.enabled = false;                                                  
    timer2Thread.enabled = false;  
    tft.fillScreen(TFT_BLACK);
    while (digitalRead(BUTTON_PIN)) {
      t1 = analogRead(POT1_PIN)/17;
      tft.setTextColor(TFT_YELLOW); 
      tft.drawString ("Timer1= ",55,25,1);
      tft.drawNumber(t1, 145, 25, 1);
      tft.drawString ("sec ",175,25,1);
      delay(30);
      tft.setTextColor(TFT_BLACK);
      tft.drawNumber(t1, 145, 25, 1);
  }

  tft.fillScreen(TFT_BLACK);
  delay (200);
  
    while (digitalRead(BUTTON_PIN)) {
      t2 = analogRead(POT2_PIN)/34;
      tft.setTextColor(TFT_YELLOW); 
      tft.setCursor(55, 25);
      tft.drawString ("Timer2= ",55,25,1);
      tft.drawNumber(t2, 145, 25, 1);
       tft.drawString ("sec ",175,25,1);
      delay(30);
      tft.setTextColor(TFT_BLACK);
      tft.drawNumber(t2, 145, 25, 1);
  }

  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW); 
  tft.drawString ("WORKING...",0,0,1);
  digitalWrite (RL1_PIN,HIGH);
  
  sec = 0;
  delay(2000);
  timer1Thread.enabled = true;  
  digitalWrite (RL2_PIN,HIGH);
}

void timer1ThreadCallback(){

  if (t1 != 0){t1--;}
  sec++;}

void timer2ThreadCallback(){
  if (t2 != 0){t2--;}
  sec++;}
  

void timerCallback(){
  controll.run();
}

void testFillScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.fillScreen(TFT_RED);
  tft.fillScreen(TFT_GREEN);
  tft.fillScreen(TFT_BLUE);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW); 
  tft.setTextSize(2);
  tft.drawString ("Ready. Press start button",0,0,1);
}

