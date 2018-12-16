/* 
 *  =============================================================================================================================================
 *  Titre    : GPIO_esp8266.cpp
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

// Librairies
#include "GPIO_esp8266.h"
#include "registres_esp8266.h"

/*===============================================================================
  FONCTION      : Choix_fonction_GPIO
  DESCRIPTION   : Permet de choisir la fonction à appliquer à une GPIO
                  (Voir fichier ESP8266_Pin_List.xlsx pour plus de détails)
  PARAMETRES    : N° de la GPIO concernée, numéro de fonction voulue
  RETOUR        : rien   
===============================================================================*/
void Choix_fonction_GPIO(uint8 GPIO, uint8 fonction)
{
    if (fonction >= GPIO_FONCTION_1 && fonction <= GPIO_FONCTION_5)
    {
        Set_buffer_to_Registre(&Registre_IOMUX->GPIO[index_iomux(GPIO)],BIT_IOMUX_FUNCTION,fonction,3);
    }
}



/*===============================================================================
  FONCTION      : init_GPIO
  DESCRIPTION   : Initialise une GPIO en entrée ou sortie numérique
  PARAMETRES    : N° de la GPIO concernée, type voulu (entrée ou sortie)
  RETOUR        : rien   
===============================================================================*/
void init_GPIO(uint8 GPIO, uint8 type_GPIO)
{
    // Etape 1 : on choisi la fonction GPIO (voir document ESP8266_Pin_List.xlsx)
    if (GPIO == 0 || GPIO == 2 || GPIO == 4 || GPIO == 5)
    {
        Choix_fonction_GPIO(GPIO,GPIO_FONCTION_1);
    }
    else
    {
        Choix_fonction_GPIO(GPIO,GPIO_FONCTION_4);
    }
    
    // Etape 2 : On détermine le type de la GPIO (entrée ou sortie)
    switch (type_GPIO)
    {
        // Entrée numérique
        case GPIO_INPUT :
            SET_BIT(Registre_GPIO->ENABLE_W1TC,GPIO);
        break;

        // Sortie numérique
        case GPIO_OUTPUT :
            CLR_BIT(Registre_GPIO->PIN[GPIO],BIT_GPIO_DRIVER); // On ferme le drain
            SET_BIT(Registre_IOMUX->GPIO[index_iomux(GPIO)] ,BIT_IOMUX_PULLUP); // on active le pull-up
            SET_BIT(Registre_GPIO->ENABLE_W1TS,GPIO); // on active la gpio en sortie
            SET_BIT(Registre_GPIO->OUT_W1TC,GPIO); // Valeur par défaut de la sortie
        break;

        // Par défaut, la GPIO est une entrée
        default : 
            CLR_BIT(Registre_IOMUX->GPIO[index_iomux(GPIO)],BIT_IOMUX_PULLUP); // on désactive le pull-up
            SET_BIT(Registre_GPIO->ENABLE_W1TC,GPIO); // on désactive la gpio en sortie
        break;
    }
}

/*===============================================================================
  FONCTION      : GPIO_Write
  DESCRIPTION   : Affecte un état logique à une sortie numérique
  PARAMETRES    : N° de la GPIO concernée, etat logique voulu
  RETOUR        : rien   
===============================================================================*/
void GPIO_Write(uint8 GPIO, bool etat)
{
    
    SET_BIT(Registre_GPIO->ENABLE_W1TS,GPIO);
    
    if (etat == ETAT_HAUT)
    {
		SET_BIT(Registre_GPIO->OUT_W1TS,GPIO);
	}
    else
    {
		SET_BIT(Registre_GPIO->OUT_W1TC,GPIO);
	}
}

/*===============================================================================
  FONCTION      : GPIO_Toggle
  DESCRIPTION   : Inverse l'état logique d'une sortie numérique
  PARAMETRES    : N° de la GPIO concerné
  RETOUR        : rien
===============================================================================*/
void GPIO_Toggle(uint8 GPIO)
{
    GPIO_Write(GPIO, 1 - READ_BIT(Registre_GPIO->IN,GPIO));
}


/*===============================================================================
  FONCTION      : GPIO_Read
  DESCRIPTION   : Lit l'état logique d'une GPIO
  PARAMETRES    : N° de la GPIO concerné
  RETOUR        : Etat logique de la GPIO
===============================================================================*/
uint8 GPIO_Read(uint8 GPIO)
{
    return READ_BIT(Registre_GPIO->IN,GPIO);
}


/*===============================================================================
  FONCTION      : Set_GPIO_Interrupt
  DESCRIPTION   : Permet de définir une interruption sur une GPIO
  PARAMETRES    : N° de la GPIO concerné, type d'interruption voulue
  (Aucune, Front montant, Front descendant , Double front, 'Low level', 'High level')
  RETOUR        : rien
===============================================================================*/
void Set_GPIO_Interrupt(uint8 GPIO, GPIO_Interrupt type_interruption)
{
    Set_buffer_to_Registre(&Registre_GPIO->PIN[GPIO],BIT_GPIO_INT_TYPE,type_interruption,3);
}
