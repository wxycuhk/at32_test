/*移植自正点原子ADC文件，由于数据手册中没有ADC_INx -- GPIO对应关系，按照STM32处理， 此处调用PB0, PB1 即ADC1 8/9 通道*/
/* 更改代码加入DMA, 提高采样率 */
#include "at32f423.h"
#include "sys.h"
#include "delay.h"
#include "at32f423_adc.h"

__IO uint16_t 

void ADC_DMA_Init(void){
    /* config the dma configuration*/
    dma_init_type dma_init_struct;
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    nvic_irq_enable(DMA1_Channel1_IRQn, 0, 0);

    dma_reset(DMA1_CHANNEL1);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.buffer_size = 30;
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_base_addr = (uint32_t)adc1_ordinary_valuetab;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC1->odt);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL1, &dma_init_struct);

    dmamux_enable(DMA1, TRUE);
    dmamux_init(DMA1MUX_CHANNEL1, DMAMUX_DMAREQ_ID_ADC1);

    /* enable dma transfer complete interrupt */
    dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);
    dma_channel_enable(DMA1_CHANNEL1, TRUE);



    /* ADC Structure init */
    adc_common_config_type adc_common_struct;
    adc_base_config_type adc_base_struct;
    crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
    nvic_irq_enable(ADC1_IRQn, 0, 0);
    crm_adc_clock_select(CRM_ADC_CLOCK_SOURCE_HCLK);

    adc_common_default_para_init(&adc_common_struct);

    adc_common_struct.div = ADC_HCLK_DIV_4;
    adc_common_struct.tempervintrv_state = FALSE;

    adc_common_config(&adc_common_struct);
    adc_base_default_para_init(&adc_base_struct);

    adc_base_struct.sequence_mode = TRUE;
    adc_base_struct.repeat_mode = TRUE;
    adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
    adc_base_struct.ordinary_channel_length = 3;
    adc_base_config(ADC1, &adc_base_struct);
    adc_resolution_set(ADC1, ADC_RESOLUTION_12B);

    /*config ordinary channel*/
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_8, 1, ADC_SAMPLETIME_2_5);
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9, 2, ADC_SAMPLETIME_2_5);

    adc_ordinary_conversion_trigger_set(ADC1, ADC_ORDINARY_TRIG_SOFTWARE, ADC_ORDINARY_TRIG_EDGE_NONE); //软件触发
    adc_dma_mode_enable(ADC1, TRUE);

    adc_dma_request_repeat_enable(ADC1, FALSE);
    adc_interrupt_enable(ADC1, ADC_OCCO_INT, TRUE);

      /* adc enable */
    adc_enable(ADC1, TRUE);
    while(adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET);

    adc_calibration_init(ADC1);
    while(adc_calibration_init_status_get(ADC1));
    adc_calibration_init(ADC1);  //双通道校准
    while(adc_calibration_status_get(ADC1));

    while(adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET);

}

