//0.1  10 led simple demo
//0.2  simple random color by button and random button shutdown
//0.3 
//0.4  add the function diviatioh
//0.5  timer_1 integration and simple 3 led random blinking
//0.51 tickle every 2 sec on 3 differnt pins, add array filler with distinct check
//0.52 leds on and off
//0.53 only 3 from 7 is on and unique checking
//0.54 level selection added
//     ledcount selection added
//0.55 just keyboard integration
//0.56 wrong/correct added
//     score counting added
//0.57 start timings added (gameplay see rules)
//0.58 bug with led strip fixed. FIRST STABLE
//0.59 gameplay timings fixed
//0.60 add UART gameplay initialization (1 byte = level, 2 byte = players count, 3 byte = color code)
//0.61 move keyboard pins to analog port, extend ledpins port, add simple demo, debug updated 
//0.62 p1 inеgrated, button_count removed
//0.63 multiplayer integration easy level 
//0.64 bug with score counting fixed
//0.65 added game stop procedure
//0.67 two player integration and debug 2 color for led added
//0.68 user output is only when new led added
//0.69 add level 2 (hard), case bug for color fixed, demo modified
//0.70 release code fixed, button back to the old order, add diferent color for players support, bug with long ledout delay fixed
//0.71 score counting to 10 points set, web server added, no blackbutton counting now
//0.72 add sound module support
//0.73 add multiple key support
//0.74 gameplay timer added
//0.75 debug routines changed to new proteus variant 
//0.76 add led masive player 3 player 4 no button 
//0.77 add button support for player3 and player 4 and serial print keycode, just debug
//0.78 gameplay for 4 gamers on level 0 and level 1 realised
//0.79 unique check for 4 gamers check only for player 1
//0.80 player 2 dash check added
//0.81
//0.82 player 3,4 dash check added first stable
//0.83 removed delay in white and black button debug routines (1_01)

// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS

//#include "FastLED.h" //NOT USE IN DEBUG
#include <TimerThree.h>
#include <Keypad.h>
#include <SoftReset.h>
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
//#include "D:\+Atomic Rush\atomic_0_57\debug.ino"
#define NUM_LEDS 168//How many leds are in the strip? 52ma * 42 = 2184ma
#define DATA_PIN 3// Data pin that led data will be written out over

// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8


///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
// 


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 177 }; // arduino server ip in lan
EthernetServer server(80); //arduino server port

int timer1_counter;
const byte ROWS = 7; //количество рядов для клавиатуры
const byte COLS = 8; //количество столбцов клавиатуры

//массив для клавиатуры в ascii кодах
char hexaKeys[ROWS][COLS] = {
  {'0','7','>','E','L','S','Z','a'},
  {'1','8','?','F','M','T','[','b'},
  {'2','9','@','G','N','U',0x5C,'c'},
  {'3',':','A','H','O','V',']','d'},
  {'4',';','B','I','P','W',0x5E,'e'},
  {'5','<','C','J','Q','X','_','f'},
  {'6','=','D','K','R','Y',0x60,'g'}
  };


byte rowPins[ROWS] = {36,37,38,39,40,41,42}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {43,44,45,46,47,48,49,35};
//byte rowPins[ROWS] = {A8,A9,A10,A11,A12,A13,A14}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {A0,A1,A2,A3}; //connect to the column pinouts of the keypad




int prev_x=0;
int prev_x_1_p1=0;
int prev_x_2_p1=0;
int prev_x_3_p1=0;
int prev_x_4_p1=0;
int prev_x_1_p2=0;
int prev_x_2_p2=0;
int prev_x_3_p2=0;
int prev_x_4_p2=0;

int recieved=0; //Flag of recieved from lan

int level = 0;     //level select 0=easy 1=hard
int players = 1;
int a = 50;
int by_one_button=6;    //Количество елементов в одном сегменте
int startLed;           //Первый елемент в сегменте
int endLed;             //Последний елемент в сегменте
int Pin;
int num_button;
int but_num;
int key; //служебная при чтении клавиатуры
int timer = 0;

int col_R = 255;
int col_G = 0;
int col_B = 0;

int col_R_p1 = 255;
int col_G_p1 = 0;
int col_B_p1 = 0;


int col_R_p2 = 0;
int col_G_p2 = 255;
int col_B_p2 = 0;

int newVal_p1 = 1;
int newVal_p2 = 1;
int newVal_p3 = 1;
int newVal_p4 = 1;
//int button_count = 14;          //количество кнопок. Не исепользуется?

int score_p1 = 0;                  //счетчик заработаных очков, игрок 1
int prev_score_p1 = 0;             //предыдущие очки (используется для проверки успешного нажатия), игрок 1

int score_p2 = 0;                  //счетчик заработаных очков, игрок 2
int prev_score_p2 = 0;             //предыдущие очки (используется для проверки успешного нажатия), игрок 2

int score_p3 = 0;                  //счетчик заработаных очков, игрок 3
int prev_score_p3 = 0;             //предыдущие очки (используется для проверки успешного нажатия), игрок 3

int score_p4 = 0;                  //счетчик заработаных очков, игрок 3
int prev_score_p4 = 0;             //предыдущие очки (используется для проверки успешного нажатия), игрок 3

