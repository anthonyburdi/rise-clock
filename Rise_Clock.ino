// Rise_Clock.ino

// ------------ Set time via computer time each upload
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

// const char *monthName[12] = {
//   "Jan", "Feb", "Mar", "Apr", "May", "Jun",
//   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
// };

// tmElements_t tm;

// void timeWriteSetup() {
//   bool parse=false;
//   bool config=false;

//   // get the date and time the compiler was run
//   if (getDate(__DATE__) && getTime(__TIME__)) {
//     parse = true;
//     // and configure the RTC with this info
//     if (RTC.write(tm)) {
//       config = true;
//     }
//   }

//   Serial.begin(9600);
//   while (!Serial) ; // wait for Arduino Serial Monitor
//   delay(200);
//   if (parse && config) {
//     Serial.print("DS1307 configured Time=");
//     Serial.print(__TIME__);
//     Serial.print(", Date=");
//     Serial.println(__DATE__);
//   } else if (parse) {
//     Serial.println("DS1307 Communication Error :-{");
//     Serial.println("Please check your circuitry");
//   } else {
//     Serial.print("Could not parse info from the compiler, Time=\"");
//     Serial.print(__TIME__);
//     Serial.print("\", Date=\"");
//     Serial.print(__DATE__);
//     Serial.println("\"");
//   }
// }

// //void loop() {
// //}

// bool getTime(const char *str)
// {
//   int Hour, Min, Sec;

//   if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
//   tm.Hour = Hour;
//   tm.Minute = Min;
//   tm.Second = Sec;
//   return true;
// }

// bool getDate(const char *str)
// {
//   char Month[12];
//   int Day, Year;
//   uint8_t monthIndex;

//   if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
//   for (monthIndex = 0; monthIndex < 12; monthIndex++) {
//     if (strcmp(Month, monthName[monthIndex]) == 0) break;
//   }
//   if (monthIndex >= 12) return false;
//   tm.Day = Day;
//   tm.Month = monthIndex + 1;
//   tm.Year = CalendarYrToTm(Year);
//   return true;
// }

// ------------- Time Write

// --------- Time Read
void timeReadSetup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

void timeRead() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  //delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
// --


#include <Adafruit_NeoPixel.h>

// This is a test for the Rise Clock

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN 6
#define PIXEL_COUNT 58
#define PIXEL_TYPE NEO_GRB + NEO_KHZ800

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

const unsigned long interval = 1000; // milliseconds, 1000 = one second
// unsigned long previousMillis = 0; // last time the display was updated
// Set the number of pixels per segment
const int pixelsPerSegment = 2;
// Set time parameters
// int countdownTime = 30*60;
// int secsLeft = countdownTime;

bool switched = false;

unsigned long red = strip.Color(255,0,0);
unsigned long green = strip.Color(0,255,0);
unsigned long blue = strip.Color(0,0,255);
unsigned long yellow = strip.Color(255,255,0);
unsigned long magenta = strip.Color(255,0,255);
unsigned long cyan = strip.Color(0,255,255);
unsigned long white = strip.Color(255,255,255);

// SEMI_AUTOMATIC mode is to allow the clock to operate without a wifi connection
//SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() 
{
  // Set realtime clock from computer clock
  // timeWriteSetup();
  // Read realtime clock
  timeReadSetup();
  timeRead();
  // Intialize pixels
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // ***** insert read here for input values set by making digital pins high
}

void loop() 
{
  
    // timeRead();
    // 10 sec "get ready" startup countdown
    countdown(10, 10, interval, red);
    timeRead();

//    // Show the word "GO"    
//    blankAll();
////    displayDigit(0, 10, green); // change string values to int
//    displayDigit(14, 10, green);
//    displayDigit(30, 0, green);
////    displayDigit(44, 0, green);
//    strip.show();
//    delay(3000);
    
    
    // Wed Workout countdown
    
    countdown(30*60, 6*60, interval, green);
    countdown(24*60, 5*60, interval, blue);
    countdown(19*60, 5*60, interval, yellow);
    countdown(14*60, 4*60, interval, red);
    countdown(10*60, 5*60, interval, cyan);
    countdown(5*60, 5*60, interval, magenta);
    
    blankAll();
    displayDigit(0,8,red);
    displayDigit(14,8,red);
    displayDigit(30,8,red);
    displayDigit(44,8,red);
    strip.show();
    delay(100*1000);

    
    // Mon Night Track CountUp
//    countup(4,30*60, interval, green);

//    if (Spark.connected() == false) {
//        Spark.connect();
//    }

}

