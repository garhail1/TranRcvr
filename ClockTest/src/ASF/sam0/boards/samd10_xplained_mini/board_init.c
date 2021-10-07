/**
 * \file
 *
 * \brief SAM D10 Xplained Mini board initialization
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <compiler.h>
#include <board.h>
#include <conf_board.h>
#include <port.h>
#include <memory.h>

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void system_board_init(void)
{
    tail = head = 0;
    memset( (void*)buff, 0, buff_size );
}

void adjust_buf_head( uint16_t data_to_send )
{
    if( head >= tail )
    {
        if( ++head == buff_size ) head = 0;
        buff[head] = data_to_send;
    }
    else
        buff[++head] = data_to_send;
    
}

bool adjust_buf_tail( uint16_t* data_to_read )
{
    if( tail < head )
    {
        *data_to_read = buff[tail++];
        if( tail == buff_size ) tail = 0;
    }
    else if( tail == head )
        return false;
    else
    {
        *data_to_read = buff[head++];
        if( head == buff_size) head = 0;
    }
    return true;
}

void write_at_buf_tail( uint16_t spec_data )
{
    spec_data |= 0x0100;
    if( 0 == tail)
        tail = buff_size - 1;
    else
        tail--;
        
    buff[tail] = spec_data;
}

//void write_at_head( uint16_t spec_data )
//{
    //spec_data |= 0x0100;
    //buff[head] = spec_data;
//}

