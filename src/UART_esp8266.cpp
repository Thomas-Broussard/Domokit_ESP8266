/* 
 *  =============================================================================================================================================
 *  Titre    : UART_esp8266.cpp
 *  Auteur   : Thomas Broussard
 *  Projet   : Industrialisation ESP8266
 *  Création : Octobre 2018
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description :
 *  Ensemble des définitions nécessaires pour manipuler les liaisons UART des ESP8266
 * 
 *  Lien utile : https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map
 * =============================================================================================================================================
 */

// Librairies
#include "UART_esp8266.h"

/*===============================================================================
  FONCTION      : init_UART
  DESCRIPTION   : initialise la liaison série UART voulue
  PARAMETRES    : N° de l'UART (0 ou 1)
                  Vitesse de communication (Bauds)
                  Nombre de bits de données (5,6,7 ou 8)
                  Parité (Aucune, Even, Odd)
                  Nombre de bits de stop (0,1,1.5 ou 2)
  RETOUR        : rien   
===============================================================================*/
void init_UART(uint8 UART, uint32 Bauds,UART_BitData Nb_BitData, UART_Parite Parite, UART_BitStop Nb_BitStop)
{
    uint32 clk_div = ESP8266_CLOCK_FREQ / Bauds ;
    switch(UART)
    {
        case UART0 :
            // Etape 1 : choix des fonctions uart
            Choix_fonction_GPIO(GPIO1,GPIO_FONCTION_1); // TX
            Choix_fonction_GPIO(GPIO3,GPIO_FONCTION_1); // RX 

            // Etape 2 : paramétrage de la com'
            // Parité
            Set_buffer_to_Registre(&Registre_UART0->CONF0,BIT_UART_PARITY,Parite,2);

            // Nombre de données
            Set_buffer_to_Registre(&Registre_UART0->CONF0,BIT_UART_NBDATA,Nb_BitData,2);

            // Nombre de bits de Stop
            Set_buffer_to_Registre(&Registre_UART0->CONF0,BIT_UART_STOPBIT,Nb_BitStop,2);

            // Vitesse de communication
            Set_buffer_to_Registre(&Registre_UART0->CLKDIV,0,clk_div,20);
            
        break;

        case UART1 :
            // Etape 1 : choix des fonctions uart (TX uniquement pour UART1 !)
            Choix_fonction_GPIO(GPIO2,GPIO_FONCTION_3); // TX

            // Etape 2 : paramétrage de la com'
            // Parité
            Set_buffer_to_Registre(&Registre_UART1->CONF0,BIT_UART_PARITY,Parite,2);

            // Nombre de données
            Set_buffer_to_Registre(&Registre_UART1->CONF0,BIT_UART_NBDATA,Nb_BitData,2);

            // Nombre de bits de Stop
            Set_buffer_to_Registre(&Registre_UART1->CONF0,BIT_UART_STOPBIT,Nb_BitStop,2);

            // Vitesse de communication
            Set_buffer_to_Registre(&Registre_UART1->CLKDIV,0,clk_div,20);
        break;

        default:break;
    }
}

/*===============================================================================
  FONCTION      : UART_send_tx
  DESCRIPTION   : Envoie un caractère sur la liaison série
  PARAMETRES    : N° de l'UART (0 ou 1)
                  Caractère à envoyer
  RETOUR        : rien   
===============================================================================*/
void UART_send_tx(uint8 UART,uint8 caractere)
{
    switch(UART)
    {
        case UART0 :
            // Tant que la FIFO TX est pleine, on ne peut pas écrire de nouveaux caractères
            while(Get_buffer_from_Registre(&Registre_UART0->STATUS,BIT_UART_TXFIFO_CNT,8) == 0xFF);
            // Dès que la fifo a un espace libre, on peut envoyer le caractère : 
            Set_buffer_to_Registre(&Registre_UART0->FIFO,0,caractere,8);
        break;

        case UART1 : 
            // Tant que la FIFO TX est pleine, on ne peut pas écrire de nouveaux caractères
            while(Get_buffer_from_Registre(&Registre_UART1->STATUS,BIT_UART_TXFIFO_CNT,8) == 0xFF);
            // Dès que la fifo a un espace libre, on peut envoyer le caractère : 
            Set_buffer_to_Registre(&Registre_UART1->FIFO,8,caractere,8);
        break;
    }
    
    
}

/*===============================================================================
  FONCTION      : UART_send_tx
  DESCRIPTION   : Envoie un caractère sur la liaison série 
                  (en prenant en compte les cas spéciaux)
  PARAMETRES    : N° de l'UART (0 ou 1)
                  Caractère à envoyer
  RETOUR        : rien   
===============================================================================*/
void UART_WriteChar(uint8 UART, uint8 Caractere)
{
    // Gestion du retour-chariot
    if (Caractere == '\n')
    {
        UART_send_tx(UART,'\r');
        UART_send_tx(UART,'\n');
    }
    else
    {
        UART_send_tx(UART,Caractere);
    }
}

/*===============================================================================
  FONCTION      : UART_send_tx
  DESCRIPTION   : Envoie un buffer sur la liaison série 
                  (en prenant en compte les cas spéciaux)
  PARAMETRES    : N° de l'UART (0 ou 1)
                  buffer à envoyer
                  taille du buffer
  RETOUR        : rien   
===============================================================================*/
void UART_WriteBuffer(uint8 UART, uint8 *buffer, uint16 len)
{
    uint16 i;
    for (i = 0; i < len; i++)
    {
        UART_WriteChar(UART,buffer[i]);
    }
}

/*===============================================================================
  FONCTION      : UART_send_tx
  DESCRIPTION   : Envoie une chaîne de caractère sur la liaison série 
                  (en prenant en compte les cas spéciaux)
  PARAMETRES    : N° de l'UART (0 ou 1)
                  chaîne de caractère à envoyer
  RETOUR        : rien   
===============================================================================*/
void UART_WriteString(uint8 UART,const char *str)
{
    while(*str){
        UART_WriteChar(UART,*str++);
    }
}