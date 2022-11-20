/*
 * File:   main.c
 * Author: Frederik Borregaard
 *
 * Created on 7. november 2022, 12:10
 */


#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "config.h" // Henter alle vores indstillinger fra en samlede fil

// Denne værdi bruges til at få en timeren til at interrupt efter 1 millisekund
const uint8_t TMR0_TIME = 100; 

// Disse variabler bruges til at gemme hvor langt tid vi har kørt vores program
int sec_time = 0;
uint16_t ms_time = 0; 

int time_since_start = 0;

char BW_Temp = 0;
int revs = 0;

int wheel_pos_time_array[][2];

int ADC_Read(int channel);
void CheckWheelPos();
void UpdateRevArray();

void MapBane();


// ------------------- interrupt timer -------------------
void __interrupt() ISR() {
    if (INTCONbits.TMR0IF == 1) {
        INTCONbits.TMR0IF = 0; //  Reset timer flaget
        
        time_since_start++;
        ms_time++;
        if (ms_time == 100) {
            ms_time = 0;
            sec_time++;
        }

        TMR0 = TMR0_TIME; // Genindsæt timerværdien
        
        void CheckWheelPos();
    }
}

void setup()	{
    OSCCON = 0b01110000;        // INT clock used, set for 8 MHz.
    CMCON0 = 0b00000111;      //comparators off

    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA5 = 0;
    
    TRISCbits.TRISC2 = 1;       // ADC til pot
    TRISCbits.TRISC3 = 1;       // ADC til Optokobler
    TRISCbits.TRISC5 = 0;       // PWM output
        
    CMCON0 = 0b00000111;
    ANSEL  = 0b11100000;	
    ADCON0 = 0b00011001;
    ADCON1 = 0b00010000;

    STATUS = 0b00000111;   
    T2CON = 0b11111111; 
    TMR2 = 100;                 
    PR2 = 255;                  // Periode
    CCP1CON = 0b00001100;   
    CCPR1L = 120;               // Puls
    TMR2ON = 1;   
    
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;

    OPTION_REGbits.T0CS = 0; //intern clock
    OPTION_REGbits.T0SE = 0; // lige gyldig
    OPTION_REGbits.PSA = 0; // timer ikke WDT
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    
}

int main()	{
    setup();        
    
    
    while(1) {
        RA5 = 0;

        int ADC_PWM = ADC_Read(6);
        // int ADC_OPTO = ADC_Read(7);
        
        // if (ADC_OPTO > 500 || ADC_OPTO < 100) {
        //     RA5 = 1;
        // }
        
        CCPR1L = ADC_PWM;
        
    }
}

int ADC_Read(int channel) {
    if (channel > 7) {
        return 0;
    }

    ADCON0bits.CHS0 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;

    ADCON0 |= channel << 2;
    __delay_ms(2);
    GO_DONE = 1; // Start ADC
    while (GO_DONE); // Vent til ADC er færdig
    return ADRESH;
    return ((ADRESH << 8) + ADRESL); // returnér resultatet
}


unsigned char Rounds(unsigned char RoundNum) {
    switch (RoundNum) {
        case 0:
            // MapBane();
            break;
        case 1:
            // Tidskørelse
            break;
        case 2:
            // 5 % hurtigere Tidskørelse
            break;
        case 3:
            // stop
            break;
    }
}


void CheckWheelPos() {
    int ADC_Value = ADC_Read(5); // Update to actual pin for RPM monitering
    if (BW_Temp == 1) {
        if (ADC_Value > 512) {
            break;
        }
        else {
            BW_Temp~;
            revs++;
        }
    }
    else if (BW_Temp == 0) {
        if (ADC_Value < 512) {
            break;
        }
        else {
            BW_Temp~;
        }
    }
}

void UpdateRevArray() {
    wheel_pos_time_array[revs][] = {revs, time_since_start};
}

void MapBane() {
    CCPR1L = 64;        // 25% Duty cycle 
    
}
