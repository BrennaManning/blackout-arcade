int numCircles = 500;
Circle[] circles = new Circle[numCircles]; // define the array
int tim = 2500;
boolean notfirst = false;

void setup() {
  size(800,800);
  smooth();
  noStroke();
  int k = 0;
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++){
      circles[k] = new Circle(i*200+100,j*200+100); // fill the array with circles at random positions
      k++;
    }
  }
}

void draw() {
  background(0);
  int light = int(random(0,16));
  for (int i=0; i<16; i++) {
    if (i == light){
      circles[i].c = 250;
    }
    circles[i].display(); // display all the circles
    circles[i].c = 50;
  }
  if (notfirst){
    println(tim);
    delay(tim);
  }
  notfirst = true;
}

class Circle {
  float x,y; // location
  float dim; // dimension
  color c; // color
 
  Circle(float x, float y) {
    this.x = x;
    this.y = y;
    dim = 50;
    this.c = 50;
  }
 
  void display() {
    fill(c);
    ellipse(x,y,dim,dim); // a circle at position xy
  }
}