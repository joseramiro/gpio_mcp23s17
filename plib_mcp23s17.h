#ifndef PLIB_MCP23S17_H
#define PLIB_MCP23S17_H

/**
 * @file plib_mcp23s17.h
 * @brief Pilote pour le MCP23S17 (Expander I/O SPI)
 * @author Ramiro Najera
 * @version 1.0.6
 * @date 2025-03-18
 */

#include "libs/common_c_libs/plib_comm_struct.h"

/** @brief Nombre total de broches du MCP23S17 */
#define MCP23S17_NUM_PINS           16

/** @defgroup MCP23S17_IODirMask Masques pour registre IODIR du MCP23S17
 * @{ */
/** @brief Masque de configuration pour pins d'entrée */
#define MCP23S17_IODIR_INPUT        0xFF
/** @brief Masque de configuration pour pins d'sortie */
#define MCP23S17_IODIR_OUTPUT       0x00
/** @} */

#define MCP23S17_GET_MODULE_ID(pin)   ((unsigned int)((pin) / MCP23S17_NUM_PINS))
#define MCP23S17_GET_PIN_ID(pin)      ((unsigned int)((pin) % MCP23S17_NUM_PINS))

typedef enum
{
    MCP23S17_REG_IODIR_A, /**< Contrôle la direction des E/S du port A  */
    MCP23S17_REG_IODIR_B, /**< Contrôle la direction des E/S du port B  */
    MCP23S17_REG_IPOL_A, /**< Configure la polarité des entrées du port A  */
    MCP23S17_REG_IPOL_B, /**< Configure la polarité des entrées du port B  */
    MCP23S17_REG_GPINTEN_A, /**< Active les interruptions sur changement pour le port A */
    MCP23S17_REG_GPINTEN_B, /**< Active les interruptions sur changement pour le port B  */
    MCP23S17_REG_DEFVAL_A, /**< Définit les valeurs de référence pour les interruptions du port A  */
    MCP23S17_REG_DEFVAL_B, /**< Définit les valeurs de référence pour les interruptions du port B  */
    MCP23S17_REG_INTCON_A, /**< Configure la comparaison des valeurs d'interruption du port A */
    MCP23S17_REG_INTCON_B, /**< Configure la comparaison des valeurs d'interruption du port B */
    MCP23S17_REG_IOCON_A, /**< Configuration générale du module  */
    MCP23S17_REG_IOCON_B, /**< Deuxième registre de configuration générale */
    MCP23S17_REG_GPPU_A, /**< Active les résistances de pull-up pour le port A  */
    MCP23S17_REG_GPPU_B, /**< Active les résistances de pull-up pour le port B  */
    MCP23S17_REG_INTF_A, /**< Indique les interruptions actives sur le port A  */
    MCP23S17_REG_INTF_B, /**< Indique les interruptions actives sur le port B  */
    MCP23S17_REG_INTCAP_A, /**< Capture l'état du port A lors d'une interruption  */
    MCP23S17_REG_INTCAP_B, /**< Capture l'état du port B lors d'une interruption  */
    MCP23S17_REG_GPIO_A, /**< Accède aux broches du port A  */
    MCP23S17_REG_GPIO_B, /**< Accède aux broches du port B  */
    MCP23S17_REG_OLAT_A, /**< Verrouillage de sortie pour le port A  */
    MCP23S17_REG_OLAT_B, /**< Verrouillage de sortie pour le port B  */
}MCP23S17Reg_t;

/**
 * @enum MCP23S17IOCONBank_t
 * @brief Mode de regroupement des registres (BANK).
 */
typedef enum 
{
    MCP23S17_IOCON_BANK_SAME,       /**< Registres groupés (mode par défaut). */
    MCP23S17_IOCON_BANK_SEPARATED   /**< Registres séparés par port. */
} MCP23S17IOCONBank_t;

/**
 * @enum MCP23S17IOCONMirror_t
 * @brief Contrôle du mode miroir des interruptions (MIRROR).
 */
typedef enum 
{
    MCP23S17_IOCON_MIRROR_DISABLED, /**< Interruptions gérées séparément. */
    MCP23S17_IOCON_MIRROR_ENABLED   /**< Interruptions groupées. */
} MCP23S17IOCONMirror_t;

/**
 * @enum MCP23S17IOCONSeqop_t
 * @brief Contrôle du mode d'adressage séquentiel (SEQOP).
 */
