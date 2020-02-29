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
#define INTERVAL_TIME                   250 
#define PREVIOUS_AVG_WEIGHT             0.6f
#define CURRENT_AVG_WEIGHT              0.4f
#define DEFAULT_TIME                    1

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
    uint8_t counter = 0;

    for( ;; ){
        //reset bytes_recieved
        for( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
            sn_bytes_recieved[i] = 0;
        }
        for( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
            socket_num = i;
            vTaskDelay( ( sn_alloced_time[i] * INTERVAL_TIME ) / portTICK_PERIOD_MS );
        }
        if ( counter >= 1000/INTERVAL_TIME ){
            counter = 0;
            char temp[10];
            updateTiming( sn_alloced_time );
            writeString("Time Alloced: \n");
            for ( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
                dtostrf(sn_alloced_time[i], 4, 2,temp);
                writeString(temp);
                writeString("\n");
            }
            writeString("Bytes Recieved: \n");
            for ( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
                itoa(sn_bytes_recieved[i], temp, 10);
                writeString(temp);
                writeString("\n");
            }

        }
        else
            counter++;
    }
}

void updateTiming( float sn_alloced_time[] ){
    static float average_recieved[NUMBER_OF_SOCKETS] = {0};
    float total = 0;
    for ( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
        if ( sn_bytes_recieved[i] == 0 )
            sn_bytes_recieved[i] = DEFAULT_TIME;
        //if this is the first pass
        if ( average_recieved[i] == 0 )
            average_recieved[i] = sn_bytes_recieved[i];
        else
            average_recieved[i] = PREVIOUS_AVG_WEIGHT * average_recieved[i] + CURRENT_AVG_WEIGHT * sn_bytes_recieved[i];
        total += average_recieved[i];
    }
    for ( uint8_t i = 0; i < NUMBER_OF_SOCKETS; i ++ ){
        sn_alloced_time[i] = average_recieved[i] / total;
    }
}
