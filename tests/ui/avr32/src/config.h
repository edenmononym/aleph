#ifndef _CONFIG_H_
#define _CONFIG_H_

///// UASRT1 in spi mode for OLED
#define OLED_USART_SPI                 (&AVR32_USART1)
// clock is PA07
#define OLED_USART_SPI_SCK_PIN         AVR32_USART1_CLK_0_PIN
#define OLED_USART_SPI_SCK_FUNCTION    AVR32_USART1_CLK_0_FUNCTION
// rx is PA05
#define OLED_USART_SPI_MISO_PIN        AVR32_USART1_RXD_0_0_PIN
#define OLED_USART_SPI_MISO_FUNCTION   AVR32_USART1_RXD_0_0_FUNCTION
// tx is PA06 
#define OLED_USART_SPI_MOSI_PIN        AVR32_USART1_TXD_0_0_PIN
#define OLED_USART_SPI_MOSI_FUNCTION   AVR32_USART1_TXD_0_0_FUNCTION
// rts (chip select) is PA08
#define OLED_USART_SPI_NSS_PIN         AVR32_USART1_RTS_0_0_PIN
#define OLED_USART_SPI_NSS_FUNCTION    AVR32_USART1_RTS_0_0_FUNCTION
// PA09 in GPIO for OLED command/data register select
#define OLED_REGISTER_PIN AVR32_PIN_PA09

#endif // header guard
