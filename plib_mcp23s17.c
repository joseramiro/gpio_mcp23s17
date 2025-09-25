/**
 * @file plib_mcp23s17.c
 * @brief Pilote pour le MCP23S17 (Expander I/O SPI)
 * @author Ramiro Najera
 * @version 1.0.3
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
    if(spi->en.clear != NULL)
        spi->en.clear();
    if(spi->cs.clear != NULL) 
        spi->cs.clear();
}
 
void MCP23S17_EndTranmission(SPI_t *spi)
{
    // Check function before calling it
    if(spi->cs.set != NULL)
        spi->cs.set();
    if(spi->en.set != NULL)
        spi->en.set();
}

/* ==== Fonctions de base ==== */

unsigned char MCP23S17_Init(MCP23S17_t *obj)
{
    unsigned char error = 0;
    // Reset CS
    MCP23S17_EndTranmission(&obj->spi);
    // Set init configuration (conf, pullup, direction, interrupt)
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_IOCON_A,   CONCAT(obj->confB.reg, obj->confA.reg))) error |= (1 << 0);
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_GPPU_A,    CONCAT(obj->dirB.reg, obj->dirA.reg)))   error |= (1 << 1);
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_IODIR_A,   CONCAT(obj->dirB.reg, obj->dirA.reg)))   error |= (1 << 2);
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_DEFVAL_A,  0x00))                                   error |= (1 << 3);
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_INTCON_A,  0x00))                                   error |= (1 << 4);
    if(MCP23S17_WriteCheckDoubleRegister(&obj->spi, MCP23S17_REG_GPINTEN_A, CONCAT(obj->dirB.reg, obj->dirA.reg)))   error |= (1 << 5);
    // Read ports to process INT trigger and update ports
    MCP23S17_ReadDoubleRegister(&obj->spi, MCP23S17_REG_GPIO_A);
    MCP23S17_WriteDoubleRegister(&obj->spi, MCP23S17_REG_GPIO_A, obj->ports);
    return error;
}

void MCP23S17_EnableHWAddress(MCP23S17_t *obj)
{
    // Set configuration in mask, send register
    MCP23S17IOCONReg_t mask;
    mask.bits.haen = MCP23S17_IOCON_HAEN_ENABLED;
    MCP23S17_WriteDoubleRegister(&obj->spi, MCP23S17_REG_IOCON_A, CONCAT(mask.bits.haen, mask.bits.haen));
}

void MCP23S17_WriteSingleRegister(SPI_t *spi, MCP23S17Reg_t reg, unsigned char value)
{
    // Write single register
    MCP23S17_StartTranmission(spi);
    MCP23S17_Write(spi, reg, value);
    MCP23S17_EndTranmission(spi);
}

void MCP23S17_WriteDoubleRegister(SPI_t *spi, MCP23S17Reg_t reg, unsigned int value)
{
    // Write 2 registers from same family
    MCP23S17_WriteSingleRegister(spi, reg, value);
    MCP23S17_WriteSingleRegister(spi, reg + 1, (value >> 8));
}

unsigned char MCP23S17_ReadSingleRegister(SPI_t *spi, unsigned char reg)
{
    // Read single register
    MCP23S17_StartTranmission(spi);
    unsigned char data = MCP23S17_Read(spi, reg);
    MCP23S17_EndTranmission(spi);
    return data;
}

unsigned int MCP23S17_ReadDoubleRegister(SPI_t *spi, unsigned char reg)
{
    // Read 2 registers from same family
    unsigned char dataLow = MCP23S17_ReadSingleRegister(spi, reg);
    unsigned char dataHigh = MCP23S17_ReadSingleRegister(spi, reg + 1);
    return CONCAT(dataHigh, dataLow);
}

unsigned char MCP23S17_WriteCheckSingleRegister(SPI_t *spi, MCP23S17Reg_t reg, unsigned char value)
{
    // Write and check single register
    MCP23S17_WriteSingleRegister(spi, reg, value);
    return(value != MCP23S17_ReadSingleRegister(spi, reg));
}

unsigned char MCP23S17_WriteCheckDoubleRegister(SPI_t *spi, MCP23S17Reg_t reg, unsigned int value)
{
    // Write and check 2 registers from same family
    return(MCP23S17_WriteCheckSingleRegister(spi, reg, value)
        || MCP23S17_WriteCheckSingleRegister(spi, reg + 1, (value >> 8)));
}

/* ==== Fonctions d'écriture ==== */

unsigned char MCP23S17_WriteCheckPin(MCP23S17_t *obj, unsigned char pin, unsigned char value)
{
    unsigned char reg  = (pin < 8) ? MCP23S17_REG_GPIO_A : MCP23S17_REG_GPIO_B; // true PORTA, false PORTB
    unsigned char port = MCP23S17_ReadSingleRegister(&obj->spi, reg);

    if (value == 0)
        CLEAR_FLAG_BIT(port, (pin % 8));
    else
        SET_FLAG_BIT(port, (pin % 8));

    return MCP23S17_WriteCheckSingleRegister(&obj->spi, reg, port);
}

unsigned char MCP23S17_ReadPin(SPI_t *spi, unsigned char pin)
{
    unsigned char reg  = (pin < 8) ? MCP23S17_REG_GPIO_A : MCP23S17_REG_GPIO_B; // true PORTA, false PORTB
    unsigned char port = MCP23S17_ReadSingleRegister(spi, reg);
    return (port >> (pin % 8)) & (0x01);
}

void MCP23S17_AcknowledgeInterrupt(MCP23S17_t *obj, unsigned char size)
{
    // Read ports from all MCP23S17 modules
    for(unsigned char i = 0; i < size; i++)
    {
        obj->ports = MCP23S17_ReadDoubleRegister(&obj[i].spi, MCP23S17_REG_GPIO_A);
        MCP23S17_ReadDoubleRegister(&obj[i].spi, MCP23S17_REG_INTF_A);
        MCP23S17_ReadDoubleRegister(&obj[i].spi, MCP23S17_REG_INTCAP_A);
    }
}