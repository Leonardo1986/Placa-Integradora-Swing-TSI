/*
 * protocolo_swing.c
 *
 *  Created on: Sep 8, 2021
 *      Author: gustavo600003
 */

#include "protocolo_swing.h"
//#include "usart.h"
#include <stddef.h>

static uint8_t Package_ChecksumCalc(uint8_t *p_pu8Buffer, uint8_t p_u8QuantBufferCalc)
{
	uint8_t i = 0;
	uint8_t u8Checksum = 0;

	for(i = 0; i < p_u8QuantBufferCalc; i++)
	{
		u8Checksum ^= p_pu8Buffer[i];
	}

	return(u8Checksum);
}


uint8_t Package_Dismount(uint8_t *p_pu8Buffer, uint8_t p_u8Qtd, PortocolPack_t *p_psPacket)
{

	uint8_t checksum = 0;

	if(p_pu8Buffer == NULL || p_psPacket == NULL){
		return(1);
	}

	if(p_u8Qtd != PACKET_SIZE){
		return(2);
	}

	if(p_pu8Buffer[INDEX_STX] != STX || p_pu8Buffer[INDEX_ETX] != ETX){
		return(3);
	}

	checksum = Package_ChecksumCalc(&p_pu8Buffer[INDEX_CMD], p_u8Qtd - 3);		// STX, CHECKSUM e ETX nao entram no calculo do checksum

	if (p_pu8Buffer[INDEX_BCC] != checksum){
		return(4);
	}

	p_psPacket->u8Cmd 			=	p_pu8Buffer[INDEX_CMD];
	p_psPacket->u8Sentido_1 	=	p_pu8Buffer[INDEX_SENTIDO_1];
	p_psPacket->u8Posicao_1 	=	p_pu8Buffer[INDEX_POSICAO_1];
	p_psPacket->u8Velocidade_1 	=	p_pu8Buffer[INDEX_VELOCIDADE_1];
	p_psPacket->u8Sentido_2 	=	p_pu8Buffer[INDEX_SENTIDO_2];
	p_psPacket->u8Posicao_2 	=	p_pu8Buffer[INDEX_POSICAO_2];
	p_psPacket->u8Velocidade_2 	=	p_pu8Buffer[INDEX_VELOCIDADE_2];
	p_psPacket->u8AcessoIn 		=	p_pu8Buffer[INDEX_ACESSO_IN];
	p_psPacket->u8AcessoOut		=	p_pu8Buffer[INDEX_ACESSO_OUT];

	return(0);
}

uint8_t Package_Mount(uint8_t *p_pu8Buffer, uint8_t p_u8Qtd, PortocolPack_t p_sPacket)
{
	uint8_t i = 0;

	if(p_pu8Buffer == NULL){
		return(0);
	}

	if(p_u8Qtd < PACKET_SIZE){
		return(0);
	}

	p_pu8Buffer[i++] = STX;
	p_pu8Buffer[i++] = p_sPacket.u8Cmd;
	p_pu8Buffer[i++] = p_sPacket.u8Sentido_1;
	p_pu8Buffer[i++] = p_sPacket.u8Posicao_1;
	p_pu8Buffer[i++] = p_sPacket.u8Velocidade_1;
	p_pu8Buffer[i++] = p_sPacket.u8Sentido_2;
	p_pu8Buffer[i++] = p_sPacket.u8Posicao_2;
	p_pu8Buffer[i++] = p_sPacket.u8Velocidade_2;
	p_pu8Buffer[i++] = p_sPacket.u8AcessoIn;
	p_pu8Buffer[i++] = p_sPacket.u8AcessoOut;
	p_pu8Buffer[i++] = Package_ChecksumCalc(&p_pu8Buffer[INDEX_CMD], PACKET_SIZE - 3);	// STX, CHECKSUM e ETX nao entram no calculo do checksum
	p_pu8Buffer[i++] = ETX;

	return(i);

}
