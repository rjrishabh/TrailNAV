#include <GxEPD2_BW.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>


#include <Wire.h>
#include <math.h>
#include <DFRobot_QMC5883.h>
#include <TinyGPSPlus.h>

const unsigned char epd_bitmap_locations[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xf8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x3e,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0xfc, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc1, 0xfe, 0x1e,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0xcf, 0x0f,
  0x00, 0x00, 0x00, 0x00, 0x03, 0x87, 0x87, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x03, 0x87, 0x87, 0x0f,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0xcf, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0xff, 0x0f,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc1, 0xfc, 0x1e,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x78, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x3e,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x7c,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0xf0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x03, 0xe0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x87, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xcf, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xc1, 0xfc, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x1f, 0xe0, 0xf8, 0x00,
  0x01, 0xff, 0xe0, 0x00, 0x7f, 0xe0, 0x30, 0x00, 0x03, 0xff, 0xf8, 0x00, 0xff, 0xc0, 0x00, 0x00,
  0x0f, 0xff, 0xfc, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x7e, 0x01, 0xf0, 0x00, 0x00, 0x00,
  0x1f, 0x00, 0x3f, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x0f, 0x03, 0xc0, 0x00, 0x00, 0x00,
  0x7c, 0x00, 0x07, 0x83, 0xc0, 0x00, 0x00, 0x00, 0x78, 0x3f, 0x07, 0x83, 0xc0, 0x00, 0x00, 0x00,
  0x78, 0x7f, 0x83, 0xc3, 0xc0, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xc3, 0xc3, 0xc0, 0x00, 0x00, 0x00,
  0xf0, 0xfb, 0xe3, 0xc1, 0xe0, 0x00, 0x00, 0x00, 0xf0, 0xe1, 0xe1, 0xc1, 0xf0, 0x00, 0x00, 0x00,
  0xf0, 0xe1, 0xe1, 0xc0, 0xf8, 0x00, 0x00, 0x00, 0xf0, 0xf3, 0xe3, 0xc0, 0xff, 0xff, 0xff, 0x80,
  0xf0, 0xff, 0xc3, 0xc0, 0x7f, 0xff, 0xff, 0xe0, 0xf0, 0x7f, 0xc3, 0xc0, 0x1f, 0xff, 0xff, 0xf8,
  0x78, 0x3f, 0x83, 0xc0, 0x07, 0xff, 0xff, 0xfc, 0x78, 0x1e, 0x07, 0x80, 0x00, 0x00, 0x00, 0x7c,
  0x7c, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x3e, 0x3c, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1e,
  0x3e, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1f, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x0f,
  0x0f, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x80, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x0f,
  0x07, 0xc0, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x03, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1e,
  0x01, 0xf3, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7e,
  0x00, 0x7f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0x81, 0xff, 0xff, 0xff, 0xff, 0xf8,
  0x00, 0x1f, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0c, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 528)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
  epd_bitmap_locations
};

#define ENABLE_GxEPD2_GFX 0

#define GxEPD2_DISPLAY_CLASS GxEPD2_BW
#define GxEPD2_DRIVER_CLASS GxEPD2_270_GDEY027T91  // GDEY027T91 176x264, SSD1680, (FB)

// somehow there should be an easier way to do this
#define GxEPD2_BW_IS_GxEPD2_BW true
#define GxEPD2_3C_IS_GxEPD2_3C false
#define GxEPD2_7C_IS_GxEPD2_7C false
#define GxEPD2_1248_IS_GxEPD2_1248 false
#define IS_GxEPD(c, x) (c##x)
#define IS_GxEPD2_BW(x) IS_GxEPD(GxEPD2_BW_IS_, x)
#define IS_GxEPD2_3C(x) IS_GxEPD(GxEPD2_3C_IS_, x)
#define IS_GxEPD2_7C(x) IS_GxEPD(GxEPD2_7C_IS_, x)
#define IS_GxEPD2_1248(x) IS_GxEPD(GxEPD2_1248_IS_, x)

