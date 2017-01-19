#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define NAV1          2
#define NAV2          3
#define BUTTON_SUBMIT 9

enum menu_layers {mn_main,mn_submenu1};
extern menu_layers act_menu_layer;
//extern String menu_items [];
//extern uint8_t size_a;

int8_t set_menuItemPrint(Adafruit_PCD8544 disp, String array[], String header, int8_t pos, uint8_t a_size);
void set_subMenuValuePrint(Adafruit_PCD8544 disp, String array[], int8_t pos, uint8_t value);
void trigger_nav1(void);
void trigger_nav2(void);
