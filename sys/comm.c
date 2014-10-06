//------------------------------------------------------------------------------
//
// Module : COMM_
//
// Description : Communication Module Core
//               Manages the communication protocol with the host PC
//
// F.C.
// 2014.08.16
//
// Protocol description:
//
//    Start bits | Command | Payload | Content |   CRC   | 
//  |-----------------------------------------------------
//  | 'A' | 'W'  |    C    |    p    | ...D... | C1 | C2 |   
//  |_____________________________________________________
//
//
//------------------------------------------------------------------------------
#define COMM_M

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include <string.h>

#include "uart.h"
#include "comm.h"


//------------------------------------------------------------------------------
//                                                               Defines & Types
//------------------------------------------------------------------------------
typedef enum
{
	PARSER_IDLE = 0,              //Doing nothing
	PARSER_HEADER_ONLY,           //Has received a start sequence
	PARSER_PACKET_INCOMPLETE      //Waiting for the cmd, payload, data and so on.
} E_PARSERSTATE;

typedef enum
{
	CRC_CHECK,
	CRC_WRITE
} E_CRC_MODE;

typedef struct
{
	UINT16 u16CommError;
} S_COMM;

//------------------------------------------------------------------------------
//                                                                     Variables
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
static RNGB_Buffer* pRxBuffer = NULL;
static S_COMMAND command;
static S_COMM Com;

//------------------------------------------------------------------------------
//                                             Prototypes des fonctions internes
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
static Ret_t _ComputeCRC(S_COMMAND* cmd, E_CRC_MODE mode);
static void _SendCommand(S_COMMAND* cmd);

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

void COMM_Init(void)
{
    UART_Init(CFG_UART_BAUDRATE);
    pRxBuffer = UART_GetRxBuffer();
    memset(&Com, 0x00, sizeof(Com));
    memset(&command, 0x00, sizeof(command));
}

/* Update function, to be called periodically
 * This will check the buffer for incoming packets and parse them.
 */
void COMM_Update(void)
{
	static E_PARSERSTATE eStatus = PARSER_IDLE;
	UINT8 tmp;

	/* Handles possible loss of data if the comm. update function
	 * has not been called for some time for ex. */
	if (!RNGB_isDataConsistent(pRxBuffer))
	{
		eStatus = PARSER_IDLE;
		Com.u16CommError++;
	}

	/* Waiting for a header and there might be one in the RxBuffer */
	if (eStatus == PARSER_IDLE)
	{
		/* look for a start sequence */
		while(RNGB_Count(pRxBuffer) >= 2)
		{
			RNGB_Get(pRxBuffer, &tmp);
			if (tmp == COMM_HEADER_1)
			{
				/* look for the next part of the header */
				RNGB_Get(pRxBuffer, &tmp);
				if (tmp == COMM_HEADER_2)
				{
					eStatus = PARSER_HEADER_ONLY;
					break;
				}
			}
		}
	}

	/* The correct header has been received. Now, handles the rest of the data */
	if (eStatus == PARSER_HEADER_ONLY
		&& RNGB_Count(pRxBuffer) >= 2) //Wait for command & payload
	{
		UINT8 plSz= 0; //Payload size
		RNGB_Spy(pRxBuffer, 1, &plSz);

		tmp = 2 + CFG_COMM_CRC_LEN + plSz; /* cmd, pl, data[pl], crc[2] */
		/* check if the whole packet is in the buffer */
		if (RNGB_Count(pRxBuffer) >= tmp)
		{
			RNGB_Get(pRxBuffer, &(command.cmd));
			RNGB_Get(pRxBuffer, &(command.payload));
			if (plSz <= CFG_COMM_MAX_CMD_DATA)
			{
				for(tmp = 0; plSz; tmp++, plSz--)
					RNGB_Get(pRxBuffer, &(command.data[tmp]));
			}
			RNGB_Get(pRxBuffer, &(command.crc[0]));
			RNGB_Get(pRxBuffer, &(command.crc[1]));
			command.eCommandStatus = COMMAND_NEW;
			eStatus = PARSER_IDLE;
		}
	}

	/*TODO: amŽliorer, c'est temporaire a.
	 * ex: mettre en place une file pour les commandes ˆ traiter, et gŽrer
	 * a ailleurs, pour plus de portabilitŽ.
	 */
	if (command.eCommandStatus == COMMAND_NEW)
	{
		COMM_CheckCRC(&command);
		CMD_Execute(&command);
	}
}

/* Check the command's CRC.
 * Returns Ret_OK if CRC is good. Ret_Err otherwise. */
Ret_t COMM_CheckCRC(S_COMMAND* cmd)
{
	//TODO: Implement a CRC mechanism.
	cmd->eCommandStatus = COMMAND_CHECKED;
	return RET_OK;
}

/* Build and send a packet from a command.
 * The CRC and Status fields do not need to be filled */
Ret_t COMM_SendCommand(S_COMMAND* cmd)
{
	if (cmd->payload > CFG_COMM_MAX_CMD_DATA)
		return RET_ERROR;

	_ComputeCRC(cmd, CRC_WRITE);
	_SendCommand(cmd);
	return RET_OK;
}

//------------------------------------------------------------------------------
//                                                           Fonctions internes
//------------------------------------------------------------------------------
/* Compute the checksum for a given command.
 * The function can be used by two ways:
 *   1. Check a CRC (when mode = CRC_CHECK). It will return Ret_OK if the CRC
 *      is OK. Ret_Error otherwise
 *   2. Compute a CRC before sending a packet. here, mode = CRC_WRITE. Ret_OK will
 *      always be returned.
 */
Ret_t _ComputeCRC(S_COMMAND* cmd, E_CRC_MODE mode)
{
	//TODO
	if(mode==CRC_WRITE)
	{
		cmd->crc[0] = 0xAA;
		cmd->crc[1] = 0xBB;
	}
	return RET_OK;
}

/* Send a command to the UART. */
void _SendCommand(S_COMMAND* cmd)
{
	UINT8 i;
	/* We do not do a memcpy for several reasons:
	 * 1. structure padding is compiler dependent.
	 * 2. not really easy to do with a ring buffer.
	 *
	 * there is actually the Uart_sendBuffer function, but we go back to point 1.,
	 * and we do not want our software to look like some famous regulator's */

	/* Header */
	UART_Write_u8(COMM_HEADER_1);
	UART_Write_u8(COMM_HEADER_2);

	/* Packet's core data */
	UART_Write_u8(cmd->cmd);
	UART_Write_u8(cmd->payload);
	for (i = 0; i < cmd->payload; ++i)
	{
		UART_Write_u8(cmd->data[i]);
	}

	/* CRC...
	 * May not seem to be the most efficient way here, but I count on the compiler's optimisation
	 * to unroll the loop. That way the code is easier to modify without loosing on performance. */
	for (i = 0; i < CFG_COMM_CRC_LEN; ++i)
	{
		UART_Write_u8(cmd->crc[i]);
	}
}
//------------------------------------------------------------------------------
