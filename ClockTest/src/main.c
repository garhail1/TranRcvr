/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <samd10_xplained_mini.h>

#define IO_1            PIN_PA22
#define IO_0            PIN_PA27

#define TC_COUNT_VALUE  2
#define BUFF_SIZE       1024

#ifdef  CHK_READER
#   define QUANT_FREQUENCY 2000
#else
#   define QUANT_FREQUENCY 2000
#endif

volatile const  uint16_t     buff_size = BUFF_SIZE;
volatile uint16_t   buffer[BUFF_SIZE];
volatile uint16_t*  buff = &buffer[0];
volatile uint16_t   head = 0, tail = 0,

#ifdef CHK_READER
    temp9 = 0, temp8 = 0;
#elif defined(CHK_WRITER)
    temp9 = 0b010010101, temp8 = 0b01010101;
#else
    temp;                    
#endif

volatile uint8_t    old_state = 0, new_state = 0;
volatile uint32_t   rate_flag = 0;


struct usart_module  usart_instance0;
struct usart_module  usart_instance1;
//struct tc_module     tc_instance;

//void SysTickInit()
//{
	//// Configure SysTick to trigger every time using the CPU Clock
	//SysTick->CTRL = 0;					// Disable SysTick
	//SysTick->LOAD = 999UL;				// Set reload register for interrupts
	//NVIC_SetPriority(SysTick_IRQn, 3);	// Set interrupt priority to least urgency
	//SysTick->VAL = 0;					// Reset the SysTick counter value
	//SysTick->CTRL = 0x00000007;			// Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
	//NVIC_EnableIRQ(SysTick_IRQn);		// Enable SysTick Interrupt   
//}

static inline bool SysTick_Init()
{
    struct port_config  pin_cfg;

    port_get_config_defaults(&pin_cfg);
   	pin_cfg.direction  = PORT_PIN_DIR_OUTPUT;

	port_pin_set_config( TOGGLE, &pin_cfg );
    
    if( SysTick_Config( SystemCoreClock/QUANT_FREQUENCY ))
    {
        //Error Code
        __ASM volatile ("nop");
        return false;
    }
    
    return true;
}

static inline void conigure_pins()
{
    struct port_config  pin_cfg;

#if defined(TRANSMITTER)
    port_get_config_defaults(&pin_cfg);
  	pin_cfg.direction  = PORT_PIN_DIR_INPUT;
    port_pin_set_config( WT_IO, &pin_cfg );
    
    port_get_config_defaults(&pin_cfg);
   	pin_cfg.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config( DIR_IO, &pin_cfg );
#endif

#if defined(RECEIVER)
    port_get_config_defaults(&pin_cfg);
  	pin_cfg.direction  = PORT_PIN_DIR_OUTPUT;
    port_pin_set_config( WT_IO, &pin_cfg );
    
    port_get_config_defaults(&pin_cfg);
   	pin_cfg.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config( DIR_IO, &pin_cfg );
#endif
    
}
    
void SysTick_Handler()
{
    rate_flag++;
#if     defined(CHK_WRITER)
    usart_write_wait( &usart_instance0, temp9 );
    usart_write_wait( &usart_instance1, temp8 );
#elif   defined(CHK_READER)
    //if( usart_read_wait( &usart_instance0, &temp9 ) == STATUS_OK )
        //usart_write_wait( &usart_instance0, temp9 );
    if( usart_read_wait( &usart_instance1, &temp8 ) == STATUS_OK )
        usart_write_wait( &usart_instance1, temp8 );
#elif   defined(TRANSMITTER)
    transmitter();
#elif   defined(RECEIVER)
    receiver();
#else
    port_pin_toggle_output_level( TOGGLE );
#endif
    rate_flag--;
}

/**
 *  Configure SERCOM0 TRANSMITTER
 */
static inline void configure_sercom0(void)
{
	struct usart_config     usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = SERCOM0_MUX_SETTING;
	usart_conf.pinmux_pad0 = SERCOM0_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = SERCOM0_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = SERCOM0_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = SERCOM0_PINMUX_PAD3;
	usart_conf.baudrate    = SERCOM0_BAUDRATE;
#ifdef TRANSMITTER
	//usart_conf.character_size = USART_CHARACTER_SIZE_9BIT;
#else
	usart_conf.character_size = USART_CHARACTER_SIZE_8BIT;
#endif

	while(usart_init(&usart_instance0, MODULE0, &usart_conf) != STATUS_OK);
	usart_enable(&usart_instance0);
}

