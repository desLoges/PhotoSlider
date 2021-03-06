/*
    Rotary Encoder - Interrupt Example

    The circuit:
    * encoder pin A to Arduino pin 2
    * encoder pin B to Arduino pin 3
    * encoder ground pin to ground (GND)
*/

#include <Rotary.h>

Rotary r = Rotary(2, 3);

void setup() {
  Serial.begin(9600);
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
}

void loop() {

}
