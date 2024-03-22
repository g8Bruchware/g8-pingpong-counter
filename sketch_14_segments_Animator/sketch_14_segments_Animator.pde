// Controls
// e = add Frame
// d = delete lastFrame
// c = clear all Segments
// a = select all Segments
// s = save Animation as txt

String name = "p2Win";

Digit[] digits = new Digit[4];
boolean added = false;


ArrayList<int[]> frameData = new ArrayList<int[]>();

float segM = 10;
float segW = 140;
float segH = 20;

void setup() {
  size(800, 400);
  digits[0] = new Digit(0, 0);
  digits[1] = new Digit(200, 0);
  digits[2] = new Digit(400, 0);
  digits[3] = new Digit(600, 0);
}

void draw() {
  background(0);

  for (Digit d : digits) {
    d.display();
  }
}

void mousePressed() {
  for (Digit d : digits) {
    for (Bit b : d.bits) {
      b.checkBound();
    }
  }
}

void keyPressed() {
  if (key == 'e') {
    int[] frame = new int[digits.length];
    for (int i = 0; i < digits.length; i++) {
      frame[i] = digits[i].exportInt();
    }

    frameData.add(frame);
    //if (key == 'e') {
    //  println(d1.exportInt());
    //}
    renderAnimation(false);
    added = true;
  }
  
  if (key == 'd'){
    frameData.remove(frameData.size()-1);
    renderAnimation(false);
  }
  
  if (key == 'c'){
    clearBits();
  }
  
  if (key == 'a'){
    activateAll();
  }
  
  if (key == 's'){
    saveAnimation();
  }
}

void activateAll(){
  for (Digit d : digits){
    for (Bit b : d.bits){
      b.active = true;
    }
  }
}

void clearBits(){
  for (Digit d : digits){
    for (Bit b : d.bits){
      b.active = false;
    }
  }
}

void saveAnimation(){
  renderAnimation(true);
}

void renderAnimation(boolean save){
  String data = "int " + name + "[" + frameData.size() + "][4] = {";
    for (int i = 0; i < frameData.size(); i++) {
      data += "{";
      for (int j = 0; j < frameData.get(i).length; j++){
        data += frameData.get(i)[j];
        if (j < 3) data +=  ", ";
      }
      data += "}";
      if (i < frameData.size()-1) data += ", ";
    }
    data += "};";
    String len = "int " + name + "len = " + frameData.size() + ";";
    
    println("##########################################");
    println(data);
    println(len);
    
    if (save){
      String[] saves = new String[2];
      saves[0] = data;
      saves[1] = len;
      saveStrings("saves/" + name + ".txt", saves); 
      print("saved: " + name);
    }
}
