/* 
 *  =============================================================================================================================================
 *  Titre    : registres_esp8266.cpp
 *  Auteur   : Thomas Broussard
 *  Projet   : Industrialisation ESP8266
 *  Création : Octobre 2018
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description :
 *  Ensemble des définitions nécessaires pour manipuler les registres des ESP8266
 * 
 *  Lien utile : https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map
 * =============================================================================================================================================
 */

#ifndef __REGISTRES_ESP8266_H__
#define __REGISTRES_ESP8266_H__

// Dépendances
#include "ets_sys.h"

// ##########################################################################################################################
//                                          Définition des types
// ##########################################################################################################################
// données 8 bits
typedef unsigned char       uint8;  // entier 8 bits non signé
typedef signed char         int8;   // entier 8 bits signé

// données 16 bits
typedef unsigned short      uint16; // entier 16 bits non signé
typedef signed short        int16;  // entier 16 bits signé

// données 32 bits
typedef unsigned int       uint32;  // entier 32 bits non signé
typedef signed int         int32;   // entier 32 bits signé

// types spécifiques
typedef uint32 __Registre; // Registre 32 bits (utilisé pour la création du mapping mémoire)


#ifndef NULL
  #define NULL __null
#endif
// ##########################################################################################################################
//                                          Manipulation des registres ESP8266
// ##########################################################################################################################
// -------------------------------------------------
// Macros pour manipuler les bits des registres
// -------------------------------------------------
#define SET_BIT(registre,bit)   ((registre) |= ((1) << (bit)))    // Passe le bit d'un registre à '1'
#define CLR_BIT(registre,bit)   ((registre) &= ~((1) << (bit)))   // Passe le bit d'un registre à '0'
#define TOG_BIT(registre,bit)   ((registre) ^= ~((1) << (bit)))   // Inverse le bit d'un registre
#define READ_BIT(registre,bit)  ((registre >> bit) & (1))         // Lit l'état d'un bit d'un registre 


// -------------------------------------------------
// Mapping mémoire de l'esp8266
// -------------------------------------------------
#define ADDR_UART0	0x60000000
#define ADDR_SPI1	  0x60000100 
#define ADDR_SPIO0	0x60000200
#define ADDR_GPIO	  0x60000300
#define ADDR_TIMER1	0x60000600
#define ADDR_TIMER2 0x60000620
#define ADDR_RTC	  0x60000700
#define ADDR_IOMUX	0x60000800
#define ADDR_I2C	  0x60000D00
#define ADDR_UART1	0x60000F00
#define ADDR_RTCB	  0x60001000
#define ADDR_RTCS	  0x60001100
#define ADDR_RTCU	  0x60001200

// Registres spéciaux
#define ADDR_TIMER1_INT 0x3ff00004 // utilisée pour activer l'interruption "Edge" sur le TIMER1

// ##########################################################################################################################
//                                          Constantes utiles
// ##########################################################################################################################
// Fréquence du microcontrôleur (Hz)
#define ESP8266_CLOCK_FREQ 80000000

// Définition des GPIO
#define GPIO0   0x00
#define GPIO1   0x01
#define GPIO2   0x02
#define GPIO3   0x03
#define GPIO4   0x04
#define GPIO5   0x05
#define GPIO6   0x06
#define GPIO7   0x07
#define GPIO8   0x08
#define GPIO9   0x09
#define GPIO10  0x0a
#define GPIO11  0x0b
#define GPIO12  0x0c
#define GPIO13  0x0d
#define GPIO14  0x0e
#define GPIO15  0x0f

// Correspondance des GPIO avec le NodeMCU
#define D1  GPIO5
#define D2  GPIO4
#define D3  GPIO0
#define D4  GPIO2
#define D5  GPIO14
#define D6  GPIO12
#define D7  GPIO13
#define D8  GPIO15
#define RX  GPIO3
#define TX  GPIO1
#define SD2 GPIO9
#define SD3 GPIO10

// Fonction possibles sur les GPIO (voir fichier ESP8266_pin_list.xlsx)
#define GPIO_FONCTION_1    0x00
#define GPIO_FONCTION_2    0x01
#define GPIO_FONCTION_3    0x02 
#define GPIO_FONCTION_4    0x03
#define GPIO_FONCTION_5    0x04

// ##########################################################################################################################
//                                          FONCTIONS
// ##########################################################################################################################
// Manipulation des registres

/*===============================================================================
  FONCTION      : Set_buffer_to_Registre
  DESCRIPTION   : Ecrit le contenu d'un buffer dans un registre
  PARAMETRES    : Registre ciblé
                  Bit de début ciblé dans le registre
                  Buffer à écrire 
                  Taille du buffer (correspondant au nombre de bits à écrire)
  RETOUR        : rien   
===============================================================================*/
void Set_buffer_to_Registre(__Registre *Registre, uint8 bit_debut, uint32 buffer, uint8 taille_buffer);

/*===============================================================================
  FONCTION      : Get_buffer_from_Registre
  DESCRIPTION   : Stocke le contenu (total ou partiel) d'un registre dans un buffer
  PARAMETRES    : Registre ciblé
                  Bit de début ciblé dans le registre
                  Taille du buffer (correspondant au nombre de bits à lire)
  RETOUR        : Buffer 
===============================================================================*/
uint32 Get_buffer_from_Registre(__Registre *Registre, uint8 bit_debut, uint8 taille_buffer);

/*===============================================================================
  FONCTION      : index_iomux
  DESCRIPTION   : Accès au registre IOMUX selon la GPIO voulue
  (Le registre IOMUX est désordonné par rapport au n° des GPIO)
  PARAMETRES    : n° de la GPIO concernée
  RETOUR        : index correspondant au registre ciblé 
===============================================================================*/
uint8 index_iomux(uint8 gpio);

/* fin du fichier */
#endif