volatile const int ledpin[] =     {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};     //пины светодиодов
volatile int int_cnt_but_p1[5] = {0,0,0,0,0};    //Масив для счета "тиков" по прерываниях игрок 1
volatile int int_cnt_but_p2[5] = {0,0,0,0,0};    //Масив для счета "тиков" по прерываниях игрок 2
volatile int int_cnt_but_p3[5] = {0,0,0,0,0};    //Масив для счета "тиков" по прерываниях игрок 3
volatile int int_cnt_but_p4[5] = {0,0,0,0,0};    //Масив для счета "тиков" по прерываниях игрок 4

volatile int led_array_p1 [5];      //Масив для вывода в порт игрок 1
volatile int led_array_p2 [5];      //Масив для вывода в порт игрок 2
volatile int led_array_p3 [5];      //Масив для вывода в порт игрок 3
volatile int led_array_p4 [5];      //Масив для вывода в порт игрок 3

//volatile int led_count = 28;     //количество светодиодов оно же граничное значение для рандома (14 для уровня 1, 28 для уровня 2)
volatile int led_on_count_p1 = 5;     //счетчик включенных светодиодов игрок 1
volatile int led_on_count_p2 = 5;     //счетчик включенных светодиодов игрок 2
volatile int led_on_count_p3 = 5;     //счетчик включенных светодиодов игрок 3
volatile int led_on_count_p4 = 5;     //счетчик включенных светодиодов игрок 4


volatile int max_led_on_count_p1 = 3; //максимальное количество включенных светодиодов для игрока 1
volatile int max_led_on_count_p2 = 3; //максимальное количество включенных светодиодов для игрока 2
volatile int max_led_on_count_p3 = 3; //максимальное количество включенных светодиодов для игрока 3
volatile int max_led_on_count_p4 = 3; //максимальное количество включенных светодиодов для игрока 4

volatile int stop_time = 8;        //максимальное время свечения светодиода t= 8 * 500 ms (500ms время сработки таймера)
volatile int customKey;
//volatile int int_cnt;
//volatile int int_cnt_but_1 =0;
//volatile int int_cnt_but_2 =0;
//volatile int int_cnt_but_3 =0;

//volatile int but_1 = 0;
//volatile int but_2 = 0;
//volatile int but_3 = 0;

String readString;  //Used in ethernet 
String msg;
String color = "";
String input_buffer = "";
char charbuffer [3];

// Массив светодиодов, один элемент - один светодиод
//CRGB leds[NUM_LEDS];//NOT USE IN DEBUG


SoftwareSerial mySerial(8, 9); // RX, TX
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// This function sets up the ledsand tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
 // delay(2000);
 // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);


 
  Wire.begin(); //i2c start 
  Serial.begin(9600);    
  delay (1);


  all_led_off();

  mp3start();         //Настройка мп3 модуля и проигрывание приветствия

  SerialInit();   //Вывод приветствия и ожидания ввода
  ethernet_init (); //webserver start
 
  



//while (ethernet_available()==0){if (Serial.available()==1 || recieved ==1){break;}else{demo();}}   //С демкой
//while (ethernet_available()==0){if (Serial.available()==1 || recieved ==1){break;}} //без демо 0,74

 
  
 //while (Serial.available()==0 || ethernet_available()==0) {demo();}             //Пока нету ввода UART или Ethernet - крутим демку

 //while (Serial.available()==0) {demo();}
  
 //Serial.print (level);



 // initialize timer1 
  if (level == 0){
    max_led_on_count_p1 = 3;
    max_led_on_count_p2 = 3; 
    max_led_on_count_p3 = 3; 
    max_led_on_count_p4 = 3; 
    stop_time = 7;
    int_cnt_but_p1[0] = 5;//первый светодиод согласно правилам должен загорется на 2.0сек
    int_cnt_but_p1[1] = 3;//второй светодиод согласно правилам должен загорется на 2.5сек
    int_cnt_but_p2[0] = 5;//первый светодиод согласно правилам должен загорется на 2.0сек
    int_cnt_but_p2[1] = 3;//второй светодиод согласно правилам должен загорется на 2.5сек
    int_cnt_but_p3[0] = 5;//первый светодиод согласно правилам должен загорется на 2.0сек
    int_cnt_but_p3[1] = 3;//второй светодиод согласно правилам должен загорется на 2.5сек
    int_cnt_but_p4[0] = 5;//первый светодиод согласно правилам должен загорется на 2.0сек
    int_cnt_but_p4[1] = 3;//второй светодиод согласно правилам должен загорется на 2.5сек
    }                             
  if (level >= 1){
    max_led_on_count_p1 = 5;
    max_led_on_count_p2 = 5;
    max_led_on_count_p3 = 5;
    max_led_on_count_p4 = 5;
    stop_time = 5;
    int_cnt_but_p1[0] = 3;//5;//первый светодиод согласно правилам должен загорется на 1.0сек
    int_cnt_but_p1[1] = 2;//3;//второй светодиод согласно правилам должен загорется на 1.5сек
    int_cnt_but_p2[0] = 3;//5;//первый светодиод согласно правилам должен загорется на 1.0сек
    int_cnt_but_p2[1] = 2;//3;//второй светодиод согласно правилам должен загорется на 1.5сек
    int_cnt_but_p3[0] = 3;//5;//первый светодиод согласно правилам должен загорется на 1.0сек
    int_cnt_but_p3[1] = 2;//3;//второй светодиод согласно правилам должен загорется на 1.5сек
    int_cnt_but_p4[0] = 3;//5;//первый светодиод согласно правилам должен загорется на 1.0сек
    int_cnt_but_p4[1] = 2;//3;//второй светодиод согласно правилам должен загорется на 1.5сек
  }
  
  led_on_count_p1 = max_led_on_count_p1;                                                // при запуске во избежание глюка кол-во включенных равно максимальному к-ву
  led_on_count_p2 = max_led_on_count_p2;                                                // при запуске во избежание глюка кол-во включенных равно максимальному к-ву
  led_on_count_p3 = max_led_on_count_p3;                                                // при запуске во избежание глюка кол-во включенных равно максимальному к-ву
  led_on_count_p4 = max_led_on_count_p4;                                                // при запуске во избежание глюка кол-во включенных равно максимальному к-ву
  





 /*  //Инициализация портов ввода для кнопок
  for (int x = 0; x < button_count; x++){                                            
    pinMode(butpin[x], INPUT);
    delay(10);
    digitalWrite(butpin[x], HIGH);             //подключим подтягивающие резисторы, кнопки инверсные
   }*/




