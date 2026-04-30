/**
 * @file plib_mcp23s17.c
 * @brief Pilote pour le MCP23S17 (Expander I/O SPI)
 * @author Ramiro Najera
 * @version 1.0.8
 * @date 2025-03-18
 */

#include <stddef.h>
#include "plib_mcp23s17_spi.h"
#include "plib_mcp23s17.h"
#include "libs/common_c_libs/plib_data_struct.h"

/* ==== Fonctions Chip Select ==== */

void MCP23S17_StartTranmission(SPI_t *spi)
{
    // Check function before  calling it
    if(spi->pinEN.Clear != NULL)
        spi->pinEN.Clear();
    if(spi->pinCS.Clear != NULL) 
        spi->pinCS.Clear();
}
 
void MCP23S17_EndTranmission(SPI_t *spi)
{
    // Check function before calling it
    if(spi->pinCS.Set != NULL)
        spi->pinCS.Set();
    if(spi->pinEN.Set != NULL)
        spi->pinEN.Set();
}

/* ==== Fonctions de base ==== */

uint8_t MCP23S17_InitChip(MCP23S17_t *obj)
{
    // Attach Write and Read SPI functions according to SPI channel
    MCP23S17_AttachFunctions(&obj->spi);
    // Set init configuration (conf, pullup, direction, interrupt)
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_IOCON_A,   CONCAT(obj->confB.reg, obj->confA.reg)))        return 1;
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_GPPU_A,    CONCAT(obj->gppuB.reg, obj->gppuA.reg)))        return 1;
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_IODIR_A,   CONCAT(obj->dirB.reg, obj->dirA.reg)))          return 1;
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_DEFVAL_A,  0x00))                                          return 1;
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_INTCON_A,  0x00))                                          return 1;
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_GPINTEN_A, CONCAT(obj->gpintenB.reg, obj->gpintenA.reg)))  return 1;
    // Read ports to process INT trigger and update ports
    if(obj->dirA.reg == MCP23S17_PORT_OUTPUT_MODE)
        MCP23S17_WriteCheckSingleRegister(&obj->spi, MCP23S17_REG_GPIO_A, obj->initPortA);
    if(obj->dirB.reg == MCP23S17_PORT_OUTPUT_MODE)
        MCP23S17_WriteCheckSingleRegister(&obj->spi, MCP23S17_REG_GPIO_B, obj->initPortB);
    // No errors
    return 0;
}

uint8_t MCP23S17_InitList(MCP23S17_t *objList, uint8_t size)
{
    uint8_t error = 0;    
    // Init each MCP23S17 module
    for(uint8_t i = 0; i < size; i++)
    {
        if(MCP23S17_InitChip(&objList[i]))
            SET_FLAG_BIT(error, i);
    }
    return error;
}

void MCP23S17_EnableHWAddress(MCP23S17_t *obj)
{
    // Set configuration in mask, send register
    MCP23S17IOCONReg_t mask;
    mask.bits.haen = MCP23S17_IOCON_HAEN_ENABLED;
    MCP23S17_WriteDoubleRegister(&obj->spi, MCP23S17_REG_IOCON_A, CONCAT(mask.bits.haen, mask.bits.haen));
}

void MCP23S17_WriteSingleRegister(SPI_t *spi, MCP23S17Reg_t reg, uint8_t value)
{
    // Write single register
    MCP23S17_StartTranmission(spi);
    MCP23S17_Write(spi, reg, value);
    MCP23S17_EndTranmission(spi);
}

void MCP23S17_WriteDoubleRegister(SPI_t *spi, MCP23S17Reg_t reg, uint16_t value)
{
    // Write 2 registers from same family
    MCP23S17_WriteSingleRegister(spi, reg, value);
    MCP23S17_WriteSingleRegister(spi, reg + 1, (value >> 8));
}

uint8_t MCP23S17_ReadSingleRegister(SPI_t *spi, uint8_t reg)
{
    // Read single register
    MCP23S17_StartTranmission(spi);
    uint8_t data = MCP23S17_Read(spi, reg);
    MCP23S17_EndTranmission(spi);
    return data;
}

uint16_t MCP23S17_ReadDoubleRegister(SPI_t *spi, uint8_t reg)
{
    // Read 2 registers from same family
    uint8_t dataLow = MCP23S17_ReadSingleRegister(spi, reg);
    uint8_t dataHigh = MCP23S17_ReadSingleRegister(spi, reg + 1);
    return CONCAT(dataHigh, dataLow);
}

uint8_t MCP23S17_WriteCheckSingleRegister(SPI_t *spi, MCP23S17Reg_t reg, uint8_t value)
{
    // Write and check single register
    MCP23S17_WriteSingleRegister(spi, reg, value);
    return(value != MCP23S17_ReadSingleRegister(spi, reg));
}

uint8_t MCP23S17_WriteCheckDoubleRegister(SPI_t *spi, MCP23S17Reg_t reg, uint16_t value)
{
    // Write and check 2 registers from same family
    return(MCP23S17_WriteCheckSingleRegister(spi, reg, value)
        || MCP23S17_WriteCheckSingleRegister(spi, reg + 1, (value >> 8)));
}

/* ==== Fonctions d'écriture ==== */

uint8_t MCP23S17_WriteCheckPin(SPI_t *spi, uint8_t pin, uint8_t value)
{
    uint8_t reg  = (pin < 8) ? MCP23S17_REG_GPIO_A : MCP23S17_REG_GPIO_B; // true PORTA, false PORTB
    uint8_t port = MCP23S17_ReadSingleRegister(spi, reg);

    if (value == 0)
        CLEAR_FLAG_BIT(port, (pin % 8));
    else
        SET_FLAG_BIT(port, (pin % 8));

    return MCP23S17_WriteCheckSingleRegister(spi, reg, port);
}

uint8_t MCP23S17_ReadPin(SPI_t *spi, uint8_t pin)
{
    uint8_t reg  = (pin < 8) ? MCP23S17_REG_GPIO_A : MCP23S17_REG_GPIO_B; // true PORTA, false PORTB
    uint8_t port = MCP23S17_ReadSingleRegister(spi, reg);
    return (port >> (pin % 8)) & (0x01);
}