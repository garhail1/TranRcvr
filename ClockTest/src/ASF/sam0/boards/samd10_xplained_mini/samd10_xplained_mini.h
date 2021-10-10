/**
 *
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef SAMD10_XPLAINED_MINI_H_INCLUDED
#define SAMD10_XPLAINED_MINI_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup samd10_xplained_mini_group SAM D10 Xplained Mini board
 *
 * @{
 */

void system_board_init(void);

/**
 * \defgroup samd10_xplained_mini_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** Name string macro */
#define BOARD_NAME                "SAMD10_XPLAINED_MINI"

/** \name Resonator definitions
 *  @{ */
#define BOARD_FREQ_SLCK_XTAL      0 /* Not Mounted */
#define BOARD_FREQ_SLCK_BYPASS    0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_XTAL    0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_BYPASS  0 /* Not Mounted */
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625
/** @} */

#define BOUDRATE            9600    //  460800 

#define MODULE0             SERCOM0
#define SERCOM0_MUX_SETTING USART_RX_3_TX_0_XCK_1
#define SERCOM0_PINMUX_PAD0 PINMUX_PA14C_SERCOM0_PAD0
#define SERCOM0_PINMUX_PAD1 PINMUX_PA15C_SERCOM0_PAD1
#define SERCOM0_PINMUX_PAD2 PINMUX_UNUSED
#define SERCOM0_PINMUX_PAD3 PINMUX_UNUSED
#define SERCOM0_BAUDRATE    BOUDRATE 
#define CONF_TC_INSTANCE    TC2
/** @} */

#define MODULE1             SERCOM1
#define SERCOM1_MUX_SETTING USART_RX_3_TX_2_XCK_3
#define SERCOM1_PINMUX_PAD0 PINMUX_UNUSED
#define SERCOM1_PINMUX_PAD1 PINMUX_UNUSED
#define SERCOM1_PINMUX_PAD2 PINMUX_PA24C_SERCOM1_PAD2
#define SERCOM1_PINMUX_PAD3 PINMUX_PA25C_SERCOM1_PAD3
#define SERCOM1_BAUDRATE    BOUDRATE

#define TOGGLE              PIN_PA11
#define WT_IO               PIN_PA02
#define DIR_IO              PIN_PA04

#define MASK_STATE10        0x0010
#define MASK_STATE01        0x0001
#define SYMPTOM_PIN_IO      0x0100

#define STATE00     1
#define STATE01     MASK_STATE01 + 1 
#define STATE10     MASK_STATE10 + 1 
#define STATE11     (MASK_STATE10 | MASK_STATE01) + 1
    

void    transmitter();
void    receiver();

#ifdef __cplusplus
}
#endif

#endif  /* SAMD10_XPLAINED_MINI_H_INCLUDED */
