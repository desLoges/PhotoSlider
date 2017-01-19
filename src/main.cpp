#include "slider.h"
#include <Fonts/FreeSans9pt7b.h>

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 8);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

//#include <Rotary.h>

//Rotary r = Rotary(2, 3);

int8_t menu_position = 1;
bool submit_pushed = false;
bool was_irq_enc = false;
bool enc_CW = false;
String menu_items [] =  {">>>>>>", "<<<<<<", "Manual", "GoHome" };
String submenu_items [] = {" START", " Set X", " Speed", "Return", " Exit"};
menu_layers act_menu_layer = mn_main;

//String menu_items [] = {"item 1", "item 2", "item 3", "item 4", "item 5", "item 6" };
uint8_t menu_size = (sizeof(menu_items)/sizeof(menu_items[0]));
uint8_t submenu_size = (sizeof(submenu_items)/sizeof(submenu_items[0]));
uint8_t x = 0;

void setup()   {
  Serial.begin(9600);

  display.begin();
  // init done
  pinMode(BUTTON_SUBMIT, INPUT_PULLUP);
  pinMode(NAV1, INPUT_PULLUP);
  pinMode(NAV2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(NAV1), trigger_nav1, FALLING);
  attachInterrupt(digitalPinToInterrupt(NAV2), trigger_nav2, FALLING);


  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(40);
  display.clearDisplay();


  Serial.println("Init done");
  delay(2000);
  menu_position = set_menuItemPrint(display, menu_items, "MENU", menu_position, menu_size);

  //sei();

}


void loop() {

  if (was_irq_enc){
    was_irq_enc=false;

    if(enc_CW) menu_position++;
    else menu_position--;


    //display.setFont(&FreeSans9pt7b);
    if (act_menu_layer == mn_main){
      Serial.println("MAIN");
      menu_position = set_menuItemPrint(display, menu_items, "MENU", menu_position, menu_size);
    } else if (act_menu_layer == mn_submenu1) {
      Serial.println("SUBMENU");
      menu_position = set_menuItemPrint(display, submenu_items, "Config", menu_position, submenu_size);
    }
  }

  if( digitalRead(BUTTON_SUBMIT) == LOW ){
    //wait to release
    Serial.println("\nSubmit");
    while (digitalRead(BUTTON_SUBMIT) == LOW){
      delay(1);
    }

    if (act_menu_layer == mn_submenu1){
        switch (menu_position){
          //start
          case 0:
            Serial.println(submenu_items[menu_position]);
          break;
          //set x
          case 1:
            Serial.println(submenu_items[menu_position]);
            set_subMenuValuePrint(display, submenu_items, menu_position, 2);
          break;
          //speed
          case 2:
            Serial.println(submenu_items[menu_position]);
            set_subMenuValuePrint(display, submenu_items, menu_position, 2);
          break;
          //return
          case 3:
            Serial.println(submenu_items[menu_position]);
            set_subMenuValuePrint(display, submenu_items, menu_position, 2);
          break;
          //exit
          case 4:
            menu_position = 1;
            menu_position = set_menuItemPrint(display, menu_items, "MENU", menu_position, menu_size);
            act_menu_layer = mn_main;
          break;
          default:
            Serial.println("Err 100");
          break;
        }
    }else if (act_menu_layer == mn_main){
      act_menu_layer = mn_submenu1;
      menu_position = 1;
      menu_position = set_menuItemPrint(display, submenu_items, "Config", menu_position, submenu_size);
    }
  }

  delay(2);

}

void trigger_nav1(void){
  detachInterrupt(digitalPinToInterrupt(NAV1));
  delay(500);
  Serial.println("\nnav 1");
  was_irq_enc=true;
  enc_CW=false;
  attachInterrupt(digitalPinToInterrupt(NAV1), trigger_nav1, FALLING);
}

void trigger_nav2(void){
  detachInterrupt(digitalPinToInterrupt(NAV2));
  delay(500);
  Serial.println("\nnav 2");
  was_irq_enc=true;
  enc_CW=true;
  attachInterrupt(digitalPinToInterrupt(NAV2), trigger_nav2, FALLING);
}