#if defined(ESP32)
#define MAX_DISPLAY_BUFFER_SIZE 65536ul  // e.g.
#if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#elif IS_GxEPD2_3C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#elif IS_GxEPD2_7C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2))
#endif
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=*/21, /*DC=*/22, /*RST=*/23, /*BUSY=*/2));
#endif

DFRobot_QMC5883 compass(&Wire, /*I2C addr*/ QMC5883_ADDRESS);

#define BTN_LEFT 6
#define BTN_RIGHT 5
#define BTN_OK 7

float currentLat = 0;
float currentLon = 0;

float currentSpeed = 0;
float currentAlt = 0;

float currentHDOP = 0;

int currentSats = 0;
float currentCourse = 0;
// compass
float currentHeading = 0;

int gpsHour = 0;
int gpsMinute = 0;

int gpsDay = 0;
int gpsMonth = 0;
int gpsYear = 0;

// GPS status
bool gpsValid = false;
float navStartLat = 0;
float navStartLon = 0;

bool navStarted = false;
enum ScreenMode {
  SCREEN_STATUS,
  SCREEN_DESTINATIONS,
  SCREEN_COMPASS,
  SCREEN_NAVIGATION
};

ScreenMode currentScreen = SCREEN_STATUS;

float destLat = 28.4325;
float destLon = 77.4785;

String targetName = "BASE CAMP";

float targetDistance = 0;
float targetBearing = 0;

String navInstruction = "GO STRAIGHT";

int selectedDestination = 0;

const int totalDestinations = 5;

String destinations[5] = {
  "BASE CAMP",
  "WATER POINT",
  "CAR PARK",
  "HOME",
  "EXIT TRAIL"
};
String etaText = "--";
bool lastLeftState = HIGH;
bool lastRightState = HIGH;
bool lastOkState = HIGH;

TinyGPSPlus gps;
void setup() {
  Serial.begin(115200);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  Serial1.begin(9600, SERIAL_8N1, 17, 16);

  Wire.begin(0, 1);  //sda scl

  while (!compass.begin()) {
    Serial.println("Could not find a valid 5883 sensor, check wiring!");
    delay(500);
  }
  float declinationAngle = 1.33 * PI / 180.0;
  compass.setDeclinationAngle(declinationAngle);
  delay(1000);

  SPI.begin(
    19,  // SCK
    -1,  // MISO not used
    18,  // MOSI
    21   // CS
  );

  display.epd2.selectSPI(
    SPI,
    SPISettings(
      4000000,
      MSBFIRST,
      SPI_MODE0));
  delay(1000);
  display.init(115200);
  // display.init();
  display.setRotation(0);
  display.setTextColor(GxEPD_BLACK);
  drawWelcomeScreen();

  delay(1500);
  // drawStatusScreen();
  loadCurrentScreen();
}

unsigned long lastUIUpdate = 0;

const int uiInterval = 1000;
unsigned long lastTopBarUpdate = 0;
unsigned long lastStatusUpdate = 0;
unsigned long lastHeadingUpdate = 0;
unsigned long lastNavUpdate = 0;
void loop() {
  // =====================================
  // ALWAYS FAST
  // =====================================

  handleButtons();

  updateGPS();

  updateCompass();

  // =====================================
  // TOP BAR
  // every 60 sec
  // =====================================

  if (millis() - lastTopBarUpdate > 60000) {
    lastTopBarUpdate = millis();

    updateTopBar();
  }

  // =====================================
  // STATUS VALUES
  // every 3 sec
  // =====================================

  if (currentScreen == SCREEN_STATUS) {
    if (millis() - lastStatusUpdate > 3000) {
      lastStatusUpdate = millis();

      updateStatusValues();
    }
  }

  // =====================================
  // COMPASS
  // faster update
  // =====================================

  if (currentScreen == SCREEN_COMPASS) {
    if (millis() - lastHeadingUpdate > 300) {
      Serial.println("needleupdate!");

      lastHeadingUpdate = millis();
      updateCompassUI(currentHeading);
    }
  }

  if (currentScreen == SCREEN_NAVIGATION) {
    if (millis() - lastNavUpdate > 1000) {
      lastNavUpdate = millis();
    updateNavigation(); 

    updateNavigationUI();

    updateMapUI();
    }
  }
}

