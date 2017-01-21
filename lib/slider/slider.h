#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>

#define NAV1                2
#define NAV2                3
#define BUTTON_SUBMIT       9
#define BUTTON_HOME         A0
#define SLIDER_X_10         1
#define SLIDER_X_20         2
#define SLIDER_X_30         3
#define SLIDER_X_40         4
#define SLIDER_X_50         5
#define SLIDER_X_60         6
#define SLIDER_X_70         7
#define SLIDER_X_80         8
#define SLIDER_X_90         9
#define SLIDER_X_100        10
#define SLIDER_SPEED_1      1
#define SLIDER_SPEED_2      2
#define SLIDER_SPEED_3      3
#define SLIDER_SPEED_4      4
#define MOTOR_DIRECTION     11 //pin
#define MOTOR_STEP          10 //pin
#define MOTOR_MS1
#define MOTOR_MS2


enum menu_layers {mn_main, mn_submenu1, mn_submenu2};
extern menu_layers act_menu_layer;
//extern String menu_items [];
//extern uint8_t size_a;

struct s_values{
  int8_t speed_r;
  int8_t speed_l;
  int8_t speed_m;
  int8_t x_r;
  int8_t x_l;
  bool ret_r;
  bool ret_l;
};

int8_t set_menuItemPrint(Adafruit_PCD8544 disp, String array[], String header, int8_t pos, uint8_t a_size);
void set_subMenuValuePrint(Adafruit_PCD8544 disp, String array[], int8_t pos, String value);
void set_slideMenuValuePrint(Adafruit_PCD8544 disp, int8_t speed, int8_t x, uint8_t status);
void set_eepromInit(void);
void set_eepromUpdate(s_values lider_values);
bool go_home(void);
void go_right(uint8_t speed);
void go_left(uint8_t speed);
void trigger_nav1(void);
void trigger_nav2(void);
