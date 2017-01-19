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

//struct s_values slider_values;

s_values slider_values = {};



int8_t menu_position_main = 1;
int8_t menu_position_submenu1 = 0;
int8_t menu_position_submenu2 = 0;

bool submit_pushed = false;
bool was_irq_enc = false;
bool enc_CW = false;
bool enc_CCW = false;
String menu_items [] =  {">>>>>>", "<<<<<<", "Manual", "GoHome" };
String submenu_items_slide [] = {" START", " Speed", " Exit", "Set X", "Return"};

menu_layers act_menu_layer = mn_main;

//String menu_items [] = {"item 1", "item 2", "item 3", "item 4", "item 5", "item 6" };
uint8_t menu_size = (sizeof(menu_items)/sizeof(menu_items[0]));
uint8_t submenu_size_slide = (sizeof(submenu_items_slide)/sizeof(submenu_items_slide[0]));

uint8_t x = 0;

void setup()   {

  slider_values.ret_l = false;
  slider_values.ret_r = false;
  slider_values.speed_l = SLIDER_SPEED_1;
  slider_values.speed_r = SLIDER_SPEED_1;
  slider_values.x_l = SLIDER_X_2;
  slider_values.x_r = SLIDER_X_2;

  Serial.begin(9600);

  display.begin();
  // init done
  pinMode(BUTTON_SUBMIT, INPUT_PULLUP);
  pinMode(BUTTON_HOME, INPUT_PULLUP);
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
  set_subMenuValuePrint(display, menu_items, 3, " Wait!");
  go_home();
  set_subMenuValuePrint(display, menu_items, 3, "  OK");
  delay(1000);
  menu_position_main = set_menuItemPrint(display, menu_items, "MENU", menu_position_main, menu_size);
  //sei();

}


void loop() {

  //////////////////////////////////////////////////////////////////////////////
  //////////////////////////// NAVIGATION ACTION ///////////////////////////////
  //////////////////////////////////////////////////////////////////////////////
  if (was_irq_enc){
    was_irq_enc=false;

    //display.setFont(&FreeSans9pt7b);
    if (act_menu_layer == mn_main){
      Serial.println("MAIN");

      if(enc_CW) menu_position_main++;
      else if(enc_CCW) menu_position_main--;

      menu_position_main = set_menuItemPrint(display, menu_items, "MENU", menu_position_main, menu_size);
      menu_position_submenu1 = 0;//menu_position_main;
    } else if (act_menu_layer == mn_submenu1) {
      //enter to submenu
      if(enc_CW) menu_position_submenu1++;
      else if(enc_CCW) menu_position_submenu1--;

      switch (menu_position_main){
        //>>>>>>>>
        case 0:
          Serial.println("SUBMENU >>");
          menu_position_submenu1 = set_menuItemPrint(display, submenu_items_slide, "Config", menu_position_submenu1, submenu_size_slide);
        break;
        //<<<<<<<<
        case 1:
          Serial.println("SUBMENU <<");
          menu_position_submenu1 = set_menuItemPrint(display, submenu_items_slide, "Config", menu_position_submenu1, submenu_size_slide);
        break;
        //Manual
        case 2:
          Serial.println("SUBMENU man");
          menu_position_submenu1 = set_menuItemPrint(display, submenu_items_slide, "Config", menu_position_submenu1, 3);
        break;
        //go home
        case 3:
          Serial.println("GOHOME");
          set_subMenuValuePrint(display, menu_items, menu_position_main, " Wait!");
          go_home();
          set_subMenuValuePrint(display, menu_items, menu_position_main, "  OK");
          delay(1000);
          Serial.println("at home");
          act_menu_layer=mn_main;
          was_irq_enc = true;
        break;
        default:
          Serial.println("E100");
        break;
      }
    }else if(act_menu_layer == mn_submenu2){
          Serial.println("submenu 2");
          switch (menu_position_submenu1){
          //start
          case 0:
            Serial.println(submenu_items_slide[menu_position_submenu1]);
            set_subMenuValuePrint(display, menu_items, menu_position_main, " ");
            delay(5000); //replace this with slide function
            act_menu_layer=mn_main;
            was_irq_enc = true;
          break;
          //speed
          case 1:
            Serial.println(submenu_items_slide[menu_position_submenu1]);
            if( menu_position_main == 0 ){
              if(enc_CW) slider_values.speed_r++;
              else if(enc_CCW) slider_values.speed_r--;
              set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, String(slider_values.speed_r));
            }else if( menu_position_main == 1 ){
              if(enc_CW) slider_values.speed_l++;
              else if(enc_CCW) slider_values.speed_l--;
              set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, String(slider_values.speed_l));
            }else if( menu_position_main == 2 ){
              if(enc_CW) slider_values.speed_m++;
              else if(enc_CCW) slider_values.speed_m--;
              set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, String(slider_values.speed_m));
            }
          break;
          //exit skipped
          //set x
          case 3:
            Serial.println(submenu_items_slide[menu_position_submenu1]);
            if( menu_position_main == 0 ){
              if(enc_CW) slider_values.x_r++;
              else if(enc_CCW) slider_values.x_r--;
              set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, String(slider_values.x_r));
            }else if( menu_position_main == 1 ){
              if(enc_CW) slider_values.x_l++;
              else if(enc_CCW) slider_values.x_l--;
              set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, String(slider_values.x_l));
            }
          break;
          //return
          case 4:
            Serial.println(submenu_items_slide[menu_position_submenu1]);
            if( menu_position_main == 0 ){
              if(enc_CW) slider_values.ret_r = !slider_values.ret_r;
              else if(enc_CCW) slider_values.ret_r = !slider_values.ret_r;

              if (slider_values.ret_r) set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, "YES");
              else set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, "NO");
            }else if( menu_position_main == 1 ){
              if(enc_CW) slider_values.ret_l = !slider_values.ret_l;
              else if(enc_CCW) slider_values.ret_l = !slider_values.ret_l;

              if (slider_values.ret_l) set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, "YES");
              else set_subMenuValuePrint(display, submenu_items_slide, menu_position_submenu1, "NO");
            }
          break;
          default:
            Serial.println("E101");
          break;
        }
    }else{
      //what kind of menu is there???
      Serial.println("E103");
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  //////////////////////////// SUBMIT ACTION ///////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////

  if( digitalRead(BUTTON_SUBMIT) == LOW ){
    //wait to release
    Serial.println("\nSubmit");
    while (digitalRead(BUTTON_SUBMIT) == LOW){
      delay(1);
    }

    was_irq_enc=true;

    if (act_menu_layer == mn_main) act_menu_layer = mn_submenu1;
    else if (act_menu_layer == mn_submenu1){
      if (menu_position_submenu1 == 2) act_menu_layer = mn_main;
      else act_menu_layer = mn_submenu2;
    }
    else if (act_menu_layer == mn_submenu2) act_menu_layer = mn_submenu1;
  }

  enc_CW=false;
  enc_CCW=false;

  delay(2);

}

void trigger_nav1(void){
  detachInterrupt(digitalPinToInterrupt(NAV1));
  delay(500);
  Serial.println("\nnav 1");
  was_irq_enc=true;
  enc_CCW=true;
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
