#define uchar unsigned char
/*int min(int i, int j){*/
/*    return i;*/
/*    if(i>j) return j; else return i;*/
/*}*/

int interp(uchar* src, int sw, int sh, double x, double y){
    // interpolate src points to emulate continuity
    if(!(0<=x && x<sw-1 && 0<=y && y<sh-1)) return 0;
    int ix = (int) x, iy = (int) y;
    double dx = 1-(x-ix);
    double dy = 1-(y-iy);
    float l1 =    dx *   dy *src[(ix+sw*iy<<2)+3];
    float l2 = (1-dx)*   dy *src[(1+ix+sw*iy<<2)+3];
    float l3 =    dx *(1-dy)*src[(sw+ix+sw*iy<<2)+3];
    float l4 = (1-dx)*(1-dy)*src[(sw+1+ix+sw*iy<<2)+3];
    float l = l1+l2+l3+l4;
    int a = (int) l;
    if(a==0) return 0;
    uchar r = (uchar) ((l1*src[(ix+sw*iy<<2)+2]    + l2*src[(1+ix+sw*iy<<2)+2] +
                        l3*src[(sw+ix+sw*iy<<2)+2] + l4*src[(sw+1+ix+sw*iy<<2)+2])/l);
    uchar v = (uchar) ((l1*src[(ix+sw*iy<<2)+1]    + l2*src[(1+ix+sw*iy<<2)+1] +
                        l3*src[(sw+ix+sw*iy<<2)+1] + l4*src[(sw+1+ix+sw*iy<<2)+1])/l);
    uchar b = (uchar) ((l1*src[(ix+sw*iy<<2)]      + l2*src[(1+ix+sw*iy<<2)] +
                        l3*src[(sw+ix+sw*iy<<2)]   + l4*src[(sw+1+ix+sw*iy<<2)])/l);
    return (((a<<8)+r<<8)+v<<8)+b;
}

void trans(int* src, int sw, int sh, int* dst, int dw, int dh,
           double a, double b, double c, double d, int xm, int ym){
    // cf trans.py
    double dx, dy;
    for(int x=0; x<dw; x++){
        dx = (x+xm)*a; dy = (x+xm)*c;
        for(int y=0; y<dh; y++)
            dst[x+dw*y] = interp((uchar*)src, sw, sh, (y+ym)*b+dx, (y+ym)*d+dy);
    }
}

