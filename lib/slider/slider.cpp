#include "slider.h"

extern s_values slider_values;

void testfillrect(Adafruit_PCD8544 disp) {
  uint8_t color = 1;
  for (int16_t i=0; i<disp.height()/2; i+=3) {
    // alternate colors
    disp.fillRect(i, i, disp.width()-i*2, disp.height()-i*2, color%2);
    disp.display();
    color++;
  }
}

int8_t set_menuItemPrint(Adafruit_PCD8544 disp, String array[], String header, int8_t pos, uint8_t a_size){

  //uint8_t a_size = sizeof(array)/sizeof(array[0]);

  String menu_item_prev = "";
  String menu_item_act = "";
  String menu_item_next = "";

  if (pos >= a_size){
    pos = 0;
    //Serial.print("pos set to 0");
  }
  if (pos < 0){
    pos = a_size-1;
    //Serial.print("pos set s-1");
  }

  Serial.print("pos = ");
  Serial.println(pos);
  Serial.println(a_size);

  if ( pos == 0 ) {
    menu_item_prev = array[a_size-1];
    menu_item_act = array[pos];
    menu_item_next = array[pos+1];
  } else if (pos < (a_size-1) ){
    menu_item_prev = array[pos-1];
    menu_item_act = array[pos];
    menu_item_next = array[pos+1];
  }else{ // pos == a_size
    menu_item_prev = array[pos-1];
    menu_item_act = array[pos];
    menu_item_next = array[0];
  }

  Serial.println(menu_item_prev);
  Serial.println(menu_item_act);
  Serial.println(menu_item_next);

  disp.clearDisplay();

  //
  disp.drawFastHLine(3,3,66,BLACK);


  //battery sign
  disp.fillRect(72,1,6,5,BLACK);
  disp.drawRect(77,1,6,5,BLACK);
  disp.drawFastVLine(83,2,3,BLACK);

  //visual signing
  disp.setTextSize(1);
  disp.setCursor(3,8);
  disp.setTextColor(BLACK);
  disp.println("/           \\");

  disp.setCursor(3,38);
  disp.setTextColor(BLACK);
  disp.println("\\           /");


  // text display tests
  disp.setTextSize(1);
  disp.setCursor(22,8);
  disp.setTextColor(BLACK);
  disp.println(menu_item_prev);

  disp.fillRect(0,17,83,18,BLACK);
  disp.setTextSize(2);
  disp.setCursor(3,18);
  disp.setTextColor(WHITE, BLACK);
  disp.println(menu_item_act);

  disp.setTextSize(1);
  disp.setCursor(22,38);
  disp.setTextColor(BLACK);
  disp.println(menu_item_next);

  disp.display();

  return pos;
}

void set_slideMenuValuePrint(Adafruit_PCD8544 disp, int8_t speed, int8_t x, uint8_t status){

  //disp.clearDisplay();

  disp.setTextSize(1);
  disp.setCursor(1,38);
  disp.setTextColor(WHITE);
  disp.print("Speed: ");
  disp.print(speed);
  disp.print("  X: ");
  disp.print(x);
  disp.println("%");

  disp.display();
}

void set_subMenuValuePrint(Adafruit_PCD8544 disp, String array[], int8_t pos, String value){

  disp.clearDisplay();

  //header
  disp.fillRect(0,0,70,8,BLACK);
  disp.setTextSize(1);
  disp.setCursor(16,0);
  disp.setTextColor(WHITE, BLACK);
  disp.println(array[pos]);
  //disp.drawFastHLine(0,8,83,BLACK);

  //battery sign
  disp.fillRect(72,1,6,5,BLACK);
  disp.drawRect(77,1,6,5,BLACK);
  disp.drawFastVLine(83,2,3,BLACK);

  //disp.fillRect(0,14,83,18,BLACK);
  disp.setTextSize(2);
  disp.setCursor(3,18);
  disp.setTextColor(BLACK);
  //disp.println("<    >");
  //disp.setCursor(30,20);
  disp.print(value);
  //disp.println("%");


  disp.display();
}

//Go home
bool go_home(void){
  bool ret = false;

  while(digitalRead(BUTTON_HOME)){
    go_right(SLIDER_SPEED_1);
  }

  //go_left(SLIDER_SPEED_1);

  ret = true;
  return ret;
}

void go_right(uint8_t speed){
  Serial.println(">>");
  delay(1000);
}

void go_left(uint8_t speed){
  Serial.println("<<");
  delay(1000);
}

void set_eepromInit(void){
  Serial.println("EEPROM init");
  if (EEPROM.read(0) != 23){
    //EEPROM is epmty
    //write default values
    Serial.println("EEPROM first use?");
    EEPROM.write(0, 23);
    EEPROM.write(1, SLIDER_SPEED_2);
    EEPROM.write(2, SLIDER_SPEED_2);
    EEPROM.write(3, SLIDER_SPEED_2);
  }
  Serial.print("EEPROM read...");
  slider_values.speed_l = EEPROM.read(1);
  slider_values.speed_r = EEPROM.read(2);
  slider_values.speed_m = EEPROM.read(3);
  slider_values.x_l = EEPROM.read(4);
  slider_values.x_r = EEPROM.read(5);
  slider_values.ret_l = (uint8_t)EEPROM.read(6);
  slider_values.ret_r = (uint8_t)EEPROM.read(7);
  Serial.println("done");
}

void set_eepromUpdate(s_values lider_values){
  Serial.print("EEPROM write...");
  EEPROM.write(1, slider_values.speed_l);
  EEPROM.write(2, slider_values.speed_r);
  EEPROM.write(3, slider_values.speed_m);
  EEPROM.write(4, slider_values.x_l);
  EEPROM.write(5, slider_values.x_l);
  EEPROM.write(6, (uint8_t)slider_values.ret_l);
  EEPROM.write(7, (uint8_t)slider_values.ret_r);
  Serial.println("done");
}
