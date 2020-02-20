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
#define INTERVAL_TIME                   400 
#define PREVIOUS_AVG_WEIGHT             0.8
#define CURRENT_AVG_WEIGHT              0.2 
#define DEFAULT_TIME                    0.01

extern uint8_t socket_num;
extern uint32_t sn_bytes_recieved[NUMBER_OF_SOCKETS];

portTASK_FUNCTION_PROTO( vControlTask, pvParameters );

void updateTiming( float sn_alloced_time[] );

void vStartControlTask( void )
{
   xTaskCreate( vControlTask, "Ctrl", 128, NULL, ctrlCONTROL_TASK_PRIORITY, NULL ); 
}

portTASK_FUNCTION( vControlTask, pvParameters )
{
    /* Remove compiler warning */
    ( void ) pvParameters;

    float sn_alloced_time[NUMBER_OF_SOCKETS];

    writeString("Control Created\n");

    for( ;; ){
        //reset bytes_recieved
        //for( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
        //    sn_bytes_recieved[i] = 0;
        //}
        for( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
            socket_num = i;
            vTaskDelay( ( sn_alloced_time[i] * INTERVAL_TIME ) / portTICK_PERIOD_MS );
        }
        char temp[10];
        updateTiming( sn_alloced_time );
        writeString("Time Alloced: \n");
        dtostrf(sn_alloced_time[0], 4, 2,temp);
        writeString(temp);
        writeString("\n");
        dtostrf(sn_alloced_time[1], 4, 2,temp);
        writeString(temp);
        writeString("\n");
        dtostrf(sn_alloced_time[2], 4, 2,temp);
        writeString(temp);
        writeString("\n");
        dtostrf(sn_alloced_time[3], 4, 2,temp);
        writeString(temp);
        writeString("\n");
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

void updateTiming( float sn_alloced_time[] ){
    static float average_recieved[NUMBER_OF_SOCKETS] = {0};
    float total = 0;
    for ( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
        float current_average = sn_bytes_recieved[i] / INTERVAL_TIME;
        if ( current_average == 0 )
            current_average = DEFAULT_TIME;
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
