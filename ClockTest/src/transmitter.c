/*
 * transmitter.c
 *
 * Created: 20.09.2021 16:44:12
 *  Author: garhail
 */ 

#include <compiler.h>
#include <usart.h>
#include <board.h>
#include <port.h>
#include <conf_board.h>

void transmitter()
{
    if( read_data8() )
    {
        write_data9();
        return;
    }

    bool wt_state   = port_pin_get_input_level( WT_IO  );
    bool dir_state  = port_pin_get_input_level( DIR_IO );
    uint8_t tmp_state = 0;

    if( wt_state ) 
        if( dir_state )
            tmp_state = STATE11;
        else
            tmp_state = STATE10;
    else
        if( dir_state )
            tmp_state = STATE01;
        else
            tmp_state = STATE00;
            
    if( new_state != tmp_state )
    {
        old_state = new_state;
        new_state = tmp_state;
        
        write_at_buf_tail( tmp_state | SYMPTOM_PIN_IO );
        write_data9();
    }
}    


