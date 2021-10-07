/*
 * receiver.c
 *
 * Created: 20.09.2021 16:40:18
 *  Author: garhail
 */ 
#include <compiler.h>
#include <usart.h>
#include <board.h>
#include <port.h>
#include <conf_board.h>

void receiver()
{
    if( read_data9() )
    {
        if( is_data_has_pin_io() )
        {
            uint16_t tmp = 0;
            if( adjust_buf_tail( &tmp ) )
            {
                tmp &= ~SYMPTOM_PIN_IO;
                tmp--;
                if( tmp & MASK_STATE10 )
                {
                    port_pin_set_output_level( WT_IO, true );
                    if( tmp & MASK_STATE01 )
                    {
                        port_pin_set_output_level( DIR_IO, true );
                    }
                    else
                    {
                        port_pin_set_output_level( DIR_IO, false );
                    }
                }                    
                else
                {
                    port_pin_set_output_level( WT_IO, false );
                    if( tmp & MASK_STATE01 )
                    {
                        port_pin_set_output_level( DIR_IO, true );
                    }
                    else
                    {
                        port_pin_set_output_level( DIR_IO, false );
                    }
                }
            }                                    
        }
        else
        {
            write_data8();
        }            
    }
}    

