// =========================================================================
// ||                          TOOLS - DISPLAY                            ||
// =========================================================================
void printSelected(uint8_t p1, uint8_t p2) {
  if (p1 == p2) {
    display.print(F("--> "));

  } else {
    display.print(F("   "));
  }
}

void clearScreen(void) {
  //for (uint8_t i = 0; i < 100; i++) { display.print(); }
  display.clearWindow(0, 0, 96, 64);
}

void printDivider(void) {
  for (uint8_t i = 0; i < 40; i++) { display.print("-"); }
}

void printDateTime(void) {
  display.clearWindow(20, 45, 50, 16);
  //display.setFont(liberationSansNarrow_12ptFontInfo);   //Set the font type

  while (true) {
    months = rtc.getMonth();
    days = rtc.getDay();
    years = rtc.getYear();
    hours = rtc.getHours();
    minutes = rtc.getMinutes();
    seconds = rtc.getSeconds();

    // Print date in US format MM:DD:YY (Switch the order in which day, month, year that you like to use)
    display.setCursor(15, 8);  //Set the cursor where you want to start printing the date
    if (months < 10) { display.print(0); }
    display.print(months);
    display.print("/");
    if (days < 10) { display.print(0); }
    display.print(days);
    display.print("/");
    display.print(years);

    display.setCursor(30, 25);  //Set the cursor where you want to start printing the date
    setTime(hours, minutes, seconds, days, months, 2000 + years);
    char wkday[16];
    strcpy(wkday, dayStr(weekday()));
    wkday[3] = ' ';
    wkday[4] = '\0';
    display.print(wkday);

    //display.setFont(liberationSansNarrow_16ptFontInfo);   //Set the font type

    // display time in HH:MM:SS 24 hour format
    display.setCursor(20, 45);          //Set the cursor where you want to start printing the time
    if (hour() < 10) display.print(0);  //print a leading 0 if hour value is less than 0
    display.print(hours);
    display.print(":");
    if (minute() < 10) display.print(0);  //print a leading 0 if minute value is less than 0
    display.print(minutes);
    display.print(":");
    if (second() < 10) display.print(0);  //print a leading 0 if seconds value is less than 0
    display.print(seconds);
    display.print(" ");  //just a empty space after the seconds
    //delay(1000); //delay 60 second
  }

  
}

void printBtnArrows(void) {
  leftArrow(0, 15 + 2);
  rightArrow(0, 45 + 5);

  upArrow(90, 15 + 2);
  downArrow(90, 45 + 4);
}


// =========================================================================
// ||                          TOOLS - BUTTON PRESSING                     ||
// =========================================================================

boolean btnIsDown(int btn) {
  //return digitalRead(btn) == LOW && digitalRead(btn) == LOW;
  return (display.getButtons() & btn);
}

boolean btnIsUp(int btn) {
  //return digitalRead(btn) == HIGH && digitalRead(btn) == HIGH;
  return !(display.getButtons() & btn);
}
// =========================================================================
// ||                                 ARROWS                              ||
// =========================================================================
// Arrows for scroll menu after clicking into the menu button
void draw_Arrows(void) {
  leftArrow(0, 15 + 2);
  rightArrow(0, 45 + 5);

  upArrow(90, 15 + 2);
  downArrow(90, 45 + 4);
}

void leftArrow(int x, int y) {
  display.drawLine(x + 2, y - 2, x + 2, y + 2, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
  display.drawLine(x + 0, y - 0, x + 0, y + 0, 0xFFFF);
}

void rightArrow(int x, int y) {
  display.drawLine(x + 0, y - 2, x + 0, y + 2, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
  display.drawLine(x + 2, y - 0, x + 2, y + 0, 0xFFFF);
}

void upArrow(int x, int y) {
  display.drawLine(x + 0, y - 0, x + 4, y - 0, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 3, y - 1, 0xFFFF);
  display.drawLine(x + 2, y - 2, x + 2, y - 2, 0xFFFF);
}
void downArrow(int x, int y) {
  display.drawLine(x + 0, y + 0, x + 4, y + 0, 0xFFFF);
  display.drawLine(x + 1, y + 1, x + 3, y + 1, 0xFFFF);
  display.drawLine(x + 2, y + 2, x + 2, y + 2, 0xFFFF);
}
// =========================================================================
// ||                          ALARM DISPLAY                              ||
// =========================================================================
void drawCircle(int x0, int y0, int radius, uint8_t color)
{
  int x = radius;
  int y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    //drawPixel(x,y,color);//set pixel (x,y) to specified color. This is slow because we need to send commands setting the x and y, then send the pixel data.
    display.drawPixel(x + x0, y + y0, color);
    display.drawPixel(y + x0, x + y0, color);
    display.drawPixel(-x + x0, y + y0, color);
    display.drawPixel(-y + x0, x + y0, color);
    display.drawPixel(-x + x0, -y + y0, color);
    display.drawPixel(-y + x0, -x + y0, color);
    display.drawPixel(x + x0, -y + y0, color);
    display.drawPixel(y + x0, -x + y0, color);
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x) + 1;
    }
  }
}

void drawHand(int xStart, int yStart, int angle, int radius, unsigned char color)

{
  static int sine[16] = {0, 27, 54, 79, 104, 128, 150, 171, 190, 201, 221, 233, 243, 250, 254, 255};
  int xEnd, yEnd, quadrant, x_flip, y_flip;

  // calculate which quadrant the hand lies in
  quadrant = angle/15 ;

  switch ( quadrant )
  {
    case 0 : x_flip = 1 ; y_flip = -1 ; break ;
    case 1 : angle = abs(angle-30) ; x_flip = y_flip = 1 ; break ;
    case 2 : angle = angle-30 ; x_flip = -1 ; y_flip = 1 ; break ;
    case 3 : angle = abs(angle-60) ; x_flip = y_flip = -1 ; break ; 
    default:  x_flip = y_flip = 1; // this should not happen
  }

  xEnd = xStart;
  yEnd = yStart;
  xEnd += (x_flip * (( sine[angle] * radius ) >> 8));
  yEnd += (y_flip * (( sine[15-angle] * radius ) >> 8));

  display.drawLine(xStart, yStart, xEnd, yEnd, color);
}
