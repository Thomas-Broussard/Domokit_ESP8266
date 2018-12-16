/* 
 *  =============================================================================================================================================
 *  Titre    : Scheduler.cpp
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

#include "Scheduler.h"

// ##########################################################################################################################
//                                     VARIABLES GLOBALES
// ##########################################################################################################################

// Flag d'acquittement
volatile bool flag_tick_us = false;
volatile bool flag_tick_ms = false;
volatile bool flag_tick_s  = false;

// Ticks de cadencement
volatile uint32 tick_ms = TICK_MS_VALUE;
volatile uint32 tick_s  = TICK_S_VALUE;

Compteur_Virtuel Compteur_virtuel_us[NB_COMPTEUR_US];
Compteur_Virtuel Compteur_virtuel_ms[NB_COMPTEUR_MS];
Compteur_Virtuel Compteur_virtuel_s[NB_COMPTEUR_S];
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
void init_TIMER1_Scheduler()
{
    // initialisation des variables
    flag_tick_us = false;
    flag_tick_ms = false;
    flag_tick_s  = false;

    tick_ms = TICK_MS_VALUE;
    tick_s  = TICK_S_VALUE;

    // initialisation du timer
    init_TIMER1(FREQ_SCHEDULER,PREDIV_SCHEDULER);

    // Interruption(s)
    ETS_FRC_TIMER1_INTR_ATTACH(Interruption_SCHEDULER,NULL);
    ETS_FRC1_INTR_ENABLE();
}


/*===============================================================================
  FONCTION      : Interruption_SCHEDULER
  DESCRIPTION   : Interruption déclenchée toute les 10us 
  Utilisée pour cadencer les timers virtuels (1ms et 1s) du scheduler
  PARAMETRES    : aucun
  RETOUR        : rien   
===============================================================================*/
void ICACHE_RAM_ATTR Interruption_SCHEDULER()
{ 
        // décrémentation des compteurs
        tick_ms--; // on décrémente le compteur 1ms
        tick_s--; // on décrémente le compteur 1s
        
        // -------------------------
        // Tâches toutes les 10us
        // -------------------------
        flag_tick_us = true;

        // -------------------------
        // Tâches toutes les 1ms
        // -------------------------
        if (tick_ms == 0)
        {
            tick_ms = TICK_MS_VALUE; // on reset la valeur du compteur
            flag_tick_ms = true;
        }

        // -------------------------
        // Tâches toutes les 1s
        // -------------------------
        if (tick_s == 0)
        {
            tick_s = TICK_S_VALUE; // on reset la valeur du compteur
            flag_tick_s = true;
        }
}


/*===============================================================================
  FONCTION      : Scheduler
  DESCRIPTION   : Routine permettant de gérer les actions à réaliser selon les timers virtuels
  PARAMETRES    : 
  * Fonction à exécuter toute les 10us
  * Fonction à exécuter toute les 1ms
  * Fonction à exécuter toute les 1s
  RETOUR        : rien   
===============================================================================*/
void Scheduler(void (*Fonction_Task_10us)(void),void (*Fonction_Task_1ms)(void),void (*Fonction_Task_1s)(void))
{   
    // -------------------------
    // Tâches toutes les 10us
    // -------------------------
    if(flag_tick_us)
    {
        flag_tick_us = false; // acquittement
        // tâches à exécuter 
        Fonction_Task_10us();
    }

    // -------------------------
    // Tâches toutes les 1ms
    // -------------------------
    if(flag_tick_ms)
    {
        flag_tick_ms = false;// acquittement
        // tâches à exécuter 
        Fonction_Task_1ms();
    }
    
    // -------------------------
    // Tâches toutes les 1s
    // -------------------------
    if(flag_tick_s)
    {
        flag_tick_s = false; // acquittement 
        // tâches à exécuter 
        Fonction_Task_1s(); 
    }
}

/*===============================================================================
  FONCTION      : init_Compteurs_Virtuels
  DESCRIPTION   : initialise tous les compteurs virtuels potentiellement utilisables
  - Par défaut, les timers doivent tous être à 1 et avoir le mode delay désactivé
  
  PARAMETRES    : rien
  RETOUR        : rien   
===============================================================================*/
void init_Compteurs_Virtuels()
{
    for (uint16 i = 0; i < NB_COMPTEUR_US; i++)
    {
        Compteur_virtuel_us[i].valeur = 1;
        Compteur_virtuel_us[i].delay = false;
    }
    for (uint16 i = 0; i < NB_COMPTEUR_MS; i++)
    {
        Compteur_virtuel_ms[i].valeur = 1;
        Compteur_virtuel_ms[i].delay = false;
    }
    for (uint16 i = 0; i < NB_COMPTEUR_S; i++)
    {
        Compteur_virtuel_s[i].valeur = 1;
        Compteur_virtuel_s[i].delay = false;
    }
}

/*===============================================================================
  FONCTION      : Attente
  DESCRIPTION   : active un compteur en mode attente (delay)
  PARAMETRES    : - Compteur à mettre en attente
                  - Temps d'attente  
  RETOUR        : rien   
===============================================================================*/
void Attente(Compteur_Virtuel *mon_compteur, uint16 temps)
{
    mon_compteur->valeur = temps;
    mon_compteur->delay = true;
}