void blinkTime(int secsToDisplay, unsigned long color) {
    // captured in countdown(), may not need separate function
}

void countdownFromTo(int fromSecs, int toSecs, long interval, unsigned long color) {
    // code goes here
}

// Countdown function
void countdown(int fromSecs, int duration, long interval, unsigned long color) {
    
    int secsLeft = duration; // how long this function will run
    unsigned long previousMillis = 0; // last time the display was updated
    unsigned long startMillis = millis(); // start at whatever time the function is called
    
    while(secsLeft >= 0) {
        // Process input from server
        //Spark.process();
        // Check the time
        unsigned long currentMillis = millis() - startMillis;
        
        // ***** Check the inputs (start / stop / reset)
        
        // If time interval has been reached, drop into subloop.
        if(currentMillis - previousMillis > interval) {
            // save the last time updated
            previousMillis = (currentMillis / interval) * interval;
            // if less than 30 sec left, blink for half the interval
            if(secsLeft <= 30){
                blankAllShow();
                delay(interval / 2);
                updateDigits(fromSecs - (duration - secsLeft), color);
            }else{
                updateDigits(fromSecs - (duration - secsLeft), color);
            }
            // Increment time
            secsLeft = secsLeft - 1;
        }
    }
}

// Countdown function
void countup(int fromSecs, int duration, long interval, unsigned long color) {
    
    int secsLeft = duration; // how long this function will run
    unsigned long previousMillis = 0; // last time the display was updated
    unsigned long startMillis = millis(); // start at whatever time the function is called
    
    while(secsLeft >= 0) {
        // Process input from server
        //Spark.process();
        // Check the time
        unsigned long currentMillis = millis() - startMillis;
        
        // If time interval has been reached, drop into subloop.
        if(currentMillis - previousMillis > interval) {
            // save the last time updated
            previousMillis = (currentMillis / interval) * interval;
            // if less than 30 sec left, blink for half the interval
            if(secsLeft <= 30){
                blankAllShow();
                delay(interval / 2);
                updateDigits(fromSecs + (duration - secsLeft), color);
            }else{
                updateDigits(fromSecs + (duration - secsLeft), color);
            }
            // Increment time
            secsLeft = secsLeft - 1;
        }
    }
}

// Function to update digits
void updateDigits(int secsToDisplay, unsigned long color) {
    // Generate string from time
    char display[] = "0000"; // holds the current display
    sprintf (display, "%02d%02d", secsToDisplay / 60, secsToDisplay % 60);
        
    // Change display to new time
    blankAll();
    displayDigit(0, int(display[0]-'0'), color); // change string values to int
    displayDigit(14, int(display[1]-'0'), color);
    displayDigit(30, int(display[2]-'0'), color);
    displayDigit(44, int(display[3]-'0'), color);
    strip.setPixelColor(28, color); // turn on colon
    strip.setPixelColor(29, color); // turn on colon
    strip.show();
    
}

// Function to blank all pixels except colon, but does not show
void blankAll() {
    for(int pixel=0; pixel<strip.numPixels(); pixel++) {
        strip.setPixelColor(pixel, strip.Color(0,0,0));
    }
    //strip.setPixelColor(28, strip.Color(255,255,255));
    //strip.setPixelColor(29, strip.Color(255,255,255));
    //strip.show();
}

void blankAllShow() {
    blankAll();
    strip.show();
}

