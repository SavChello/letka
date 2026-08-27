#include <TXLib.h>
#include <stdio.h>
#include <math.h>

#define SCALE_X 1
#define SCALE_Y 1  // 1:10


int main() {
    double a = 1, b = 50, c = 6;
    //double x1 = 0.0, x2 = 0.0;
    double y = 0.0;
    txCreateWindow (1200, 700);
    //         x   y    x     y
    //
    //  ----> x
    //  |
    //  |
    //  V y
    txLine   (50, 350, 1150, 350);
    txLine   (1150, 350, 1140, 340);
    txLine   (1150, 350, 1140, 360);
    txLine   (600, 50, 600, 650);
    txLine  (600, 50, 610, 60);
    txLine  (600, 50, 590, 60);

    double x0 = -b / (a * 2);
    double y0 = c - (b * b / (a * 4));

    for (double x = -600; x < 600; x += 0.01) {

        y = a * x * x + b * x + c;

        txSetPixel(x * SCALE_X + 600 - x0, 350 - y * SCALE_Y - y0, TX_PINK);
        //txSetPixel(-x * SCALE + 600 - x0, 350 - y * SCALE - y0, TX_PINK);

    }

    return 0;
}
