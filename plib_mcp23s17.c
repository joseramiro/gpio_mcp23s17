/**
 * @file plib_mcp23s17.c
 * @brief Pilote pour le MCP23S17 (Expander I/O SPI)
 * @author Ramiro Najera
 * @version 1.0
 * @date 2025-03-18
 */

#include "plib_mcp23s17_spi.h"
#include "plib_mcp23s17.h"

/* ==== Fonctions SPI ==== */

void MCP23S17_StartTranmission(SPIConfiguration_t *spi)
{
   spi->cs.clear();
}
 
void MCP23S17_EndTranmission(SPIConfiguration_t *spi)
{
   spi->cs.set();
}

/* ==== Fonctions de configuration ==== */

void MCP23S17_Init(MCP23S17Conf_t *conf)
{
    // Set SPI channel selection, control registers and pullup
    MCP23S17_EndTranmission(&conf->spi);
    MCP23S17_ConfigureControl(conf);
    MCP23S17_ConfigurePullupRes(conf);
    MCP23S17_ConfigureDirection(conf);
    MCP23S17_ConfigureINTDefault(conf);
    MCP23S17_ConfigureINTControl(conf);
    MCP23S17_ConfigureINTEnabled(conf);
    MCP23S17_ReadPorts(&conf->spi);
    MCP23S17_UpdatePorts(conf, conf->value);
}

void MCP23S17_EnableHWAddress(MCP23S17Conf_t *conf)
{
    // Set configuration in mask
    unsigned char maskPortA = MCP23S17_IOCR_HAEN;
    unsigned char maskPortB = MCP23S17_IOCR_HAEN;

    // Send register with value
    MCP23S17_WriteIOControlReg(&conf->spi, CONCAT_BYTES(maskPortB, maskPortA));
}

void MCP23S17_ConfigureControl(MCP23S17Conf_t *conf)
{
    // Set configuration in mask
    unsigned char maskPortA = ((conf->mirror << 6) | MCP23S17_IOCR_HAEN);
    unsigned char maskPortB = ((conf->mirror << 6) | MCP23S17_IOCR_HAEN);

    // Send register with value
    MCP23S17_WriteIOControlReg(&conf->spi, CONCAT_BYTES(maskPortB, maskPortA));
}

void MCP23S17_ConfigurePullupRes(MCP23S17Conf_t *conf)
{
    // Set configuration in mask
    unsigned char maskPortA = 0;
    unsigned char maskPortB = 0;

    // Configure pullup resistors by port: 1 enabled, 0 disabled
    // Port A
    if(conf->modeA == MCP23S17_DIRECTION_IN)
        maskPortA = MCP23S17_INPUT_CONF_MASK;
    else
        maskPortA = MCP23S17_OUTPUT_CONF_MASK;

    // Port B
    if(conf->modeB == MCP23S17_DIRECTION_IN)
        maskPortB = MCP23S17_INPUT_CONF_MASK;
    else
        maskPortB = MCP23S17_OUTPUT_CONF_MASK;
    
    // Send register with value
    MCP23S17_WritePullupResReg(&conf->spi, CONCAT_BYTES(maskPortB, maskPortA));
}

void MCP23S17_ConfigureDirection(MCP23S17Conf_t *conf)
{
    // Set configuration in mask
    unsigned char maskPortA = 0;
    unsigned char maskPortB = 0;

    // Configure direction by port: 1 input, 0 output
    // Port A
    if(conf->modeA == MCP23S17_DIRECTION_IN)
        maskPortA = MCP23S17_INPUT_CONF_MASK;
    else
        maskPortA = MCP23S17_OUTPUT_CONF_MASK;

    // Port B
    if(conf->modeB == MCP23S17_DIRECTION_IN)
        maskPortB = MCP23S17_INPUT_CONF_MASK;
    else
        maskPortB = MCP23S17_OUTPUT_CONF_MASK;
    
    // Send register with value
    MCP23S17_WriteDirectionReg(&conf->spi, CONCAT_BYTES(maskPortB, maskPortA));
}

void MCP23S17_ConfigureINTEnabled(MCP23S17Conf_t *conf)
{
    // Set configuration in mask
    unsigned char maskPortA = 0;
    unsigned char maskPortB = 0;

    // Configure enable interrupt: 1 enabled, 0 disabled
    // Port A
    if(conf->modeA == MCP23S17_DIRECTION_IN)
        maskPortA = MCP23S17_INPUT_CONF_MASK;
    else
        maskPortA = MCP23S17_OUTPUT_CONF_MASK;
    
    // Port B
    if(conf->modeB == MCP23S17_DIRECTION_IN)
        maskPortB = MCP23S17_INPUT_CONF_MASK;
    else
        maskPortB = MCP23S17_OUTPUT_CONF_MASK;

    // Send register with value
    MCP23S17_WriteINTEnableReg(&conf->spi, CONCAT_BYTES(maskPortB, maskPortA));
}