// ***** Add generalization including different pixels per segment
void displayDigit(int start_pixel, int digit, unsigned long color) {
    
    int zero[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int one[] = {2, 3, 4, 5};
    int two[] = {0, 1, 2, 3, 6, 7, 8, 9, 12, 13};
    int three[] = {0, 1, 2, 3, 4, 5, 6, 7, 12, 13};
    int four[] = {2, 3, 4, 5, 10, 11, 12, 13};
    int five[] = {0, 1, 4, 5, 6, 7, 10, 11, 12, 13};
    int six[] = {0, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    int seven[] = {0, 1, 2, 3, 4, 5};
    int eight[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    int nine[] = {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13};
    
    int g[] = {0, 1, 4, 5, 6, 7, 8, 9, 10, 11, 13};
    // ****** and so on
    
    int *digitArray; // Digit Array Pointer
    int size = 0;
    
    if(digit == 0) {digitArray = zero; size = sizeof(zero)/sizeof(zero[0]);}
    if(digit == 1) {digitArray = one; size = sizeof(one)/sizeof(one[0]);}
    if(digit == 2) {digitArray = two; size = sizeof(two)/sizeof(two[0]);}
    if(digit == 3) {digitArray = three; size = sizeof(three)/sizeof(three[0]);}
    if(digit == 4) {digitArray = four; size = sizeof(four)/sizeof(four[0]);}
    if(digit == 5) {digitArray = five; size = sizeof(five)/sizeof(five[0]);}
    if(digit == 6) {digitArray = six; size = sizeof(six)/sizeof(six[0]);}
    if(digit == 7) {digitArray = seven; size = sizeof(seven)/sizeof(seven[0]);}
    if(digit == 8) {digitArray = eight; size = sizeof(eight)/sizeof(eight[0]);}
    if(digit == 9) {digitArray = nine; size = sizeof(nine)/sizeof(nine[0]);}
    if(digit == 10) {digitArray = g; size = sizeof(g)/sizeof(g[0]);}
    
    // for(int pixel=0; pixel < sizeof(digitArray)/sizeof(digitArray[0]); pixel++) {
    for(int pixel=0; pixel < size; pixel++) {
        strip.setPixelColor(digitArray[pixel] + start_pixel, color);
    }
}






// ---------------------------TEST FUNCTIONS---------------------------------
// void testFunc() {
//     for(int i=0; i<10; i++) {
//       unsigned long color = strip.Color(255,0,0);
//       displayDigit(0,i, color);
//       displayDigit(14,i, color);
//       displayDigit(30,i, color);
//       displayDigit(44,i, color);
//       delay(250);
//     //   blankAll();
//     }
//     // rainbow(20);
// }

// void rainbow(uint8_t wait) {
//   uint16_t i, j;

//   for(j=0; j<256; j++) {
//     for(i=0; i<strip.numPixels(); i++) {
//       strip.setPixelColor(i, Wheel((i+j) & 255));
//     }
//     strip.show();
//     delay(wait);
//   }
// }

// // Input a value 0 to 255 to get a color value.
// // The colours are a transition r - g - b - back to r.
// uint32_t Wheel(byte WheelPos) {
//   if(WheelPos < 85) {
//    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//   } else if(WheelPos < 170) {
//    WheelPos -= 85;
//    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//   } else {
//    WheelPos -= 170;
//    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//   }
// }

// void dispZero() {
// // void DigitDisplay(uint8_t start_light, uint8_t lights_per_segment, uint8_t digit_to_display) {
//     // Take arbitrary position, lights per segment and return lights to light up for 0-9 on 7 segment display. All inputs as integers, return lights as list.

// // 	int lights_per_segment = 2;
// // 	int start_light = 0;
// 	int digit_to_display = 0;
	
// // 	int segment_arr[lights_per_segment * 7];
// //     int a[lights_per_segment];
// //     int b[lights_per_segment];
// //     int c[lights_per_segment];
// //     int d[lights_per_segment];
// //     int e[lights_per_segment];
// //     int f[lights_per_segment];
// //     int g[lights_per_segment];
    
// //     int i, j;
    
// //     for(i=start_light; i<(lights_per_segment + 1); i++) {
// //         a[i] = i + lights_per_segment * 0;
// //         b[i] = i + lights_per_segment * 1;
// //         c[i] = i + lights_per_segment * 2;
// //         d[i] = i + lights_per_segment * 3;
// //         e[i] = i + lights_per_segment * 4;
// //         f[i] = i + lights_per_segment * 5;
// //         g[i] = i + lights_per_segment * 6;
// //     }
    
// //     if (digit_to_display == 0) {
// //         for(j=0; j<(7*lights_per_segment); j++) {
// //             segment_arr[j] = a[j];
// //             printf("%d\n",segment_arr[j]);
// //         }
// //     }

//     int i;
//     segment_arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

//     if (digit_to_display == 0) {
//         segment_arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

//     }
    
//     for(i=0; i<strip.numPixels(); i++) {
//       strip.setPixelColor(i, strip.Color(0,0,0));
//     }
    
//     for(i=0, i<sizeof(segment_arr)/sizeof(int), i++) {
//         strip.setPixelColor(segment_arr[i], strip.Color(255, 255, 255));
//     }
//     strip.show();
//     delay(20);

// }

