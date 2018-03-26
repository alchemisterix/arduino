//RELEASE FILE
// timer compare interrupt service routine
void int_led()
{

  for (int x=0; x < max_led_on_count_p1; x++){
    if (int_cnt_but_p1[x] == stop_time){
      //black_button(x);
      int_cnt_but_p1[x] = 0;
      led_on_count_p1 --;  
      };
  }

   
   //for (int z=0; z < max_led_on_count; z++){int_cnt_but [z] ++;}
  
  int_cnt_but_p1 [0] ++;
  int_cnt_but_p1 [1] ++;
  int_cnt_but_p1 [2] ++;
  int_cnt_but_p1 [3] ++;
  int_cnt_but_p1 [4] ++;

  for (int x=0; x < max_led_on_count_p2; x++){
    if (int_cnt_but_p2 [x] == stop_time){
          int_cnt_but_p2[x] = 0;
      led_on_count_p2 --;  
      };
  }

   
   //for (int z=0; z < max_led_on_count; z++){int_cnt_but [z] ++;} //Глюк, чому?
  
  int_cnt_but_p2 [0] ++;
  int_cnt_but_p2 [1] ++;
  int_cnt_but_p2 [2] ++;
  int_cnt_but_p2 [3] ++;
  int_cnt_but_p2 [4] ++;

  timer++;

}

void mp3start(){
  mySerial.begin (9600);
  mp3_set_serial (mySerial);  //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_reset();
  delay (1000);
  mp3_set_volume (90);
  delay (400);
  mp3_play (6);
  delay (100);
  }


void demo1(){
   for (int x =0; x < NUM_LEDS; x++){
    leds[x] = CRGB::Red;
    }
    FastLED.show();
    delay (1000);
     for (int x =0; x < NUM_LEDS; x++){
    leds[x] = CRGB::Black;
    }
    FastLED.show();
}


void demo (){
  int x = random(0, 28);
  int y = random(1,3);
  white_button (x,y);
  black_button (x+1,y);
  white_button (1,1);
  delay (500);
  }

void all_led_off(){
  //for (int x =0; x<led_count; x++){digitalWrite (ledpin [x], LOW);}

for (int x = 0; x < 28; x++){black_button (x,1);}//выключим нужные светодиоды
//for (int x = 0; x < 14; x++){black_button (x,2);}//выключим нужные светодиоды
}//all_led_off end 



//leds by button shutdown
void black_button(int num_button, int PlayerCode)
{
   //Timer3.stop();
  //num_button = random (0,7);
  //if (PlayerCode == 1){startLed = by_one_button * num_button;}
  //  if (PlayerCode == 2){num_button = num_button + 14 ; startLed = by_one_button * num_button;}
  startLed = by_one_button * num_button;
  endLed = startLed + by_one_button;

  
  
  //if (num_button == 0) {startLed = 1; endLed = 6;}
  while (startLed < endLed){
    leds[startLed] = CRGB::DarkMagenta;
    startLed++;
   
       
    }
    FastLED.show();
  //Timer3.start();
}


void white_button(int num_button,  int PlayerCode)
{
  //Timer3.stop();
  //num_button = random (0,7);
  startLed = num_button * by_one_button;
  //if (PlayerCode == 1){startLed = by_one_button * num_button;}
  //if (PlayerCode == 2){num_button=14 + num_button ; startLed = by_one_button * num_button;}
  endLed = startLed + by_one_button;
  
  if (PlayerCode == 2){col_R = col_R_p2; col_G = col_G_p2; col_B = col_B_p2;}
  if (PlayerCode == 1){col_R = col_R_p1; col_G = col_G_p1; col_B = col_B_p1;}

//if (num_button == 0) {startLed = 1; endLed = 6;}
  while (startLed < endLed)
    {
     // leds[startLed] = CRGB::Green;
     
      leds[startLed].setRGB(col_R,col_G,col_B);      
      startLed++;
      }
      FastLED.show();
 //Timer3.start();
}

//All leds red by button
void white_button1()
{
  
  for (num_button =0; num_button<7; num_button ++)
  {
    startLed = num_button * by_one_button;
    endLed = startLed + by_one_button;
    while (startLed < endLed)
    {
      leds[startLed] = CRGB::Red;
      FastLED.show();
      startLed++;
    }
  }
}

