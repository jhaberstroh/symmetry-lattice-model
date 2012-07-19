#include <pngwriter.h>

int main(){
  pngwriter png(100, 100, 1.0, "blah.png");
  png.plot(30,40,1.0,0.0,0.0);
  png.close();
  return 0;
}
