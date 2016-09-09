// MVP for scultpure 9/7

// MVP Functionality:
// If a facet is on, that ring turns blue (each ring is a different shade of blue)
// If a facet if off, that ring turns off
// If a facet is corr, that ring turns green (each ring is a different shade of green)
// If proximity is on (1 or 2 state), every facet turns candle at full brightness
// If user touches one facet, all facets (candle) for prox turn off
// Default state is "resting" which is candle at half brightness
// When proximity is off, AND all facets are off, resting state starts back up

#include <Adafruit_NeoPixel.h> //include adafruit neopixel library
#include "color_type.h"

#define NUM_PIXELS_IN_RING 24
#define PROXIMITY_FACET_NUMBER 6

//using these instead of #define PIN 6 //neopixels are on pin 6, neopixel library knows it's an output
int led0 = 7;
int led1 = 6;
int led2 = 5;
int led3 = 4;
int led4 = 3;
int led5 = 2;

//declares neopixel object
// # LEDs on ring, pin #, type of neopixels connected
Adafruit_NeoPixel rings[] = {
  Adafruit_NeoPixel(NUM_PIXELS_IN_RING, led0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS_IN_RING, led1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS_IN_RING, led2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS_IN_RING, led3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS_IN_RING, led4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS_IN_RING, led5, NEO_GRB + NEO_KHZ800)
};
int rings_length = 6;

color_type touch_colors[] = {
  {0, 0, 255},//blue 0,0,255: KEEP
  {0,50,255},//made up color 0,60,255
  {0,97,255},//made up: 0,97,255: KEEP
  {0, 127, 255},//don't know name-bluish aqua 0,127,255: KEEP
  {40,191,255},//made up 40,191,255
  {30,144,255}//dodger blue 30,144,255: KEEP
};
int touch_colors_length = 6;

color_type correspondence_colors[] = {
  {0, 255, 0},//lime: KEEP 0,255,0
  {0,128,0},//green: YES 0,128,0
  {0,100,0},//dark green: KEEP 0,100,0
  {20,255,40},//MADE IT UP: KEEP 20,255,40
  {50,205,20},//lime green: KEEP 50,205,20
  {34,139,34}//forest green: KEEP 34,139,34
};
int correspondence_colors_length = 6;

int facets[] = {0, 0, 0, 0, 0, 0};
int facets_length = 6;
boolean is_prox = false;
boolean is_resting = true;

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  // initialize the digital pins as outputs
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  for (int i = 0; i < rings_length; i++) {
    rings[i].begin(); // This initializes the NeoPixel library.
    rings[i].show(); // This sends the updated pixel color to the hardware.
    rings[i].clear(); // Initialize all pixels to 'off'
  }

  Serial.setTimeout(50);//because the arduino is probably left waiting without a newline, this will speed up the time before it gives up
}

void loop() {
  while (Serial.available() > 0) { //when you receive data execute the following animations

    int number = Serial.parseInt(); // declaring local variable for storing incoming serial data (sensor number)
    int state = Serial.parseInt(); // declaring local variable for storing incoming serial data (sensor state)
    // Using Serial.parseInt() to separate the data by commas, & read the information into my variables

    update_proximity(number, state);
    update_facets(number, state);
  } // end serial while() loop

  is_resting = calculate_resting();
  is_prox = calculate_proximity();

  // RESTING
  if (is_resting == true) {
    resting_lights(); // all facets to candle half brightness
  }

  // PROXIMITY
  if (is_prox == true) {
    proximity_lights();
  }

  facet_lights();
} // end void loop()

// Decide if lights should be resting
// Based on whether facets are on or proximity is on
// If both are off, it is resting
boolean calculate_resting() {
  boolean calculated_resting = true;
  for (int i = 0; i < facets_length; i++) {
    if ( (facets[i] == 1) || (facets[i] == 2) ) {
      calculated_resting = false;
    }
  }
  if (is_prox == true) {
    calculated_resting = false;
  }
  return calculated_resting;
}

boolean calculate_proximity() {
  boolean calculated_proximity = true;

  // if the proximity from controller is 0
  if (is_prox == false) {
    calculated_proximity = false;
  }

  // or, if any of the facets are on / correspondence
  for (int i = 0; i < facets_length; i++) {
    if ( (facets[i] == 1) || (facets[i] == 2) ) {
      calculated_proximity = false;
    }
  }
  return calculated_proximity;
}

void update_proximity(int facetNumber, int facetState) {
  if (facetNumber == PROXIMITY_FACET_NUMBER) {
    if (facetState == 0) {
      is_prox = false;
    } else {
      is_prox = true;
    }
  }
}

void update_facets(int facetNumber, int facetState) {
  if (facetNumber != PROXIMITY_FACET_NUMBER) {
    facets[facetNumber] = facetState;
  }
}

// LIGHTS FUNCTIONS
void resting_lights() {
  for (int r = 0; r < rings_length; r++) {
    for (int p = 0; p < NUM_PIXELS_IN_RING; p++) {
      // rings[r].Color takes RGB values, from 0,0,0 up to 255,255,255
      rings[r].setPixelColor(p, rings[r].Color(30, 30, 30));//all facets half brightness white
      //if using candle: 127, 73, 20
      rings[r].show(); // This sends the updated pixel color to the hardware.
    }
  }
}

void proximity_lights() {
  for (int r = 0; r < rings_length; r++) {
    for (int p = 0; p < NUM_PIXELS_IN_RING; p++) {
      // rings[r].Color takes RGB values, from 0,0,0 up to 255,255,255
      //      rings[r].setPixelColor(p, rings[r].Color(255, 147, 41)); // facets at full brightness white
      rings[r].setPixelColor(p, rings[r].Color(100, 149, 237));//corn flower blue: 100, 149, 237
      rings[r].show(); // This sends the updated pixel color to the hardware.
    }
  }
}

void facet_lights() {
  if (are_all_facets_off() == false) {
    for (int f = 0; f < facets_length; f++) {
      change_light(f);
    }
  }
}

boolean are_all_facets_off() {
  boolean facets_are_off = true;
  for (int i = 0; i < facets_length; i++) {
    if ( (facets[i] == 1) || (facets[i] == 2) ) {
      facets_are_off = false;
    }
  }
  return facets_are_off;  
}

void change_light(int ring_num) {
  if (facets[ring_num] == 0) {
    light_off(ring_num);
  } else if (facets[ring_num] == 1) {
    light_touch(ring_num);
  } else if (facets[ring_num] == 2) {
    light_correspondence(ring_num);
  }
}

void light_off(int ring_num) {
  rings[ring_num].clear();  // Initialize all pixels to 'off'on facet
  rings[ring_num].show();   // “push” the color data to the ring & updates the whole ring at once
}

void light_touch(int ring_num) {
  color_type c = touch_colors[ring_num];
  for (int p = 0; p < NUM_PIXELS_IN_RING; p++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    rings[ring_num].setPixelColor(p, rings[ring_num].Color(c.r, c.g, c.b));
    rings[ring_num].show(); // This sends the updated pixel color to the hardware.
  }
}

void light_correspondence(int ring_num) {
  color_type c = correspondence_colors[ring_num];
  for (int p = 0; p < NUM_PIXELS_IN_RING; p++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    rings[ring_num].setPixelColor(p, rings[ring_num].Color(c.r, c.g, c.b));
    rings[ring_num].show(); // This sends the updated pixel color to the hardware.
  }
}


