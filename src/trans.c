//#include "trans.h"

void trans(int* src, int sw, int sh, int* dst, int dh,
           double a, double b, double c, double d, int xm, int ym){
  double dx, dy;
  for(int x=0; x<sw; x++){
    dx = x*a-xm; dy = x*c-ym;
    for(int y=0; y<sh; y++) dst[(int)(y*b+dx)+dh*(int)(y*d+dy)] = src[x+sh*y];
  }
}