void updateGPS() {
  while (Serial1.available()) {
    gps.encode(Serial1.read());
  }

  if (gps.location.isUpdated()) {
    gpsValid = gps.location.isValid();

    currentLat = gps.location.lat();

    currentLon = gps.location.lng();

    currentSats = gps.satellites.value();

    currentHDOP = gps.hdop.hdop();

    currentAlt = gps.altitude.meters();

    currentSpeed = gps.speed.kmph();

    // remove GPS drift
    if (currentSpeed < 1)
      currentSpeed = 0;

    if (currentSpeed > 1) {
      currentCourse = gps.course.deg();
    }
    // TIME
    gpsHour = gps.time.hour();
    gpsMinute = gps.time.minute();

    gpsMinute += 30;
    if (gpsMinute >= 60) {
      gpsMinute -= 60;
      gpsHour++;
    }

    gpsHour += 5;
    if (gpsHour >= 24) {
      gpsHour -= 24;
    }
    // DATE
    gpsDay = gps.date.day();
    gpsMonth = gps.date.month();
    gpsYear = gps.date.year();
  }
}

void updateCompass() {
  sVector_t mag = compass.readRaw();

  compass.getHeadingDegrees();

  currentHeading = mag.HeadingDegress;

  // fix reverse orientation
  currentHeading += 180;

  if (currentHeading >= 360)
    currentHeading -= 360;
}

String getDirection(float deg) {
  if (deg >= 337.5 || deg < 22.5) return "N";
  if (deg < 67.5) return "NE";
  if (deg < 112.5) return "E";
  if (deg < 157.5) return "SE";
  if (deg < 202.5) return "S";
  if (deg < 247.5) return "SW";
  if (deg < 292.5) return "W";

  return "NW";
}

void drawTopBar() {
  display.setFont();

  char timeBuf[10];

  sprintf(
    timeBuf,
    "%02d:%02d",
    gpsHour,
    gpsMinute);

  display.setCursor(5, 4);

  display.print(timeBuf);

  char dateBuf[20];

  sprintf(
    dateBuf,
    "%02d/%02d/%04d",
    gpsDay,
    gpsMonth, gpsYear);

  display.setCursor(60, 4);

  display.print(dateBuf);

  display.setCursor(150, 4);

  display.print("100%");

  // =====================================
  // TOP LINE
  // =====================================

  display.drawLine(
    0,
    15,
    264,
    15,
    GxEPD_BLACK);
}

void updateTopBar() {
  // =====================================
  // PARTIAL TOP AREA
  // =====================================

  display.setPartialWindow(
    0,
    0,
    264,
    15);

  display.firstPage();

  do {
    display.fillRect(
      0,
      0,
      264,
      15,
      GxEPD_WHITE);

    display.setFont();
    char timeBuf[10];

    sprintf(
      timeBuf,
      "%02d:%02d",
      gpsHour,
      gpsMinute);

    display.setCursor(5, 4);

    display.print(timeBuf);
    char dateBuf[20];

    sprintf(
      dateBuf,
      "%02d/%02d/%04d",
      gpsDay,
      gpsMonth, gpsYear);

    display.setCursor(60, 4);

    display.print(dateBuf);

    display.setCursor(150, 4);
    display.print("100%");

    display.drawLine(
      0,
      15,
      264,
      15,
      GxEPD_BLACK);

  } while (display.nextPage());
}


