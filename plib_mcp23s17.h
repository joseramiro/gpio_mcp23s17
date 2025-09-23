#ifndef PLIB_MCP23S17_H
#define PLIB_MCP23S17_H

/**
 * @file plib_mcp23s17.h
 * @brief Pilote pour le MCP23S17 (Expander I/O SPI)
 * @author Ramiro Najera
 * @version 1.0.1
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

/** @defgroup MCP23S17_Registers Registres du MCP23S17
 *  @{ */
/** @brief Contrôle la direction des E/S du port A  */
#define MCP23S17_REG_IODIR_A        0x00
/** @brief Contrôle la direction des E/S du port B */
#define MCP23S17_REG_IODIR_B        0x01
/** @brief Configure la polarité des entrées du port A */
#define MCP23S17_REG_IPOL_A         0x02
/** @brief Configure la polarité des entrées du port B */
#define MCP23S17_REG_IPOL_B         0x03
/** @brief Active les interruptions sur changement pour le port A */
#define MCP23S17_REG_GPINTEN_A      0x04
/** @brief Active les interruptions sur changement pour le port Be */
#define MCP23S17_REG_GPINTEN_B      0x05
/** @brief Définit les valeurs de référence pour les interruptions du port A */
#define MCP23S17_REG_DEFVAL_A       0x06
/** @brief Définit les valeurs de référence pour les interruptions du port B */
#define MCP23S17_REG_DEFVAL_B       0x07
/** @brief Configure la comparaison des valeurs d'interruption du port A */
#define MCP23S17_REG_INTCON_A       0x08
/** @brief Configure la comparaison des valeurs d'interruption du port B */
#define MCP23S17_REG_INTCON_B       0x09
/** @brief Configuration générale du module */
#define MCP23S17_REG_IOCON_A        0x0A
/** @brief Deuxième registre de configuration générale */
#define MCP23S17_REG_IOCON_B        0x0B
/** @brief Active les résistances de pull-up pour le port A */
#define MCP23S17_REG_GPPU_A         0x0C
/** @brief Active les résistances de pull-up pour le port B */
#define MCP23S17_REG_GPPU_B         0x0D
/** @brief Indique les interruptions actives sur le port A */
#define MCP23S17_REG_INTF_A         0x0E
/** @brief Indique les interruptions actives sur le port B */
#define MCP23S17_REG_INTF_B         0x0F
/** @brief Capture l'état du port A lors d'une interruption */
#define MCP23S17_REG_INTCAP_A       0x10
/** @brief Capture l'état du port B lors d'une interruption */
#define MCP23S17_REG_INTCAP_B       0x11
/** @brief Accède aux broches du port A  */
#define MCP23S17_REG_GPIO_A         0x12
/** @brief Accède aux broches du port B */
#define MCP23S17_REG_GPIO_B         0x13
/** @brief Verrouillage de sortie pour le port A */
#define MCP23S17_REG_OLAT_A         0x14
/** @brief Verrouillage de sortie pour le port B */
#define MCP23S17_REG_OLAT_B         0x15
/** @} */

/** 
 * @enum MCP23S17IODIR_t
 * @brief Modes de direction des broches 
 */
typedef enum
{
    MCP23S17_IODIR_OUT,  /**< Mode sortie */
    MCP23S17_IODIR_IN    /**< Mode entrée */
}MCP23S17IODIR_t;

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
    unsigned int value;
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

/* ==== Fonctions de configuration ==== */

/**
 * @brief Initialise le module MCP23S17 avec la configuration fournie.
 * @param obj Pointeur vers la configuration du module
 */
void MCP23S17_Init(MCP23S17_t *obj);

/**
 * @brief Active l'adressage matériel du MCP23S17.
 * @param obj Pointeur vers la configuration du module
 */
void MCP23S17_EnableHWAddress(MCP23S17_t *obj);

/* ==== Fonctions d'écriture ==== */

/**
 * @brief Ecrit un ensemble de 2 registres de la même famille
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registrer à écrire
 * @param value Données à écrire
 */
