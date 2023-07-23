/**************************************************************

	The program reads an 8BPP BMP image file and creates a
	new image showing the input file's palette's colors in a
	16x16 grid.

**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qdbmp.h"
// #include "graphlib.h"

#define	PALETTE_SIZE		256						/* Number of colors in an 8 bit palette */
#define OUTPUT_WIDTH		640						/* Width of output image (pixels) */
#define OUTPUT_HEIGHT		480						/* Height of output image (pixels) */ 

/* This alhoritm is not universal enougch. It need a warper to draw a lines in any direction.
Bresenham alhorithm is for sloped lines only 												 */
static void plotBresenhamLine1(int x0, int y0, int x1, int y1, BMP* bmp, int color) {
	int dx, dy, D, x, y;
	dx = x1 - x0;
	dy = y1 - y0;
	D = 2*dy - dx;
	y = y0;

	for ( x = x0 ; x < x1 ; ++x ) {
		BMP_SetPixelIndex( bmp, x, y, color );
        	if (D > 0) 
		{
			y = y + 1;
			D = D - 2*dx;
		}
        D = D + 2*dy;
	}
}

/* More universal Bresenham alhorithm ic compare with plotBresenhamLine1. 
But only for sloped lines.
It does not draw line ends for lines with small slope. And does not draw line at all 
is line is horizontal or vertical.
It still needs a warper function to process a horizontal anv vertical lines and lines with
small deviation from horizontal and vertical. But with alhorim should I use for this cases? */

static void plotBresenhamLine2( int x0, int y0, int x1, int y1, BMP* bmp, int color) {
	int dx, sx, dy, sy, err, e2;
	dx =  abs(x1-x0);
	sx = x0<x1 ? 1 : -1;
	dy = -abs(y1-y0);
	sy = y0<y1 ? 1 : -1;
	err = dx+dy;  /* error value e_xy */

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
		BMP_SetPixelIndex( bmp, x0, y0, color );
	}
}

static void Gradient(BMP* bmp) 
{
	int i, j;

	/* Draw gradient on screen */
	for ( i = 0 ; i < OUTPUT_WIDTH ; ++i ) 
	{
		for ( j = 0 ; j < 241 ; ++j ) 
		{
			BMP_SetPixelIndex( bmp, i, j, j );
			BMP_SetPixelIndex( bmp, i, OUTPUT_HEIGHT-j, j );
		}
	}
	/* Easy, dirty way to make palette 	*/
	for ( i = 0 ; i < PALETTE_SIZE ; ++i ) 
	{
		BMP_SetPaletteColor( bmp, i, i, i, 0);
	}
}

static void DrawLines(BMP* bmp) {
	int i;

	//plotBresenhamLine1( 50, 80, 500, 400, bmp, 5 );
	//plotBresenhamLine2( 520, 100, 240, 400, bmp, 12 );
	for ( i = 0 ; i < OUTPUT_WIDTH ; ++i ) {
		BMP_SetPixelIndex( bmp, i, 100, 5 );
		BMP_SetPixelIndex( bmp, i, 400, 5 );
	}
/* Check line draw alhorithm restrictions */
	plotBresenhamLine2( 100, 100, 140, 400, bmp, 5 ); //x+40
	plotBresenhamLine2( 150, 100, 186, 400, bmp, 5 ); //x+36
	plotBresenhamLine2( 200, 100, 232, 400, bmp, 5 ); //x+32
	plotBresenhamLine2( 250, 100, 278, 400, bmp, 5 ); //x+28
	plotBresenhamLine2( 300, 100, 324, 400, bmp, 5 ); //x+24
	plotBresenhamLine2( 350, 100, 370, 400, bmp, 5 ); //x+20
	plotBresenhamLine2( 400, 100, 416, 400, bmp, 5 ); //x+16
	plotBresenhamLine2( 450, 100, 462, 400, bmp, 5 ); //x+12
	plotBresenhamLine2( 500, 100, 508, 400, bmp, 5 ); //x+8
	plotBresenhamLine2( 550, 100, 554, 400, bmp, 5 ); //x+4
	plotBresenhamLine2( 600, 100, 600, 400, bmp, 5 ); //x+0
}

int main( int argc, char* argv[] )
{
	BMP*	canvas; 

	/* Create canvas for drawing */
	canvas = BMP_Create( OUTPUT_WIDTH, OUTPUT_HEIGHT, 8 ); 
	// BMP_CHECK_ERROR( stderr, -3 );
	
	Gradient( canvas );
	DrawLines( canvas );

	/* Save output image */ 
	BMP_WriteFile( canvas, "Gradient.bmp" );
	// BMP_CHECK_ERROR( stderr, -5 ); 

	/* Free output image memory */
	BMP_Free( canvas );

	return 0; 
}


