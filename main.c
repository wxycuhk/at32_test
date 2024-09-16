/* Note: 此代码只用于调试，具体结构后续优化 */
// Debug 2024/08/27: UR5 TX PB3 复用 mux10, rx PD2复用 mux8, 如需调整，建议更改为PB4(同复用mux10，可复用 Mux7与Tx切换至串口1)
// Debug 2024/09/04: 外部高速晶振频率为12MHz, 更改at32f423_conf内hext至12000000

#include "at32f423_clock.h"
#include "stdio.h"

#define MS_TICK  (system_core_clock / 1000U)

// UR5 TX: PB3      RX: PD2
/**************** define print uart ******************/
#define PRINT_UART                       USART5
#define PRINT_UART_CRM_CLK               CRM_USART5_PERIPH_CLOCK
#define PRINT_UART_TX_PIN                GPIO_PINS_3
#define PRINT_UART_TX_GPIO               GPIOB
#define PRINT_UART_TX_GPIO_CRM_CLK       CRM_GPIOB_PERIPH_CLOCK
#define PRINT_UART_TX_PIN_SOURCE         GPIO_PINS_SOURCE3
#define PRINT_UART_TX_PIN_MUX_NUM        GPIO_MUX_10


void systick_init(void)
{
	systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
	SysTick_Config(MS_TICK);
}

static uint32_t sys_tick_ms = 0;
void ms_polling(void)
{
	if(sys_tick_ms > 0)
	{
		sys_tick_ms--;
	}
}
void delay_ms(uint32_t ms)
{
	sys_tick_ms = ms;
	
}
// UR5 TX: PB3      RX: PD2
void uart_print_init(uint32_t baudrate)
{
  gpio_init_type gpio_init_struct;

#if defined (__GNUC__) && !defined (__clang__)
  setvbuf(stdout, NULL, _IONBF, 0);
#endif

  /* enable the uart and gpio clock */
  crm_periph_clock_enable(PRINT_UART_CRM_CLK, TRUE);
  crm_periph_clock_enable(PRINT_UART_TX_GPIO_CRM_CLK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the uart tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = PRINT_UART_TX_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(PRINT_UART_TX_GPIO, &gpio_init_struct);

  gpio_pin_mux_config(PRINT_UART_TX_GPIO, PRINT_UART_TX_PIN_SOURCE, PRINT_UART_TX_PIN_MUX_NUM);

  /* configure uart param */
  usart_init(PRINT_UART, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(PRINT_UART, TRUE);
  usart_enable(PRINT_UART, TRUE);
}

int fputc(int ch, FILE *f)
{
  while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(PRINT_UART, (uint16_t)ch);
  //while(usart_flag_get(PRINT_UART, USART_TDC_FLAG) == RESET);
  return ch;
}

int main(void)
{
  system_clock_config();
	
	uart_print_init(115200);

  while(1)
  {
		printf("Hello\r\n");
		delay_ms(1000);
		printf("%d\r\n", sys_tick_ms);
		printf("Test\r\n");
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
  }
}

