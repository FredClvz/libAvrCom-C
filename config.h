//------------------------------------------------------------------------------
//
//
// Description : Configuration file
//
// F.C.
// 2014.08.16
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef CONFIG_H
#define CONFIG_H
/* Important pre-definitions */
#define YES (0==0)
#define NO  (0!=0)

/* Config goes here: */

/* Debug flags: */
/* Enable all module's debug, or forbid it. */
//#define CFG_ENABLE_ALL_DEBUG
//#define CFG_DISABLE_ALL_DEBUG


//Communication - low level
#define CFG_UART_BAUDRATE         57600  //Bauds
#define CFG_UART_USE_ISR          YES    //Use interrupts in handling the UART
#define CFG_UART_TX_BUFFER_SIZE   0    //Bytes
#define CFG_UART_RX_BUFFER_SIZE   20     //Bytes

//Communication - high level
/*Maximum number of bytes that can be received as the data field in a packet */
#define CFG_COMM_MAX_CMD_DATA     4      //Bytes
#define CFG_COMM_CRC_LEN          2     //Bytes

/*Commands IDS*/
#define CMD_TEST                  0x42

//Sanity checks
#if defined(CFG_ENABLE_ALL_DEBUG) && defined(CFG_DISABLE_ALL_DEBUG)
#error What is wrong with you, seriously?!
#endif
#endif /* CONFIG_H */
