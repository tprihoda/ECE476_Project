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
#define INTERVAL_TIME                   10 
#define PREVIOUS_AVG_WEIGHT             0.5
#define CURRENT_AVG_WEIGHT              0.5 

extern uint8_t socket_num;
extern int sn_bytes_recieved[NUMBER_OF_SOCKETS];

portTASK_FUNCTION_PROTO( vControlTask, pvParameters );

void updateTiming( int sn_alloced_time[] );

void vStartControlTask( void )
{
   xTaskCreate( vControlTask, "Ctrl", 128, NULL, ctrlCONTROL_TASK_PRIORITY, NULL ); 
}

portTASK_FUNCTION( vControlTask, pvParameters )
{
    /* Remove compiler warning */
    ( void ) pvParameters;

    uint8_t sn_alloced_time[NUMBER_OF_SOCKETS];

    writeString("Control Created\n");

    for( ;; ){
        for( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
            socket_num = i;
            vTaskDelay( sn_alloced_time[i] * INTERVAL_TIME / portTICK_PERIOD_MS );
        }
        updateTiming( sn_alloced_time );
    }
/*
    for( ;; )
    {
        socket_num = 0;
        vTaskDelay( sn_alloced_time[0] * INTERVAL_TIME / portTICK_PERIOD_MS );
        socket_num = 1;
        vTaskDelay( sn_alloced_time[1] * INTERVAL_TIME / portTICK_PERIOD_MS );
        socket_num = 2;
        vTaskDelay( sn_alloced_time[2] * INTERVAL_TIME / portTICK_PERIOD_MS );
        socket_num = 3;
        vTaskDelay( sn_alloced_time[3] * INTERVAL_TIME / portTICK_PERIOD_MS );
    }
*/
}

void updateTiming( int sn_alloced_time[] ){
    static int average_recieved[NUMBER_OF_SOCKETS] = {0};
    int total = 0;
    for ( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
        int current_average = sn_bytes_recieved[i] / INTERVAL_TIME;
        //if this is the first pass
        if ( average_recieved[i] == 0 )
            average_recieved[i] = current_average;
        else
            average_recieved[i] = PREVIOUS_AVG_WEIGHT * average_recieved[i] + CURRENT_AVG_WEIGHT * current_average;
        total += average_recieved[i];
    }
    for ( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
        sn_alloced_time[i] = average_recieved[i] / total;
    }
}
