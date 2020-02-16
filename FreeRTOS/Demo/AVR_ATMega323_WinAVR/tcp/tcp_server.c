/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* AVR include files. */
#define F_CPU       16000000
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Application include files. */
#include "tcp_server.h"
#include "socket.h"
#include "uart_32u4.h"

uint8_t socket_num = 0;
portBASE_TYPE xServerConnEstablished = pdFALSE;

/* TCP server tasks prototype */
portTASK_FUNCTION_PROTO( vTCPServerTask, pvParameters );

/* Static prototypes for methods in this file. */
static void vTCPServerInit( void );
static portBASE_TYPE xServerStatus( eSocketNum sn );

void vStartTCPServerTask( void )
{
    vTCPServerInit();
    xTaskCreate( vTCPServerTask, "TCP", 1024, NULL, tcpTCP_SERVER_TASK_PRIORITY, NULL );
}

static void vTCPServerInit( void )
{
    portENTER_CRITICAL();
    {
        vInitSPI();

        /* Wiznet chip setup time. */
        _delay_ms(500);
        
        struct wiz_NetInfo_t network_config = 
        {
            { tcpMAC },
            { tcpIP },
            { tcpSUBNET },
            { tcpGATEWAY },
            { tcpDNS },
            2
        };

        /* Allocate 1KB for tx and rx buffer for socket 0 */
        uint8_t txsize[8] = { 1, 1, 1, 1, 0, 0, 0, 0 };
        uint8_t rxsize[8] = { 1, 1, 1, 1, 0, 0, 0, 0 };
        
        /* Initialize network configuration and buffer size. */
        wizchip_setnetinfo( &network_config );
        wizchip_init( txsize, rxsize );
        setPHYCFGR(0xF8);
    }
    portEXIT_CRITICAL();
}

static portBASE_TYPE xServerStatus( eSocketNum sn )
{
int8_t ret;

    switch( getSn_SR( sn ) )
    {
        /* Socket connection established with peer - SYN packet received */
        case SOCK_ESTABLISHED:
            if( getSn_IR( sn ) & Sn_IR_CON )
            {
                writeString("Socket Established\n");
                /* Clear CON interrupt bit issued from successful connection */
                setSn_IR( sn, Sn_IR_CON );
            }
            xServerConnEstablished = pdTRUE;
            break;

        /* Socket n received disconnect-request (FIN packet) from connected peer */
        case SOCK_CLOSE_WAIT:
            if( ( ret = disconnect( sn ) ) != SOCK_OK ) return ret;
            xServerConnEstablished = pdFALSE;
            writeString("Closing socket...\n");
            break;

        /* Socket n is opened with TCP mode, start listening for peer */
        case SOCK_INIT:
            if( ( ret = listen( sn ) ) != SOCK_OK ) return ret;
            writeString("Socket initialized... listening for connection\n");
            break;

         /* Socket n is closed, configure TCP server for socket n on port 8080 */
        case SOCK_CLOSED:
            if( ( ret = socket( sn, Sn_MR_TCP, 8080 + sn, 0x00 ) ) != sn ) return ret;
            writeString("Socket closed... opening\n");
            break;

        default:
            break;
    }
    return 1;
} 

portTASK_FUNCTION( vTCPServerTask, pvParameters )
{
    /* Remove compiler warning */
    ( void ) pvParameters;

uint8_t buffer[ DATA_BUF_SIZE ];
int32_t ret;
int16_t size = 0, sentsize = 0;
portBASE_TYPE status;

    for( ;; )
    {
        status = xServerStatus( socket_num );

        if( xServerConnEstablished == pdTRUE )
        {
            if( ( size = getSn_RX_RSR( socket_num ) ) > 0 )
            {
                if( size > DATA_BUF_SIZE ) size = DATA_BUF_SIZE;
                ret = recv( socket_num, buffer, size );

                if(ret <= 0) 
                {
                    //error
                }
                size = (uint16_t) ret;
                sentsize = 0;

                while(size != sentsize)
                {
                    ret = send( socket_num, buffer+sentsize, size-sentsize );
                    if( ret < 0)
                    {
                        close( socket_num );
                    }
                    sentsize += ret;
                }
                //setSn_IR( socket_num, Sn_IR_RECV );
            }
        }
    }
}
