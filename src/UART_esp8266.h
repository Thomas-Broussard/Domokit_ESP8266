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
 
#ifndef __UART_ESP8266_H__
#define __UART_ESP8266_H__

// Dépendances
#include "registres_esp8266.h"
#include "GPIO_esp8266.h"

// Intéressant, à creuser :  https://github.com/scottjgibson/esp8266/blob/master/esp_iot_sdk_v0.6/ld/eagle.rom.addr.v6.ld
/*
PROVIDE ( uartAttach = 0x4000383c );
PROVIDE ( uart_baudrate_detect = 0x40003924 );
PROVIDE ( uart_buff_switch = 0x400038a4 );
PROVIDE ( uart_div_modify = 0x400039d8 );
PROVIDE ( uart_rx_intr_handler = 0x40003bbc );
PROVIDE ( uart_rx_one_char = 0x40003b8c );
PROVIDE ( uart_rx_one_char_block = 0x40003b64 );
PROVIDE ( uart_rx_readbuff = 0x40003ec8 );
PROVIDE ( uart_tx_one_char = 0x40003b30 );
*/

// ##########################################################################################################################
//                                      REGISTRES UART
// ##########################################################################################################################
// -------------------------------------------------
// Structure du registre
// -------------------------------------------------
typedef struct {
    __Registre FIFO;

    __Registre INT_RAW;
    __Registre INT_ST;
    __Registre INT_ENA;
    __Registre INT_CLR;

    __Registre CLKDIV;
    __Registre AUTOBAUD;
    __Registre STATUS;

    __Registre CONF0;
    __Registre CONF1;

    __Registre LOWPULSE;
    __Registre HIGHPULSE;

    __Registre RXD_CNT;
    __Registre DATE;
    __Registre ID;
}UART_Struct;

// -------------------------------------------------
// définition des registres
// -------------------------------------------------
#define Registre_UART0 ((UART_Struct*) ADDR_UART0)
#define Registre_UART1 ((UART_Struct*) ADDR_UART1)

// -------------------------------------------------
// Bits utilisés
// -------------------------------------------------
// UART->STATUS
#define BIT_UART_LEVEL_TXD      31 // Etat de la pin TX
#define BIT_UART_LEVEL_RXD      15 // Etat de la pin RX
#define BIT_UART_TXFIFO_CNT     16 // [23:16] : nombre de données dans la fifo TX

// UART->CONF0
#define BIT_UART_TXFIFO_RST		18 // Mettre à '1' pour faire un reset de la fifo TX
#define BIT_UART_RXFIFO_RST		17 // Mettre à '1' pour faire un reset de la fifo RX

#define BIT_UART_STOPBIT 		4 // [5:4] choix du nombre de bit de stop (0:aucun 1:1bit 2:1.5bits 3:2bits)
#define BIT_UART_NBDATA			2 // [3:2] choix du nombre de données (0:5bits 1:6bits 2:7bits 3:8bits)
#define BIT_UART_PARITY_EN		1 // [1] active la parité (0:inactif 1:actif)
#define BIT_UART_PARITY			0 // [0] défini la parité (0:even 1:odd)

// ----------------------------------------------------------------------------------------------
// Définition de constantes utiles
// ----------------------------------------------------------------------------------------------

// Parité
typedef enum {NONE=0x0,EVEN=0x2,ODD=0x3} UART_Parite;

// Bits de stop
typedef enum {STOP_0,STOP_1,STOP_15,STOP_2} UART_BitStop; // Remarque : STOP_15 signifie 1.5bits de stop

// Nombre de données
typedef enum{DATA_5,DATA_6,DATA_7,DATA_8} UART_BitData;

// Types d'interruption
typedef enum {RX_FULL,RX_OVERFLOW,RX_TIMEOUT,TX_EMPTY_FIFO,TX_ERROR,TX_FLOW_CONTROL} UART_Interrupt;

// UART utilisé
#ifndef UART0
  #define UART0 0x00
#endif
#ifndef UART1
  #define UART1 0x01
#endif


// ##########################################################################################################################
//                                              FONCTIONS UART
// ##########################################################################################################################

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
void init_UART(uint8 UART, uint32 Bauds,UART_BitData Nb_BitData, UART_Parite Parite, UART_BitStop Nb_BitStop);

/*===============================================================================
  FONCTION      : UART_send_tx
  DESCRIPTION   : Envoie un caractère sur la liaison série
  PARAMETRES    : N° de l'UART (0 ou 1)
                  Caractère à envoyer
  RETOUR        : rien   
===============================================================================*/
void UART_send_tx(uint8 UART,uint8 caractere);

/*===============================================================================
  FONCTION      : UART_send_tx
  DESCRIPTION   : Envoie un caractère sur la liaison série 
                  (en prenant en compte les cas spéciaux)
  PARAMETRES    : N° de l'UART (0 ou 1)
                  Caractère à envoyer
  RETOUR        : rien   
===============================================================================*/
void UART_WriteChar(uint8 UART, uint8 Caractere);

/*===============================================================================
  FONCTION      : UART_send_tx
  DESCRIPTION   : Envoie un buffer sur la liaison série 
                  (en prenant en compte les cas spéciaux)
  PARAMETRES    : N° de l'UART (0 ou 1)
                  buffer à envoyer
                  taille du buffer
  RETOUR        : rien   
===============================================================================*/
void UART_WriteBuffer(uint8 UART, uint8 *buffer, uint16 len);

/*===============================================================================
  FONCTION      : UART_send_tx
  DESCRIPTION   : Envoie une chaîne de caractère sur la liaison série 
                  (en prenant en compte les cas spéciaux)
  PARAMETRES    : N° de l'UART (0 ou 1)
                  chaîne de caractère à envoyer
  RETOUR        : rien   
===============================================================================*/
void UART_WriteString(uint8 UART,const char *str);

/* fin du fichier */
#endif
