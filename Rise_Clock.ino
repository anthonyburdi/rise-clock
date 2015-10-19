// Rise_Clock.ino

// includes for realtime clock (RTC) code
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
// includes for Neopixel code
#include <Adafruit_NeoPixel.h>

// Neopixel initialization
#define PIXEL_PIN 6
#define PIXEL_COUNT 58
#define PIXEL_TYPE NEO_GRB + NEO_KHZ800
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// milliseconds, 1000 = one second (realtime). Reduce from 1000 to speed up testing.
const unsigned long timeInterval = 1000;

// Easy access colors & days of the week:
const unsigned long red = strip.Color(255,0,0), green = strip.Color(0,255,0), blue = strip.Color(0,0,255);
const unsigned long yellow = strip.Color(255,255,0), magenta = strip.Color(255,0,255), cyan = strip.Color(0,255,255);
const unsigned long white = strip.Color(255,255,255);
const int sun = 1, mon = 2, tue = 3, wed = 4, thu = 5, fri = 6, sat = 7;

// Function declarations
void setupPixels();

void setup() 
{  
  // timeWriteSetup(); // Set realtime clock from computer clock
  // serialSetup();
  setupPixels();
}

void loop() 
{
    timePrintToSerial();
    // read time from Real Time Clock (RTC) in tmElements_t form (tm) and unix time_t (t)
    // Elements: {tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tm.Year}
    tmElements_t tm;
    time_t t;
    if (RTC.read(tm)) {
        t = RTC.get();
        workoutChooser(tm, t); // check time and date, run all workouts
    } else {
        countup (1,90*60, timeInterval, red); // if RTC is misbehaving just count up from red for 90 min
    }
    timePrintToSerial();
    delay(500);
}

// ------------------------------------ WORKOUTS ------------------------------------

void workoutChooser(tmElements_t tm, time_t t) {

    // ********** TESTING
    if (weekday(t) == tue && tm.Hour == 16 && tm.Minute < 25) {
        preWorkoutCountdown( 16, 25, 30, tm);
//        mondayTrack();
    } else if (weekday(t) == mon && tm.Hour >= 17) {
        countdown(10,10, timeInterval, red);
        mondayTrack();
    } else if (weekday(t) == wed && tm.Hour == 10 && tm.Minute < 30) {
        preWorkoutCountdown(10, 30, 30, tm);
    } else if (weekday(t) == wed && tm.Hour == 10 && tm.Minute >= 30) {
        mondayTrack();

    // ********** TESTING
    
    } else if (weekday(t) == wed && tm.Hour == 6 && tm.Minute < 30) {
        preWorkoutCountdown(6, 30 , 30, tm);
    } else if (weekday(t) == wed && tm.Hour == 6 && tm.Minute >= 30) {
        wednesdayWorkout(30,timeInterval,5,10,5,10,0,0);
        endWorkout(150); // turn on all nodes red

    } else if (weekday(t) == fri && tm.Hour == 6 && tm.Minute > 30) {
        fridayHillSprints();

    } else {
        Serial.println("No workouts scheduled for now ");
        //updateDigits(tm.Hour*60 + tm.Minute, green); // display the time in HH:MM
        countdown(10,10, timeInterval, red);
        mondayTrack();
    }
}

void mondayTrack() {
    // Show the word "GO"
    dispGo(3);
    // Mon Night Track CountUp
    countup(4,120*60, timeInterval, green);
    endWorkout(150); // turn on all nodes red
}

void mondayHIIT() {
    // Show the word "GO"
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 8; i++) {
        dispGo(3);
        countdown(16,16,timeInterval, green);
        dispStop(3);
        countdown(6,6, timeInterval, red);        
        }
        dispStop(10);
        countdown(35,35, timeInterval, red);
    }
}

void wednesdayWorkout(int startMin, int timeInterval, int a, int b, int c, int d, int e, int f) {
    // Wed Workout countdown
    countdown(startMin*60, a*60, timeInterval, green);
    countdown((startMin-a)*60, b*60, timeInterval, blue);
    countdown((startMin-a-b)*60, c*60, timeInterval, yellow);
    countdown((startMin-a-b-c)*60, d*60, timeInterval, red);
    countdown((startMin-a-b-c-d)*60, e*60, timeInterval, cyan);
    countdown((startMin-a-b-c-d-e)*60, f*60, timeInterval, magenta);
}