void MCP23S17_WriteRegister(SPI_t *spi, unsigned char reg, unsigned int value);

/**
 * @brief Configure la direction des broches d'un port.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteDirectionReg(SPI_t *spi, unsigned int value);

/**
 * @brief Active ou désactive les interruptions pour un port.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteINTEnableReg(SPI_t *spi, unsigned int value);

/**
 * @brief Définit les valeurs de comparaison des interruptions.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteINTDefaultReg(SPI_t *spi, unsigned int value);

/**
 * @brief Configure la façon dont les interruptions sont déclenchées.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteINTControlReg(SPI_t *spi, unsigned int value);

/**
 * @brief Configure les options du registre de contrôle I/O.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WriteIOControlReg(SPI_t *spi, unsigned int value);

/**
 * @brief Configure les résistances de pull-up pour un port.
 * @param spi Pointeur vers la configuration SPI
 * @param value Données à écrire
 */
void MCP23S17_WritePullupResReg(SPI_t *spi, unsigned int value);

/**
 * @brief Écrit une valeur sur l'ensemble des ports du MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 * @param mask Données à écrire
 */
void MCP23S17_WritePorts(SPI_t *spi, unsigned int mask);

/**
 * @brief Écrit une valeur sur une broche spécifique du MCP23S17.
 * @param obj Pointeur vers la configuration du module
 * @param pin Numéro de la broche (0 à 15)
 * @param value Valeur à écrire (0 ou 1)
 */
void MCP23S17_UpdatePin(MCP23S17_t *obj, unsigned char pin, unsigned char value);

/**
 * @brief Met à jour l'état des ports en fonction d'un masque binaire.
 * @param obj Pointeur vers la configuration du module
 * @param mask Masque binaire des ports à mettre à jour
 */
void MCP23S17_UpdatePorts(MCP23S17_t *obj, unsigned int mask);

/* ==== Fonctions de lecture ==== */

/**
 * @brief Lit un ensemble de 2 registres de la même famille
 * @param spi Pointeur vers la configuration SPI
 * @param reg Registre à lire
 * @return unsigned int Données lues
 */
unsigned int MCP23S17_ReadRegister(SPI_t *spi, unsigned char reg);

/**
 * @brief Lit la direction des broches d'un port.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned int Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadDirectionReg(SPI_t *spi);

/**
 * @brief Lit le flag d'activation des interruptions du port
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadINTEnableReg(SPI_t *spi);

/**
 * @brief Lit les valeurs de comparaison des interruptions.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadINTDefaultReg(SPI_t *spi);

/**
 * @brief Lit la façon dont les interruptions sont déclenchées.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadINTControlReg(SPI_t *spi);

/**
 * @brief Lit les options du registre de contrôle I/O.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadIOControlReg(SPI_t *spi);

/**
 * @brief Lit les résistances de pull-up pour un port.
 * @param spi Pointeur vers la configuration SPI
 * @return unsigned char Valeur du registre sur le port désiré
 */
unsigned int MCP23S17_ReadPullupResReg(SPI_t *spi);

/**
 * @brief Lit les broches ayant déclenché une interruption.
 * @param spi Pointeur vers la configuration SPI
 * @return Masque binaire des broches ayant déclenché une interruption
 */
unsigned int MCP23S17_ReadINTTriggered(SPI_t *spi);

/**
 * @brief Capture l'état des broches lors d'une interruption.
 * @param spi Pointeur vers la configuration SPI
 * @return Valeur capturée des ports
 */
unsigned int MCP23S17_ReadINTCapture(SPI_t *spi);

/**
 * @brief Lit l'état actuel des broches du MCP23S17.
 * @param spi Pointeur vers la configuration SPI
 * @return Masque binaire des états des broches
 */
unsigned int MCP23S17_ReadPorts(SPI_t *spi);

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
void MCP23S17_ReadPortsAllModules(MCP23S17_t* obj, unsigned char size);

/* ==== Fonctions de calcul ==== */

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
