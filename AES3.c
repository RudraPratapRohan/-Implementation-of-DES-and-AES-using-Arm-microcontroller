#include <LPC214x.h>
#include "D:\rudra\AES_config.h"
#include "D:\rudra\aes.h"
#include <LCD.h>

// Define the key and plaintext
uint32_t key = 0X12345678;
uint32_t plaintext ="HELLO123";

// Define the AES context
AES_Context aes_ctx;

// Define the LCD control pins
#define LCD_RS P0_19
#define LCD_EN P0_20

// Define the LCD data pins
#define LCD_D0 P0_0
#define LCD_D1 P0_1
#define LCD_D2 P0_2
#define LCD_D3 P0_3
#define LCD_D4 P0_4
#define LCD_D5 P0_5
#define LCD_D6 P0_6
#define LCD_D7 P0_7

// Initialize the LCD
void init_lcd() {
  // Set the LCD control pins to output
  IODIR0 |= (1 << LCD_RS) | (1 << LCD_EN);

  // Set the LCD data pins to output
  IODIR0 |= (1 << LCD_D0) | (1 << LCD_D1) | (1 << LCD_D2) | (1 << LCD_D3) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);

  // Write the command to the LCD to initialize it
  lcd_command(0x38);

  // Write the command to the LCD to turn it on
  lcd_command(0x0E);

  // Clear the LCD display
  lcd_clear();
}

// Write a command to the LCD
void lcd_command(uint8_t cmd) {
  // Set the RS pin to low to indicate a command
  IOPIN0 &= ~(1 << LCD_RS);

  // Write the command to the LCD data pins
  IOPIN0 = (IOPIN0 & 0xFF00) | cmd;

  // Enable the LCD for 10us
  IOPIN0 |= (1 << LCD_EN);
  delay_us(10);
  IOPIN0 &= ~(1 << LCD_EN);
}

// Write a character to the LCD
void lcd_write_char(uint8_t data) {
  // Set the RS pin to high to indicate data
  IOPIN0 |= (1 << LCD_RS);

  // Write the data to the LCD data pins
  IOPIN0 = (IOPIN0 & 0xFF00) | data;

  // Enable the LCD for 10us
  IOPIN0 |= (1 << LCD_EN);
  delay_us(10);
  IOPIN0 &= ~(1 << LCD_EN);
}

// Clear the LCD display
void lcd_clear() {
  // Write the command to the LCD to clear it
  lcd_command(0x01);

  // Wait for the LCD to clear
  delay_ms(2);
}

// Initialize the AES context
void init_aes() {
  // Set the AES key and plaintext
  AES_set_key(&aes_ctx, key, 128);
}

// Encrypt the plaintext using AES
void encrypt_aes() {
  // Encrypt the plaintext
  AES_encrypt(&aes_ctx, plaintext);
}

// Display the encrypted text on the LCD
void display_encrypted_text() {
  // Clear the LCD display
  lcd_clear();

  // Display the encrypted text on the LCD
  for (int i = 0; i < 16; i++) {
    lcd_write_char(plaintext[i]);
  }
}

// Main function
int main() {
  // Initialize the LCD
  init_lcd();
  // Initialize the AES context
  init_aes();

  // Encrypt the plaintext using AES
  encrypt_aes();

  // Display the encrypted text on the LCD
  display_encrypted_text();

  // Wait for a key press
  while (1) {
    if (IOPIN0 & (1 << 0)) {
      break;
    }
  }

  return 0;
}
