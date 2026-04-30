/**
 * @file plib_mcp23s17_spi.c
 * @brief Bibliothèque SPI pour le module MCP23S17
 * @author Ramiro Najera
 * @version 1.0.8
 * @date 2025-04-23
 * @copyright Copyright (c) 2025
 */

#include "plib_mcp23s17_spi.h"
#include "peripheral/spi/spi_master/plib_spi2_master.h"
#include "peripheral/spi/spi_master/plib_spi1_master.h"

void MCP23S17_AttachFunctions(SPI_t *spi)
{
    if(spi->channel == SPI_CH1)
    {
        spi->Write = SPI1_Write;
        spi->Read = SPI1_Read;
    }
    else if(spi->channel == SPI_CH2)
    {
        spi->Write = SPI2_Write;
        spi->Read = SPI2_Read;
    }
}

void MCP23S17_Write(SPI_t *spi, uint8_t reg, uint8_t val)
{
    uint8_t TXBuffer[3] = {(MCP23S17_ADDRESS) | (spi->address << 1), reg, val};

    // Disable interrupts in critical part
    __builtin_disable_interrupts();
    
    // Send buffer, return error code
    spi->Write(TXBuffer, 3);
    
    // Enable interrupts after critical part
    __builtin_enable_interrupts();
}

uint8_t MCP23S17_Read(SPI_t *spi, uint8_t reg)
{
    uint8_t TXBuffer[2] = {((MCP23S17_ADDRESS) |(spi->address << 1) | 1), reg};
    uint8_t RXBuffer[1];

    // Disable interrupts in critical part
    __builtin_disable_interrupts();

    // Send buffer and read data according to SPI channel
    spi->Write(TXBuffer, 2);
    spi->Read(RXBuffer, 1);

    // Enable interrupts after critical part
    __builtin_enable_interrupts();
    return RXBuffer[0];
}