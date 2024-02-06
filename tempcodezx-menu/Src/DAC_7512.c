#include "DAC_7512.h"
 
void delay7512_us(volatile uint16_t us)
{    
	
    while(us > 0)
    {
        us --; 
    }
}

void DAC_7512_Write( unsigned short data )
{
     unsigned char i = 0 ;
     SYNC_LOW ;
     data &= 0x0FFF ;
     for( i = 0 ; i < 16 ; i++ )
     {
         delay7512_us( 20 ) ;
         DAC_SCK_HIGH ;
         if( 0x8000 == ( data&0x8000 ) )
         {
             DAC_DATA_HIGH ;
         }
         else
         {
             DAC_DATA_LOW ;
         }
         data <<= 1 ;
         delay7512_us( 20 ) ;
         DAC_SCK_LOW ;
     }
     SYNC_HIGH ;

}