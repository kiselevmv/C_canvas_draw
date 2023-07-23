/**************************************************************

	The program creates an Godseye image
	It could be used to compare different line draw algorithmes.
	This is a test for classic Bresenham line draw.

**************************************************************/

#include "graphlib.h"

#define SIDES				15
#define LINE_SCALE			100.0

/* Creates Godseye  */
int main( int argc, char* argv[] )
{
    int i;
    /* i for cycle counters only */
    int id1, id2;

    /* Create canvas for drawing */
    InitGraph ( 300, 220, 800, 600 );
	SetOrigin( 150, 110 );

    /* Draw N-gon using precalculated vertex coordinates  */
    for ( i = 0 ; i < (SIDES+1) ; ++i ) 
    {
		id1 = (i * LINE_SCALE / SIDES);
		id2 = (( SIDES - i ) * LINE_SCALE / SIDES);
		MoveTo(id1, 0);
		LineTo(0, id2);
		LineTo(-id1, 0);
		LineTo(0, -id2);
		LineTo(id1, 0);
    }
	

}
