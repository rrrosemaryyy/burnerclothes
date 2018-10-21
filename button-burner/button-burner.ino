/* MakeFashion Kit Button demo
   -------------------------------------------

   Requires:
   ---------
    MakeFashion controller board
    SeeedStudio Button ( http://wiki.seeedstudio.com/Grove-Button/ )
    FastLED-compatible LEDs ( https://github.com/FastLED/FastLED/wiki/Overview#chipsets )

   Dependencies:
   -------------
    FastLED ( Available via Arduino package manager )
*/

#include "patterns.h"

/*#define PIN_LEDARRAY     5*/
#define PIN_LEDARRAY1    5
#define PIN_LEDARRAY2    6
#define PIN_BUTTON       3

#define LED_TYPE           WS2812B
#define COLOR_ORDER        GRB
#define NUM_LEDS           24

#define FRAMES_PER_SECOND  30
#define TOGGLE_INTERVAL    2000 // milliseconds


typedef void (*SimplePatternList)(CRGB *, int);
SimplePatternList gPatterns[] = { fire, outrun, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, sinelon_red, \
                                            sinelon_blue, juggle_red, juggle_blue, sinelon_redblue, juggle_redblue
                                };

CRGB leds_array[NUM_LEDS];
unsigned int gArray_PatternNumber = 0;
/*unsigned int gHue = 0; // rotating "base color" used by many of the patterns
unsigned int gBlinky = 0;*/
bool ledpower;
bool state_toggle;
unsigned int state_now;
unsigned int state_last;
unsigned long time_start;
unsigned long time_interval;

void setup() {
  delay(3000); // 3 second delay for recovery

  Serial.begin(9600);

  ledpower = true;
  state_toggle = false;
  state_now = LOW;
  state_last = LOW;
  time_start = millis();
  time_interval = 0;

  //FastLED.addLeds<LED_TYPE, PIN_LEDARRAY, COLOR_ORDER>(leds_array, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LEDARRAY1, COLOR_ORDER>(leds_array, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LEDARRAY2, COLOR_ORDER>(leds_array, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  state_now = digitalRead(PIN_BUTTON);

  check_power_toggle();
  gPatterns[gArray_PatternNumber](leds_array, NUM_LEDS);

  if (ledpower) {
    FastLED.show();
  }
  else {
    FastLED.clear();
  }

  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
  }
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void check_power_toggle() {
  if (state_now != state_last) {
    if ( state_toggle) {
      state_toggle = false;
      nextPattern();
    } else {
      if ( state_now ) {
        state_toggle = true;
        time_start = millis();
      }
    }
  } else {
    if (state_toggle) {
      time_interval = millis() - time_start;
      if (time_interval > TOGGLE_INTERVAL) {
        ledpower = not ledpower;
        state_toggle = false;
        time_start = millis();
      }
    }
  }
  state_last = state_now;
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gArray_PatternNumber = (gArray_PatternNumber + 1) % ARRAY_SIZE(gPatterns);
}





