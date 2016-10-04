#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const int weeksInYear = 52;
const int numYears = 75;
const int numBlocksWeeksInLife = weeksInYear * numYears;

const int weekNumber = 40; //This year's week number;
const int numberOfYearsToDateExclusive = 30 * weeksInYear;
const int weeksToDate = numberOfYearsToDateExclusive + weekNumber;

struct calendar {
  unsigned int deviceWidth;
  unsigned int deviceHeight;
  unsigned int numBlocksToDraw;
  unsigned int totalBlocks;
  unsigned int columns;
  unsigned int rows;
  unsigned int spacing;
  float blockWidth;
  float blockHeight;
} cal; 

void setup() {
  Serial.begin(9600);
  tft.begin();
  int canvasWidth = floor((float)tft.width() * 0.99);
  int canvasHeight = floor((float)tft.height() * 0.99);
  initCalendar(canvasWidth, canvasHeight, weeksToDate, numBlocksWeeksInLife);
  printCalendarInfo();
  tft.fillScreen(tft.color565(0, 0, 0));
}

void initCalendar(unsigned int deviceWidth, unsigned int deviceHeight, unsigned int blockCount, unsigned int maxBlocks){ 
  cal.numBlocksToDraw = blockCount;
  cal.totalBlocks = maxBlocks;
  cal.spacing = 1;
  cal.deviceWidth = deviceWidth;
  cal.deviceHeight = deviceHeight;
  int factors[2];
  getMultiples(cal.totalBlocks, factors, cal.deviceHeight, cal.deviceWidth);
  cal.columns = factors[1];
  cal.rows = factors[0]; 
  cal.blockWidth = round((float)cal.deviceWidth / (float)cal.columns) - cal.spacing;
  cal.blockHeight = round((float)cal.deviceHeight / (float)cal.rows) - cal.spacing;
}

void printCalendarInfo(){
  Serial.print("Device Dimensions: ");  Serial.print(cal.deviceWidth); Serial.print("x"); Serial.println(cal.deviceHeight);
  Serial.print("Blocks: "); Serial.print(cal.numBlocksToDraw); Serial.print("/"); Serial.println(cal.totalBlocks);
  Serial.print("Block Dimensions: "); Serial.print(cal.blockWidth); Serial.print("x:"); Serial.println(cal.blockHeight);
  Serial.print("Grid: "); Serial.print(cal.columns); Serial.print("x"); Serial.println(cal.rows);
}

int* getMultiples(int number, int multiples[], int w, int h) {
  for (int i = number-1; i > 1; i--) {
    if (number % i == 0) {      
      int d = number / i;
      if (w / i > 1 && h / d > 1){
        multiples[0] = i;
        multiples[1] = d;
        return multiples;        
      }
    }
  }

  return multiples;
}

void loop(void) {
  draw();
}

void draw() {
  drawCalendarInLandscape();  
  delay(60 * 1000);
}

void drawCalendarInLandscape(){
  drawCalendarBlocksInInLandscape(0, cal.numBlocksToDraw, 0, 128, 0);
  drawCalendarBlocksInInLandscape(cal.numBlocksToDraw, cal.totalBlocks, 0, 0, 64);    
}

void drawCalendarBlocksInInLandscape(unsigned int blockStart, unsigned int blockEnd, uint16_t r, uint16_t g, uint16_t b){
  const unsigned int blockWidth = (cal.blockWidth + cal.spacing);
  const unsigned int blockHeight = (cal.blockHeight + cal.spacing);
  unsigned int blockIndex = 0;
  for (unsigned int col= 0; col < cal.columns; col += 1){
    for (unsigned int row = 0; row < cal.rows; row +=1){
      bool isVisible = blockIndex >= blockStart && blockIndex < blockEnd;
      if (isVisible) {
        tft.fillRect(col * blockWidth, row * blockHeight, cal.blockWidth, cal.blockHeight, tft.color565(r, g, b));
      }      
      blockIndex +=1;
    }
  }  
}

