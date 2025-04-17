/**
 * @file plib_mcp23s17_spi.c
 * @brief Bibliothèque SPI pour le module MCP23S17
 * @author Ramiro Najera
 * @version 1.0
 * @date 2025-03-18
 * @copyright Copyright (c) 2025
 */

#include "plib_mcp23s17_spi.h"
#include "peripheral/spi/spi_master/plib_spi2_master.h"
#include "peripheral/spi/spi_master/plib_spi1_master.h"

/* ==== Fonctions Génériques ==== */

unsigned char MCP23S17_Write(SPIConfiguration_t *spi, unsigned char reg, unsigned char val)
{
    unsigned char errorCode = 0;
    unsigned char TXBuffer[3];

    // Disable interrupts in critical part
    __builtin_disable_interrupts();
    
    // Populate buffer
    TXBuffer[0] = (MCP23S17_ADDRESS) | (spi->address << 1);
    TXBuffer[1] = reg;
    TXBuffer[2] = val;
    
    // Send buffer, return error code
    if(spi->channel == SPI_CH1)
        errorCode = SPI1_Write(TXBuffer, 3);
    else if(spi->channel == SPI_CH2)
        errorCode = SPI2_Write(TXBuffer, 3);
    
    // Enable interrupts after critical part
    __builtin_enable_interrupts();
    return errorCode;
}

unsigned char MCP23S17_Read(SPIConfiguration_t *spi, unsigned char reg)
{
    unsigned char TXBuffer[3];
    unsigned char RXBuffer[2];

    // Disable interrupts in critical part
    __builtin_disable_interrupts();

    // Populate buffer
    TXBuffer[0] = ((MCP23S17_ADDRESS) |(spi->address << 1) | 1);
    TXBuffer[1] = reg;
    TXBuffer[2] = MCP23S17_DUMMY_DATA;

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