typedef enum 
{
    MCP23S17_IOCON_SEQOP_ENABLED,   /**< Adressage séquentiel actif. */
    MCP23S17_IOCON_SEQOP_DISABLED   /**< Adressage séquentiel désactivé. */
} MCP23S17IOCONSeqop_t;

/**
 * @enum MCP23S17IOCONDisslw_t
 * @brief Contrôle du ralentissement de la sortie (DISSLW).
 */
typedef enum 
{
    MCP23S17_IOCON_SLW_DISABLED,    /**< Sortie rapide. */
    MCP23S17_IOCON_SLW_ENABLED      /**< Sortie ralentie (anti-rebond). */
} MCP23S17IOCONDisslw_t;

/**
 * @enum MCP23S17IOCONHaen_t
 * @brief Activation de l'adresse matérielle (HAEN).
 */
typedef enum 
{
    MCP23S17_IOCON_HAEN_DISABLED,   /**< Adresse matérielle ignorée. */
    MCP23S17_IOCON_HAEN_ENABLED     /**< Adresse matérielle activée. */
} MCP23S17IOCONHaen_t;

/**
 * @enum MCP23S17IOCONOdr_t
 * @brief Configuration de la sortie d'interruption (ODR).
 */
typedef enum 
{
    MCP23S17_IOCON_ODR_DISABLED,    /**< Mode push-pull. */
    MCP23S17_IOCON_ODR_ENABLED      /**< Mode open-drain. */
} MCP23S17IOCONOdr_t;

/**
 * @enum MCP23S7IOCONIntpol_t
 * @brief Polarité des interruptions (INTPOL).
 */
typedef enum
{
    MCP23S17_IOCON_INTPOL_LOW,      /**< Active à l'état bas. */
    MCP23S17_IOCON_INTPOL_HIGH      /**< Active à l'état haut. */
} MCP23S7IOCONIntpol_t;

/**
 * @union MCP23S17IODirReg_t
 * @brief Registre de direction des broches (IODIR) du MCP23S17
 */
typedef union
{
    /** @brief Accès individuel aux bits du registre. */
    struct {
        /** @brief Bit 0 */
        unsigned char b0 : 1;
        /** @brief Bit 1 */
        unsigned char b1 : 1;
        /** @brief Bit 2 */
        unsigned char b2 : 1;
        /** @brief Bit 3 */
        unsigned char b3 : 1;
        /** @brief Bit 4 */
        unsigned char b4 : 1;
        /** @brief Bit 5 */
        unsigned char b5 : 1;
        /** @brief Bit 6 */
        unsigned char b6 : 1;
        /** @brief Bit 7 */
        unsigned char b7 : 1;
    } bits;
    /** @brief Accès complet au registre sous forme d'un octet. */
    unsigned char reg; /**< Accès complet au registre sous forme d'un octet. */
} MCP23S17IODirReg_t;

/**
 * @union MCP23S17IOCONReg_t
 * @brief Registre de configuration des broches (IOCON) du MCP23S17
 */
typedef union
{
    /** @brief Accès individuel aux bits du registre. */
    struct {
        /** @brief Bit 0 (réservé)*/
        unsigned char b0        : 1;
        /** @brief Bit 1 polarité de sortie de la broche INT */
        unsigned char intpol    : 1;
        /** @brief Bit 2 configuration open-drain de la broche INT */
        unsigned char odr       : 1;
        /** @brief Bit 3 addresse hardware */
        unsigned char haen      : 1;
        /** @brief Bit 4 configuration de slew rate (SDA output) */ 
        unsigned char disslw    : 1;
        /** @brief Bit 5 configuration de mode séquentiel */
        unsigned char seqop     : 1;
        /** @brief Bit 6 broches INT interconnectées */
        unsigned char mirror    : 1;
        /** @brief Bit 7 Configuration des addresses des registres */
        unsigned char bank      : 1;
    } bits;
    /** @brief Accès complet au registre sous forme d'un octet. */
    unsigned char reg; /**< Accès complet au registre sous forme d'un octet. */
} MCP23S17IOCONReg_t;

/** 
 * @struct MCP23S17_t
 * @brief Structure de configuration du MCP23S17 
 */
