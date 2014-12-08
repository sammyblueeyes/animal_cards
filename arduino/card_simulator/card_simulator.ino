#include <stdint.h>


// Connect pin 12 to the IR LED
int led = 11;
// Connect pin 11 to the relay
int relay = 13;

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
  for (int i = 191; i < 1024; i++) {
    digitalWrite(relay, HIGH);
 delay(200);
    digitalWrite(relay, LOW);
    delay(200);


    digitalWrite(led, HIGH); 
    delay(500);

    
    print_bits(generate_barcode(i), 13);
    digitalWrite(led, HIGH); 
    delay(500);
    digitalWrite(led, LOW); 

    delay(5000);

  }
}


