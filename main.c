/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <stdio.h> 

#endif

#include "can.h"
#include "uart.h"
//#include "i2c.h"
#include "user.h"
#include "config.h"
//#include "interrupts.h"
#include "ADC_Config.h"    /* ADC Channel selects and read function */


/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */
// Adjust to system clock
#define _XTAL_FREQ 16000000

Message flash;
Message nodeA;
Message nodeB;
Message nodeC;
Message nodeD;
//Message nodeE;
//Message nodeF;

char dataA[8];
char dataB[8];
char dataC[8];
char dataD[8];
//char dataE = '';
//char dataF = '';

//initialize all methods
void sendC(char command);
void sendD(char command);

void main(void)
{
    /* Initialization functions called below */
    ConfigureOscillator();
    
    uart_init(9600);
    ecan_init();
    //i2c_init();   
    //setAnalogIn();
     
    TRISAbits.TRISA0 = 0;    //makes through while loop
    TRISAbits.TRISA2 = 0;    //node a talk back  
    TRISAbits.TRISA4 = 0;    //node b talk back
    TRISAbits.TRISA6 = 0;    //node c talk back
    TRISCbits.TRISC0 = 0;    //flashes if we receive correct data  

    int wait = 10;
   
    while(1)
    {       
        flash.sid = 0x00A;
        flash.len = 1;
        flash.data[0] = 0x02;
        ecan_send(&flash);
                
            while(!(RXB0CONbits.RXB0FUL)){}; //wait for receiving 
            ecan_receive_rxb0(&nodeA);
            LATAbits.LATA2 ^= 1;
                for (int i = 0; i < 8; i++) 
                {
                    dataA[i] = nodeA.data[i];
                }
            //dataA = nodeA.data[0];
            ecan_rxb0_clear();          // Clear flag   
            wait_ms(wait);

        
        flash.sid = 0x00B;
        flash.len = 1; 
        flash.data[0] = 0x02;
        ecan_send(&flash);
        
            while(!(RXB0CONbits.RXB0FUL)){};
            ecan_receive_rxb0(&nodeB);
            LATAbits.LATA4 ^= 1;
            for (int i = 0; i < 8; i++) 
            {
                dataB[i] = nodeB.data[i];
            }
            ecan_rxb0_clear();          // Clear flag   
            wait_ms(wait);       
       
//        flash.sid = 0x00C;
//        flash.len = 1;
//        flash.data[0] = 0x02;                
//        ecan_send(&flash);
//        
//            while(!(RXB0CONbits.RXB0FUL)){};
//            ecan_receive_rxb0(&nodeC);
//            LATAbits.LATA6 ^= 1;
//            ecan_rxb0_clear();          // Clear flag   
//            wait_ms(wait); 
            
//        flash.sid = 0x00D;
//        flash.len = 1;
//        flash.data[0] = 0x02;                
//        ecan_send(&flash);
//        
//            while(!(RXB0CONbits.RXB0FUL)){};
//            ecan_receive_rxb0(&nodeD);
//            LATAbits.LATA6 ^= 1;
//            ecan_rxb0_clear();          // Clear flag   
//            wait_ms(wait);
                                
        
        
        uart_write(dataA[0]);
        uart_write(dataA[1]);
        uart_write(dataA[2]);
        uart_write(dataA[3]);
        uart_write(dataB[0]);
        uart_write(dataB[1]);
        
        
        // attempt to read data from PI
        if(uart_data_ready()) {
            char message = uart_read();
            if(message == 'e' || message == 'r' || message == 'm' || message == '+' || message == '-'){
                sendC(message);
            } else if( message == '0' || message == '5' || message == '9')
                sendD(message);
            else{
                //do nothing for now
            }
        }    
        wait_ms(wait);        
    }
}

void sendC(char command){
        flash.sid = 0x00C;
        flash.len = 1; 
        flash.data[0] = command;
        ecan_send(&flash);
        LATCbits.LATC0 ^= 1;
}

void sendD(char command){
        flash.sid = 0x00D;
        flash.len = 1; 
        flash.data[0] = command;
        ecan_send(&flash);
        LATCbits.LATC0 ^= 1;
}