typedef struct
{
    /** @brief Identifiant du module */
    unsigned char id;
    /** @brief Valeur des ports */
    unsigned int ports;
    /** @brief Registre IODIR du port A */
    MCP23S17IODirReg_t dirA;
    /** @brief Registre IODIR du port B */
    MCP23S17IODirReg_t dirB;
    /** @brief Registre IOCON du port A */
    MCP23S17IOCONReg_t confA;
    /** @brief Registre IOCON du port B */
    MCP23S17IOCONReg_t confB;
    /** @brief Configuration de communication série */
    SPI_t spi;
}MCP23S17_t;

/* ==== Fonctions Chip Select ==== */

/**
 * @brief Démarre une transmission SPI avec le MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 */
void MCP23S17_StartTranmission(SPI_t *spi);

/**
 * @brief Termine une transmission SPI avec le MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 */
void MCP23S17_EndTranmission(SPI_t *spi);

/* ==== Fonctions de base ==== */

/**
 * @brief Initialise le module MCP23S17 avec la configuration fournie.
 * @param obj Pointeur vers la configuration du module
 * @return unsigned char 0 ok, 1 erreur
 */
unsigned char MCP23S17_InitChip(MCP23S17_t *obj);

/**
 * @brief Initialise une liste de modules MCP23S17
 * @param objList Liste de modules MCP23S17
 * @param size Taille de liste
 * @return unsigned char Code d'erreur (bitmap de modules de la liste. voir MCP23S17_InitChip)
 */
unsigned char MCP23S17_InitList(MCP23S17_t *objList, unsigned char size);

/**
 * @brief Active l'adressage matériel du MCP23S17.
 * @param obj Pointeur vers la configuration du module
 */
void MCP23S17_EnableHWAddress(MCP23S17_t *obj);

/**
 * @brief Ecrit un registre
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registrer à écrire
 * @param value Données à écrire
 */
void MCP23S17_WriteSingleRegister(SPI_t *spi, MCP23S17Reg_t reg, unsigned char value);

/**
 * @brief Ecrit un ensemble de 2 registres de la même famille
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registrer à écrire
 * @param value Données à écrire
 */
void MCP23S17_WriteDoubleRegister(SPI_t *spi, MCP23S17Reg_t reg, unsigned int value);

/**
 * @brief Lit un registre
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registre à lire
 * @return unsigned char Données lues
 */
unsigned char MCP23S17_ReadSingleRegister(SPI_t *spi, unsigned char reg);

/**
 * @brief Lit un ensemble de 2 registres de la même famille
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registre à lire
 * @return unsigned int Données lues
 */
unsigned int MCP23S17_ReadDoubleRegister(SPI_t *spi, unsigned char reg);

/**
 * @brief Ecrit et vérifie la valeur d'un registre
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registre à écrire
 * @param value Données à écrire
 * @return unsigned char 0 pas d'erreur, sinon erreur
 */
unsigned char MCP23S17_WriteCheckSingleRegister(SPI_t *spi, MCP23S17Reg_t reg, unsigned char value);

/**
 * @brief Ecrit et vérifie la valeur de 2 registres de la même famille
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registre à écrire
 * @param value Données à écrire
 * @return unsigned char 0 pas d'erreur, sinon erreur
 */
unsigned char MCP23S17_WriteCheckDoubleRegister(SPI_t *spi, MCP23S17Reg_t reg, unsigned int value);

/**
 * @brief Écrit une valeur sur une broche spécifique du MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 * @param pin Numéro de la broche (0 à 15)
 * @param value Valeur à écrire (0 ou 1)
 * @return unsigned char 0 pas d'erreur, sinon erreur
 */
unsigned char MCP23S17_WriteCheckPin(SPI_t *spi, unsigned char pin, unsigned char value);

/**
 * @brief Lit l'état d'une broche spécifique.
 * @param spi Pointeur vers la configuration SPI
 * @param pin Numéro de la broche (0 à 15)
 * @return État de la broche (0 ou 1)
 */
unsigned char MCP23S17_ReadPin(SPI_t *spi, unsigned char pin);

/**
 * @brief Lit les états des ports de plusieurs modules MCP23S17.
 * @param obj Tableau contenant les configurations des modules
 * @param size Nombre de modules à lire
 */
void MCP23S17_AcknowledgeInterrupt(MCP23S17_t* obj, unsigned char size);

/* ==== Fonctions de calcul ==== */

#endif  // PLIB_MCP23S17_H
