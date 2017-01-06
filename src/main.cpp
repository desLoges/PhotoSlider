/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

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

#include <Rotary.h>

Rotary r = Rotary(2, 3);

String menu_items [] = {">>>>>", "<<<<<", " >>>| ", " |<<< ", "Manual", "GoHome" };
//String menu_items [] = {"item 1", "item 2", "item 3", "item 4", "item 5", "item 6" };
uint8_t a_size = sizeof(menu_items)/sizeof(menu_items[0]);
int8_t menu_position = 1;
bool was_irq_enc=false;
bool enc_CW=false;


void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void set_menuItemPrint(String array[], uint8_t pos){

  //uint8_t a_size = sizeof(array)/sizeof(array[0]);
  String menu_item_prev = "";
  String menu_item_act = "";
  String menu_item_next = "";

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

  Serial.print("\npos = ");
  Serial.println(pos);
  Serial.println(menu_item_prev);
  Serial.println(menu_item_act);
  Serial.println(menu_item_next);


  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setCursor(12,5);
  display.setTextColor(BLACK);
  display.println(menu_item_prev);

  display.setCursor(2,16);
  display.setTextSize(2);
  display.setCursor(2,16);
  display.setTextColor(WHITE, BLACK);
  display.println(menu_item_act);

  display.setTextSize(1);
  display.setCursor(12,35);
  display.setTextColor(BLACK);
  display.println(menu_item_next);
  display.display();
}



void setup()   {
  Serial.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);
  display.clearDisplay();

  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();

}


void loop() {

  if (was_irq_enc){
    was_irq_enc=false;

    if(enc_CW) menu_position++;
    else menu_position--;

    if (menu_position >= a_size-1){
      menu_position = 0;
    }
    if (menu_position < 0){
      menu_position = a_size-1;
    }

    Serial.println(menu_position);
    set_menuItemPrint(menu_items, menu_position);
  }

  //menu_position++;

  delay(200);

}

ISR(PCINT2_vect) {
  cli();
  delayMicroseconds(1000);
  was_irq_enc=true;
  unsigned char result = r.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {
    //Serial.println("ClockWise");
    enc_CW=true;
  }
  else if (result == DIR_CCW) {
    //Serial.println("CounterClockWise");
    enc_CW=false;
  }
  sei();
}