// Наполнение масива для игрока 1 и проверка уникальности
  while (newVal_p1 != 0) {
     newVal_p1 = 0;
     for (int x = 0; x < max_led_on_count_p1; x++){  //Заполняем масив
        led_array_p1[x] = random(0, 14);             //случайными значениями
        delay(1);
     }

      for (int x = 0; x < max_led_on_count_p1; x++){   //Проверка уникальности
        int origVal = led_array_p1[x];
        for (int k = x+1; k < max_led_on_count_p1; k++){
          if (origVal == led_array_p1[k]){newVal_p1 = 1; Serial.print ("BADP1"); }
        }
      }
  }
  
  //Заполенение масива для игрока 2
      while (newVal_p2 != 0) {
      newVal_p2 = 0;
      for (int x = 0; x < max_led_on_count_p2; x++){// Наполнение массива
        led_array_p2[x] = random(14, 28); // рандом для второй половины игрового поля
        delay(1);
       }
  
      for (int x = 0; x < max_led_on_count_p2; x++){
        int origVal = led_array_p2[x];
        for (int k = x+1; k < max_led_on_count_p2; k++){
          if (origVal == led_array_p2[k]){newVal_p2 = 1; Serial.print ("BADP2"); }
        }
       }
      }
    
 // Наполнение масива для игрока 3 и проверка уникальности
  while (newVal_p3 != 0) {
     newVal_p3 = 0;
     for (int x = 0; x < max_led_on_count_p3; x++){  //Заполняем масив
        led_array_p3[x] = random(28, 42);             //случайными значениями
        delay(1);
     }

      for (int x = 0; x < max_led_on_count_p3; x++){   //Проверка уникальности
        int origVal = led_array_p3[x];
        for (int k = x+1; k < max_led_on_count_p3; k++){
          if (origVal == led_array_p3[k]){newVal_p3 = 1; Serial.print ("BADP3"); }
        }
      }
  }

   // Наполнение масива для игрока 4 и проверка уникальности
  while (newVal_p4 != 0) {
     newVal_p4 = 0;
     for (int x = 0; x < max_led_on_count_p4; x++){  //Заполняем масив
        led_array_p4[x] = random(42, 56);             //случайными значениями
        delay(1);
     }

      for (int x = 0; x < max_led_on_count_p4; x++){   //Проверка уникальности
        int origVal = led_array_p4[x];
        for (int k = x+1; k < max_led_on_count_p4; k++){
          if (origVal == led_array_p4[k]){newVal_p4 = 1; Serial.print ("BADP4"); }
        }
      }
  }




//all_led_off(); //Выключили все светодиоды включеные во время демки

 for (int x = 0; x < max_led_on_count_p1; x++){white_button (led_array_p1[x],1);} //Вывод в порт игрока 1
 
 if (players >= 2){for (int x = 0; x < max_led_on_count_p2; x++){white_button (led_array_p2[x],2);}} //Если игроков 2 или больше выводим масив игрока 2
 if (players >= 3){for (int x = 0; x < max_led_on_count_p3; x++){white_button (led_array_p3[x],3);}} //Если игроков 3 или больше выводим масив игрока 3
 if (players >= 4){for (int x = 0; x < max_led_on_count_p4; x++){white_button (led_array_p4[x],4);}} //Если игроков 4 или больше выводим масив игрока 4



  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts



Timer3.initialize(500000);
  Timer3.attachInterrupt(int_led);                                                 // прерывание каждые 0.5 сек
  //Timer3.stop();
}


ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
    

  //digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}

//ОСНОВНОЙ ЦИКЛ ПРОГРАММЫ

