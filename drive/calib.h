#ifndef _CALIB_H_

#define _CALIBR_H_

/****************************************************/
/*                                                  */
/* Included files                                   */
/*                                                  */
/****************************************************/

#include <math.h>


/****************************************************/
/*                                                  */
/* Definitions                                      */
/*                                                  */
/****************************************************/

#ifndef		_CALIB_C_
	#define		EXTERN         extern
#else
	#define		EXTERN
#endif











/****************************************************/
/*                                                  */
/* Function declarations                            */
/*                                                  */
/****************************************************/


EXTERN int setCalibrationMatrix( POINT * display,
                                 POINT * screen,
                                 MATRIX * matrix) ;


EXTERN int getDisplayPoint( POINT * display,
                            POINT * screen,
                            MATRIX * matrix ) ;


#endif  /* _CALIB_H_ */
