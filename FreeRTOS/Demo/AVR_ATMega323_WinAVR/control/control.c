/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "control.h"

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "../uart_32u4.h"

#define ctrlCONTROL_TASK_PRIORITY       (tskIDLE_PRIORITY + 3)

extern uint8_t socket_num;

portTASK_FUNCTION_PROTO( vControlTask, pvParameters );

void vStartControlTask( void )
{
   xTaskCreate( vControlTask, "Ctrl", 256, NULL, ctrlCONTROL_TASK_PRIORITY, NULL ); 
}

portTASK_FUNCTION( vControlTask, pvParameters )
{
    /* Remove compiler warning */
    ( void ) pvParameters;

    for( ;; )
    {
        socket_num = 0;
        vTaskDelay( 10 / portTICK_PERIOD_MS );
        socket_num = 1;
        vTaskDelay( 10 / portTICK_PERIOD_MS );
        socket_num = 2;
        vTaskDelay( 10 / portTICK_PERIOD_MS );
        socket_num = 3;
        vTaskDelay( 10 / portTICK_PERIOD_MS );
    }
}
