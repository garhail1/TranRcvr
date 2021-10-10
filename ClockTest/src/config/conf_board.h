/**
 * \file
 *
 * \brief SAM D10 Xplained Mini board configuration.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

#include <samd10_xplained_mini.h>

extern  volatile const  uint16_t    buff_size;
extern  volatile uint16_t*  buff;
extern  volatile uint16_t   head;
extern  volatile uint16_t   tail;

extern  volatile uint32_t   rate_flag;
extern  volatile uint8_t    old_state, new_state;

extern  struct usart_module  usart_instance0;
extern  struct usart_module  usart_instance1;

void adjust_buf_head( uint16_t data_to_send );
bool adjust_buf_tail( uint16_t* data_to_read );
void write_at_buf_tail( uint16_t spec_data );
//void write_at_head( uint16_t spec_data );


bool read_data( struct usart_module *const module, bool bit9 );
bool write_data( struct usart_module *const module, bool bit9 );

static inline bool is_data_to_write()
{
    return tail != head;
}

static inline uint16_t  data_to_write()
{
    return buff[tail];
}

static inline bool is_data_has_pin_io()
{
    if( tail != head )
    {
        if( buff[head] & SYMPTOM_PIN_IO )
            return true;
        else
            return false;
    }
    else
        return false;
}

static inline bool read_data8()
{
#ifdef  TRANSMITTER
   return read_data( &usart_instance1, false );
#else
   return read_data( &usart_instance0, false );
#endif
}

static inline bool read_data9()
{
#ifdef  TRANSMITTER
   return read_data( &usart_instance0, true );
#else
   return read_data( &usart_instance1, true );
#endif
}

static inline bool write_data8()
{
#ifdef  TRANSMITTER
   return write_data( &usart_instance1, false );
#else
   return write_data( &usart_instance0, false );
#endif
}

static inline bool write_data9()
{
#ifdef  TRANSMITTER
   return write_data( &usart_instance0, true );
#else
   return write_data( &usart_instance1, true );
#endif
}

#endif /* CONF_BOARD_H_INCLUDED */
