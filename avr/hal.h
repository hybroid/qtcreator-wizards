#ifndef HAL_H
#define HAL_H

#include <avr/io.h>

#define CONCAT(x,y)			x ## y

#define DDR(x)				CONCAT(DDR,x)
#define PORT(x)				CONCAT(PORT,x)
#define PIN(x)				CONCAT(PIN,x)


#define LED_GREEN			C
#define LED_GREE_LINE		(1<<0)

#endif /* HAL_H */