void drawStatusScreen() {
  display.setFullWindow();

  display.firstPage();

  do {

    display.fillScreen(GxEPD_WHITE);

    // =====================================
    // TOP BAR
    // =====================================

    display.setFont();
    drawTopBar();
    // display.setCursor(5,2);
    // display.print("12:45");

    // display.setCursor(150,2);
    // display.print("100%");

    // display.drawLine(0,15,264,15,GxEPD_BLACK);

    // =====================================
    // MODE
    // =====================================

    display.setCursor(5, 22);
    display.print("MODE: STATUS");

    display.drawLine(0, 35, 264, 35, GxEPD_BLACK);

    // =====================================
    // DATA
    // =====================================

    display.setFont(&FreeSerifBold9pt7b);

    display.setCursor(5, 60);

    if (gpsValid)
      display.print("GPS : LOCKED");
    else
      display.print("GPS : SEARCH");

    display.setCursor(5, 85);
    display.print("SATS: ");
    display.print(currentSats);

    display.setCursor(5, 110);
    display.print("HDOP: ");
    display.print(currentHDOP, 1);

    display.setCursor(5, 135);
    display.print("LAT : ");
    display.print(currentLat, 4);

    display.setCursor(5, 160);
    display.print("LON : ");
    display.print(currentLon, 4);

    display.setCursor(5, 185);
    display.print("SPD : ");
    display.print(currentSpeed, 1);
    display.print(" km/h");

    display.setCursor(5, 210);
    display.print("ALT : ");
    display.print(currentAlt, 0);
    display.print(" m");

    display.setCursor(5, 235);

    display.print("CRS : ");
    display.print(currentCourse, 0);

    // =====================================
    // BOTTOM BAR
    // =====================================

    // display.drawLine(0,240,264,240,GxEPD_BLACK);

    display.setCursor(5, 257);

    display.print("HEAD: ");

    display.print((int)currentHeading);

    //display.print((char)247);

    display.print(" ");

    display.print(getDirection(currentHeading));

  } while (display.nextPage());
}

void updateStatusValues()
{
  display.setPartialWindow(
    0,
    40,
    264,
    224
  );

  display.firstPage();

  do {

    display.setFont(
      &FreeSerifBold9pt7b
    );

    // =====================================
    // GPS STATUS
    // =====================================

    display.fillRect(
      0,
      45,
      220,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,60);

    if(gpsValid)
      display.print("GPS : LOCKED");
    else
      display.print("GPS : SEARCH");

    // =====================================
    // SATS
    // =====================================

    display.fillRect(
      0,
      70,
      220,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,85);

    display.print("SATS: ");

    display.print(currentSats);

    // =====================================
    // HDOP
    // =====================================

    display.fillRect(
      0,
      95,
      220,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,110);

    display.print("HDOP: ");

    display.print(currentHDOP,1);

    // =====================================
    // LAT
    // =====================================

    display.fillRect(
      0,
      120,
      260,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,135);

    display.print("LAT : ");

    display.print(currentLat,4);

    // =====================================
    // LON
    // =====================================

    display.fillRect(
      0,
      145,
      260,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,160);

    display.print("LON : ");

    display.print(currentLon,4);

    // =====================================
    // SPEED
    // =====================================

    display.fillRect(
      0,
      170,
      240,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,185);

    display.print("SPD : ");

    display.print(currentSpeed,1);

    display.print(" km/h");

    // =====================================
    // ALTITUDE
    // =====================================

    display.fillRect(
      0,
      195,
      240,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,210);

    display.print("ALT : ");

    display.print(currentAlt,0);

    display.print(" m");

    // =====================================
    // COURSE
    // =====================================

    display.fillRect(
      0,
      220,
      260,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,235);

    display.print("CRS : ");

    display.print(
      (int)currentCourse
    );

    display.print((char)247);

    display.print(" ");

    display.print(
      getDirection(
        currentCourse
      )
    );

    // =====================================
    // HEADING
    // =====================================

    display.fillRect(
      0,
      242,
      260,
      18,
      GxEPD_WHITE
    );

    display.setCursor(5,257);

    display.print("HEAD: ");

    display.print(
      (int)currentHeading
    );

    display.print((char)247);

    display.print(" ");

    display.print(
      getDirection(
        currentHeading
      )
    );

  } while(display.nextPage());
}

