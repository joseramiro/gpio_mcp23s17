/**
 * @file plib_mcp23s17_spi.c
 * @brief Bibliothèque SPI pour le module MCP23S17
 * @author Ramiro Najera
 * @version 1.0.1
 * @date 2025-04-23
 * @copyright Copyright (c) 2025
 */

#include "plib_mcp23s17_spi.h"
#include "peripheral/spi/spi_master/plib_spi2_master.h"
#include "peripheral/spi/spi_master/plib_spi1_master.h"

void MCP23S17_Write(SPI_t *spi, unsigned char reg, unsigned char val)
{
    unsigned char TXBuffer[3] =
    {
        (MCP23S17_ADDRESS) | (spi->address << 1),
        reg,
        val
    };

    // Disable interrupts in critical part
    __builtin_disable_interrupts();
    
    // Send buffer, return error code
    if(spi->channel == SPI_CH1)
        SPI1_Write(TXBuffer, 3);
    else if(spi->channel == SPI_CH2)
        SPI2_Write(TXBuffer, 3);
    
    // Enable interrupts after critical part
    __builtin_enable_interrupts();
}

unsigned char MCP23S17_Read(SPI_t *spi, unsigned char reg)
{
    unsigned char TXBuffer[3] =
    {
        ((MCP23S17_ADDRESS) |(spi->address << 1) | 1),
        TXBuffer[1] = reg,
        TXBuffer[2] = MCP23S17_DUMMY_DATA
    };
    
    // Disable interrupts in critical part
    __builtin_disable_interrupts();

    unsigned char RXBuffer[2];

    // Send buffer and read data according to SPI channel
    if(spi->channel == SPI_CH1)
    {
        SPI1_Write(TXBuffer, 2);
        SPI1_Read(RXBuffer, 1);
    }
    else if(spi->channel == SPI_CH2)
    {
        SPI2_Write(TXBuffer, 2);
        SPI2_Read(RXBuffer, 1);
    }

    // Enable interrupts after critical part
    __builtin_enable_interrupts();
    return RXBuffer[0];
}