/**
 * @file plib_mcp23s17.c
 * @brief Pilote pour le MCP23S17 (Expander I/O SPI)
 * @author Ramiro Najera
 * @version 1.0.1
 * @date 2025-03-18
 */

#include <stddef.h>
#include "plib_mcp23s17_spi.h"
#include "plib_mcp23s17.h"
#include "libs/common_c_libs/plib_data_struct.h"

/* ==== Fonctions Chip Select ==== */

void MCP23S17_StartTranmission(SPI_t *spi)
{
    if(spi->en.clear != NULL)
        spi->en.clear();
    if(spi->cs.clear != NULL) 
        spi->cs.clear();
}
 
void MCP23S17_EndTranmission(SPI_t *spi)
{
    if(spi->cs.set != NULL)
        spi->cs.set();
    if(spi->en.set != NULL)
        spi->en.set();
}

/* ==== Fonctions de configuration ==== */

void MCP23S17_Init(MCP23S17_t *obj)
{
    // Reset CS
    MCP23S17_EndTranmission(&obj->spi);
    // Set init configuration
    MCP23S17_WriteIOControlReg(&obj->spi, CONCAT_BYTES(obj->confB.reg, obj->confA.reg));
    MCP23S17_WritePullupResReg(&obj->spi, CONCAT_BYTES(obj->dirB.reg, obj->dirA.reg));
    MCP23S17_WriteDirectionReg(&obj->spi, CONCAT_BYTES(obj->dirB.reg, obj->dirA.reg));
    MCP23S17_WriteINTDefaultReg(&obj->spi, 0x00);
    MCP23S17_WriteINTControlReg(&obj->spi, 0x00);
    MCP23S17_WriteINTEnableReg(&obj->spi, CONCAT_BYTES(obj->dirB.reg, obj->dirA.reg));
    // Read ports to process INT trigger
    MCP23S17_ReadPorts(&obj->spi);
    // Set init value
    MCP23S17_UpdatePorts(obj, obj->value);
}

void MCP23S17_EnableHWAddress(MCP23S17_t *obj)
{
    // Set configuration in mask
    MCP23S17IOCONReg_t mask;
    mask.bits.haen = MCP23S17_IOCON_HAEN_ENABLED;
    // Send register with value
    MCP23S17_WriteIOControlReg(&obj->spi, CONCAT_BYTES(mask.bits.haen, mask.bits.haen));
}

/* ==== Fonctions d'écriture ==== */

void MCP23S17_WriteRegister(SPI_t *spi, unsigned char reg, unsigned int value)
{
   // Write in Port A
   MCP23S17_StartTranmission(spi);
   MCP23S17_Write(spi, reg, value);
   MCP23S17_EndTranmission(spi);

   // Write in Port B
   MCP23S17_StartTranmission(spi);
   MCP23S17_Write(spi, reg+1, (value >> 8));
   MCP23S17_EndTranmission(spi);
}

void MCP23S17_WriteDirectionReg(SPI_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_IODIR_A, value);
}

void MCP23S17_WriteINTEnableReg(SPI_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_GPINTEN_A, value);
}

void MCP23S17_WriteINTDefaultReg(SPI_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_DEFVAL_A, value);
}

void MCP23S17_WriteINTControlReg(SPI_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_INTCON_A, value);
}

void MCP23S17_WriteIOControlReg(SPI_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_IOCON_A, value);
}

void MCP23S17_WritePullupResReg(SPI_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_GPPU_A, value);
}

void MCP23S17_WritePorts(SPI_t *spi, unsigned int mask)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_GPIO_A, mask);
}

void MCP23S17_UpdatePin(MCP23S17_t *obj, unsigned char pin, unsigned char value)
{
    // Read current mask
    unsigned int currentPorts = MCP23S17_ReadPorts(&obj->spi);
    // Calculate Mask
    if(value == 1)
        Utils_Set_16bits_Flag(&currentPorts, pin);
    else
        Utils_Clear_16bits_Flag(&currentPorts, pin);
    // Write port
    MCP23S17_UpdatePorts(obj, currentPorts);
}

void MCP23S17_UpdatePorts(MCP23S17_t *obj, unsigned int mask)
{
    MCP23S17_WritePorts(&obj->spi, mask);
    obj->value = MCP23S17_ReadPorts(&obj->spi);
}

/* ==== Fonctions de lecture ==== */

unsigned int MCP23S17_ReadRegister(SPI_t *spi, unsigned char reg)
{
    unsigned char dataPortA, dataPortB;
    
    // Read in Port A
    MCP23S17_StartTranmission(spi);
    dataPortA = MCP23S17_Read(spi, reg);
    MCP23S17_EndTranmission(spi);

    // Read in Port B
    MCP23S17_StartTranmission(spi);
    dataPortB = MCP23S17_Read(spi, reg+1);
    MCP23S17_EndTranmission(spi);

    // Return concatenated read
    return CONCAT_BYTES(dataPortB, dataPortA);
}

unsigned int MCP23S17_ReadDirectionReg(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_IODIR_A);
}

unsigned int MCP23S17_ReadINTEnableReg(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_GPINTEN_A);
}

unsigned int MCP23S17_ReadINTDefaultReg(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_DEFVAL_A);
}

unsigned int MCP23S17_ReadINTControlReg(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_INTCON_A);
}

unsigned int MCP23S17_ReadIOControlReg(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_IOCON_A);
}

unsigned int MCP23S17_ReadPullupResReg(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_GPPU_A);
}

unsigned int MCP23S17_ReadINTTriggered(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_INTF_A);
}

unsigned int MCP23S17_ReadINTCapture(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_INTCAP_A);
}

unsigned int MCP23S17_ReadPorts(SPI_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_GPIO_A);
}

unsigned char MCP23S17_ReadPin(SPI_t *spi, unsigned char pin)
{
    // Read Register
    unsigned int currentMask = MCP23S17_ReadPorts(spi);
    // Calculate Mask
    return (currentMask >> pin) & (0x01);
}

void MCP23S17_ReadPortsAllModules(MCP23S17_t *obj, unsigned char size)
{
    unsigned char id = 0;

    // Read ports from all MCP23S17 modules
    for(id = 0; id < size; id++)
    {
        obj->value = MCP23S17_ReadPorts(&obj[id].spi);
        MCP23S17_ReadINTTriggered(&obj[id].spi);
        MCP23S17_ReadINTCapture(&obj[id].spi);
    }
}

/* ==== Fonctions de calcul ==== */

unsigned int MCP23S17_CalculateModuleId(unsigned int pin)
{
    return (pin / MCP23S17_NUM_PINS);
}

unsigned int MCP23S17_CalculatePinId(unsigned int pin)
{
    return (pin % MCP23S17_NUM_PINS);
}