void drawWelcomeScreen() {
  display.setFullWindow();

  display.firstPage();

  do {

    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(
      55,  // x
      55,  // y
      epd_bitmap_locations,
      64,  // width
      64,  // height
      GxEPD_BLACK);

    display.setFont(&FreeSerifBold9pt7b);

    display.setCursor(42, 150);
    display.print("TRAILNAV");
    display.setFont();
    display.drawRect(30, 200, 115, 10, GxEPD_BLACK);

    display.fillRect(
      33,   // x
      203,  // y
      30,   // width
      4,    // height
      GxEPD_BLACK);
  } while (display.nextPage());

  delay(500);

  display.setPartialWindow(33, 203, 109, 4);

  display.firstPage();

  do {

    display.fillRect(
      33,
      203,
      65,
      4,
      GxEPD_BLACK);
  } while (display.nextPage());

  delay(500);

  display.firstPage();

  do {

    display.fillRect(
      33,
      203,
      108,
      4,
      GxEPD_BLACK);

  } while (display.nextPage());

  delay(500);
}

void drawSavedDestinationsScreen() {
  display.setFullWindow();

  display.firstPage();

  do {

    display.fillScreen(GxEPD_WHITE);

    drawTopBar();

    display.setFont();

    display.setCursor(5, 22);

    display.print("MODE: DESTINATIONS");

    display.drawLine(0, 35, 264, 35, GxEPD_BLACK);

    display.setFont(&FreeSerifBold9pt7b);

    int y = 70;

    for (int i = 0;
         i < totalDestinations;
         i++) {
      display.setCursor(20, y);

      if (i == selectedDestination)
        display.print("> ");
      else
        display.print("  ");

      display.print(destinations[i]);

      y += 35;
    }

  } while (display.nextPage());
}

void updateDestinationMenu() {
  display.setPartialWindow(
    0,
    40,
    264,
    180);

  display.firstPage();

  do {

    display.fillRect(
      0,
      40,
      264,
      180,
      GxEPD_WHITE);

    display.setFont(&FreeSerifBold9pt7b);

    int y = 70;

    for (int i = 0;
         i < totalDestinations;
         i++) {
      display.setCursor(20, y);

      if (i == selectedDestination)
        display.print("> ");
      else
        display.print("  ");

      display.print(destinations[i]);

      y += 35;
    }

  } while (display.nextPage());
}

void loadCurrentScreen() {

  switch (currentScreen) {
    case SCREEN_STATUS:
      drawStatusScreen();
      break;

    case SCREEN_DESTINATIONS:
      drawSavedDestinationsScreen();
      break;

    case SCREEN_COMPASS:
      drawCompassScreen();
      break;

    case SCREEN_NAVIGATION:
      drawNavigationScreen();
      break;
  }
}

void handleButtons() {
  bool leftState =
    digitalRead(BTN_LEFT);

  bool rightState =
    digitalRead(BTN_RIGHT);

  bool okState =
    digitalRead(BTN_OK);

  if (lastRightState == HIGH && rightState == LOW) {
    currentScreen =
      (ScreenMode)((currentScreen + 1) % 4);
    Serial.println("loadCurrentScreen!");

    loadCurrentScreen();

    delay(200);
  }

  if (lastLeftState == HIGH && leftState == LOW) {
    if (currentScreen == SCREEN_DESTINATIONS) {
      selectedDestination++;
      Serial.println("destination!");

      if (selectedDestination >= totalDestinations) {
        selectedDestination = 0;
      }

      updateDestinationMenu();
    }

    delay(200);
  }

  if (lastOkState == HIGH && okState == LOW) {
    if (currentScreen == SCREEN_DESTINATIONS) {
      Serial.print(
        "SELECTED: ");
  // lock start position
  navStartLat = currentLat;
  navStartLon = currentLon;

  navStarted = true;
      Serial.println(
        destinations[selectedDestination]);
    }

    delay(200);
  }

  // save states
  lastLeftState = leftState;
  lastRightState = rightState;
  lastOkState = okState;
}

