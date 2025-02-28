/*
 * protocolo_swing.h
 *
 *  Created on: Sep 8, 2021
 *      Author: gustavo600003
 */


#include <sys/_stdint.h>

#ifndef DRV_PROTOCOLO_SWING_H_
#define DRV_PROTOCOLO_SWING_H_

#define INDEX_STX			0
#define INDEX_CMD			1
#define INDEX_SENTIDO_1		2
#define INDEX_POSICAO_1		3
#define INDEX_VELOCIDADE_1	4
#define INDEX_SENTIDO_2		5
#define INDEX_POSICAO_2		6
#define INDEX_VELOCIDADE_2	7
#define INDEX_ACESSO_IN		8
#define INDEX_ACESSO_OUT	9
#define INDEX_BCC			10
#define INDEX_ETX			11
#define PACKET_SIZE			12

#define STX					0X02
#define	ETX					0X03

typedef struct PortocolPack
{
	uint8_t u8Cmd;
	uint8_t u8Sentido_1;
	uint8_t u8Posicao_1;
	uint8_t u8Velocidade_1;
	uint8_t u8Sentido_2;
	uint8_t u8Posicao_2;
	uint8_t u8Velocidade_2;
	uint8_t u8AcessoIn;
	uint8_t u8AcessoOut;
} PortocolPack_t;

uint8_t Package_Dismount(uint8_t *p_pu8Buffer, uint8_t p_u8Qtd, PortocolPack_t *p_psPacket);
uint8_t Package_Mount(uint8_t *p_pu8Buffer, uint8_t p_u8Qtd, PortocolPack_t p_sPacket);

#endif /* DRV_PROTOCOLO_SWING_H_ */
