class Bit {
  float x;
  float y;
  float w;
  float h;
  boolean active;
  int loc;

  Bit(float _x, float _y, float _w, float _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;

    active = false;
  }

  void display() {
    if (active) {
      if (added){
        fill(0, 255, 0); 
      } else {
        fill(255, 0, 0);
      }
      noStroke();
    } else {
      fill(255, 50);
      stroke(255, 50);
    }
    rect(x, y, w, h);
  }

  void checkBound() {
    if (mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+h) {
      active = !active;
      added = false;
    }
  }
}
