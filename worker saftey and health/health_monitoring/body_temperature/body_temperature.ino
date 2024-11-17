#include <Wire.h>
#include <Adafruit_GFX.h>       // Include core graphics library for the display
#include <Adafruit_SSD1306.h>   // Include Adafruit_SSD1306 library to drive the display
#include <Fonts/FreeMonoBold9pt7b.h>  // Add a custom font
#include <Adafruit_MLX90614.h>  //for infrared thermometer

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();  //for infrared thermometer
int temp;  // Create a variable to have something dynamic to show on the display

void setup()
{                
  delay(100);  // This delay is needed to let the display to initialize
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  mlx.begin();  //start infrared thermometer
}

void loop()
{
  temp++;  // Increase value for testing
  if(temp > 43)  // If temp is greater than 150
  {
    temp = 0;  // Set temp to 0
  }

  temp = mlx.readObjectTempC(); //comment this line if you want to test

  display.clearDisplay();  // Clear the display so we can refresh

  // Print text:
  display.setFont();
  display.setCursor(45,5);  // (x,y)
  display.println("TEMPERATURE");  // Text or value to print

  // Print temperature
  char string[10];  // Create a character array of 10 characters
  // Convert float to a string:
  dtostrf(temp, 4, 0, string);  // (variable, no. of digits we are going to use, no. of decimal digits, string name)
  
  display.setFont(&FreeMonoBold9pt7b);  // Set a custom font
  display.setCursor(22,25);  // (x,y)
  display.println(string);  // Text or value to print
  display.setCursor(90,25);  // (x,y)
  display.println("C");  // Text or value to print
  display.setFont(); 
  display.setCursor(78,15);  // (x,y)
  display.cp437(true);
  display.write(167);
  
  // Draw a filled circle:
    display.fillCircle(18, 27, 5, WHITE); // Draw filled circle (x,y,radius,color). X and Y are the coordinates for the center point
    
  // Draw rounded rectangle:
   display.drawRoundRect(16, 3, 5, 24, 2, WHITE); // Draw rounded rectangle (x,y,width,height,radius,color)
   
   // It draws from the location to down-right
    // Draw ruler step

   for (int i = 3; i<=18; i=i+2){
    display.drawLine(21, i, 22, i, WHITE);  // Draw line (x0,y0,x1,y1,color)
  }
  
  //Draw temperature
  temp = temp*0.43; //ratio for show
  display.drawLine(18, 23, 18, 23-temp, WHITE);  // Draw line (x0,y0,x1,y1,color)
  
  display.display();  // Print everything we set previously
}
