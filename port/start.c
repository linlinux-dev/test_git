#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define GLOBAL_VAR 	4
QueueHandle_t xQueue;

static void vTask1(void *pvParameters){
	BaseType_t xstatus;
	int32_t ValueToSend;

	ValueToSend = (int32_t) pvParameters;
	
	xstatus = xQueueSendToBack(xQueue, &ValueToSend, 0);

	if(xstatus == pdPASS){

	}
	
	volatile int a = 0;
	while(1){
		a++;
	};
}

static void vTask2(void *pvParameters){

	int32_t lReceivedValue;
	BaseType_t xStatus;
	// const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );

	xStatus = xQueueReceive( xQueue, &lReceivedValue, 0 );

	volatile int b = 0;
	while(1){
		b++;
	};
}

extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void start()
{
	BaseType_t xReturn;

	uint32_t *src = &_sidata;
	uint32_t *dst = &_sdata;
	while(dst < &_edata){
		*dst++ = *src++;
	}

	dst = &_sbss;
	while(dst < &_ebss){
		*dst++ = 0;
	}

	xQueue = xQueueCreate( 5, sizeof( int32_t ) );
	if( xQueue != NULL )
	{
		xReturn = xTaskCreate(vTask1, "T1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xReturn = xTaskCreate(vTask2, "T2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		vTaskStartScheduler();
	}
	return ;
}