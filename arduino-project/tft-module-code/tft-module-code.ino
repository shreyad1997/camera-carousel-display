// Will Init Screen and Show Menu or Home Screen. Then Waits for presuure touch. Then shows Co-ordinates
// with Home BMP Button, using home1.bmp in sd card of TFT

#include <SPI.h>            
#include <SD.h>             

#include <Adafruit_GFX.h>    
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>

#define LCD_CS A3     // Chip Select goes to Analog 3
#define LCD_CD A2     // Command/Data goes to Analog 2
#define LCD_WR A1     // LCD Write goes to Analog 1
#define LCD_RD A0     // LCD Read goes to Analog 0
#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin

#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

// Define some TFT readable colour codes to human readable names
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

// from Kuman_SC3A
#define TS_MINX 85
#define TS_MAXX 925
#define TS_MINY 135
#define TS_MAXY 910

#define MINPRESSURE 10
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
boolean buttonEnabled = true;
int currentPage = 0;


#define SD_CS     10
#define PALETTEDEPTH   0     // do not support Palette modes

char namebuf[32] = "/";   //BMP files in root directory
//char namebuf[32] = "/bitmaps/";  //BMP directory e.g. files in /bitmaps/*.bmp


void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Finding out the type of TFT LCD... ID is : "));
  tft.reset();
  uint16_t identifier = tft.readID();
  Serial.print(identifier, HEX);
  tft.begin(identifier);
  Serial.print(". TFT size is : "); Serial.print(tft.width()); Serial.print(" x "); Serial.println(tft.height());
  tft.setRotation(1);
  //tft.invertDisplay(1);                           // UNCOMMENT THIS IF COLORS get inverted
  tft.fillScreen(BLACK);

  Serial.println("Drawing Init Screen...");
  drawInitScreen();
  delay(100);
  
  Serial.println("Drawing Home or Menu Screen...");
  drawHomeScreen();
  delay(100);

  bool good = SD.begin(SD_CS);
  if (!good) {
        Serial.print(F("cannot start SD"));
        while (1);
  }
  Serial.print(F("SD Card Started!"));
    
  Serial.println("Waiting for Touch Screen Pressure Now...");
  
  
}

void drawInitScreen()
{
  tft.setTextSize(3);

  tft.setTextColor(YELLOW);
  tft.setCursor(30, 100);
  tft.print("LCD Intialization...");

  tft.setTextColor(WHITE);
  tft.setCursor(30, 140);
  tft.print("Loading...");

  for ( int i = 1; i < 400; i++)
  {
      tft.fillRect(30, 200, i, 15, RED);
      delay(1);
  }
}


void drawHomeScreen()
{
  
  tft.fillScreen(BLACK);
  tft.drawRoundRect(0, 0, 479, 319, 8, RED);     //Page border

  
  tft.setTextSize(2);
  tft.setCursor(170, 30);
  tft.setTextColor(GREEN);
  tft.print("My Own Menu");
  

  tft.fillRoundRect(60, 80, 360, 40, 8, RED);
  tft.drawRoundRect(60, 80, 360, 40, 8, WHITE); //Oscilloscope

  tft.fillRoundRect(60, 140, 360, 40, 8, RED);   //RGB led
  tft.drawRoundRect(60, 140, 360, 40, 8, WHITE);

  tft.fillRoundRect(60, 200, 360, 40, 8, RED);
  tft.drawRoundRect(60, 200, 360, 40, 8, WHITE);  //Game

    
  tft.setTextColor(BLACK);
  
  tft.setCursor(175, 95);
  tft.print("Choice 1");

  
  tft.setCursor(175, 155);
  tft.print("Choice 2");

  
  tft.setCursor(175, 210);
  tft.print("Choice 3");

  tft.setTextSize(1);
  tft.setCursor(160, 305);
  tft.setTextColor(WHITE);
  tft.print("Select one of the options...");    
}


