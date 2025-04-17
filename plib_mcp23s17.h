#ifndef PLIB_MCP23S17_H
#define PLIB_MCP23S17_H

/**
 * @file plib_mcp23s17.h
 * @brief Pilote pour le MCP23S17 (Expander I/O SPI)
 * @author Ramiro Najera
 * @version 1.0
 * @date 2025-03-18
 */

#include "libs/util/plibs_utils.h"  // used for SPI struct

#define MCP23S17_NUM_PINS               16      /**< Nombre total de broches du MCP23S17 */
#define MCP23S17_NUM_PINS_IN_PORT       8       /**< Nombre de broches par port (A ou B) */
#define MCP23S17_INPUT_CONF_MASK        0xFF    /**< Masque de configuration pour pins d'entrée */
#define MCP23S17_OUTPUT_CONF_MASK       0x00    /**< Masque de configuration pour pins d'entrée */

/** @defgroup MCP23S17_Registers Registres du MCP23S17
 *  @{
 */
#define MCP23S17_REG_IODIR_A        0x00 /**< Contrôle la direction des E/S du port A */
#define MCP23S17_REG_IODIR_B        0x01 /**< Contrôle la direction des E/S du port B */
#define MCP23S17_REG_IPOL_A         0x02 /**< Configure la polarité des entrées du port A */
#define MCP23S17_REG_IPOL_B         0x03 /**< Configure la polarité des entrées du port B */
#define MCP23S17_REG_GPINTEN_A      0x04 /**< Active les interruptions sur changement pour le port A */
#define MCP23S17_REG_GPINTEN_B      0x05 /**< Active les interruptions sur changement pour le port B */
#define MCP23S17_REG_DEFVAL_A       0x06 /**< Définit les valeurs de référence pour les interruptions du port A */
#define MCP23S17_REG_DEFVAL_B       0x07 /**< Définit les valeurs de référence pour les interruptions du port B */
#define MCP23S17_REG_INTCON_A       0x08 /**< Configure la comparaison des valeurs d'interruption du port A */
#define MCP23S17_REG_INTCON_B       0x09 /**< Configure la comparaison des valeurs d'interruption du port B */
#define MCP23S17_REG_IOCON_A        0x0A /**< Configuration générale du module */
#define MCP23S17_REG_IOCON_B        0x0B /**< Deuxième registre de configuration générale */
#define MCP23S17_REG_GPPU_A         0x0C /**< Active les résistances de pull-up pour le port A */
#define MCP23S17_REG_GPPU_B         0x0D /**< Active les résistances de pull-up pour le port B */
#define MCP23S17_REG_INTF_A         0x0E /**< Indique les interruptions actives sur le port A */
#define MCP23S17_REG_INTF_B         0x0F /**< Indique les interruptions actives sur le port B */
#define MCP23S17_REG_INTCAP_A       0x10 /**< Capture l'état du port A lors d'une interruption */
#define MCP23S17_REG_INTCAP_B       0x11 /**< Capture l'état du port B lors d'une interruption */
#define MCP23S17_REG_GPIO_A         0x12 /**< Accède aux broches du port A */
#define MCP23S17_REG_GPIO_B         0x13 /**< Accède aux broches du port B */
#define MCP23S17_REG_OLAT_A         0x14 /**< Verrouillage de sortie pour le port A */
#define MCP23S17_REG_OLAT_B         0x15 /**< Verrouillage de sortie pour le port B */
/** @} */

/** @defgroup MCP23S17_ConfigBits Bits de configuration du registre IOCR
 *  @{
 */
#define MCP23S17_IOCR_BANK      0x80 /**< Mode de gestion des registres (banques ou séquentiel) */
#define MCP23S17_IOCR_MIRROR    0x40 /**< Relie les broches INT en interne */
#define MCP23S17_IOCR_SEQOP     0x20 /**< Mode d'opération séquentielle */
#define MCP23S17_IOCR_DISSLW    0x10 /**< Désactive la réduction de vitesse sur SDA */
#define MCP23S17_IOCR_HAEN      0x08 /**< Active les broches d'adresse matérielle */
#define MCP23S17_IOCR_ODR       0x04 /**< Configure la sortie INT en drain ouvert */
#define MCP23S17_IOCR_INTPOL    0x02 /**< Détermine la polarité des interruptions */
/** @} */

/** 
 * @brief Modes de direction des broches 
 */
