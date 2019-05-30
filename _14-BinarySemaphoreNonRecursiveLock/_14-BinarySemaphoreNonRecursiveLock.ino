//14.BinarySemaphoreNonRecursiveLock .. 
//can be used for MutexSemaphore01Priority Inheritance
//Description: Program to demonstrate Non-Recursive Locks in Binary semaphore.
#include <Arduino_FreeRTOS.h>
TaskHandle_t LPT_TaskHandle;
TaskHandle_t HPT_TaskHandle;
SemaphoreHandle_t binSemaphore_A = NULL;
void setup()
{  
    Serial.begin(115200);
    Serial.println(F("In Setup function, Creating Binary Semaphore"));
    vSemaphoreCreateBinary(binSemaphore_A);  /* Create binary semaphore */
    if(binSemaphore_A != NULL)
    {
        Serial.println(F("Creating low priority task"));
        xTaskCreate(LPT_Task, "LPT_Task", 100, NULL, 1, &LPT_TaskHandle);
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
    Serial.print(F("LPT_Task Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Acquiring semaphore"));        // Print user string 
    xSemaphoreTake(binSemaphore_A,portMAX_DELAY);
    Serial.print(F("LPT_Task Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Creating HPT"));        // Print user string
    xTaskCreate(HPT_Task, "HPT_Task", 100, NULL, 3, &HPT_TaskHandle); 
    Serial.print(F("LPT_Task Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Trying to take sempahore again"));        // Print user string
    xSemaphoreTake(binSemaphore_A,portMAX_DELAY);   
    Serial.print(F("LPT_Task Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(LPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("LPT_Task Finally Exiting"));        // Print user string
    vTaskDelete(LPT_TaskHandle);
}
/*HPT: High priority task*/
void HPT_Task(void* pvParameters)
{
    Serial.print(F("HPT_Task Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(HPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("HPT_Task Trying to Acquire the semaphore"));        // Print user string
    xSemaphoreTake(binSemaphore_A,portMAX_DELAY);

    Serial.print(F("HPT_Task Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(HPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("HPT_Task Acquired the semaphore"));        // Print user string

    Serial.print(F("HPT_Task Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(HPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("HPT_Task Releasing the semaphore"));        // Print user string
    xSemaphoreGive(binSemaphore_A);    

    Serial.print(F("HPT_Task Priority "));  // Print task priority 
    Serial.print(uxTaskPriorityGet(HPT_TaskHandle));
    Serial.print(F(" : "));
    Serial.println(F("HPT_Task About to Exit"));        // Print user string
    vTaskDelete(HPT_TaskHandle);
}
