void ledTrakOpozorilo() {
  ledIndex++; /* motion speed */
  if (ledIndex > NUM_LEDS)ledIndex = 0;

  int a   = round(400 / (ledIndex + 5.5) - 11); //= round(200 / (ledIndex + 6) - 5); // -> 30 ledic
  int pos = NUM_LEDS - a;

  for (int i = 0; i < NUM_LEDS; i++) {
    long c = 255 - (pos - i) * pos * 2;

    if(c < 0) c = 0;

    leds[i] = c << 16; // red: c << 16, green: c << 8, blue: c
    
    if (i > pos) leds[i] = 0;
  }
  FastLED.show();
}
