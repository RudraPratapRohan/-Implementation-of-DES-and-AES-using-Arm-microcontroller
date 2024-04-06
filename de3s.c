#include <LPC214X.H>
#include <stdio.h>
//#include <lcd.h>

// LCD pin assignments
#define LCD_RS P0.4
#define LCD_RW P0.5
#define LCD_EN P0.6
#define LCD_DATA_BUS P0.12-P0.15

// DES constants
#define IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 20, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17, 9, 1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7};

#define IP_1[64] = {40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 5);
// DES functions
uint32 f(uint32 R, uint32 K) {//mangler
  uint32 temp = R ^ K;
  for (int i = 0; i < 8; i++) {
    temp = S[i][(temp >> (i * 6)) & 0x3F];
  }
  return temp;
}

void des(uint32 *plaintext, uint32 *ciphertext, uint32 *key) {
  // Apply initial permutation
	plaintext ="HELLO123";
  plaintext = permute(plaintext, IP);
	
  // Perform 16 rounds of DES
  for (int i = 0; i < 16; i++) {
    // Split the plaintext into left and right halves
    uint32 L = plaintext >> 32;
    uint32 R = plaintext & 0xFFFFFFFF;

    // Apply the round function
    R = f(R, key[i]);

    // XOR the left and right halves
    plaintext = L ^ R;
  }

  // Apply final permutation
  ciphertext = permute(plaintext, IP_1);
}

// Permutation function
uint32 *permute(uint32 *input, const uint32 *table) {
  uint32 *output = (uint32 *) malloc(sizeof(uint32) * 64);
  for (int i = 0; i < 64; i++) {
    output[i] = input[table[i]];
  }
  return output;
}

// Main function
int main() {
  // Define the plaintext and key
  uint32 plaintext = "HELLO123";
  uint32 key = 0xABCDEFGH;

  // Initialize the LCD
  lcd_init(LCD_RS, LCD_RW, LCD_EN, LCD_DATA_BUS);

  // Perform DES encryption
  uint32 ciphertext[4];
  des(&plaintext, ciphertext, &key);

  // Display the ciphertext on the LCD
  lcd_write_string("Ciphertext: ");
  lcd_write_hex(ciphertext[0]);

  return 0;
}