void drawCompassNeedle(float heading) {
  int cx = 90;
  int cy = 140;

  float r = radians(heading);

  int len = 40;

  // tip points
  int xN = cx + len * sin(r);
  int yN = cy - len * cos(r);

  int xS = cx - len * sin(r);
  int yS = cy + len * cos(r);

  // side width
  int w = 6;

  // perpendicular
  float px = cos(r);
  float py = sin(r);

  // north base
  int xNL = cx + w * px;
  int yNL = cy + w * py;

  int xNR = cx - w * px;
  int yNR = cy - w * py;

  // south base
  int xSL = cx + w * px;
  int ySL = cy + w * py;

  int xSR = cx - w * px;
  int ySR = cy - w * py;

  // center stem
  display.drawLine(xS, yS, xN, yN, GxEPD_BLACK);

  // north filled
  display.fillTriangle(
    xN, yN,
    xNL, yNL,
    xNR, yNR,
    GxEPD_BLACK);

  // south hollow
  display.drawTriangle(
    xS, yS,
    xSL, ySL,
    xSR, ySR,
    GxEPD_BLACK);

  // center hub
  display.fillCircle(cx, cy, 4, GxEPD_BLACK);
}

void drawDir(const char* txt, int x, int y) {
  display.setCursor(x - 8, y);
  display.print(txt);
}

void updateCompassUI(float heading) {
  int cx = 90;
  int cy = 140;
  int radius = 45;

  display.setPartialWindow(0, 40, 264, 224);

  display.firstPage();

  do {

    // clear inner compass
    display.fillCircle(cx, cy, radius, GxEPD_WHITE);

    // redraw rings
    display.drawCircle(cx, cy, 55, GxEPD_BLACK);
    display.drawCircle(cx, cy, 53, GxEPD_BLACK);

    // tick marks
    for (int a = 0; a < 360; a += 30) {
      float r = radians(a);

      int x1 = cx + (55 - 6) * sin(r);
      int y1 = cy - (55 - 6) * cos(r);

      int x2 = cx + 55 * sin(r);
      int y2 = cy - 55 * cos(r);

      display.drawLine(x1, y1, x2, y2, GxEPD_BLACK);
    }

    // direction labels
    display.setFont(&FreeSerifBold9pt7b);

    drawDir("N", cx, cy - 55 - 4);
    drawDir("S", cx, cy + 55 + 18);

    drawDir("E", cx + 55 + 10, cy + 4);
    drawDir("W", cx - 55 - 12, cy + 4);

    drawDir("NE", cx + 40, cy - 48);
    drawDir("NW", cx - 50, cy - 48);

    drawDir("SE", cx + 50, cy + 50);
    drawDir("SW", cx - 60, cy + 50);

    // compass needle
    drawCompassNeedle(heading);

    // heading bar
    display.fillRect(0, 240, 264, 24, GxEPD_WHITE);

    display.drawLine(0, 240, 264, 240, GxEPD_BLACK);

    display.setCursor(5, 257);

    display.print("HEAD: ");
    display.print((int)heading);
    display.print((char)247);
    display.print(" ");
    display.print(getDirection(heading));

  } while (display.nextPage());
}