void fridayHillSprints() {
    for (int i = 0; i<10; i++) {
        countdown(45, 45, timeInterval, green);
        countdown(60+45, 60+45, timeInterval, red);
    }
    endWorkout(30);
}

void fridayCountdown() {
    countdown(30*60, 30*60, timeInterval, green);
    endWorkout(30);
}
// ------------------------------------ WORKOUTS ------------------------------------

// ------------------------------------ WORKOUT BUILDING BLOCKS ------------------------------------
void preWorkoutCountdown(int workoutStartHour, int workoutStartMin, int minutesToCountdown, tmElements_t current_tm) {
    // from minutesToStart before workoutStartHour:workoutStartMin begin counting down in red until the start time
    int currSec = convertToSec(current_tm);
    int workoutStartSec = workoutStartHour * 3600 + workoutStartMin * 60;
    int countdownStartSec = workoutStartSec - minutesToCountdown * 60;

    if ( currSec < workoutStartSec && currSec > countdownStartSec ) {
        updateDigits( workoutStartSec - currSec , red );
    }
}

int convertToSec(tmElements_t tm) {
    return tm.Hour*3600 + tm.Minute*60 + tm.Second;
}

void endWorkout(int endTime) {
    blankAll();
    displayDigit(0,8,red);
    displayDigit(14,8,red);
    displayDigit(30,8,red);
    displayDigit(44,8,red);
    strip.show();
    delay(endTime*1000);
}

void blinkTime(int secsToDisplay, unsigned long color) {
    // captured in countdown(), may not need separate function
}

int countUpFromTo(int fromSecs, int duration, unsigned long color) {
    // Calculate the remaining time using timeElapsed() and duration. fromSecs is measured from startSecs.
    // Return the time in seconds that should be displayed
    return 0;
}

int countdownFromTo(int fromSecs, int duration, unsigned long color) {
    // Calculate the remaining time using timeElapsed() and duration. fromSecs is measured from startSecs.
    // Return the time in seconds that should be displayed
    return 0;
}

int timeElapsed(int startSecs) {
    // take the time measured from midnight in seconds and return the time elapsed in seconds from startSecs
    return 0;
}

// Countdown function
void countdown(int fromSecs, int duration, long timeInterval, unsigned long color) {
    
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
        if(currentMillis - previousMillis > timeInterval) {
            // save the last time updated
            previousMillis = (currentMillis / timeInterval) * timeInterval;
            // if less than 30 sec left, blink for half the interval
            if(secsLeft <= 30){
                blankAllShow();
                delay(timeInterval / 2);
                updateDigits(fromSecs - (duration - secsLeft), color);
            }else{
                updateDigits(fromSecs - (duration - secsLeft), color);
            }
            // Increment time
            secsLeft = secsLeft - 1;
        }
    }
}

// Countup function
void countup(int fromSecs, int duration, long timeInterval, unsigned long color) {
    
    int secsLeft = duration; // how long this function will run
    unsigned long previousMillis = 0; // last time the display was updated
    unsigned long startMillis = millis(); // start at whatever time the function is called
    
    while(secsLeft >= 0) {
        // Process input from server
        //Spark.process();
        // Check the time
        unsigned long currentMillis = millis() - startMillis;
        
        // If time interval has been reached, drop into subloop.
        if(currentMillis - previousMillis > timeInterval) {
            // save the last time updated
            previousMillis = (currentMillis / timeInterval) * timeInterval;
            // if less than 30 sec left, blink for half the interval
            if(secsLeft <= 30){
                blankAllShow();
                delay(timeInterval / 2);
                updateDigits(fromSecs + (duration - secsLeft), color);
            }else{
                updateDigits(fromSecs + (duration - secsLeft), color);
            }
            // Increment time
            secsLeft = secsLeft - 1;
        }
    }
}
// ------------------------------------ WORKOUT BUILDING BLOCKS ------------------------------------

