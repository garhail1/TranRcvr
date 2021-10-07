/*
 * trio.c
 *
 * Created: 17.09.2021 15:18:43
 *  Author: garhail
 */
 
#include <compiler.h>
#include <usart.h>
#include <board.h>
#include <conf_board.h>
#include <port.h>
#include <samd10_xplained_mini.h>


bool    read_data( struct usart_module *const module, bool bit9 )
{
    uint16_t tmp = 0;
    if( usart_read_wait( module, &tmp ) == STATUS_OK )
    {
        if( !bit9 )
            tmp &= 0xff;
        adjust_buf_head( tmp );
        return true;
    }
    
    return false;
}

bool    write_data( struct usart_module *const module, bool bit9  )
{
    if( is_data_to_write() )
    {
        uint16_t tmp = 0;
        if( adjust_buf_tail(&tmp) )
        {
            if( !bit9 )
                tmp &= 0xff;
            while( usart_write_wait( module, tmp ) != STATUS_OK );
            return true;
        }
    }
    return false;
}


