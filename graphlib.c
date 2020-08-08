#include <stdio.h>
#include <math.h>
#include "qdbmp\qdbmp.h"
#include "graphlib.h"

static int ColorIndex = 1;	// Draw with index color = 1 by default.
static int XOrig, YOrig;
static int XPen, YPen;
static int NXPix, NYPix;
static int Horiz, Vert;
static float XYScale;
static BMP* Canvas;


/* 
This alhoritm is not universal enougch. It need a warper to draw a lines in any direction.
Bresenham alhorithm is for sloped lines only 												 
*/
void plotBresenhamLine( UINT x0, UINT y0, UINT x1, UINT y1 ) 
{
	int dx, sx, dy, sy, err, e2;
	if ( abs (x0 - x1) < 2 ) /* Special case, vertical or almost vertical line. By thumb of thrust only lines with one pixel vert shift drawn badly */
	{ sy = y0<y1 ? 1 : -1 ; while(y0 != y1) { BMP_SetPixelIndex( Canvas, x0, y0, ColorIndex ); y0 += sy; } }
	if ( abs (y0 - y1) <2 ) /* Special case, horinontal or almost horizontal line */
	{ sx = x0<x1 ? 1 : -1 ; while(x0 != x1) { BMP_SetPixelIndex( Canvas, x0, y0, ColorIndex ); x0 += sx; } }
	
	dx =  abs(x1-x0);
	sx = x0<x1 ? 1 : -1;
	dy = -abs(y1-y0);
	sy = y0<y1 ? 1 : -1;
	err = dx+dy;  /* error value e_xy */
	BMP_SetPixelIndex( Canvas, x0, y0, ColorIndex ); /* do not loose first point */
	
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
		BMP_SetPixelIndex( Canvas, x0, y0, ColorIndex );
	}
}

int X ( int C )
{
	return floor(( XOrig + C ) * XYScale );
}

int Y ( int C )
{
	return floor(( YOrig + C ) * XYScale );
}

void InitGraph (UINT H, UINT V, UINT width, UINT height)
{
	float YScale;
	XPen = 0;	
	YPen = 0; 	// Move grid origin to zero
	NXPix = width;
	NYPix = height;	// Save viewport param
	Horiz = H;
	Vert = V;
	XOrig = 0;
	YOrig = 0;
	ColorIndex = 1;
	XYScale = (float)NXPix / H;
	YScale = (float)NYPix / V;
	XYScale = (XYScale > YScale) ? YScale : XYScale ;
	printf("XYScale = %f\n", XYScale);
	Canvas = BMP_Create( width, height, 8 );
	BMP_CHECK_ERROR( stderr, -3 );
}

void SetOrigin ( int XMove, int YMove )
{
	XOrig += XMove;
	YOrig += YMove;
//	printf("XOrig = %d\tYOrig = %d\n", XOrig, YOrig);
	XPen = X( 0 );
	YPen = Y( 0 );
}

void MoveTo ( int Xpt, int Ypt )
{
	XPen = X( Xpt );
	YPen = Y( Ypt );
}

void LineTo ( int Xpt, int Ypt )
{
	int NXPen, NYPen;
	NXPen = X( Xpt );
	NYPen = Y( Ypt );
	//printf("Lineto %d, %d\n", NXPen, NYPen);
	plotBresenhamLine( XPen, YPen, NXPen, NYPen );
	XPen = NXPen;
	YPen = NYPen;
	//printf("Lineto %d, %d\n", XPen, YPen);
}

void Line ( int X0, int Y0, int X1, int Y1 )
{
	plotBresenhamLine( X(X0), Y(Y0), X(X1), Y(Y1) );
	XPen = X(X1);
	YPen = Y(Y1);
}

inline void Plot( UINT *x, UINT *y, BMP* *bmp) 
/* This is inline function, because it just too short. 
Smart compiler will find a way to substitut this inline function into the main function 
#define used instead of inline fuction, juse because this is oneliner */
{ 
    BMP_SetPixelIndex( *bmp, *x, *y, ColorIndex ); 
} 

void CloseGraph ( void )
{
	int i;		// Cycle counter
	/* Define colors. White background, black and yellow indexed colors */
    for ( i = 0; i < 250; ++i )
    {
        BMP_SetPaletteColor( Canvas, i, 255, 255, 255);
    }
    BMP_SetPaletteColor( Canvas, 1, 0, 0, 0);
    BMP_SetPaletteColor( Canvas, 128, 128, 128, 0);


    /* Save output image */ 
    BMP_WriteFile( Canvas, "godseye1.bmp" );
    BMP_CHECK_ERROR( stderr, -5 );  

    /* Free output image memory */
    BMP_Free( Canvas );
	printf("XYScale = %f\n", XYScale);
}