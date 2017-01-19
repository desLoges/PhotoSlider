#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define NAV1                2
#define NAV2                3
#define BUTTON_SUBMIT       9
#define BUTTON_HOME         A0
#define SLIDER_X_1          1
#define SLIDER_X_2          2
#define SLIDER_SPEED_1      1
#define SLIDER_SPEED_2      2
#define SLIDER_SPEED_3      3
#define SLIDER_SPEED_4      4

enum menu_layers {mn_main, mn_submenu1, mn_submenu2};
extern menu_layers act_menu_layer;
//extern String menu_items [];
//extern uint8_t size_a;

struct s_values{
  int8_t speed_r;
  int8_t speed_l;
  int8_t speed_m;
  int32_t x_r;
  int32_t x_l;
  bool ret_r;
  bool ret_l;
};

int8_t set_menuItemPrint(Adafruit_PCD8544 disp, String array[], String header, int8_t pos, uint8_t a_size);
void set_subMenuValuePrint(Adafruit_PCD8544 disp, String array[], int8_t pos, String value);
void set_slideMenuValuePrint(Adafruit_PCD8544 disp, String array[], int8_t pos);
bool go_home(void);
void go_right(uint8_t speed);
void go_left(uint8_t speed);
void trigger_nav1(void);
void trigger_nav2(void);
