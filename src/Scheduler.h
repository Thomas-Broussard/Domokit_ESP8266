/* 
 *  =============================================================================================================================================
 *  Titre    : Scheduler.h
 *  Auteur   : Thomas Broussard
 *  Projet   : Industrialisation ESP8266
 *  Création : Octobre 2018
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description :
 *  Permet d'utiliser le TIMER1 de l'ESP8266 pour générer des timers virtuels
 *  Ces timers seront utilisés pour cadencer des actions à des temps définis.
 *  Exemples : 10us, 1ms, 1s
 * =============================================================================================================================================
 */

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

// Dépendance(s)
#include "TIMER_esp8266.h"
#include "GPIO_esp8266.h"


typedef struct{
  uint16 valeur;
  bool delay;
} Compteur_Virtuel;




// ##########################################################################################################################
//                                     DEFINE ET VARIABLES GLOBALES
// ##########################################################################################################################

// Paramètres du scheduler
#define FREQ_SCHEDULER 100000
#define PREDIV_SCHEDULER DIV16
#define VAL_PREDIV_SCHEDULER 16

// Tick pour le scheduler
// initial :  1 tick / 10us
#define TICK_MS_VALUE 100 // 1 tick / 1ms
#define TICK_S_VALUE  (1000 * TICK_MS_VALUE) // 1 tick / 1s

// Nombre de timers programmés
#define NB_COMPTEUR_US 10
#define NB_COMPTEUR_MS 10
#define NB_COMPTEUR_S 10

// Création de compteurs virtuels
extern Compteur_Virtuel Compteur_virtuel_us[NB_COMPTEUR_US];
extern Compteur_Virtuel Compteur_virtuel_ms[NB_COMPTEUR_MS];
extern Compteur_Virtuel Compteur_virtuel_s[NB_COMPTEUR_S];

// ##########################################################################################################################
//                                      FONCTIONS SCHEDULER
// ##########################################################################################################################

/*===============================================================================
  FONCTION      : init_TIMER1_Scheduler
  DESCRIPTION   : initialise le TIMER1 avec les paramètres requis pour faire le Scheduler : 
                  - Fréquence = 100kHz
                  - Prédiviseur = 16
  Le Scheduler permettra de cadencer des actions à 10us , 1ms et 1s
  PARAMETRES    : aucun
  RETOUR        : rien   
===============================================================================*/
void init_TIMER1_Scheduler();


/*===============================================================================
  FONCTION      : Interruption_SCHEDULER
  DESCRIPTION   : Interruption déclenchée toute les 10us 
  Utilisée pour cadencer les timers virtuels (1ms et 1s) du scheduler
  PARAMETRES    : aucun
  RETOUR        : rien   
===============================================================================*/
void ICACHE_RAM_ATTR Interruption_SCHEDULER();


/*===============================================================================
  FONCTION      : Scheduler
  DESCRIPTION   : Routine permettant de gérer les actions à réaliser selon les timers virtuels
  PARAMETRES    : 
  * Fonction à exécuter toute les 10us
  * Fonction à exécuter toute les 1ms
  * Fonction à exécuter toute les 1s
  RETOUR        : rien   
===============================================================================*/
void Scheduler(void (*Fonction_Task_10us)(void),void (*Fonction_Task_1ms)(void),void (*Fonction_Task_1s)(void));

/*===============================================================================
  FONCTION      : init_Compteurs_Virtuels
  DESCRIPTION   : initialise tous les compteurs virtuels potentiellement utilisables
  PARAMETRES    : rien
  RETOUR        : rien   
===============================================================================*/
void init_Compteurs_Virtuels();

/*===============================================================================
  FONCTION      : Attente
  DESCRIPTION   : passe un compteur en mode attente
  PARAMETRES    : - Compteur à mettre en attente
                  - Temps d'attente  
  RETOUR        : rien   
===============================================================================*/
void Attente(Compteur_Virtuel *mon_compteur, uint16 temps);

#endif