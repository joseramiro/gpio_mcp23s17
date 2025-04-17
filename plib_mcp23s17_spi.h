#ifndef PLIB_MCP23S17_SPI_H
#define PLIB_MCP23S17_SPI_H

/**
 * @file plib_mcp23s17_spi.h
 * @brief Bibliothèque SPI pour le module MCP23S17
 * @author Ramiro Najera
 * @version 1.0
 * @date 2025-03-18
 * @copyright Copyright (c) 2025
 */

#include "libs/util/plibs_utils.h"  // used for SPI struct

/** @brief Adresse SPI du MCP23S17 */
#define MCP23S17_ADDRESS        0x40 /**< Adresse de base */

/** @brief Valeur de pause obligatoire pour la lecture */
#define MCP23S17_DUMMY_DATA     0xFF

/** 
 * @brief Enumération des ports du MCP23S17.
 */
typedef enum 
{
    MCP23S17_PORT_A = 0, /**< Port A */
    MCP23S17_PORT_B = 1  /**< Port B */
}MCP23S17Ports_t;

/* ==== Fonctions Génériques ==== */

/**
 * @brief Écrit une valeur dans un registre du MCP23S17.
 * @param spi Configuration SPI (canal, CS pin, adresse)
 * @param reg Registre cible
 * @param val Valeur à écrire
 * @return 0 si succès, sinon code d'erreur
 */
unsigned char MCP23S17_Write(SPIConfiguration_t *spi, unsigned char reg, unsigned char val);

/**
 * @brief Lit une valeur depuis un registre du MCP23S17.
 * @param spi Configuration SPI (canal, CS pin, adresse)
 * @param reg Registre cible
 * @return Valeur lue
 */
unsigned char MCP23S17_Read(SPIConfiguration_t *spi, unsigned char reg);

#endif  // PLIB_MCP23S17_SPI_H