void ethernet_init(){
  Ethernet.begin(mac, ip);
  server.begin();
  }


  int ethernet_available() {
  
  // Create a client connection
  //Serial.print (recieved);
  //while (recieved==0){
   // Serial.print (recieved);
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string 
          readString += c; 
          //Serial.print(c); //print what server receives to serial monitor
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          //Serial.println(readString);

          //now output HTML data header
         
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino GET test page</TITLE>");
          client.println("<meta charset=\"UTF-8 \" />");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<CENTER>");

          client.println("<H1>COLOR DASH GAMEPLAY SELECT</H1>");

          client.println("<FORM ACTION=\"http://192.168.1.177:80\" method=get >");

          //client.println("Pin 5 \"on\" or \"off\": <INPUT TYPE=TEXT NAME=\"LED\" VALUE=\"\" SIZE=\"25\" MAXLENGTH=\"50\"><BR>");


          client.println("<BR>");
          client.println("<B>Количество игроков:</B>");
          client.println("<BR>");
          client.println("<input type=\"radio\" name=\"p\" value=\"1\">Один");
          client.println("<BR>");
          client.println("<input type=\"radio\" name=\"p\" value=\"2\">Два");
          client.println("<BR>");
          client.println("<input type=\"radio\" name=\"p\" value=\"4\">Четыре");
          client.println("<BR>");

          client.println("<BR>");
          client.println("<B>Выберите уровень игры:</B>");
          client.println("<BR>");
          client.println("<input type=\"radio\" name=\"l\" value=\"0\">Простой");
          client.println("<BR>");
          client.println("<input type=\"radio\" name=\"l\" value=\"1\">Средний");
          client.println("<BR>");
          client.println("<input type=\"radio\" name=\"l\" value=\"2\">Сложный");
          client.println("<BR>");


          client.println("<BR>");
          client.println("<B>Цвет ИГРОК 1:</B>");
          client.println("<p><select  name=\"e\" >");
          client.println("<option value=\"1\" style=\"background: #FF0000;\">Красный</option>");
          client.println("<option value=\"2\" style=\"background: #00FF00;\">Зеленый</option>");
          client.println("<option value=\"3\" style=\"background: #0000FF;\">Синий</option>");
          client.println("<option value=\"4\" style=\"background: #FF00FF;\">Розовый</option>");
          client.println("<option value=\"5\" style=\"background: #FFFF00;\">Желтый</option>");
          client.println("</p></select>");


          client.println("<BR>");
          client.println("<BR>");
          client.println("<B>Цвет ИГРОК 2:</B>");
          client.println("<p><select  name=\"w\" >");
          client.println("<option value=\"1\" style=\"background: #FF0000;\">Красный</option>");
          client.println("<option value=\"2\" style=\"background: #00FF00;\">Зеленый</option>");
          client.println("<option value=\"3\" style=\"background: #0000FF;\">Синий</option>");
          client.println("<option value=\"4\" style=\"background: #FF00FF;\">Розовый</option>");
          client.println("<option value=\"5\" style=\"background: #FFFF00;\">Желтый</option>");
          client.println("</p></select>");
          
          client.println("<BR>");
          client.println("<BR>");
          client.println("<INPUT TYPE=SUBMIT NAME=\"submit\" VALUE=\"НАЧАТЬ ИГРУ\">");




          client.println("</FORM>");

          client.println("<BR>");

          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          //stopping client
          client.stop();


          for (int x=0; x<100; x++){
            if (readString [x] == 112) {players =(readString[x+2]- 48); recieved=1;} //p for Players
            if (readString [x] == 108) {level =  (readString[x+2]- 48);} //l for Level
            if (readString [x] == 101) {color_transform_p1 (readString[x+2]);} //e for player onE color
            if (readString [x] == 119) {color_transform_p2 (readString[x+2]);} //w for player tWo color
            
          }
          /////////////////////
          if(readString.indexOf("on") >0)//checks for on
          {
            
          }

          //clearing string for next read
          readString="";
          


        }
      }
    }
  } 
  //}//end while (recieved==0){
return recieved;
}//End 


void SerialInit (){
  input_buffer =Serial.readString();                 //Read user input into input_buffer

  input_buffer.toCharArray(charbuffer,10);          //поместили принятые команды в масив
 
  
 
 //Serial.println (sizeof (charbuffer));
 
 
 for (int x = 0; x < 3; x++){         // перебор массива
  level = charbuffer[0] - 48;
  players = charbuffer[1] - 48;
 

 switch (charbuffer [2] - 48)
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


switch (charbuffer [3] - 48)
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

  

    delay(1);
   
 }

}


