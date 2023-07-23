#include "qdbmp.h"


#define PLOT(x, y) 			BMP_SetPixelIndex( bmp, x, y, ColorIndex );
#define	PALETTE_SIZE		256		/* Number of colors in an 8 bit palette */

/*********************************** Public methods **********************************/

/* Construction/destruction */
void 	InitGraph 			( UINT H, UINT V, UINT width, UINT height );
void 	CloseGraph 			( void );

/* Draw */
void MoveTo ( int Xpt, int Ypt );
void LineTo ( int Xpt, int Ypt );
void Line ( int X0, int Y0, int X1, int Y1 );

/* Coordinate system */
void SetOrigin (  int XMove, int YMove  );

/* I/O */
/* static void Stroke ( void ); */
/* Stroke is for future development or obsolette */

/* Basic draw */
void plotBresenhamLine( int x0, int y0, int x1, int y1 );

int X ( int C );
int Y ( int C );
