#include "Multiplex.h"

int separacionL(int valueADC){  //separar los 4 bits menos significativos de la conversion del ADC (cadena de 8 bits)
    
    int bitsL = 0x0F; // 0000 1111    
    int grupoL = (valueADC & bitsL);
      
    return grupoL;
    
}

int separacionH(int valueADC){  //separar los 4 bits mas significativos de la conversion del ADC (cadena de 8 bits)
    
    int bitsH = 0XF0; // 1111 0000
    int grupoH = (valueADC & bitsH)>>4;
      
    return grupoH;
    
}

uint8_t tablaHex(int separacion){ //conversion de notacion binaria a Hex (cadena de 4 bits)
    switch(separacion){
        case 0:
            return 0b00111111;  //0
            break;
        case 1:
            return 0b00000110;  //1
            break;
        case 2:
            return 0b01011011;  //2
            break;
        case 3:
            return 0b01001111;  //3
            break;
        case 4:
            return 0b01100110;  //4
            break;
        case 5:
            return 0b01101101;  //5
            break;
        case 6:
            return 0b01111101;  //6
            break;
        case 7:
            return 0b00000111;  //7
            break;
        case 8:
            return 0b01111111;  //8
            break;
        case 9:
            return 0b01101111;  //9
            break;
        case 10:
            return 0b01110111;  //A
            break;
        case 11:
            return 0b01111100;  //b
            break;
        case 12:
            return 0b00111001;  //C
            break;
        case 13:
            return 0b01011110;  //d
            break;
        case 14:
            return 0b01111001;  //E
            break;
        case 15:
            return 0b01110001;  //F
            break;
        default:
            return 0b00111111;  //0
            break;
    }
    
}