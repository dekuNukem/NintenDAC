if(linear_buf_line_available(&debug_lb))
		{
		  printf("debug_lb: %s\n", debug_lb.buf);

		  if(strstr(debug_lb.buf, "s ") != NULL)
		  {
		  	uint8_t to_send = get_arg(debug_lb.buf, 0);
		  	uint8_t received = 0;
		  	printf("sending %d...\n", to_send);
		  	spi_cs_low();
		  	HAL_SPI_TransmitReceive(max572x_spi_ptr, &to_send, &received, 1, 500);
		  	spi_cs_high();
		  	printf("received: %d\n", received);
		  }
		  linear_buf_reset(&debug_lb);
		}


  SPI2_CS_GPIO_Port->ODR |= 0x1000; // CS high
  SPI2_CS_GPIO_Port->ODR &= 0xefff; // CS low


void max572x_POWER(uint8_t dac_multi_sel, uint8_t power_mode)
{
  uint8_t power = (uint8_t)((power_mode << 6) & 0xc0);
  printf("power: 0x%X\n", power);
  // uint8_t to_send[3] = {0x40, dac_multi_sel, (power_mode << 6) & 0xc0};
}

  printf("sending 0x%X 0x%X 0x%X...\n", to_send[0], to_send[1], to_send[2]);
void max572x_POWER(uint8_t dac_multi_sel, uint8_t power_mode)
{
  uint8_t to_send[3] = {0x40, dac_multi_sel, (uint8_t)((power_mode << 6) & 0xc0)};
  spi_cs_low();
  HAL_SPI_Transmit(max572x_spi_ptr, to_send, 3, 100);
  spi_cs_high();
}

void read_status()
{
  uint8_t spi_sb[3] = {0xe8, 0xaa, 0xaa};
  uint8_t spi_rb[3] = {0, 0, 0};
  printf("sending 0x%X 0x%X 0x%X...\n", spi_sb[0], spi_sb[1], spi_sb[2]);
  spi_cs_low();
  HAL_SPI_TransmitReceive(max572x_spi_ptr, spi_sb, spi_rb, 3, 500);
  spi_cs_high();
  printf("spi_rb: 0x%X 0x%X 0x%X\n", spi_rb[0], spi_rb[1], spi_rb[2]);
}

void read_status()
{
  uint8_t spi_sb[3] = {0xe8, 0xaa, 0xaa};
  uint8_t spi_rb[3] = {0, 0, 0};
  printf("sending: ");
  print_3b(spi_sb);
  spi_cs_low();
  HAL_SPI_TransmitReceive(max572x_spi_ptr, spi_sb, spi_rb, 3, 500);
  spi_cs_high();
  printf("received: ");
  print_3b(spi_rb);
}

void max5725_CODEn(uint8_t dac_sel, uint16_t data)
{
  uint8_t spi_sb[3] = {0x80, 0, 0};
  dac_sel &= 0xf;
  spi_sb[0] |= dac_sel;
  data &= 0xfff;
  spi_sb[1] |= data >> 4;
  data &= 0xf;
  spi_sb[2] |= (data << 4);
  spi_cs_low();
  HAL_SPI_Transmit(max572x_spi_ptr, spi_sb, 3, 100);
  spi_cs_high();
  // printf("max5725_CODEn: "); print_3b(spi_sb);
}

void max5723_CODEn(uint8_t dac_sel, uint8_t data)
{
  uint8_t spi_sb[3] = {0x80, 0, 0};
  dac_sel &= 0xf;
  spi_sb[0] |= dac_sel;
  spi_sb[1] = data;
  spi_cs_low();
  HAL_SPI_Transmit(max572x_spi_ptr, spi_sb, 3, 100);
  spi_cs_high();
  // printf("max5723_CODEn: "); print_3b(spi_sb);
}

void pack_dac_val(uint8_t part, uint16_t value, uint8_t* dh, uint8_t* dl)
{
  if(part == MAX5723)
    *dh = (value & 0xff);
  else if(part == MAX5724)
  {
    value &= 0x3ff;
    *dh |= value >> 2;
    value &= 0x3;
    *dl |= (value << 6);
  }
  else
  {
    value &= 0xfff;
    *dh |= value >> 4;
    value &= 0xf;
    *dl |= (value << 4);
  }
}