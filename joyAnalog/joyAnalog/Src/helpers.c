#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"
#include "shared.h"

int32_t linear_buf_init(linear_buf *lb, int32_t size)
{
  lb->buf_size = size;
  lb->buf = malloc(size);
  while(lb->buf == NULL)
  {
    printf("out of memory\n");
    HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
    HAL_Delay(250);
  }
  lb->last_recv = 0;
  linear_buf_reset(lb);
  return 0;
}

void linear_buf_reset(linear_buf *lb)
{
  lb->curr_index = 0;
  memset(lb->buf, 0, lb->buf_size);
}

int32_t linear_buf_add(linear_buf *lb, uint8_t c)
{
  lb->buf[lb->curr_index] = c;
  if(lb->curr_index < lb->buf_size)
    lb->curr_index++;
  lb->buf[lb->buf_size-1] = 0;
  lb->last_recv = HAL_GetTick();
  return 0;
}

int32_t linear_buf_add_str(linear_buf *lb, uint8_t *s, uint32_t len)
{
  for(uint32_t i = 0; i < len; i++)
    linear_buf_add(lb, s[i]);
  return 0;
}

int32_t linear_buf_line_available(linear_buf *lb)
{
  if(lb->curr_index >= lb->buf_size)
  {
    linear_buf_reset(lb);
    return 0;
  }
  if(lb->buf[lb->curr_index - 1] == '\n')
    return 1;
  return 0;
}

int32_t linear_buf_idle(linear_buf *lb, int32_t timeout)
{
  if(lb->curr_index >= lb->buf_size)
  {
    linear_buf_reset(lb);
    return 0;
  }
  if(lb->curr_index > 0 && HAL_GetTick() - lb->last_recv > timeout)
    return 1;
  return 0;
}

// 1.2V reading
uint16_t get_vref(void)
{
  uint16_t result;
  HAL_ADC_Start(adc_ptr);
  if(HAL_ADC_PollForConversion(adc_ptr, 100) == HAL_OK)
    result = HAL_ADC_GetValue(adc_ptr);
  HAL_ADC_Stop(adc_ptr);
  return result;
}
