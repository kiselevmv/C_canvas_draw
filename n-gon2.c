/**************************************************************

	The program creates an N-sided polygon (N-gon) image
	It could be used to compare different line draw algorithmes.
	This is a test for classic Bresenham line draw.

**************************************************************/

#include "graphlib.h"
#include  <math.h>

#define SIDES				12
#define ADIF				2 * M_PI / SIDES


/* Creates N-gon  */
int main( int argc, char* argv[] )
{
	int x[SIDES];        // declare array of x coordinates
    int y[SIDES];         // declare array of y coordinates
    int i, j;            // for cycle counters only
    float alpha = 0;    // initial angle declared and initiated

    /* Create canvas for drawing */
    InitGraph ( 140, 100, 800, 600 );
    // BMP_CHECK_ERROR( stderr, -3 );
    
	SetOrigin ( 70, 50 );

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
	
	CloseGraph();
    return 0; 

 } 