void MCP23S17_ConfigureINTDefault(MCP23S17Conf_t *conf)
{
    // Send register with value
    MCP23S17_WriteINTDefaultReg(&conf->spi, 0x00);
}

void MCP23S17_ConfigureINTControl(MCP23S17Conf_t *conf)
{
    // Send register with value
    MCP23S17_WriteINTControlReg(&conf->spi, 0x00);
}

/* ==== Fonctions d'écriture ==== */

void MCP23S17_WriteRegister(SPIConfiguration_t *spi, unsigned char reg, unsigned int value)
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

void MCP23S17_WriteDirectionReg(SPIConfiguration_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_IODIR_A, value);
}

void MCP23S17_WriteINTEnableReg(SPIConfiguration_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_GPINTEN_A, value);
}

void MCP23S17_WriteINTDefaultReg(SPIConfiguration_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_DEFVAL_A, value);
}

void MCP23S17_WriteINTControlReg(SPIConfiguration_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_INTCON_A, value);
}

void MCP23S17_WriteIOControlReg(SPIConfiguration_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_IOCON_A, value);
}

void MCP23S17_WritePullupResReg(SPIConfiguration_t *spi, unsigned int value)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_GPPU_A, value);
}

void MCP23S17_WritePorts(SPIConfiguration_t *spi, unsigned int mask)
{
    MCP23S17_WriteRegister(spi, MCP23S17_REG_GPIO_A, mask);
}

void MCP23S17_WritePin(SPIConfiguration_t *spi, unsigned char pin, unsigned char value)
{
    // Read Register
    unsigned int currentMask = MCP23S17_ReadPorts(spi);

    // Calculate Mask
    if(value == 1)
        currentMask |= (1 << pin);
    else
        currentMask &= ~(1 << pin);

    // Write Value
    MCP23S17_WritePorts(spi, currentMask);
}

void MCP23S17_UpdatePorts(MCP23S17Conf_t *conf, unsigned int mask)
{
    conf->value = mask;
    MCP23S17_WritePorts(&conf->spi, conf->value);
}

void MCP23S17_UpdatePin(MCP23S17Conf_t *conf, unsigned int pin, unsigned char state)
{
    MCP23S17_CalculateMask(&conf->value, pin, state);
    MCP23S17_UpdatePorts(conf, conf->value);
}

/* ==== Fonctions de lecture ==== */

unsigned int MCP23S17_ReadRegister(SPIConfiguration_t *spi, unsigned char reg)
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

unsigned int MCP23S17_ReadDirectionReg(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_IODIR_A);
}

unsigned int MCP23S17_ReadINTEnableReg(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_GPINTEN_A);
}

unsigned int MCP23S17_ReadINTDefaultReg(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_DEFVAL_A);
}

unsigned int MCP23S17_ReadINTControlReg(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_INTCON_A);
}

unsigned int MCP23S17_ReadIOControlReg(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_IOCON_A);
}

unsigned int MCP23S17_ReadPullupResReg(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_GPPU_A);
}

unsigned int MCP23S17_ReadINTTriggered(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_INTF_A);
}

unsigned int MCP23S17_ReadINTCapture(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_INTCAP_A);
}

unsigned int MCP23S17_ReadPorts(SPIConfiguration_t *spi)
{
    return MCP23S17_ReadRegister(spi, MCP23S17_REG_GPIO_A);
}

unsigned char MCP23S17_ReadPin(SPIConfiguration_t *spi, unsigned char pin)
{
    // Read Register
    unsigned int currentMask = MCP23S17_ReadPorts(spi);
    // Calculate Mask
    return (currentMask >> pin) & (0x01);
}

void MCP23S17_ReadPortsAllModules(MCP23S17Conf_t *conf, unsigned char size)
{
    unsigned char id = 0;

    // Read ports from all MCP23S17 modules
    for(id = 0; id < size; id++)
    {
        conf->value = MCP23S17_ReadPorts(&conf[id].spi);
        MCP23S17_ReadINTTriggered(&conf[id].spi);
        MCP23S17_ReadINTCapture(&conf[id].spi);
    }
}

/* ==== Fonctions de calcul ==== */

void MCP23S17_CalculateMask(unsigned int *port, unsigned int pin, unsigned char state)
{
    // Calculate module id, pin id and init mask
    unsigned int pinId = MCP23S17_CalculatePinId(pin);
    
    // Update mask value according to led and state values
    if (state)
        Utils_Set_16bits_Flag(port, pinId);
    else
        Utils_Clear_16bits_Flag(port, pinId);
}

unsigned int MCP23S17_CalculateModuleId(unsigned int pin)
{
    return (pin / MCP23S17_NUM_PINS);
}

unsigned int MCP23S17_CalculatePinId(unsigned int pin)
{
    return (pin % MCP23S17_NUM_PINS);
}