void drawCompassScreen() {
  // =====================================
  // COMPASS GEOMETRY
  // =====================================

  int cx = 90;
  int cy = 140;

  int radius = 55;

  display.setFullWindow();
  display.firstPage();

  do {

    display.fillScreen(GxEPD_WHITE);


    drawTopBar();

    display.drawLine(0, 15, 264, 15, GxEPD_BLACK);

    // =====================================
    // MODE BAR
    // =====================================

    display.setCursor(5, 22);
    display.print("MODE: COMPASS");

    display.drawLine(0, 35, 264, 35, GxEPD_BLACK);

    // =====================================
    // OUTER RING
    // =====================================

    display.drawCircle(cx, cy, radius, GxEPD_BLACK);
    display.drawCircle(cx, cy, radius - 2, GxEPD_BLACK);

    // =====================================
    // DIRECTION LABELS
    // =====================================

    display.setFont(&FreeSerifBold9pt7b);

    // cardinal
    drawDir("N", cx, cy - radius - 4);

    drawDir("S", cx, cy + radius + 18);

    drawDir("E", cx + radius + 10, cy + 4);

    drawDir("W", cx - radius - 12, cy + 4);

    // diagonal labels — slightly outside circle

    drawDir("NE", cx + 40, cy - 48);

    drawDir("NW", cx - 50, cy - 48);

    drawDir("SE", cx + 50, cy + 50);

    drawDir("SW", cx - 60, cy + 50);

    // =====================================
    // TICK MARKS
    // =====================================

    for (int a = 0; a < 360; a += 30) {
      float r = radians(a);

      int x1 = cx + (radius - 6) * sin(r);
      int y1 = cy - (radius - 6) * cos(r);

      int x2 = cx + radius * sin(r);
      int y2 = cy - radius * cos(r);

      display.drawLine(
        x1, y1,
        x2, y2,
        GxEPD_BLACK);
    }

    // =====================================
    // CENTER DOT
    // =====================================
    //drawCompassNeedle(45);
    display.fillCircle(cx, cy, 3, GxEPD_BLACK);

    // =====================================
    // BOTTOM HEADING BAR
    // =====================================

    display.drawLine(0, 240, 264, 240, GxEPD_BLACK);

    display.setCursor(5, 257);

    display.print("HEAD: ");

    display.print((int)currentHeading);

    display.print(" ");

    display.print(
      getDirection(currentHeading));

  } while (display.nextPage());
}

void drawNavigationScreen() {
  display.setFullWindow();

  display.firstPage();

  do {

    display.fillScreen(GxEPD_WHITE);

    // =====================================
    // TOP BAR
    // =====================================

    drawTopBar();

    // =====================================
    // MODE BAR
    // =====================================

    display.setFont();

    display.setCursor(5, 22);

    display.print("MODE: NAVIGATION");

    display.drawLine(0, 35, 264, 35, GxEPD_BLACK);

    // =====================================
    // DESTINATION INFO
    // =====================================

    display.setFont(&FreeSerifBold9pt7b);

    display.setCursor(5, 52);
    display.print("DEST : ");
    display.print(targetName);

    display.setCursor(5, 72);
    display.print("DIST : ");
    display.print((int)targetDistance);
    display.print("m");

    display.setCursor(5, 92);
    display.print("ETA  : ");
    display.print(etaText);

    display.setCursor(5, 112);

    display.print(navInstruction);

    display.setCursor(5, 132);

    display.print("HEAD : ");

    display.print((int)currentHeading);

    display.print(" ");

    display.print(
      getDirection(currentHeading));


    display.drawRect(
      0,
      140,
      250,
      120,
      GxEPD_BLACK);


  } while (display.nextPage());
}

void drawNavigationArrow(
  int x,
  int y,
  float heading) {
  float r =
    radians(heading);

  int len = 12;

  int xTip =
    x + len * sin(r);

  int yTip =
    y - len * cos(r);

  int xL =
    x + 5 * sin(r + 2.5);

  int yL =
    y - 5 * cos(r + 2.5);

  int xR =
    x + 5 * sin(r - 2.5);

  int yR =
    y - 5 * cos(r - 2.5);

  display.fillTriangle(
    xTip, yTip,
    xL, yL,
    xR, yR,
    GxEPD_BLACK);
}

void updateNavigation()
{
  // distance
  targetDistance =
    TinyGPSPlus::distanceBetween(
      currentLat,
      currentLon,
      destLat,
      destLon
    );

  // bearing
  targetBearing =
    TinyGPSPlus::courseTo(
      currentLat,
      currentLon,
      destLat,
      destLon
    );

  // relative angle
  float turnAngle =
currentHeading - targetBearing;

  while(turnAngle > 180)
    turnAngle -= 360;

  while(turnAngle < -180)
    turnAngle += 360;

  // navigation text
  if(targetDistance < 10)
  {
    navInstruction = "ARRIVED";
  }
  else if(abs(turnAngle) < 20)
  {
    navInstruction = "GO STRAIGHT";
  }
  else if(turnAngle > 0)
  {
    navInstruction = "TURN RIGHT";
  }
  else
  {
    navInstruction = "TURN LEFT";
  }

  // ETA
  if(currentSpeed > 1)
  {
    float etaHours =
      (targetDistance / 1000.0) /
      currentSpeed;

    int etaMinutes =
      etaHours * 60;

    etaText =
      String(etaMinutes) +
      " min";
  }
  else
  {
    etaText = "--";
  }
}

