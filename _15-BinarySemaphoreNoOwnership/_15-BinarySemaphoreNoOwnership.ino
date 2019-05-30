 //15-BinarySemaphoreNoOwnership
 //Program to demonstrate the No Owner ship in Binary semaphore 
#include <Arduino_FreeRTOS.h>
TaskHandle_t LPT_TaskHandle;
TaskHandle_t MPT_TaskHandle;
TaskHandle_t HPT_TaskHandle;
SemaphoreHandle_t binSemaphore_A = NULL;
void setup()
{  
    Serial.begin(9600);
    Serial.println(F("In Setup function, Creating Binary Semaphore"));

    vSemaphoreCreateBinary(binSemaphore_A);  /* Create binary semaphore */

    if(binSemaphore_A != NULL)
    {
        Serial.println(F("Creating low priority task"));
        xTaskCreate(LPT_Task, "LPT_Task", 120, NULL, 1, &LPT_TaskHandle);
    }
    else
    {
        Serial.println(F("Failed to create Semaphore"));
    }
}
void loop()
{ // Hooked to Idle Task, will run when CPU is Idle
    Serial.println(F("Loop function"));
    delay(50);
}
/*LPT: Low priority task*/
void LPT_Task(void* pvParameters)
{
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Acquiring semaphore"));        // Print user string 
    xSemaphoreTake(binSemaphore_A,portMAX_DELAY);
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Creating HPT"));        // Print user string
    xTaskCreate(HPT_Task, "HPT_Task", 120, NULL, 3, &HPT_TaskHandle); 
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Creating MPT"));        // Print user string
    xTaskCreate(MPT_Task, "MPT_Task", 120, NULL, 2, &MPT_TaskHandle); 
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Releasing Semaphore"));        // Print user string
    xSemaphoreGive(binSemaphore_A);
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Finally Exiting"));        // Print user string
    vTaskDelete(LPT_TaskHandle);
}
/*MPT: Medium priority task*/
void MPT_Task(void* pvParameters)
{
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(MPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("MPT_Task Releasing the semaphore"));
    xSemaphoreGive(binSemaphore_A);
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(MPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("MPT_Task Done and about to exit"));
    vTaskDelete(MPT_TaskHandle);
}
/*HPT: High priority task*/
void HPT_Task(void* pvParameters)
{
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(HPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("HPT_Task Trying to Acquire the semaphore"));
    xSemaphoreTake(binSemaphore_A,portMAX_DELAY);
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(HPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("HPT_Task Acquired the semaphore"));  
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(HPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("HPT_Task Releasing the semaphore")); 
    xSemaphoreGive(binSemaphore_A);    
    Serial.print(F("Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(HPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("HPT_Task About to Exit"));        // Print user string
    vTaskDelete(HPT_TaskHandle);
}
