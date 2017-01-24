#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>

#define NAV1                2
#define NAV2                3
#define BUTTON_SUBMIT       A1
#define BUTTON_HOME         A0
#define MOTOR_DIRECTION     12 //pin
#define MOTOR_STEP          10 //pin
#define MOTOR_ENABLE        A2
#define MOTOR_MS1           A3
#define MOTOR_MS2           A4
#define LCD_BACKLIGHT       11
#define LCD_CONTRAST        52

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
#define MIN_SPEED           1
#define MAX_SPEED           10
#define SLIDER_SPEED_DEFAULT 3
#define SLIDER_SPEED_GOHOME  MAX_SPEED
#define MIN_X               0
#define MAX_X               100
#define X_RESOLUTION        10

#define LCD_BACKLIGHT_OFF   digitalWrite(LCD_BACKLIGHT, LOW)
#define LCD_BACKLIGHT_ON    digitalWrite(LCD_BACKLIGHT, HIGH)
#define STEPPER_ENABLE      digitalWrite(MOTOR_ENABLE, LOW)
#define STEPPER_DISABLE     digitalWrite(MOTOR_ENABLE, HIGH)
#define STEPPER_MS1_OFF     digitalWrite(MOTOR_MS1, LOW)
#define STEPPER_MS1_ON      digitalWrite(MOTOR_MS1, HIGH)
#define STEPPER_MS2_OFF     digitalWrite(MOTOR_MS2, LOW)
#define STEPPER_MS2_ON      digitalWrite(MOTOR_MS2, HIGH)

enum menu_layers {mn_main, mn_submenu1, mn_submenu2};
extern menu_layers act_menu_layer;
//extern String menu_items [];
//extern uint8_t size_a;

struct s_values{
  uint8_t speed_r;
  uint8_t speed_l;
  uint8_t speed_m;
  uint8_t x_r;
  uint8_t x_l;
  bool ret_r;
  bool ret_l;
};

int8_t set_menuItemPrint(Adafruit_PCD8544 disp, String array[], String header, int8_t pos, uint8_t a_size);
void set_subMenuValuePrint(Adafruit_PCD8544 disp, String array[], int8_t pos, String value);
void set_slideMenuValuePrint(Adafruit_PCD8544 disp, uint8_t speed, uint8_t x, uint8_t status);
void set_eepromInit(void);
void set_eepromUpdate(s_values lider_values);
bool go_home(void);
void go_right(uint8_t speed);
void go_left(uint8_t speed);
void slide_right(uint8_t speed, uint8_t set_x);
void slide_left(uint8_t speed, uint8_t set_x);
