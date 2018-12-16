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

// Dépendances
#include "registres_esp8266.h"

/*===============================================================================
  FONCTION      : Set_buffer_to_Registre
  DESCRIPTION   : Ecrit le contenu d'un buffer dans un registre
  PARAMETRES    : Registre ciblé
                  Bit de début ciblé dans le registre
                  Buffer à écrire 
                  Taille du buffer (correspondant au nombre de bits à écrire)
  RETOUR        : rien   
===============================================================================*/
void Set_buffer_to_Registre(__Registre *Registre, uint8 bit_debut, uint32 buffer, uint8 taille_buffer)
{
    for (uint8 i = 0 ; i < taille_buffer; i++)
    {
        if(READ_BIT(buffer,i) == 0)
        {
            CLR_BIT(*Registre,bit_debut + i);
        }
        else
        {
            SET_BIT(*Registre,bit_debut + i);  
        }
    }
}

/*===============================================================================
  FONCTION      : Get_buffer_from_Registre
  DESCRIPTION   : Stocke le contenu (total ou partiel) d'un registre dans un buffer
  PARAMETRES    : Registre ciblé
                  Bit de début ciblé dans le registre
                  Taille du buffer (correspondant au nombre de bits à lire)
  RETOUR        : Buffer 
===============================================================================*/
uint32 Get_buffer_from_Registre(__Registre *Registre, uint8 bit_debut, uint8 taille_buffer)
{
    uint32 buffer_resultat = 0;
    for (uint8 i = 0 ; i < taille_buffer; i++)
    {
        if((READ_BIT(*Registre,(bit_debut + i))) == 0)
        {
            CLR_BIT(buffer_resultat,i);
        }
        else
        {
            SET_BIT(buffer_resultat,i);  
        }
    }
    return buffer_resultat;
}

/*===============================================================================
  FONCTION      : index_iomux
  DESCRIPTION   : Accès au registre IOMUX selon la GPIO voulue
  (Le registre IOMUX est désordonné par rapport au n° des GPIO)
  PARAMETRES    : n° de la GPIO concernée
  RETOUR        : index correspondant au registre ciblé 
===============================================================================*/
uint8 index_iomux(uint8 gpio)
{
    
    switch(gpio)
    {
        case GPIO0 :  return 12; break;
        case GPIO1 :  return 5;  break;  
        case GPIO2 :  return 13; break;
        case GPIO3 :  return 4;  break;
        case GPIO4 :  return 14; break;
        case GPIO5 :  return 15; break;
        case GPIO6 :  return 6;  break;
        case GPIO7 :  return 7;  break;
        case GPIO8 :  return 8;  break;
        case GPIO9 :  return 9;  break;
        case GPIO10 : return 10; break;
        case GPIO11 : return 11; break;
        case GPIO12 : return 0;  break;
        case GPIO13 : return 1;  break;
        case GPIO14 : return 2;  break;
        case GPIO15 : return 3;  break;
        default : return -1; break;
    }
}