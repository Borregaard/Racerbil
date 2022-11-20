#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded. 

#define _XTAL_FREQ 8000000
#include <xc.h>

#pragma config FOSC = INTOSCCLK	// Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF	// Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON	// Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF	 // Brown-out Reset Enable bit (BOR enabled)
#pragma config CPD = OFF	 // Data EEPROM Memory Code Protection 
#pragma config CP = OFF	// Flash Program Memory Code Protection 
#pragma config MCLRE = ON
#pragma config FCMEN  = OFF
#pragma config CPD = OFF 
#pragma config IESO  = OFF

#endif	/* XC_HEADER_TEMPLATE_H */

