/* 
 *  =============================================================================================================================================
 *  Titre    : TIMER_esp8266.cpp
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
#include "TIMER_esp8266.h"

/*===============================================================================
  FONCTION      : init_TIMER1
  DESCRIPTION   : initialise le TIMER1
  PARAMETRES    : Fréquence du Timer voulue, Prédivision requise (1,16 ou 256)
  RETOUR        : rien   
===============================================================================*/
void init_TIMER1(uint32 Frequence_Hz,TIMER_ClkDiv Prediviseur)
{
    uint16 Val_Prediviseur;
    uint32 Val_Compteur;

    // Avant toute chose, on s'assure que le timer est désactivé
    disable_TIMER1();

    // 1 - Choix du prédiviseur à appliquer
    switch(Prediviseur)
    {
        case DIV1   : Val_Prediviseur = 1  ; break; //80MHz (80 ticks/us - 104857.588 us max)
        case DIV16  : Val_Prediviseur = 16 ; break; //5MHz (5 ticks/us - 1677721.4 us max)
        case DIV256 : Val_Prediviseur = 256; break; //312.5Khz (1 tick = 3.2us - 26843542.4 us max)
        default :     Val_Prediviseur = 1  ; break; // par défaut, pas de prédivision
    }
    Set_buffer_to_Registre(&Registre_TIMER1->CTRL_ADDRESS,BIT_TIMER_DIV, Prediviseur,2);

    //Activation du timer
    enable_TIMER1();

    //Activation du mode AutoReload
    SET_BIT(Registre_TIMER1->CTRL_ADDRESS,BIT_TIMER_RELOAD);

    // Définition de la valeur de départ du compteur (calculée selon la fréquence voulue)
    Val_Compteur = (ESP8266_CLOCK_FREQ / (Val_Prediviseur*Frequence_Hz)) ;
    Set_buffer_to_Registre(&Registre_TIMER1->LOAD_ADDRESS,0,Val_Compteur,23);

    //Activation des paramètres liés à l'interruption du timer
    CLR_BIT(Registre_TIMER1->INT_ADDRESS,BIT_TIMER_INT_CLR);
    SET_BIT(Registre_TIMER1_INT->EDGE_ENABLE,1); // Activation de l'interruption type Edge
}

/*===============================================================================
  FONCTION      : enable_TIMER1
  DESCRIPTION   : active le TIMER1
  PARAMETRES    : aucun
  RETOUR        : rien   
===============================================================================*/
void enable_TIMER1()
{
    SET_BIT(Registre_TIMER1->CTRL_ADDRESS,BIT_TIMER_EN);
}

/*===============================================================================
  FONCTION      : enable_TIMER1
  DESCRIPTION   : active le TIMER1
  PARAMETRES    : aucun
  RETOUR        : rien   
===============================================================================*/
void disable_TIMER1()
{
    CLR_BIT(Registre_TIMER1->CTRL_ADDRESS,BIT_TIMER_EN);
}

