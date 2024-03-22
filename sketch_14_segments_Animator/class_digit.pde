class Digit {
  float x;
  float y;

  Bit[] bits = new Bit[15];

  Digit(float _x, float _y) {
    x = _x;
    y = _y;

    bits[14] = new Bit(x + segM + segH, y + segM, segW - segH, segH); // a
    bits[13] = new Bit(x + segM + segW, y + segM + segH, segH, segW - segH); // b
    bits[12] = new Bit(x + segM + segW, y + segM + segW + segH, segH, segW - segH); // c
    bits[11] = new Bit(x + segM + segH, y + segM + segW + segW, segW - segH, segH); // d
    bits[10] = new Bit(x + segM, y + segM + segW + segH, segH, segW - segH); // e
    bits[9] = new Bit(x + segM, y + segM + segH, segH, segW- segH); // f
    bits[8] = new Bit(x + segM + segH, y + segM + segW, segW/2 - segH, segH); // g1
    bits[7] = new Bit(x + segM + segW/2 + segH,y + segM + segW, segW/2 - segH, segH); // g2
    bits[6] = new Bit(x + segM + segH + segM, y + segM+segH + segM, segW/2 - segH - segM*2, segW - segH-segM*2); // h
    bits[5] = new Bit(x + segM + segW/2, y + segM + segH, segH, segW - segH); // j
    bits[4] = new Bit(x + segM + segW/2 + segH + segM, y + segM + segH + segM, segW/2 - segH - segM*2, segW - segH-segM*2); // k
    bits[3] = new Bit(x + segM + segH + segM, y + segM+segH + segW + segM, segW/2 - segH - segM*2, segW - segH-segM*2); // l
    bits[2] = new Bit(x + segM + segW/2, y + segM + segH + segW, segH, segW - segH); // m
    bits[1] = new Bit(x + segM + segW/2 + segH + segM, y + segM + segH + segW + segM, segW/2 - segH - segM*2, segW - segH-segM*2); // n
    bits[0] = new Bit(x + segM + segW + segH, y + segM + segW*2, segH, segH);
  }

  void display() {
    for (Bit b : bits) {
      b.display();
    }
  }
  
  int exportInt(){
    String binary = "0";
    for (int i = 0; i < bits.length; i++){
      if (bits[i].active){
        binary += "1";
      } else {
        binary += "0";
      }
    }
    return unbinary(binary);
  }
}
