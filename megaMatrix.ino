#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, PIN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB + NEO_KHZ800);

// Ethernet server
#include <UIPEthernet.h>
EthernetServer server = EthernetServer(1234);

void setup() {
  //Serial.begin(115200);

  uint8_t mac[6] = {0xa0,0xa1,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,0,99);
  Ethernet.begin(mac,myIP);
  server.begin();

  matrix.begin();
  matrix.setBrightness(25);
  matrix.fillScreen(matrix.Color(0, 128, 0));
  matrix.show();
}

void loop() {
  int size;

  if (EthernetClient client = server.available())
    {
      while((size = client.available()) > 0)
        {
          uint8_t* msg = (uint8_t*)malloc(size);
          size = client.read(msg,size);
          //Serial.println(size);
          if ( size == 768 ) {
            int p = 0;
            for (int j=0; j<16; j++) {
              for (int i=0; i<16; i++) {
                byte r = msg[p+0];
                byte g = msg[p+1];
                byte b = msg[p+2];
                p += 3;
                matrix.drawPixel(i, j, matrix.Color(r, g, b));
              }
            };
          } else if ( size == 3 ) {
            byte r = msg[0];
            byte g = msg[1];
            byte b = msg[2];
            matrix.fillScreen(matrix.Color(r, g, b));
          } else if ( size == 1 ) {
            matrix.setBrightness(msg[0]);
          }
          matrix.show();
          free(msg);
        }
      client.println("OK");
      client.stop();
    }
}
