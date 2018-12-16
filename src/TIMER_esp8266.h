/* 
 *  =============================================================================================================================================
 *  Titre    : TIMER_esp8266.h
 *  Auteur   : Thomas Broussard
 *  Projet   : Industrialisation ESP8266
 *  Création : Octobre 2018
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description :
 *  Ensemble des définitions nécessaires pour manipuler les TIMER des ESP8266
 * 
 *  Lien utile : https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map
 * =============================================================================================================================================
 */
#ifndef __TIMER_ESP8266_H__
#define __TIMER_ESP8266_H__

#include "registres_esp8266.h"

// ##########################################################################################################################
//                                      REGISTRES TIMER
// ##########################################################################################################################

// -------------------------------------------------
// Structure du registre
// -------------------------------------------------
typedef struct {
    __Registre LOAD_ADDRESS;
    __Registre COUNT_ADDRESS;
    __Registre CTRL_ADDRESS;
    __Registre INT_ADDRESS;
    __Registre ALARM_ADDRESS;       // /!\ : valable uniquement pour le TIMER2 !
}TIMER_Struct;

typedef struct {
    __Registre EDGE_ENABLE;
}TIMER1_INT_Struct;

// -------------------------------------------------
// définition des registres
// -------------------------------------------------
// Timer 1 : compteur décrémental 23 bits
#define Registre_TIMER1 ((TIMER_Struct*) ADDR_TIMER1)
#define Registre_TIMER1_INT ((TIMER1_INT_Struct*) ADDR_TIMER1_INT)

// Timer 2 : compteur incrémental 32 bits
#define Registre_TIMER2 ((TIMER_Struct*) ADDR_TIMER2)

// -------------------------------------------------
// Bits utilisés
// -------------------------------------------------
// TIMER->CTRL_ADDRESS
#define BIT_TIMER_INT       8 // statut de l'interruption (timer 1 : arrivé à zéro / timer 2: arrivé à seuil alarme)
#define BIT_TIMER_EN        7 // Activation du timer
#define BIT_TIMER_RELOAD    6 // rechargement automatique du timer
#define BIT_TIMER_DIV       2 // [3:2] : valeur du prédiviseur de fréquence (0:1 / 1:16 / 2:256 / 3:256)
#define BIT_TIMER_INT_TYPE  0 // Type d'interruption (0:edge / 1:level)

// TIMER->INT_ADDRESS
#define BIT_TIMER_INT_CLR   0 // acquittement de l'interruption (si elle est de type "level")

// ----------------------------------------------------------------------------------------------
// Définition de constantes utiles
// ----------------------------------------------------------------------------------------------
// Types d'interruption
typedef enum {EDGE,LEVEL} TIMER_Interrupt;
typedef enum {DIV1,DIV16,DIV256}TIMER_ClkDiv;

// ##########################################################################################################################
//                                      FONCTIONS TIMER
// ##########################################################################################################################

/*===============================================================================
  FONCTION      : enable_TIMER1
  DESCRIPTION   : active le TIMER1
  PARAMETRES    : aucun
  RETOUR        : rien   
===============================================================================*/
void enable_TIMER1();

/*===============================================================================
  FONCTION      : enable_TIMER1
  DESCRIPTION   : active le TIMER1
  PARAMETRES    : aucun
  RETOUR        : rien   
===============================================================================*/
void disable_TIMER1();

/*===============================================================================
  FONCTION      : init_TIMER1
  DESCRIPTION   : initialise le TIMER1
  PARAMETRES    : Fréquence du Timer voulue, Prédivision requise (1,16 ou 256)
  RETOUR        : rien   
===============================================================================*/
void init_TIMER1(uint32 Frequence_Hz,TIMER_ClkDiv Prediviseur);



#endif