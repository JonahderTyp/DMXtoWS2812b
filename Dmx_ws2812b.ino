#include <DMXSerial.h>
#include <Adafruit_NeoPixel.h>
#define LED_PIN   10

#define LED_OFFSET 50
#define LED_anzahl 173
#define split 68
#define addr 17


int LED_COUNT = LED_anzahl - LED_OFFSET;

Adafruit_NeoPixel strip(LED_anzahl, LED_PIN, NEO_GRB );

int lastr1 = 0;
int lastg1 = 0;
int lastb1 = 0;
int lastr2 = 0;
int lastg2 = 0;
int lastb2 = 0;
int lastbrg = 0;

int brg = 0;
int r1 = 0;
int g1 = 0;
int b1 = 0;
int r2 = 0;
int g2 = 0;
int b2 = 0;
int mode = 0;
int effectspeed = 0;

//mode5
long firstPixelHue = 0;
unsigned long previousMillismode5 = 0;
long interval = 10;

void setup () {
  DMXSerial.init(DMXProbe);
  DMXSerial.maxChannel(32);
  pinMode(13, OUTPUT);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50);

}

void loop() {
  //digitalWrite(13, DMXSerial.read(1));
  if (DMXSerial.receive()) {
    brg = DMXSerial.read(17);                  //brightness
    r1 = DMXSerial.read(18);                   //Red
    g1 = DMXSerial.read(19);                   //green
    b1 = DMXSerial.read(20);                   //blue
    r2 = DMXSerial.read(21);                   //Red
    g2 = DMXSerial.read(22);                   //green
    b2 = DMXSerial.read(23);                   //blue
    mode = DMXSerial.read(24);                //mode
    effectspeed = DMXSerial.read(25);         //effect speed
    if (brg > 50 ) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
  }

  if ( true) {
    //listen to dmx
    strip.setBrightness(brg);

    if (mode <= 50) {
      //mode 1
      if (r1 != lastr1 || g1 != lastg1 || b1 != lastb1 || r2 != lastr2 || g2 != lastg2 || b2 != lastb2 || brg != lastbrg) {
        for (int i = LED_OFFSET; i < LED_OFFSET + split; i++) {
          strip.setPixelColor(i, strip.Color(r1, g1, b1));
        }
        for (int i = LED_OFFSET + split; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(r2, g2, b2));
        }
      }
      strip.show();
      lastr1 = r1;
      lastg1 = g1;
      lastb1 = b1;
      lastr2 = r2;
      lastg2 = g2;
      lastb2 = b2;
      lastbrg = brg;
    } else if (mode <= 100) {
      //mode 2
    } else if (mode <= 150) {
      //mode 3
    } else if (mode <= 200) {
      //mode 4
    } else {
      //mode 5
      if(effectspeed > 10){
        interval = 10;
      } else {
        interval = map(effectspeed, 10 , 255, 245,0);
      }

      if (millis() - previousMillismode5 >= interval) {
        if (!(firstPixelHue < 5 * 65536)) {
          firstPixelHue = 0;
        }
        for (int i = LED_OFFSET; i < strip.numPixels(); i++) {
          int pixelHue = firstPixelHue + (i * 65536L / (strip.numPixels()- LED_OFFSET));
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        strip.show();

        firstPixelHue += 256;
      }

    }
  } else {
    //no signal
    //strip.clear();
    //strip.show();
    for (int i = 0;  i < 10; i++) {
      digitalWrite(13, HIGH);
      delay(10);
      digitalWrite(13, LOW);
      delay(50);
    }
  }
}
