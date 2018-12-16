/* 
 *  =============================================================================================================================================
 *  Titre    : GPIO_esp8266.h
 *  Auteur   : Thomas Broussard
 *  Projet   : Industrialisation ESP8266
 *  Création : Octobre 2018
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description :
 *  Ensemble des définitions nécessaires pour manipuler les GPIO des ESP8266
 * 
 *  Lien utile : https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map
 * =============================================================================================================================================
 */

#ifndef __GPIO_ESP8266_H__
#define __GPIO_ESP8266_H__

// Dépendances
#include "registres_esp8266.h"

// ##########################################################################################################################
//                                      REGISTRE GPIO
// ##########################################################################################################################

// -------------------------------------------------
// Structure du registre
// -------------------------------------------------
typedef struct {
    __Registre OUT;         // Indique l'état logique des sorties
    __Registre OUT_W1TS;    // Registre pour passer une sortie à l'état haut
    __Registre OUT_W1TC;    // Registre pour passer une sortie à l'état bas
    
    __Registre ENABLE;      // Indique les GPIO activées en sortie
    __Registre ENABLE_W1TS; // Registre pour activer les GPIO en sortie
    __Registre ENABLE_W1TC; // Registre pour désactiver les GPIO en sortie

    __Registre IN;          // Registre pour lire l'état d'une GPIO

    __Registre STATUS;      // Indique les interruption activées sur les GPIO
    __Registre STATUS_W1TS; // Registre pour déclencher une interruption sur une GPIO
    __Registre STATUS_W1TC; // Registre pour acquitter une interruption sur une GPIO

    __Registre PIN[15];     // Registres de configuration des GPIO

    __Registre SIGMA_DELTA;
    __Registre RTC_CALIB_SYNC;
    __Registre RTC_CALIB_VALUE;
}GPIO_Struct;

// -------------------------------------------------
// définition du registre
// -------------------------------------------------
#define Registre_GPIO ((GPIO_Struct*) ADDR_GPIO)

// -------------------------------------------------
// Bits utilisés
// -------------------------------------------------

//Registres GPIO->PIN
#define BIT_GPIO_WAKEUP_ENABLE    10 // 0: disable; 1: enable GPIO wakeup CPU, only when GPIO_PIN0_INT_TYPE is 0x4 or 0x5
#define BIT_GPIO_INT_TYPE         7  //[9:7] : type d'interruption (0: disable; 1: positive edge; 2: negative edge; 3: both types of edge; 4: low-level; 5: high-level)
#define BIT_GPIO_DRIVER           2  // 1 : drain ouvert ; 0 : normal
#define BIT_GPIO_SOURCE           0  // 1 : sigma-delta  ; 0 : GPIO_DATA

// ----------------------------------------------------------------------------------------------
// Définition de constantes utiles
// ----------------------------------------------------------------------------------------------

// Définition des etats logiques
#define ETAT_HAUT true
#define ETAT_BAS  false

// Définition des résistances de tirage
#define GPIO_PULLDOWN  0x00
#define GPIO_PULLUP    0x01

// Type de GPIO possibles
#define GPIO_INPUT         0x00
#define GPIO_OUTPUT        0x01

// Type d'interruptions
typedef enum {INACTIF,FRONT_MONTANT,FRONT_DESCENDANT,FRONT_DOUBLE,LOW_LEVEL,HIGH_LEVEL} GPIO_Interrupt;

// ##########################################################################################################################
//                                      REGISTRE IOMUX
// ##########################################################################################################################

// -------------------------------------------------
// Structure du registre
// -------------------------------------------------
typedef struct {
    __Registre RESERVED; // espace vide pour le moment
    __Registre GPIO[15]; // /!\ l'ordre des GPIO n'est pas cohérent avec l'index de ce tableau ! (ex : [0] = GPIO12)
} IOMUX_Struct;

// -------------------------------------------------
// définition du registre
// -------------------------------------------------
#define Registre_IOMUX ((IOMUX_Struct*) ADDR_IOMUX)

// Bits utilisés
#define BIT_IOMUX_PULLUP       7 // [7]
#define BIT_IOMUX_FUNCTION     4 // [6:4] Choix de la fonction à appliquer sur la GPIO
#define BIT_IOMUX_SLEEP_PULLUP 3 // [3]
#define BIT_IOMUX_SLEEP_SEL    1 // [1]
#define BIT_IOMUX_SLEEP_OE     0 // [0]

// ##########################################################################################################################
//                                              FONCTIONS GPIO
// ##########################################################################################################################

/*===============================================================================
  FONCTION      : Choix_fonction_GPIO
  DESCRIPTION   : Permet de choisir la fonction à appliquer à une GPIO
                  (Voir fichier ESP8266_Pin_List.xlsx pour plus de détails)
  PARAMETRES    : N° de la GPIO concernée, numéro de fonction voulue
  RETOUR        : rien   
===============================================================================*/
void Choix_fonction_GPIO(uint8 GPIO, uint8 fonction);

/*===============================================================================
  FONCTION      : init_GPIO
  DESCRIPTION   : Initialise une GPIO en entrée ou sortie numérique
  PARAMETRES    : N° de la GPIO concernée, type voulu (entrée ou sortie)
  RETOUR        : rien   
===============================================================================*/
void init_GPIO(uint8 GPIO, uint8 type_GPIO);

/*===============================================================================
  FONCTION      : GPIO_Write
  DESCRIPTION   : Affecte un état logique à une sortie numérique
  PARAMETRES    : N° de la GPIO concernée, etat logique voulu
  RETOUR        : rien   
===============================================================================*/
void GPIO_Write(uint8 GPIO, bool etat);

/*===============================================================================
  FONCTION      : GPIO_Toggle
  DESCRIPTION   : Inverse l'état logique d'une sortie numérique
  PARAMETRES    : N° de la GPIO concerné
  RETOUR        : rien
===============================================================================*/
void GPIO_Toggle(uint8 GPIO);

/*===============================================================================
  FONCTION      : GPIO_Read
  DESCRIPTION   : Lit l'état logique d'une GPIO
  PARAMETRES    : N° de la GPIO concerné
  RETOUR        : Etat logique de la GPIO
===============================================================================*/
uint8 GPIO_Read(uint8 GPIO);

/*===============================================================================
  FONCTION      : Set_GPIO_Interrupt
  DESCRIPTION   : Permet de définir une interruption sur une GPIO
  PARAMETRES    : N° de la GPIO concerné, type d'interruption voulue
  (Aucune, Front montant, Front descendant , Double front, 'Low level', 'High level')
  RETOUR        : rien
===============================================================================*/
void Set_GPIO_Interrupt(uint8 GPIO, GPIO_Interrupt type_interruption);

/* fin du fichier */
#endif
