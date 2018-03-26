//DEBUG
void int_led() 
{
  //счетчик "тиков" игрок 1
  for (int x=0; x < max_led_on_count_p1; x++){
    if (int_cnt_but_p1 [x] == stop_time){
      int_cnt_but_p1[x] = 0;
      led_on_count_p1 --;  
      };
  }
  //for (int z=0; z < max_led_on_count; z++){int_cnt_but [z] ++;} //Глюк, чому?
  int_cnt_but_p1 [0] ++;
  int_cnt_but_p1 [1] ++;
  int_cnt_but_p1 [2] ++;
  int_cnt_but_p1 [3] ++;
  int_cnt_but_p1 [4] ++;

  //счетчик "тиков" игрок 2
  for (int x=0; x < max_led_on_count_p2; x++){
    if (int_cnt_but_p2 [x] == stop_time){
      int_cnt_but_p2[x] = 0;
      led_on_count_p2 --;  
      };
  }
  int_cnt_but_p2 [0] ++;
  int_cnt_but_p2 [1] ++;
  int_cnt_but_p2 [2] ++;
  int_cnt_but_p2 [3] ++;
  int_cnt_but_p2 [4] ++;

  //счетчик "тиков" игрок 3
  for (int x=0; x < max_led_on_count_p3; x++){
    if (int_cnt_but_p3 [x] == stop_time){
      int_cnt_but_p3[x] = 0;
      led_on_count_p3 --;  
      };
  }
  int_cnt_but_p3 [0] ++;
  int_cnt_but_p3 [1] ++;
  int_cnt_but_p3 [2] ++;
  int_cnt_but_p3 [3] ++;
  int_cnt_but_p3 [4] ++;

  //счетчик "тиков" игрок 3
  for (int x=0; x < max_led_on_count_p4; x++){
    if (int_cnt_but_p3 [x] == stop_time){
      int_cnt_but_p4[x] = 0;
      led_on_count_p4 --;  
      };
  }
  int_cnt_but_p4 [0] ++;
  int_cnt_but_p4 [1] ++;
  int_cnt_but_p4 [2] ++;
  int_cnt_but_p4 [3] ++;
  int_cnt_but_p4 [4] ++;

  //timer ++;
    
}




void mp3start(){} //Инициализация мр3 модуля, в дебаге не используется




void demo(){
  int x = random(0, 28);
  int y = random(1,3);
  white_button (x,y);
  black_button (x+1,y);
  delay (50);
  }


void all_led_off(){}


void white_button(int Pin, int PlayerCode){
  int osnova = 14;    //количество елементов в одной доске
  int DashAddr = 32; //адрес доски
  int startelement = osnova * (PlayerCode-1);
  int element = startelement + Pin;
 
  if (Pin < 13){
    DashAddr = 32;
  }
  if (Pin > 13 & Pin <28){
    element -= 14;
    DashAddr = 33;
  }

  if (Pin > 27 & Pin <42){
    element -= 28;
    DashAddr = 34;
  }

  if (Pin > 41 & Pin <56){
    element -= 42;
    DashAddr = 35;
  }

  //delay (2); //Без задержки крашится при симуляции
  Wire.beginTransmission(DashAddr);     //Begin the transmission to PCF8574
  Wire.write(element);                                //Send the data to PCF8574
  Wire.endTransmission();            //End the Transmission
}








void black_button(int Pin, int PlayerCode){
 int osnova = 14;    //количество елементов в одной доске
  int DashAddr = 32; //адрес доски
  
  int startelement = osnova * (PlayerCode-1);
  int element = (startelement + Pin);
 
    if (Pin < 13){
    DashAddr = 32;
  }
  if (Pin > 13 & Pin <28){
    element -= 14;
    DashAddr = 33;
  }

  if (Pin > 27 & Pin <42){
    element -= 28;
    DashAddr = 34;
  }

  if (Pin > 41 & Pin <56){
    element -= 42;
    DashAddr = 35;
  }

  
  element += 100;
 // delay (2);//Без задержки крашится при симуляции
  // Serial.println (element);
  Wire.beginTransmission(DashAddr);     //Begin the transmission to PCF8574
  Wire.write(element);                                //Send the data to PCF8574
  Wire.endTransmission();            //End the Transmission
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
          client.println("<B>Цвет ИГРОК 1:</B>");
          client.println("<p><select  name=\"w\" >");
          client.println("<option value=\"1\" style=\"background: #FF0000;\">Красный</option>");
          client.println("<option value=\"2\" style=\"background: #00FF00;\">Зеленый</option>");
          client.println("<option value=\"3\" style=\"background: #0000FF;\">Синий</option>");
          client.println("<option value=\"4\" style=\"background: #FF00FF;\">Розовый</option>");
          client.println("<option value=\"5\" style=\"background: #FFFF00;\">Желтый</option>");
          client.println("</p></select>");
          
          
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


void SerialInit(){
  Serial.println("****COLOR DASH WELCOME YOU!****"); //Prompt User for input
  Serial.println("       READY TO START       LPC"); //Prompt User for input
  while (Serial.available()==0) {}             //Пока нету ввода ждем
  input_buffer =Serial.readString();                 //Read user input into input_buffer
  input_buffer.toCharArray(charbuffer,10);          //поместили принятые команды в масив
  for (int x = 0; x < 3; x++){         // перебор массива
    level = charbuffer[0] - 48;
    players = charbuffer[1] - 48; 
    delay(1);
  }
    Serial.print ("level= "); 
    Serial.println (level); 
    Serial.print ("players= "); 
    Serial.println (players); 
 }



