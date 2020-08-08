/**************************************************************

	The program creates an N-sided polygon (N-gon) image
	It could be used to compare different line draw algorithmes.
	This is a test for classic Bresenham line draw.

**************************************************************/

#include "..\qdbmp.h"
#include <stdio.h>
#include <math.h>

#define	PALETTE_SIZE		256						/* Number of colors in an 8 bit palette */
#define OUTPUT_WIDTH		640						/* Width of output image (pixels) */
#define OUTPUT_HEIGHT		640						/* Height of output image (pixels) */ 
#define SIDES				15
#define ADIF				2 * M_PI / SIDES
#define PLOT(x, y) 			BMP_SetPixelIndex( bmp, x, y, ColorIndex );
#define X(Z)				(int)( ( XOrig + Z ) * XYScale + 0.5 )
#define Y(Z)				(int)( ( YOrig + Z ) * XYScale + 0.5 )

static int ColorIndex = 1;	// Draw with index color = 1 by default.
static int XOrig = 0;
static int YOrig = 0;
static int XPen, YPen;
static int NXPix, NYPix;
static int Horiz, Vert;
static float XYScale;
static BMP* Canvas;


inline void Plot( UINT *x, UINT *y, BMP* *bmp) 
/* This is inline function, because it just too short. 
Smart compiler will find a way to substitut this inline function into the main function 
#define used instead of inline fuction, juse because this is oneliner */
{ 
    BMP_SetPixelIndex( *bmp, *x, *y, ColorIndex ); 
} 

/* 
This alhoritm is not universal enougch. It need a warper to draw a lines in any direction.
Bresenham alhorithm is for sloped lines only 												 
*/
void plotBresenhamLine( UINT x0, UINT y0, UINT x1, UINT y1, BMP* bmp, int color) {
	int dx, sx, dy, sy, err, e2;
	if ( abs (x0 - x1) < 2 ) /* Special case, vertical or almost vertical line. By thumb of thrust only lines with one pixel vert shift drawn badly */
	{ sy = y0<y1 ? 1 : -1 ; while(y0 != y1) { PLOT( x0, y0 ); y0 += sy; } }
	if ( abs (y0 - y1) <2 ) /* Special case, horinontal or almost horizontal line */
	{ sx = x0<x1 ? 1 : -1 ; while(x0 != x1) { PLOT( x0, y0 ); x0 += sx; } }
	
	dx =  abs(x1-x0);
	sx = x0<x1 ? 1 : -1;
	dy = -abs(y1-y0);
	sy = y0<y1 ? 1 : -1;
	err = dx+dy;  /* error value e_xy */
	PLOT( x0, y0 ); /* do not loose first point */
	
	while(x0 != x1 && y0 != y1)   /* loop */
	{
        //if (x0==x1 && y0==y1) break;
        	e2 = 2*err;
	        if (e2 >= dy)  /* e_xy+e_x > 0 */
		{
	            err += dy;
	            x0 += sx;
		}
	        if (e2 <= dx)  /* e_xy+e_y < 0 */
		{
	            err += dx;
	            y0 += sy;
		}
		//PLOT( x0, y0 );
		Plot( &x0, &y0, &bmp);
	}
}

BMP* InitGraph (UINT H, UINT V, UINT width, UINT height)
{
	float YScale;
	XPen = 0;	
	YPen = 0; 	// Move grid origin to zero
	NXPix = width;
	NYPix = height;	// Save viewport param
	Horiz = H;
	Vert = V;
	XYScale = NXPix / H;
	YScale = NYPix / V;
	XYScale = (XYScale > YScale) ? YScale : XYScale ;
	printf("XYScale = %f\n", XYScale);
	return BMP_Create( width, height, 8 );
}

void SetOrigin ( int XMove, int YMove )
{
	XOrig += XMove;
	YOrig += YMove;
	printf("XOrig = %d\tYOrig = %d\n", XOrig, YOrig);
	XPen = X(0);
	YPen = Y(0);
}

void MoveTo ( int Xpt, int Ypt )
{
	XPen = X(Xpt);
	YPen = Y(Ypt);
}

void LineTo ( int Xpt, int Ypt )
{
	int NXPen, NYPen;
	NXPen = X(Xpt);
	NYPen = Y(Ypt);
	plotBresenhamLine( XPen, YPen, NXPen, NYPen, Canvas, ColorIndex );
	XPen = NXPen;
	YPen = NYPen;
}

void Line ( int X0, int Y0, int X1, int Y1 )
{
	plotBresenhamLine( X(X0), Y(Y0), X(X1), Y(Y1), Canvas, ColorIndex );
	XPen = X(X1);
	YPen = Y(Y1);
}

/* Creates N-gon  */
int main( int argc, char* argv[] )
{
	int x[SIDES];        // declare array of x coordinates
    int y[SIDES];         // declare array of y coordinates
    int i, j;            // for cycle counters only
    float alpha = 0;    // initial angle declared and initiated

    /* Create canvas for drawing */
    Canvas =  InitGraph ( 140, 100, 800, 600 );
    BMP_CHECK_ERROR( stderr, -3 );
    
  
	SetOrigin ( Horiz/2, Vert/2 );

    /* Float cycle counter is not safe. Therefore I use integer counter.
    https://wiki.sei.cmu.edu/confluence/display/c/FLP30-C.+Do+not+use+floating-point+variables+as+loop+counters */    
	for ( i = 0 ; i < SIDES ; ++i ) 
    {
        x[i] = cos(alpha) * 40;
        y[i] = sin(alpha) * 40;
        alpha = alpha + ADIF;
    } 

    /* Draw N-gon using precalculated vertex coordinates  */
    for ( i = 0 ; i < (SIDES-1) ; ++i ) 
    {
        for ( j = (i+1); j < SIDES; ++j )
        {
            Line( x[i], y[i],  x[j], y[j] );
        }
    }
	
    /* Define colors. White background, black and yellow indexed colors */
    for ( i = 0; i < 255; ++i )
    {
        BMP_SetPaletteColor( Canvas, i, 255, 255, 255);
    }
    BMP_SetPaletteColor( Canvas, 1, 0, 0, 0);
    BMP_SetPaletteColor( Canvas, 128, 128, 128, 0);


    /* Save output image */ 
    BMP_WriteFile( Canvas, "ngon.bmp" );
    BMP_CHECK_ERROR( stderr, -5 ); 

    /* Free output image memory */
    BMP_Free( Canvas );

    return 0; 
}