void updateNavigationUI()
{
  display.setPartialWindow(
    0,
    40,
    264,
    100
  );

  display.firstPage();

  do {

    display.fillRect(
      0,
      40,
      264,
      100,
      GxEPD_WHITE
    );

    display.setFont(
      &FreeSerifBold9pt7b
    );

    // DEST
    display.setCursor(5,52);

    display.print("DEST : ");

    display.print(targetName);

    // DIST
    display.setCursor(5,72);

    display.print("DIST : ");

    display.print(
      (int)targetDistance
    );

    display.print("m");

    // ETA
    display.setCursor(5,92);

    display.print("ETA  : ");

    display.print(etaText);

    // NAV TEXT
    display.setCursor(5,112);

    display.print(navInstruction);

    // HEAD
    display.setCursor(5,132);

    display.print("HEAD : ");

    display.print(
      (int)currentHeading
    );

    display.print((char)247);

    display.print(" ");

    display.print(
      getDirection(
        currentHeading
      )
    );



  } while(display.nextPage());
}

void updateMapUI()
{
  int xDest = 30;
  int yDest = 160;

  // base user position
  int baseX = 160;
  int baseY = 215;

  // movement scale
  float scale = 50000.0;

  // =====================================
  // GPS MOVEMENT
  // =====================================

  float dx =
    currentLon -
    navStartLon;

  float dy =
    currentLat -
    navStartLat;

  // moving user position
  int xUser =
    baseX + dx * scale;

  int yUser =
    baseY - dy * scale;

  // keep inside map
  xUser = constrain(
    xUser,
    40,
    220
  );

  yUser = constrain(
    yUser,
    160,
    240
  );

  display.setPartialWindow(
    0,
    145,
    250,
    115
  );

  display.firstPage();

  do {

    // clear map
    display.fillRect(
      0,
      145,
      250,
      115,
      GxEPD_WHITE
    );

    // border
    display.drawRect(
      0,
      145,
      250,
      115,
      GxEPD_BLACK
    );

    // =====================================
    // DESTINATION
    // =====================================

    display.fillCircle(
      xDest,
      yDest,
      5,
      GxEPD_BLACK
    );

    display.drawCircle(
      xDest,
      yDest,
      8,
      GxEPD_BLACK
    );

    display.setFont();

    display.setCursor(
      xDest + 12,
      yDest
    );

    display.print("DEST");

    // =====================================
    // USER POSITION
    // =====================================

    display.fillCircle(
      xUser,
      yUser,
      3,
      GxEPD_BLACK
    );

    // =====================================
    // RELATIVE NAVIGATION ANGLE
    // =====================================

    float relativeAngle =
currentHeading - targetBearing;

    while(relativeAngle > 180)
      relativeAngle -= 360;

    while(relativeAngle < -180)
      relativeAngle += 360;

    float r =
      radians(relativeAngle);

    // =====================================
    // ARROW
    // =====================================

    int len = 14;

    int xTip =
      xUser + len * sin(r);

    int yTip =
      yUser - len * cos(r);

    int xL =
      xUser + 5 * sin(r + 2.5);

    int yL =
      yUser - 5 * cos(r + 2.5);

    int xR =
      xUser + 5 * sin(r - 2.5);

    int yR =
      yUser - 5 * cos(r - 2.5);

    display.fillTriangle(
      xTip, yTip,
      xL, yL,
      xR, yR,
      GxEPD_BLACK
    );

  } while(display.nextPage());
}