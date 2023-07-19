/* 
 * File:   Librerias_interrupciones.c
 * Author: Anderson Escobar
 *
 * Created on 13 de julio de 2023, 11:13 AM
 */


//**********************************************************************************************
//palabra de configuracion
//**********************************************************************************************

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//**********************************************************************************************
//prototipos de funciones
//**********************************************************************************************
#include <xc.h>
#include "config.h"
#include "ADC.h"
#include "Multiplex.h"

//**********************************************************************************************
//prototipo de funciones
//**********************************************************************************************
void setup(void);

//**********************************************************************************************
//variables
//**********************************************************************************************

#define up RB0
#define down RB1
#define changeCH RB2
#define getCH RB3

#define _XTAL_FREQ 8000000
#define _trm0_value 200

int valorADC;
int bitsH;
int bitsL;
int display1;
int display2;
uint8_t flag = 0;

//**********************************************************************************************
//ISR
//**********************************************************************************************
void __interrupt() isr(void)
{
    if(RBIF){
        
        if(up == 0){
            while(up==0);
           
            PORTD++;
        }
        else if(down == 0){
            while(down==0);
            PORTD--;
        }
        else if(changeCH == 0){
            adc_change_channel(PORTD);
        }
        else if(getCH == 0){
            PORTD = 0;
            PORTD = adc_get_channel();
        }
        
        RBIF = 0;
      
    }
    
    if(ADIF){
        valorADC = adc_read();
        bitsL = separacionL(valorADC);
        bitsH = separacionH(valorADC);
        display1 = tablaHex(bitsH);
        display2 = tablaHex(bitsL);        
        ADIF = 0;
    }
    
    if(T0IF){
        RE0 = 0;
        RE1 = 0;
        
        if(flag == 0){
            flag = 1;
            RE0 = 1;
            PORTC = display1;
        }
        else if (flag == 1){
            flag = 0;
            RE1 = 1;
            PORTC = display2;
        }
        
        
        
        TMR0 = _trm0_value;   //reinicio tmr0
        T0IF = 0;   //apaga la bandera de interrupción
    }
}


//**********************************************************************************************
//Codigo principal
//**********************************************************************************************
void main(void) {
    setup();
        
    while(1){
        if(ADCON0bits.GO == 0){
            __delay_ms(10);
            ADCON0bits.GO = 1; 
        }
        
        if(valorADC >= PORTD)
           RE2 = 1;
        else if(valorADC < PORTD)
           RE2 = 0; 
    }
}


//**********************************************************************************************
//Codigo de configuracion
//**********************************************************************************************
void setup(){
    
    configOsc(8); //configuracion del oscilador
    configPort(); //configuracion de puertos
    pullup();      //habilitacion de pullup
    configIntbits(); //configuracion de bits de interrupcion
    ioc_init (0XFF); //configuracion interrupt on change
    config_timer0(); //configuracion del timer 0
    
    adc_init(0,8); //configuracion del adc
    
    TMR0 = _trm0_value;//subrutina de reseteo
    return;
}