void loop() {
  if (led_on_count_p1 < max_led_on_count_p1) {add_one_led_p1();} //Если включенных светодиодов меньше чем максимальное к-во, то добавим один, игрок 1
  if (players >= 2) {if (led_on_count_p2 < max_led_on_count_p2) {add_one_led_p2();}} //Если включенных светодиодов меньше чем максимальное к-во, то добавим один, игрок 2
  if (players >= 3) {if (led_on_count_p3 < max_led_on_count_p3) {add_one_led_p3();}} //Если включенных светодиодов меньше чем максимальное к-во, то добавим один, игрок 3
  if (players >= 4) {if (led_on_count_p4 < max_led_on_count_p4) {add_one_led_p4();}} //Если включенных светодиодов меньше чем максимальное к-во, то добавим один, игрок 4
  
  //for (int x = 0; x < max_led_on_count_p1; x++) {white_button (led_array_p1[x],1);}//Перебор массива и включение светодиодов, игрок 1, забрал в уникчек
  //if (players >> 1) {for (int x = 0; x < max_led_on_count_p2; x++) {white_button (led_array_p2[x],2);}}//Перебор массива и включение светодиодов, игрок 2
    
  //if (Serial.available() > 0){game_over();}
  
//if (timer > 60){Serial.println (timer); mp3_play (99); game_over();}//обработчик конца игры

if (customKeypad.getKeys())
    {
      
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( customKeypad.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (customKeypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    msg = "PRESSED";
                break;
                    case HOLD:
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    msg = " RELEASED.";
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
                //Serial.print("Key ");
                //Serial.print(customKeypad.key[i].kchar);
                
            
        
    


//customKey = customKeypad.getKey();               //Обработка клавиаутры               //Обработка клавиаутры
//if (customKey){
     if (msg == "PRESSED"){
  
    //noInterrupts();
    //Serial.print ("Pressed: ");
    int z = 0; //проверка на ноль для нажатий на пустые кнопки
    //key = customKey- 48; // decrement for button ascii code = led num
    key = customKeypad.key[i].kchar;
    key -= 48;
   // Serial.println(key);
    //Serial.println("ALCHE");
    for (int x = 0; x < max_led_on_count_p1; x++){if (key == led_array_p1[x] || key == led_array_p2[x] || key == led_array_p3[x]|| key == led_array_p4[x]){z++;}} //Проверка на совпадения с массивом, чтобы не считать выключенные
    
    
    if (z !=0){
        
   // mp3_play (6); // В дебаге не используется
    
    if (level <=1) {              //Если уровень 0 (простой) или 1 (средний) игра на своих полях 
      if (key < 14) {           // Если нажата кнопка первого игрового поля то будем считать игроку 1
        for (int x = 0; x < max_led_on_count_p1; x++){
          if (key == led_array_p1[x]){
            Serial.print("SP1_");
            int_cnt_but_p1[x] = 1;
            led_on_count_p1 --;  
            add_one_led_p1();
            score_p1 +=10;
         }        
       }

      if (prev_score_p1 != score_p1){
        prev_score_p1 = score_p1;
      }
      else {
        score_p1 -= 10;
        prev_score_p1 = score_p1;
        Serial.print("FP1_");
      }
      
      Serial.println(score_p1);
    }

    if (key > 13 & key <28 & players >= 2) {       //Если нажата кнопка из поля 2 и игроков больше или равно 2 проверяем для игрока 2
      for (int x = 0; x < max_led_on_count_p2; x++){
          if (key == led_array_p2[x]){
            Serial.print("SP2_"); 
            int_cnt_but_p2[x] = 1;
            led_on_count_p2 --;  
            add_one_led_p2();
            score_p2 +=10;
          }
        }

       if (prev_score_p2 != score_p2){
        prev_score_p2 = score_p2;
        }
      else {
        score_p2 -= 10;
        prev_score_p2 = score_p2;
        Serial.print("FP2_");
      }
      Serial.println(score_p2);
     }

    if (key > 27 & key <42 & players >= 3) {       //Если нажата кнопка из поля 2 и игроков больше или равно 2 проверяем для игрока 2
      for (int x = 0; x < max_led_on_count_p3; x++){
          if (key == led_array_p3[x]){
            Serial.print("SP3_"); 
            int_cnt_but_p3[x] = 1;
            led_on_count_p3 --;  
            add_one_led_p3();
            score_p3 +=10;
          }
        }

       if (prev_score_p3 != score_p3){
        prev_score_p3 = score_p3;
        }
      else {
        score_p3 -= 10;
        prev_score_p3 = score_p3;
        Serial.print("FP3_");
      }
      Serial.println(score_p3);
     }


    if (key > 41 & key <56 & players >= 4) {       //Если нажата кнопка из поля 2 и игроков больше или равно 2 проверяем для игрока 2
      for (int x = 0; x < max_led_on_count_p4; x++){
          if (key == led_array_p4[x]){
            Serial.print("SP4_"); 
            int_cnt_but_p4[x] = 1;
            led_on_count_p4 --;  
            add_one_led_p4();
            score_p4 +=10;
          }
        }

       if (prev_score_p4 != score_p4){
        prev_score_p4 = score_p4;
        }
      else {
        score_p4 -= 10;
        prev_score_p4 = score_p4;
        Serial.print("FP4_");
      }
      Serial.println(score_p4);
     }
    
 }  //End of level <=1


//level hard, *************************************************************************************************************************************************************LEVEL HARD
 if (level >1 && players >1) {              //Если уровень сложный и игроков больше одного
      
      // BUTTON FROM BOARD 1
      if (key < 14) {           // Если нажата кнопка первого игрового поля то будем считать игроку 1
        for (int x = 0; x < max_led_on_count_p1; x++){
          if (key == led_array_p1[x]){
            Serial.print("SP1_");  
            int_cnt_but_p1[x] = 1;
            led_on_count_p1 --;  
            add_one_led_p1();
            score_p1 +=10;
         }        
       }

      if (prev_score_p1 != score_p1){
        prev_score_p1 = score_p1;
      }
      else {                              //Если кнопка чужая
        score_p1 -= 10;                   //Отнимим 10 очков
        prev_score_p1 = score_p1;

        if (players >= 2){
          for (int x = 0; x < max_led_on_count_p2; x++){//Если игроков 2 или больше, проверим на совпадение с масивом игрока 2
            if (key == led_array_p2[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p2 += 10;
              prev_score_p2 = score_p2;
              int_cnt_but_p2[x] = 1;
              led_on_count_p2 --;                      //Уменьшим количество включенных
              add_one_led_p2();                        //Добавим светодиод
              Serial.print ("FSP2_");
              Serial.println(score_p2);
            }
          } 
        }   

        if (players >= 3){
          for (int x = 0; x < max_led_on_count_p3; x++){//Если игроков 3 или больше, проверим на совпадение с масивом игрока 3
            if (key == led_array_p3[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p3 += 10;
              prev_score_p3 = score_p3;
              int_cnt_but_p3[x] = 1;
              led_on_count_p3 --;                      //Уменьшим количество включенных
              add_one_led_p3();                        //Добавим светодиод
              Serial.print ("FSP3_");
              Serial.println(score_p3);
            }
          } 
        }  

        if (players >= 4){
          for (int x = 0; x < max_led_on_count_p4; x++){//Если игроков 3 или больше, проверим на совпадение с масивом игрока 3
            if (key == led_array_p4[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p4 += 10;
              prev_score_p4 = score_p4;
              int_cnt_but_p4[x] = 1;
              led_on_count_p4 --;                      //Уменьшим количество включенных
              add_one_led_p4();                        //Добавим светодиод
              Serial.print ("FSP4_");
              Serial.println(score_p4);
            }
          } 
        }  
        
        Serial.print("FP1_");
      } 
     Serial.println(score_p1);
    }//End of key < 14 

      
      // ***************************************************************************************************************************************BUTTON FROM BOARD 2
      if (key > 13 & key <28) {           // Если нажата кнопка второго игрового поля то будем считать игроку 2
          for (int x = 0; x < max_led_on_count_p2; x++){
          if (key == led_array_p2[x]){
            Serial.print("SP2_");  
            int_cnt_but_p2[x] = 1;
            led_on_count_p2 --;  
            add_one_led_p2();
            score_p2 +=10;
         }        
       }

      if (prev_score_p2 != score_p2){
        prev_score_p2 = score_p2;
      }
      else {                              //Если кнопка чужая
        score_p2 -= 10;                   //Отнимим 10 очков
        prev_score_p2 = score_p2;

        
          for (int x = 0; x < max_led_on_count_p1; x++){//т.к. игроков 2 то игрок 1 в любом случае есть, проверим на совпадение с масивом игрока 1
            if (key == led_array_p1[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p1 += 10;
              prev_score_p1 = score_p1;
              int_cnt_but_p1[x] = 1;
              led_on_count_p1 --;                      //Уменьшим количество включенных
              add_one_led_p1();                        //Добавим светодиод
              Serial.print ("FSP1_");
              Serial.println(score_p1);
            }
          } 
           

        if (players >= 3){
          for (int x = 0; x < max_led_on_count_p3; x++){//Если игроков 3 или больше, проверим на совпадение с масивом игрока 3
            if (key == led_array_p3[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p3 += 10;
              prev_score_p3 = score_p3;
              int_cnt_but_p3[x] = 1;
              led_on_count_p3 --;                      //Уменьшим количество включенных
              add_one_led_p3();                        //Добавим светодиод
              Serial.print ("FSP3_");
              Serial.println(score_p3);
            }
          } 
        }  

        if (players >= 4){
          for (int x = 0; x < max_led_on_count_p4; x++){//Если игроков 3 или больше, проверим на совпадение с масивом игрока 3
            if (key == led_array_p4[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p4 += 10;
              prev_score_p4 = score_p4;
              int_cnt_but_p4[x] = 1;
              led_on_count_p4 --;                      //Уменьшим количество включенных
              add_one_led_p4();                        //Добавим светодиод
              Serial.print ("FSP4_");
              Serial.println(score_p4);
            }
          } 
        }  
        
        Serial.print("FP2_");
      } 
     Serial.println(score_p2);
    }//End of key > 13 & key <28
 
 
 
   // ***************************************************************************************************************************************BUTTON FROM BOARD 3
      if (key > 27 & key <42) {           // Если нажата кнопка второго игрового поля то будем считать игроку 3
          for (int x = 0; x < max_led_on_count_p3; x++){      //Проверим на совпадение со своим массивом
          if (key == led_array_p3[x]){
            Serial.print("SP3_");  
            int_cnt_but_p3[x] = 1;
            led_on_count_p3 --;  
            add_one_led_p3();
            score_p3 +=10;
         }        
       }

      if (prev_score_p3 != score_p3){
        prev_score_p3 = score_p3;
      }
      else {                              //Если кнопка чужая
        score_p3 -= 10;                   //Отнимим 10 очков
        prev_score_p3 = score_p3;

        
          for (int x = 0; x < max_led_on_count_p1; x++){//т.к. игроков 3 то игрок 1 в любом случае есть, проверим на совпадение с масивом игрока 1
            if (key == led_array_p1[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p1 += 10;
              prev_score_p1 = score_p1;
              int_cnt_but_p1[x] = 1;
              led_on_count_p1 --;                      //Уменьшим количество включенных
              add_one_led_p1();                        //Добавим светодиод
              Serial.print ("FSP1_");
              Serial.println(score_p1);
            }
          } 


          for (int x = 0; x < max_led_on_count_p1; x++){//т.к. игроков 3 то игрок 1 в любом случае есть, проверим на совпадение с масивом игрока 2
            if (key == led_array_p2[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p2 += 10;
              prev_score_p2 = score_p2;
              int_cnt_but_p2[x] = 1;
              led_on_count_p2 --;                      //Уменьшим количество включенных
              add_one_led_p2();                        //Добавим светодиод
              Serial.print ("FSP2_");
              Serial.println(score_p2);
            }
          } 
           

        
        if (players >= 4){
          for (int x = 0; x < max_led_on_count_p4; x++){//Если игроков 4 или больше, проверим на совпадение с масивом игрока 3
            if (key == led_array_p4[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p4 += 10;
              prev_score_p4 = score_p4;
              int_cnt_but_p4[x] = 1;
              led_on_count_p4 --;                      //Уменьшим количество включенных
              add_one_led_p4();                        //Добавим светодиод
              Serial.print ("FSP4_");
              Serial.println(score_p4);
            }
          } 
        }  
        
        Serial.print("FP3_");
      } 
     Serial.println(score_p3);
    }//End of key > 27 & key <42
  
  
  
  
  // ***************************************************************************************************************************************BUTTON FROM BOARD 4
      if (key > 41 & key <56) {           // Если нажата кнопка второго игрового поля то будем считать игроку 4
          for (int x = 0; x < max_led_on_count_p4; x++){      //Проверим на совпадение со своим массивом
          if (key == led_array_p4[x]){
            Serial.print("SP4_");  
            int_cnt_but_p4[x] = 1;
            led_on_count_p4 --;  
            add_one_led_p4();
            score_p4 +=10;
         }        
       }

      if (prev_score_p4 != score_p4){
        prev_score_p4 = score_p4;
      }
      else {                              //Если кнопка чужая
        score_p4 -= 10;                   //Отнимим 10 очков
        prev_score_p4 = score_p4;

        
          for (int x = 0; x < max_led_on_count_p1; x++){//т.к. игроков 3 то игрок 1 в любом случае есть, проверим на совпадение с масивом игрока 1
            if (key == led_array_p1[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p1 += 10;
              prev_score_p1 = score_p1;
              int_cnt_but_p1[x] = 1;
              led_on_count_p1 --;                      //Уменьшим количество включенных
              add_one_led_p1();                        //Добавим светодиод
              Serial.print ("FSP1_");
              Serial.println(score_p1);
            }
          } 


          for (int x = 0; x < max_led_on_count_p2; x++){//т.к. игроков 3 то игрок 2 в любом случае есть, проверим на совпадение с масивом игрока 2
            if (key == led_array_p2[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p2 += 10;
              prev_score_p2 = score_p2;
              int_cnt_but_p2[x] = 1;
              led_on_count_p2 --;                      //Уменьшим количество включенных
              add_one_led_p2();                        //Добавим светодиод
              Serial.print ("FSP2_");
              Serial.println(score_p2);
            }
          } 

         for (int x = 0; x < max_led_on_count_p3; x++){//т.к. игроков 4 то игрок 3 в любом случае есть, проверим на совпадение с масивом игрока 2
            if (key == led_array_p3[x]){                 //Если совпало то посчитаем очки игроку 2
              score_p3 += 10;
              prev_score_p3 = score_p3;
              int_cnt_but_p3[x] = 1;
              led_on_count_p3 --;                      //Уменьшим количество включенных
              add_one_led_p3();                        //Добавим светодиод
              Serial.print ("FSP3_");
              Serial.println(score_p3);
            }
          } 
           

     
        Serial.print("FP4_");
      } 
     Serial.println(score_p4);
    }//End of key > 27 & key <42
 
 
 
 
 }  //End of LEVEL HARD





    }//End of if z!=0
} //End of message=pressed
} //End of сustomkey  
    }
    }
} //End of main loop
  
  


  
void add_one_led_p1() //Добавление кода одного светодиода в массив игрок 1
{
  for (int x=0; x < max_led_on_count_p1; x++){
    if (int_cnt_but_p1[x] == 1){
      while (unique_check_p1(x) != 0){}
    };
  }
  /*ОСТАНОВКА ИГРЫ 
  if (Serial.available() > 0){
  input_buffer = Serial.readString();  
  Serial.println (input_buffer);
  if (input_buffer == "P") {soft_restart();}
  }*/
}

void game_over (){
  //input_buffer = Serial.readString();  
  //if (input_buffer == "q") {
    Serial.println ("GAME OVER");
    Serial.print ("PLAYER 1 Total Score: ");
    Serial.println (score_p1);
    if (players >> 1) {
      Serial.print ("PLAYER 2 Total Score: ");
      Serial.println (score_p2);
    }
    delay (1000);
    timer = 0;
    soft_restart();
    //}
}



void add_one_led_p2() //Добавление кода одного светодиода в массив игрок 2
{
  for (int x=0; x < max_led_on_count_p2; x++){
    if (int_cnt_but_p2[x] == 1){
      while (unique_check_p2(x) != 0){}
    };
  }
}


void add_one_led_p3() //Добавление кода одного светодиода в массив игрок 2
{
  for (int x=0; x < max_led_on_count_p3; x++){
    if (int_cnt_but_p3[x] == 1){
      while (unique_check_p3(x) != 0){}
    };
  }
}

void add_one_led_p4() //Добавление кода одного светодиода в массив игрок 2
{
  for (int x=0; x < max_led_on_count_p4; x++){
    if (int_cnt_but_p4[x] == 1){
      while (unique_check_p4(x) != 0){}
    };
  }
}


int unique_check_p1(int but){
  int err=0;
  int rand_min = 0;    //Минимальное значение для рандома игрок 1, уровень простой и средний, заполняем только свое поле
  int rand_max = 13;   //Максимальное значение для рандома игрок 1, уровень простой и средний, заполняем только свое поле

  if (level > 1 & players == 2){rand_max = 28;} //Если уровень сложный то будем заполнять и на соседнее поле
  if (level > 1 & players == 3){rand_max = 42;} //Если уровень сложный то будем заполнять и на соседнее поле
  if (level > 1 & players == 4){rand_max = 56;} //Если уровень сложный то будем заполнять и на соседнее поле
    
  int new_value = random (rand_min, rand_max); //Сгенирируем новое число
  
  for (int x=0; x < max_led_on_count_p1; x++){  //Перебор своего массива игрок 1
  int current_value = led_array_p1[x];
   if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
  }

  if (level > 1){                                  //Если уровень сложный
    if (players >=2){                                 //если игроков 2 и более
      for (int x=0; x < max_led_on_count_p2; x++){    //то проверим на совпадение нового числа с масивом игрока 2
          int current_value = led_array_p2[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    }
    if (players >=3){                                 //если игроков 3 и более
      for (int x=0; x < max_led_on_count_p3; x++){    //то проверим на совпадение нового числа с масивом игрока 2
          int current_value = led_array_p3[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    }
    if (players >=4){                                 //если игроков 4 и более
      for (int x=0; x < max_led_on_count_p4; x++){    //то проверим на совпадение нового числа с масивом игрока 2
          int current_value = led_array_p4[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    }
  }
  
  if (err==0){black_button (led_array_p1[but],1); led_array_p1[but] = new_value; led_on_count_p1++; int_cnt_but_p1 [but]++;
  for (int x = 0; x < max_led_on_count_p1; x++) {white_button (led_array_p1[x],1);}//Перебор массива и включение светодиодов, игрок 1
  }
 
  return err;
}

int unique_check_p2(int but){
  int err=0;
  int rand_min = 14;    //Минимальное значение для рандома игрок 2, уровень простой и средний, заполняем только свое поле
  int rand_max = 28;    //Максимальное значение для рандома игрок 2, уровень простой и средний, заполняем только свое поле
  
  if (level > 1 & players == 2){rand_min = 0; rand_max = 28;} //Если уровень сложный то будем заполнять и на соседнее поле
  if (level > 1 & players == 3){rand_min = 0; rand_max = 42;} //Если уровень сложный то будем заполнять и на соседнее поле
  if (level > 1 & players == 4){rand_min = 0; rand_max = 56;} //Если уровень сложный то будем заполнять и на соседнее поле
  
  
  int new_value = random (rand_min, rand_max); //Сгенирируем новое число
  
  for (int x=0; x < max_led_on_count_p2; x++){ //Перебор своего массива (игрок 2)
   int current_value = led_array_p2[x];
   if (new_value == current_value){err ++;}    //Если сгенерированое число равно числу из масива то увеличим счетчик ошибок
  }

  if (level > 1){                                  //Если уровень сложный
     for (int x=0; x < max_led_on_count_p1; x++){    //то проверим на совпадение нового числа с масивом игрока 1, без условия т.к. наличие 2го игрока подразумивает наличие 1го
          int current_value = led_array_p1[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    
    if (players >=3){                                 //если игроков 3 и более 
      for (int x=0; x < max_led_on_count_p3; x++){    //то проверим на совпадение нового числа с масивом игрока 3
          int current_value = led_array_p3[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    }
    if (players >=4){
      for (int x=0; x < max_led_on_count_p4; x++){    //то проверим на совпадение нового числа с масивом игрока 4
          int current_value = led_array_p4[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    }
  }
    if (err==0){black_button (led_array_p2[but],2); led_array_p2[but] = new_value; led_on_count_p2++; int_cnt_but_p2 [but]++;
  for (int x = 0; x < max_led_on_count_p2; x++) {white_button (led_array_p2[x],2);}//Перебор массива и включение светодиодов, игрок 2}
  }

 
  return err;
}


  int unique_check_p3(int but){
  int err=0;
  int rand_min = 28;    //Минимальное значение для рандома игрок 2, уровень простой и средний, заполняем только свое поле
  int rand_max = 42;    //Максимальное значение для рандома игрок 2, уровень простой и средний, заполняем только свое поле

  if (level > 1 & players == 2){rand_min = 0; rand_max = 28;} //Если уровень сложный то будем заполнять и на соседнее поле
  if (level > 1 & players == 3){rand_min = 0; rand_max = 42;} //Если уровень сложный то будем заполнять и на соседнее поле
  if (level > 1 & players == 4){rand_min = 0; rand_max = 56;} //Если уровень сложный то будем заполнять и на соседнее поле
  
  
  int new_value = random (rand_min, rand_max); //Сгенирируем новое число
  
  for (int x=0; x < max_led_on_count_p3; x++){ //Перебор своего массива (игрок 3)
   int current_value = led_array_p3[x];
   if (new_value == current_value){err ++;}    //Если сгенерированое число равно числу из масива то увеличим счетчик ошибок
  }

if (level > 1){                                  //Если уровень сложный
     for (int x=0; x < max_led_on_count_p1; x++){    //то проверим на совпадение нового числа с масивом игрока 1, без условия т.к. наличие 3го игрока подразумивает наличие 1го
          int current_value = led_array_p1[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    
     for (int x=0; x < max_led_on_count_p2; x++){    //то проверим на совпадение нового числа с масивом игрока 1, без условия т.к. наличие 3го игрока подразумивает наличие 2го
          int current_value = led_array_p2[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
      
    if (players >=4){
      for (int x=0; x < max_led_on_count_p4; x++){    //то проверим на совпадение нового числа с масивом игрока 4
          int current_value = led_array_p4[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    }
  }

  
  if (err==0){black_button (led_array_p3[but],3); led_array_p3[but] = new_value; led_on_count_p3++; int_cnt_but_p3 [but]++;
  for (int x = 0; x < max_led_on_count_p3; x++) {white_button (led_array_p3[x],3);}//Перебор массива и включение светодиодов, игрок 3}
  }

 
  return err;
}

 int unique_check_p4(int but){
  int err=0;
  int rand_min = 42;    //Минимальное значение для рандома игрок 2, уровень простой и средний, заполняем только свое поле
  int rand_max = 56;    //Максимальное значение для рандома игрок 2, уровень простой и средний, заполняем только свое поле

  if (level > 1 & players == 2){rand_min = 0; rand_max = 28;} //Если уровень сложный то будем заполнять и на соседнее поле
  if (level > 1 & players == 3){rand_min = 0; rand_max = 42;} //Если уровень сложный то будем заполнять и на соседнее поле
  if (level > 1 & players == 4){rand_min = 0; rand_max = 56;} //Если уровень сложный то будем заполнять и на соседнее поле
  
  
  int new_value = random (rand_min, rand_max); //Сгенирируем новое число
  
  for (int x=0; x < max_led_on_count_p4; x++){ //Перебор своего массива игрок 4
   int current_value = led_array_p4[x];
   if (new_value == current_value){err ++;}    //Если сгенерированое число равно числу из масива то увеличим счетчик ошибок
  }

 if (level > 1){                                  //Если уровень сложный
     for (int x=0; x < max_led_on_count_p1; x++){    //то проверим на совпадение нового числа с масивом игрока 1, без условия т.к. наличие 3го игрока подразумивает наличие 1го
          int current_value = led_array_p1[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    
     for (int x=0; x < max_led_on_count_p2; x++){    //то проверим на совпадение нового числа с масивом игрока 1, без условия т.к. наличие 3го игрока подразумивает наличие 2го
          int current_value = led_array_p2[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
      
    
      for (int x=0; x < max_led_on_count_p3; x++){    //то проверим на совпадение нового числа с масивом игрока 3
          int current_value = led_array_p3[x];
          if (new_value == current_value){err ++;}     //Если число равно числу из масива то увеличим счетчик ошибок
      }
    
  }

  
  if (err==0){black_button (led_array_p4[but],4); led_array_p4[but] = new_value; led_on_count_p4++; int_cnt_but_p4 [but]++;
  for (int x = 0; x < max_led_on_count_p4; x++) {white_button (led_array_p4[x],4);}//Перебор массива и включение светодиодов, игрок 3}
  }

 
  return err;
}

  



void color_transform_p1(int col_code){
  switch (col_code-48)
 {
   case 1: col_R_p1 = 255;   // 0xFF0000 Red
           col_G_p1 = 0;
           col_B_p1 = 0;
           break;
           
   case 2: col_R_p1 = 0;     //0x00ff00 Green
           col_G_p1 = 255;
           col_B_p1 = 0;
           break;
           
   case 3: col_R_p1 = 0;     //0x0000FF Blue
           col_G_p1 = 0;
           col_B_p1 = 255;
           break;
           
   case 4: col_R_p1 = 255;       // 0xFF00FF Pink
           col_G_p1 = 0;
           col_B_p1 = 255;
           break;
           
   case 5: col_R_p1 = 255;    //0xFFFF00 Yellow
           col_G_p1 = 255;
           col_B_p1 = 0;
           break;
    }
}//color_transform_p1

void color_transform_p2(int col_code){
  switch (col_code-48)
 {
   case 1: col_R_p2 = 255;   // 0xFF0000 Red
           col_G_p2 = 0;
           col_B_p2 = 0;
           break;
           
   case 2: col_R_p2 = 0;     //0x00ff00 Green
           col_G_p2 = 255;
           col_B_p2 = 0;
           break;
           
   case 3: col_R_p2 = 0;     //0x0000FF Blue
           col_G_p2 = 0;
           col_B_p2 = 255;
           break;
           
   case 4: col_R_p2 = 255;       // 0xFF00FF Pink
           col_G_p2 = 0;
           col_B_p2 = 255;
           break;
           
   case 5: col_R_p2 = 255;    //0xFFFF00 Yellow
           col_G_p2 = 255;
           col_B_p2 = 0;
           break;
    }
}//color_transform_p2



  
//}