typedef enum
{
    MCP23S17_DIRECTION_OUT,  /**< Mode sortie */
    MCP23S17_DIRECTION_IN    /**< Mode entrée */
}MCP23S17Directions_t;

/** 
 * @brief Modes de gestion des interruptions 
 */
typedef enum 
{
    MCP23S17_MIRROR_OFF,     /**< Interruptions distinctes sur INTA et INTB */
    MCP23S17_MIRROR_ON       /**< Interruptions combinées sur INTA et INTB */
}MCP23S17Mirror_t;

/** 
 * @brief Structure de configuration du MCP23S17 
 */
typedef struct
{
    unsigned char id;           /**< Identifiant du module */
    unsigned char modeA;        /**< Configuration de direction pour le port A */
    unsigned char modeB;        /**< Configuration de direction pour le port B */
    unsigned char mirror;       /**< Mode de gestion des interruptions */
    unsigned int value;         /**< Valeur courante des ports */
    SPIConfiguration_t spi;       /**< Configuration SPI associée */
}MCP23S17Conf_t;

/* ==== Fonctions Chip Select ==== */

/**
 * @brief Démarre une transmission SPI avec le MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 */
void MCP23S17_StartTranmission(SPIConfiguration_t *spi);

/**
 * @brief Termine une transmission SPI avec le MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 */
void MCP23S17_EndTranmission(SPIConfiguration_t *spi);

/* ==== Fonctions de configuration ==== */

/**
 * @brief Initialise le module MCP23S17 avec la configuration fournie.
 * @param conf Pointeur vers la configuration du module
 */
void MCP23S17_Init(MCP23S17Conf_t *conf);

/**
 * @brief Active l'adressage matériel du MCP23S17.
 * @param conf Pointeur vers la configuration du module
 */
void MCP23S17_EnableHWAddress(MCP23S17Conf_t *conf);

/**
 * @brief Configure le registre de contrôle du module (IOCON).
 * @param conf Pointeur vers la configuration du module
 */
void MCP23S17_ConfigureControl(MCP23S17Conf_t *conf);

/**
 * @brief Active ou désactive les résistances de pull-up internes.
 * @param conf Pointeur vers la configuration du module
 */
void MCP23S17_ConfigurePullupRes(MCP23S17Conf_t *conf);

/**
 * @brief Configure la direction des broches du MCP23S17.
 * @param conf Pointeur vers la configuration du module
 */
void MCP23S17_ConfigureDirection(MCP23S17Conf_t *conf);

/**
 * @brief Active les interruptions sur les broches définies.
 * @param conf Pointeur vers la configuration du module
 */
void MCP23S17_ConfigureINTEnabled(MCP23S17Conf_t *conf);

/**
 * @brief Définit la valeur de référence pour les interruptions.
 * @param conf Pointeur vers la configuration du module
 */
void MCP23S17_ConfigureINTDefault(MCP23S17Conf_t *conf);

/**
 * @brief Configure la méthode de déclenchement des interruptions.
 * @param conf Pointeur vers la configuration du module
 */
void MCP23S17_ConfigureINTControl(MCP23S17Conf_t *conf);

/* ==== Fonctions d'écriture ==== */

/**
 * @brief Ecrit un ensemble de 2 registres de la même famille
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registrer à écrire
 * @param value Données à écrire
 */
void MCP23S17_WriteRegister(SPIConfiguration_t *spi, unsigned char reg, unsigned int value);

/**
 * @brief Configure la direction des broches d'un port.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteDirectionReg(SPIConfiguration_t *spi, unsigned int value);

/**
 * @brief Active ou désactive les interruptions pour un port.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteINTEnableReg(SPIConfiguration_t *spi, unsigned int value);

/**
 * @brief Définit les valeurs de comparaison des interruptions.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteINTDefaultReg(SPIConfiguration_t *spi, unsigned int value);

/**
 * @brief Configure la façon dont les interruptions sont déclenchées.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteINTControlReg(SPIConfiguration_t *spi, unsigned int value);

/**
 * @brief Configure les options du registre de contrôle I/O.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteIOControlReg(SPIConfiguration_t *spi, unsigned int value);

/**
 * @brief Configure les résistances de pull-up pour un port.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WritePullupResReg(SPIConfiguration_t *spi, unsigned int value);

/**
 * @brief Écrit une valeur sur l'ensemble des ports du MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 * @param mask Données à écrire
 */