void loop(void) {
  
 
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  buttonEnabled = true;
  
  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  if (currentPage == 0){
      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
          /*   
          Serial.print("X = "); Serial.print(p.x);
          Serial.print("\tY = "); Serial.print(p.y);
          Serial.print("\tPressure = "); Serial.println(p.z); */
       
          //map(value, fromLow, fromHigh, toLow, toHigh)
          p.x = map( p.x, TS_MINX, TS_MAXX,0,  480 );
          p.y = map( p.y, TS_MINY, TS_MAXY, 0, 320 );
           
          /*
          Serial.print("("); Serial.print(p.x);
          Serial.print(", "); Serial.print(p.y);
          Serial.println(")");    */
          
          
          if(p.x>60 && p.x<400 && p.y>80 && p.y<120 )// The user has pressed inside the red rectangle
          {
                            
              currentPage = 1;
              tft.fillScreen(BLACK);
              tft.drawRoundRect(0, 0, 479, 319, 8, BLUE);     //Page border
              drawBackButton();
              
              tft.setCursor(50,50);
              tft.setTextColor(YELLOW);
              tft.setTextSize(2);
              tft.print("You clicked first Button!");  
    
              if(p.x>390 && p.x<435 && p.y>270 && p.y<285 ){
                  currentPage = 0;
                  drawHomeScreen();
                  
              }
              
          }
          if(p.x>60 && p.x<400 && p.y>140 && p.y<180)// The user has pressed inside the red rectangle
          {
              
              currentPage = 2;
              tft.fillScreen(BLACK);
              tft.drawRoundRect(0, 0, 479, 319, 8, BLUE);     //Page border
              drawBackButton();
              
              tft.setCursor(50,50);
              tft.setTextColor(MAGENTA);
              tft.setTextSize(2);
              tft.print("You clicked second Button!");  
    
              if(p.x>390 && p.x<435 && p.y>270 && p.y<285 ){
                  currentPage = 0;
                  drawHomeScreen();
                  
              }
              
      
          }
          if(p.x>60 && p.x<400 && p.y>200 && p.y<240)// The user has pressed inside the red rectangle
          {
                            
              currentPage = 3;
              tft.fillScreen(BLACK);
              tft.drawRoundRect(0, 0, 479, 319, 8, BLUE);     //Page border
              drawBackButton();
              
              tft.setCursor(50,50);
              tft.setTextColor(CYAN);
              tft.setTextSize(2);
              tft.print("You clicked third Button!");  
    
              
                  
              }
              
          }
      } // end of page 0


  if (currentPage == 1){
      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
         
          //map(value, fromLow, fromHigh, toLow, toHigh)
          p.x = map( p.x, TS_MINX, TS_MAXX,0,  480 );
          p.y = map( p.y, TS_MINY, TS_MAXY, 0, 320 );
           
          if(p.x>410 && p.x<470 && p.y>250 && p.y<300 ){
                      currentPage = 0;          
                      drawHomeScreen();
          } 
      }       
  } // end of page 1

  if (currentPage == 2){
      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
         
          //map(value, fromLow, fromHigh, toLow, toHigh)
          p.x = map( p.x, TS_MINX, TS_MAXX,0,  480 );
          p.y = map( p.y, TS_MINY, TS_MAXY, 0, 320 );
           
           if(p.x>410 && p.x<470 && p.y>250 && p.y<300 ){
                      currentPage = 0;          
                      drawHomeScreen();
          } 
      }       
  } // end of page 2

  if (currentPage == 3){
      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
         
          //map(value, fromLow, fromHigh, toLow, toHigh)
          p.x = map( p.x, TS_MINX, TS_MAXX,0,  480 );
          p.y = map( p.y, TS_MINY, TS_MAXY, 0, 320 );
           
          if(p.x>410 && p.x<470 && p.y>250 && p.y<300 ){
                      currentPage = 0;          
                      drawHomeScreen();
          } 
      }       
  } // end of page 3
  
}

void drawBackButton(){

    uint8_t ret;  
    ret = showBMP("/home1.bmp", 410, 250);   
}

#define BMPIMAGEOFFSET 54

#define BUFFPIXEL      20

uint16_t read16(File& f) {
    uint16_t result;         // read little-endian
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}

