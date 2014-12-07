#include <stdint.h>
/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */



// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 12;

static unsigned int count_bits(uint32_t bits)
{
  int count = 0;
  for (int i = 0; i < sizeof(bits)*8; i++) {
    if (bits & (1 << i)) count++;
  }
  return count;
}    


static void print_bits(uint32_t bits, unsigned int length)
{
  unsigned int total_len = 800 / length;
  unsigned int short_len = total_len * 0.2;
  unsigned int long_len = total_len * 0.7;

  for (int i = length-1; i >= 0; i--) {
    unsigned int black = (bits & 1 << i) ? long_len : short_len;
    digitalWrite(led, LOW); 
    delay(black);
    digitalWrite(led, HIGH);  
    delay(total_len - black);
  }
  printf("\n");
}


static uint32_t generate_barcode(uint32_t i) 
{
  unsigned int n = count_bits(i);
  bool even = n & 1;
  bool odd = !even;

  uint32_t barcode_val = 1 | 
    ((odd ? 1 : 0) << 1) | 
    ((even ? 1 : 0) << 2) |
    (i << 3);
  return barcode_val;
}




// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {

  delay(2000);
  for (int i = 181; i < 1024; i++) {
    digitalWrite(led, HIGH); 
    delay(500);
    digitalWrite(led, HIGH); 

    
    print_bits(generate_barcode(i), 13);
    digitalWrite(led, HIGH); 
    delay(500);
    digitalWrite(led, LOW); 
    delay(5000);
  }
}