void MCP23S17_WritePorts(SPIConfiguration_t *spi, unsigned int mask);

/**
 * @brief Écrit une valeur sur une broche spécifique du MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 * @param pin Numéro de la broche (0 à 15)
 * @param value Valeur à écrire (0 ou 1)
 */
void MCP23S17_WritePin(SPIConfiguration_t *spi, unsigned char pin, unsigned char value);

/**
 * @brief Met à jour l'état des ports en fonction d'un masque binaire.
 * @param conf Pointeur vers la configuration du module
 * @param mask Masque binaire des ports à mettre à jour
 */
void MCP23S17_UpdatePorts(MCP23S17Conf_t *conf, unsigned int mask);

/**
 * @brief Met à jour l'état d'une broche spécifique.
 * @param conf Pointeur vers la configuration du module
 * @param pin Numéro de la broche (0 à 15)
 * @param state Nouvel état de la broche (0 ou 1)
 */
void MCP23S17_UpdatePin(MCP23S17Conf_t *conf, unsigned int pin, unsigned char state);

/* ==== Fonctions de lecture ==== */

/**
 * @brief Lit un ensemble de 2 registres de la même famille
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registre à lire
 * @return unsigned int Données lues
 */
unsigned int MCP23S17_ReadRegister(SPIConfiguration_t *spi, unsigned char reg);

/**
 * @brief Lit la direction des broches d'un port.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadDirectionReg(SPIConfiguration_t *spi);

/**
 * @brief Lit le flag d'activation des interruptions du port
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadINTEnableReg(SPIConfiguration_t *spi);

/**
 * @brief Lit les valeurs de comparaison des interruptions.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadINTDefaultReg(SPIConfiguration_t *spi);

/**
 * @brief Lit la façon dont les interruptions sont déclenchées.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadINTControlReg(SPIConfiguration_t *spi);

/**
 * @brief Lit les options du registre de contrôle I/O.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadIOControlReg(SPIConfiguration_t *spi);

/**
 * @brief Lit les résistances de pull-up pour un port.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadPullupResReg(SPIConfiguration_t *spi);

/**
 * @brief Lit les broches ayant déclenché une interruption.
 * @param spi Pointeur vers la configuration SPI
 * @return Masque binaire des broches ayant déclenché une interruption
 */
unsigned int MCP23S17_ReadINTTriggered(SPIConfiguration_t *spi);

/**
 * @brief Capture l'état des broches lors d'une interruption.
 * @param spi Pointeur vers la configuration SPI
 * @return Valeur capturée des ports
 */
unsigned int MCP23S17_ReadINTCapture(SPIConfiguration_t *spi);

/**
 * @brief Lit l'état actuel des broches du MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 * @return Masque binaire des états des broches
 */
unsigned int MCP23S17_ReadPorts(SPIConfiguration_t *spi);

/**
 * @brief Lit l'état d'une broche spécifique.
 * @param spi Pointeur vers la configuration SPI
 * @param pin Numéro de la broche (0 à 15)
 * @return État de la broche (0 ou 1)
 */
unsigned char MCP23S17_ReadPin(SPIConfiguration_t *spi, unsigned char pin);

/**
 * @brief Lit les états des ports de plusieurs modules MCP23S17.
 * @param conf Tableau contenant les configurations des modules
 * @param size Nombre de modules à lire
 */
void MCP23S17_ReadPortsAllModules(MCP23S17Conf_t* conf, unsigned char size);

/* ==== Fonctions de calcul ==== */

/**
 * @brief Calcule le masque du port à écrire
 * @param port Valeur actuelle du port
 * @param pin Index du pin à écrire
 * @param state Valeur à écrire sur le pin
 */
void MCP23S17_CalculateMask(unsigned int *port, unsigned int pin, unsigned char state);

/**
 * @brief Calcule l'index du module
 * @param pin Index du pin à écrire (brut)
 * @return unsigned int Index du module
 */
unsigned int MCP23S17_CalculateModuleId(unsigned int pin);

/**
 * @brief Calcule l'index du pin
 * @param pin Index du pin à écrire (brut)
 * @return unsigned int Index du pin à écrire (sur module actuel)
 */
unsigned int MCP23S17_CalculatePinId(unsigned int pin);

#endif  // PLIB_MCP23S17_H