uint32_t read32(File& f) {
    uint32_t result;
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}

uint8_t showBMP(char *nm, int x, int y)
{
    File bmpFile;
    int bmpWidth, bmpHeight;    // W+H in pixels
    uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
    uint32_t bmpImageoffset;    // Start of image data in file
    uint32_t rowSize;           // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
    uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
    uint8_t bitmask, bitshift;
    boolean flip = true;        // BMP is stored bottom-to-top
    int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
    uint32_t pos;               // seek position
    boolean is565 = false;      //

    uint16_t bmpID;
    uint16_t n;                 // blocks read
    uint8_t ret;

    if ((x >= tft.width()) || (y >= tft.height()))
        return 1;               // off screen

    bmpFile = SD.open(nm);      // Parse BMP header
    bmpID = read16(bmpFile);    // BMP signature
    (void) read32(bmpFile);     // Read & ignore file size
    (void) read32(bmpFile);     // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile);       // Start of image data
    (void) read32(bmpFile);     // Read & ignore DIB header size
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    n = read16(bmpFile);        // # planes -- must be '1'
    bmpDepth = read16(bmpFile); // bits per pixel
    pos = read32(bmpFile);      // format
    if (bmpID != 0x4D42) ret = 2; // bad ID
    else if (n != 1) ret = 3;   // too many planes
    else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
    else if (bmpDepth < 16 && bmpDepth > PALETTEDEPTH) ret = 5; // palette 
    else {
        bool first = true;
        is565 = (pos == 3);               // ?already in 16-bit format
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
        if (bmpHeight < 0) {              // If negative, image is in top-down order.
            bmpHeight = -bmpHeight;
            flip = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if ((x + w) >= tft.width())       // Crop area to be loaded
            w = tft.width() - x;
        if ((y + h) >= tft.height())      //
            h = tft.height() - y;

        if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
            bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
            bitmask = 0xFF;
            if (bmpDepth < 8)
                bitmask >>= bmpDepth;
            bitshift = 8 - bmpDepth;
            n = 1 << bmpDepth;
            lcdbufsiz -= n;
            palette = lcdbuffer + lcdbufsiz;
            for (col = 0; col < n; col++) {
                pos = read32(bmpFile);    //map palette to 5-6-5
                palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
            }
        }

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
        for (row = 0; row < h; row++) { // For each scanline...
            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            uint8_t r, g, b, *sdptr;
            int lcdidx, lcdleft;
            if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
                pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            else        // Bitmap is stored top-to-bottom
                pos = bmpImageoffset + row * rowSize;
            if (bmpFile.position() != pos) { // Need seek?
                bmpFile.seek(pos);
                buffidx = sizeof(sdbuffer); // Force buffer reload
            }

            for (col = 0; col < w; ) {  //pixels in row
                lcdleft = w - col;
                if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
                for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
                    uint16_t color;
                    // Time to read more pixel data?
                    if (buffidx >= sizeof(sdbuffer)) { // Indeed
                        bmpFile.read(sdbuffer, sizeof(sdbuffer));
                        buffidx = 0; // Set index to beginning
                        r = 0;
                    }
                    switch (bmpDepth) {          // Convert pixel from BMP to TFT format
                        case 24:
                            b = sdbuffer[buffidx++];
                            g = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            color = tft.color565(r, g, b);
                            break;
                        case 16:
                            b = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            if (is565)
                                color = (r << 8) | (b);
                            else
                                color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
                            break;
                        case 1:
                        case 4:
                        case 8:
                            if (r == 0)
                                b = sdbuffer[buffidx++], r = 8;
                            color = palette[(b >> bitshift) & bitmask];
                            r -= bmpDepth;
                            b <<= bmpDepth;
                            break;
                    }
                    lcdbuffer[lcdidx] = color;

                }
                tft.pushColors(lcdbuffer, lcdidx, first);
                first = false;
                col += lcdidx;
            }           // end cols
        }               // end rows
        tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
        ret = 0;        // good render
    }
    bmpFile.close();
    return (ret);
}