// ------------------------------------ DISPLAY FUNCTIONS ------------------------------------
// Function to blank all pixels, but does not show
void blankAll() {
    for(int pixel=0; pixel<strip.numPixels(); pixel++) {
        strip.setPixelColor(pixel, strip.Color(0,0,0));
    }
}

void blankAllShow() {
    blankAll();
    strip.show();
}

void setupPixels() {
    strip.begin(); // Intialize pixels
    strip.show(); // Initialize all pixels to 'off'
}

void blinkColon() {

}

void dispGo(int secs) {
    // Show the word "GO" for int seconds preceded by blanking, center two digits, no colon
   blankAll();
   displayDigit(14, 10, green); // "G"
   displayDigit(30, 0, green); // "O"
   strip.show();
   delay(secs * 1000);
}

void dispStop(int secs) {
    // Show the word "STOP" for int seconds preceded by blanking. No colon. Rendered with best available
    blankAll();
    displayDigit(0, 5, red); // "S"
    displayDigit(14, 7, red); // "T"
    displayDigit(30, 0, red); // "O"
    displayDigit(44, 11, red); // "P"
    strip.show();
    delay(secs * 1000);
}

// Function to update digits
void updateDigits(int secsToDisplay, unsigned long color) {
    // Generate string from time
    char display[] = "0000"; // holds the current display
    sprintf (display, "%02d%02d", secsToDisplay / 60, secsToDisplay % 60); // make each two-digits
        
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

// TODO ***** Add generalization including different pixels per segment
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
    int p[] = {0, 1, 2, 3, 8, 9, 10, 11, 12, 13};
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
    if(digit == 11) {digitArray = p; size = sizeof(p)/sizeof(p[0]);}
    
    // for(int pixel=0; pixel < sizeof(digitArray)/sizeof(digitArray[0]); pixel++) {
    for(int pixel=0; pixel < size; pixel++) {
        strip.setPixelColor(digitArray[pixel] + start_pixel, color);
    }
}
// ------------------------------------ DISPLAY FUNCTIONS ------------------------------------

// ------------------------------------ TIME READ ------------------------------------
void serialSetup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("Serial output initialized");
}

void timePrintToSerial() {
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

  
//struct * getTime() {
//    tmElements_t tm;
//    time_t t;
//    if (RTC.read(tm)) {
//        // Sunday is day 1 in weekday()
//        t = RTC.get();
//        // Serial.print("Test print day of week:");
//        // Serial.println(weekday(t));
//        int a = weekday(RTC.get());
//        Serial.print("Test print day of week:");
//        Serial.println(a);
//
//        return tm;
//
//        // static int t[] = {tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tm.Year, weekday(RTC.get())};
//        // return t;
//    } else {
//    if (RTC.chipPresent()) {
//      Serial.println("The DS1307 is stopped.  Please run the SetTime");
//      Serial.println("example to initialize the time and begin running.");
//      Serial.println();
//    } else {
//      Serial.println("DS1307 read error!  Please check the circuitry.");
//      Serial.println();
//    }
//  }
//}

// int * getTime() {
//     tmElements_t tm;
//     time_t t;
//     if (RTC.read(tm)) {
//         // Sunday is day 1 in weekday()
//         t = RTC.get();
//         // Serial.print("Test print day of week:");
//         // Serial.println(weekday(t));
//         int a = weekday(RTC.get());
//         Serial.print("Test print day of week:");
//         Serial.println(a);

//         static int t[] = {tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tm.Year, weekday(RTC.get())};
//         return t;
//     } else {
//     if (RTC.chipPresent()) {
//       Serial.println("The DS1307 is stopped.  Please run the SetTime");
//       Serial.println("example to initialize the time and begin running.");
//       Serial.println();
//     } else {
//       Serial.println("DS1307 read error!  Please check the circuitry.");
//       Serial.println();
//     }
//   }
// }
// ------------------------------------ TIME READ ------------------------------------

// ------------------------------------ TIME SET ------------------------------------
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

// ------------------------------------ TIME SET ------------------------------------
