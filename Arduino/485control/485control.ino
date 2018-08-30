//https://wltd.org/dmx-to-rgbw-led
//https://ptzoptics.com/wp-content/uploads/2014/09/PTZOptics_TCP_UDP_CGI_Control-1.pdf
//http://www.mathertel.de/Arduino/DMXSerial.aspx

//zoom res. 1024 pqrs 4 16 16 0
//focus res. 4096 pqrs 0 16 16 16
//PIN A -> DMX pin 3
//PIN B -> DMX pin 2
//RO -> RX

#include <SoftwareSerial.h>
#include <DMXSerial.h>

//pines de conexión a RS485 de la cámara
SoftwareSerial SoftSerial(10, 11);

int channel = 25;

void setup() {  
  SoftSerial.begin(9600);
  DMXSerial.init(DMXReceiver);
}

void loop() {
  int zoom = (DMXSerial.read(channel)*256) + (DMXSerial.read(channel+1));
  int focus = (DMXSerial.read(channel+2)*256) + (DMXSerial.read(channel+3));

  send(zoom, 4, 16, 16, 1, 71);
  send(focus, 1, 16, 16, 16, 72);

  if (DMXSerial.read(channel+4) == 10) {
    byte message[] = {0x81, 0x01, 0x04, 0x38, 0x02, 0xFF};
    SoftSerial.write(message, 6);
  }

  if (DMXSerial.read(channel+4) == 20) {
    byte message[] = {0x81, 0x01, 0x04, 0x38, 0x03, 0xFF};
    SoftSerial.write(message, 6);
  }
}

void send (int data, int bp, int bq, int br, int bs, int btype) {
  byte s = char(data%bs);
  int sdata= (data/bs);
  byte r = char(sdata%br);
  int rdata = sdata/br;
  byte q = char(rdata%bq);
  int qdata = rdata/bq;
  byte p = char(qdata%bq);

  byte type = char(btype);
  
  byte message[] = {0x81, 0x01, 0x04, type, p, q, r, s, 0xFF};
  SoftSerial.write(message, 9);
  
  delay(30); //verificar porqué debajo de este humbral da problemas, timing de DMX ?
}

