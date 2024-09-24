#include "74hc164.h"
/*delay 已验证，基于delay输出时序*/

void hc164_init(void)
{
    gpio_init_type  GPIO_InitStructure;

    /* pc10, pb2, pc12 GPIO clock enable */
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK|CRM_GPIOC_PERIPH_CLOCK, TRUE);	 //ʹ��PB,PE�˿�ʱ��

    /*gpio default paramater*/
    gpio_default_para_init(&GPIO_InitStructure);

    /*config the gpios*/
    GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    GPIO_InitStructure.gpio_pins = GPIO_PINS_10|GPIO_PINS_12;  // PC10, PC12: PC 10 PULSE PORT, PC12 DATA PORT
    GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL; 	 
    GPIO_InitStructure.gpio_mode = GPIO_MODE_OUTPUT;
    GPIO_InitStructure.gpio_pull = GPIO_PULL_UP;
    gpio_init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    GPIO_InitStructure.gpio_pins = GPIO_PINS_2;               // PB2: PB 2 CLEAR PORT
    GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL; 	 
    GPIO_InitStructure.gpio_mode = GPIO_MODE_OUTPUT;
    GPIO_InitStructure.gpio_pull = GPIO_PULL_UP;
    gpio_init(GPIOB, &GPIO_InitStructure);

}
void ShiftOut(uint8_t data)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        if(data&(1 << (7 - i))) //从高位开始传送
        {
            HC164_DAT = 1;
        }
        else
        {
            HC164_DAT = 0;
        }
        HC164_CLK_HIGH;
        HC164_CLK_LOW;
    }
}

