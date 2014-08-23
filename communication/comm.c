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
	PARSER_PACKET_INCOMPLETE     //Waiting for the cmd, payload, data and so on.
} E_PARSERSTATE;

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
		UINT8 pl= 0;
		RNGB_Spy(pRxBuffer, 1, &pl);

		tmp = 2 + CFG_COMM_CRC_LEN + pl; /* cmd, pl, data[pl], crc[2] */
		/* check if the whole packet is in the buffer */
		if (RNGB_Count(pRxBuffer) >= tmp)
		{
			RNGB_Get(pRxBuffer, &(command.cmd));
			RNGB_Get(pRxBuffer, &(command.payload));
			if (pl <= CFG_COMM_MAX_CMD_DATA)
			{
				for(tmp = 0; pl; tmp++, pl--)
					RNGB_Get(pRxBuffer, &(command.data[tmp]));
			}
			RNGB_Get(pRxBuffer, &(command.crc[0]));
			RNGB_Get(pRxBuffer, &(command.crc[1]));
			command.eCommandStatus = COMMAND_NEW;
			eStatus = PARSER_IDLE;
		}
	}

	//TODO: amŽliorer, c'est temporaire a
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
//------------------------------------------------------------------------------
//                                                           Fonctions internes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
