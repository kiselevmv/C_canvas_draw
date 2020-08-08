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
#define SIDES				24
#define ADIF				2 * M_PI / SIDES
#define PLOT(x, y) 			BMP_SetPixelIndex( bmp, x, y, ColorIndex );

static int ColorIndex = 1;	// Draw with index color = 1 by default.

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
	{ sy = y0<y1 ? 1 : -1 ; printf("y0 = %d\ty1 = %d\t sy = %d\n", y0, y1, sy); while(y0 != y1) { PLOT( x0, y0 ); y0 += sy; } }
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


/* Creates N-gon  */
int main( int argc, char* argv[] )
{
	BMP* canvas; 
	int x[SIDES];        // declare array of x coordinates
    int y[SIDES];         // declare array of y coordinates
    int i, j;            // for cycle counters only
    float alpha = 0;    // initial angle declared and initiated
    
    for ( i = 0 ; i < SIDES ; ++i ) 
    /* Float cycle counter is not safe. Therefore I use integer counter.
    https://wiki.sei.cmu.edu/confluence/display/c/FLP30-C.+Do+not+use+floating-point+variables+as+loop+counters */
    {
        x[i] = cos(alpha) * ((OUTPUT_WIDTH / 2) - 50) + (OUTPUT_WIDTH / 2);
        y[i] = sin(alpha) * ((OUTPUT_WIDTH / 2) - 50) + (OUTPUT_WIDTH / 2);
        alpha = alpha + ADIF;
    }
    

    /* Create canvas for drawing */
    canvas = BMP_Create( OUTPUT_WIDTH, OUTPUT_HEIGHT, 8 ); 
    BMP_CHECK_ERROR( stderr, -3 );
    
    /* Draw N-gon using precalculated vertex coordinates */
    for ( i = 0 ; i < (SIDES-1) ; ++i ) 
    {
        for ( j = (i+1); j < SIDES; ++j )
        {
            plotBresenhamLine(x[i], y[i], x[j], y[j], canvas, 1);
        }
    }
    
    /* Define colors. White background, black and yellow indexed colors */
    for ( i = 0; i < 255; ++i )
    {
        BMP_SetPaletteColor( canvas, i, 255, 255, 255);
    }
    BMP_SetPaletteColor( canvas, 1, 0, 0, 0);
    BMP_SetPaletteColor( canvas, 128, 128, 128, 0);


    /* Save output image */ 
    BMP_WriteFile( canvas, "ngon.bmp" );
    BMP_CHECK_ERROR( stderr, -5 ); 

    /* Free output image memory */
    BMP_Free( canvas );

    return 0; 
}