static inline void configure_sercom1(void)
{
	struct usart_config     usart_conf;
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = SERCOM1_MUX_SETTING;
	usart_conf.pinmux_pad0 = SERCOM1_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = SERCOM1_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = SERCOM1_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = SERCOM1_PINMUX_PAD3;
    usart_conf.baudrate    = SERCOM1_BAUDRATE;
#ifdef RECEIVER
	//usart_conf.character_size = USART_CHARACTER_SIZE_9BIT;
#else
	usart_conf.character_size = USART_CHARACTER_SIZE_8BIT;
#endif

	while(usart_init(&usart_instance1, MODULE1, &usart_conf) != STATUS_OK);
	usart_enable(&usart_instance1);
}

/** TC Callback function.
 */

//static void tc_callback( struct tc_module *const module_inst)
//{
    //port_pin_toggle_output_level( TOGGLE );
//}

///** Configures  TC function with the  driver.
 //*/
//static void configure_tc(void)
//{
	//struct tc_config    config_tc;
    //struct port_config  pin_cfg;
//
	//tc_get_config_defaults(&config_tc);
	//config_tc.counter_size          = TC_COUNTER_SIZE_16BIT;
	//config_tc.counter_16_bit.value  = TC_COUNT_VALUE;
    //config_tc.clock_source          = GCLK_GENERATOR_0;
    //config_tc.clock_prescaler       = TC_CLOCK_PRESCALER_DIV1;
//
    //port_get_config_defaults(&pin_cfg);
   	//pin_cfg.direction  = PORT_PIN_DIR_OUTPUT;
//
	//port_pin_set_config( TOGGLE, &pin_cfg );
//
//
	//tc_init(&tc_instance, CONF_TC_INSTANCE, &config_tc);
    //tc_set_count_value( &tc_instance, TC_COUNT_VALUE );
	//tc_enable(&tc_instance);
//}
//
///** Registers TC callback function with the  driver.
 //*/
//static void configure_tc_callbacks(void)
//{
	//tc_register_callback( &tc_instance, tc_callback, TC_CALLBACK_OVERFLOW);
	//tc_enable_callback( &tc_instance, TC_CALLBACK_OVERFLOW );
//}

int main (void)
{
    system_init();
   
    //SysTickInit();

    //struct system_pinmux_config    cfg_pio_mux0;
    //system_pinmux_get_config_defaults( &cfg_pio_mux0 );
    //cfg_pio_mux0.direction   = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
    //cfg_pio_mux0.mux_position = PINMUX_PA27H_GCLK_IO0;
    //cfg_pio_mux0.input_pull   = SYSTEM_PINMUX_PIN_PULL_NONE;
    //system_pinmux_pin_set_config(PIN_PA27H_GCLK_IO0, &cfg_pio_mux0);
    //
    //struct system_pinmux_config    cfg_pio_mux1;
    //system_pinmux_get_config_defaults( &cfg_pio_mux1 );
    //cfg_pio_mux1.direction   = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
    //cfg_pio_mux1.mux_position = PINMUX_PA22H_GCLK_IO1;
    //cfg_pio_mux1.input_pull   = SYSTEM_PINMUX_PIN_PULL_NONE;
    //system_pinmux_pin_set_config(PIN_PA22H_GCLK_IO1, &cfg_pio_mux1);

    /*Configure SERCOMs.*/
    configure_sercom0();
    configure_sercom1();
    conigure_pins();

    /*Configures  TC driver*/
    //configure_tc();

    /*Configures TC callback*/
    //configure_tc_callbacks();

    /*Initialize the delay driver*/
    //delay_init();

    //while( usart_write_wait( &usart_instance0, '!'  ) != STATUS_OK);

    //while( usart_write_wait( &usart_instance1, temp ) != STATUS_OK);
    //while( usart_read_wait( &usart_instance1, &temp ) != STATUS_OK);

    if( !SysTick_Init(SystemCoreClock/QUANT_FREQUENCY) )
    {
        //Error Code
        __ASM volatile ("nop");
    }

    /*Enable system interrupt*/
    system_interrupt_enable_global();
     
    for( ;; )
    {
        //port_pin_toggle_output_level( TOGGLE );
    }
    /* Insert application code here, after the board has been initialized. */
}

