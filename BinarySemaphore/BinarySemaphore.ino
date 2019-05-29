/***************************************************************************************************
                                              ExploreEmbedded Copyright Notice 
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Program to demonstrate the Binary semaphore usage and priority inversion 

This code has been developed and tested on ExploreEmbedded boards.  
We strongly believe that the library works on any of development boards for respective controllers. 
Check this link http://www.exploreembedded.com/wiki for awesome tutorials on 8051,PIC,AVR,ARM,Robotics,RTOS,IOT.
ExploreEmbedded invests substantial time and effort developing open source HW and SW tools, to support consider 
buying the ExploreEmbedded boards.
 
The ExploreEmbedded libraries and examples are licensed under the terms of the new-bsd license(two-clause bsd license).
See also: http://www.opensource.org/licenses/bsd-license.php

EXPLOREEMBEDDED DISCLAIMS ANY KIND OF HARDWARE FAILURE RESULTING OUT OF USAGE OF LIBRARIES, DIRECTLY OR
INDIRECTLY. FILES MAY BE SUBJECT TO CHANGE WITHOUT PRIOR NOTICE. THE REVISION HISTORY CONTAINS THE INFORMATION 
RELATED TO UPDATES.
 

Permission to use, copy, modify, and distribute this software and its documentation for any purpose
and without fee is hereby granted, provided that this copyright notices appear in all copies 
and that both those copyright notices and this permission notice appear in supporting documentation.
**************************************************************************************************/

#include <Arduino_FreeRTOS.h>
// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define TEMP_PIN    A0
#define DHTPIN    A1 // Analog Pin sensor is connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

TaskHandle_t LCDDisplay_TaskHandle;
TaskHandle_t TempSenor_TaskHandle;
TaskHandle_t HumiditySensor_TaskHandle;

SemaphoreHandle_t binSemaphore_A = NULL;

#define printMsg(taskhandle,str)  {\
        Serial.print(F("Priority "));\  
        Serial.print(uxTaskPriorityGet(taskhandle));\
        Serial.print(F(" : "));\
        Serial.println(F(str));\        
}

void setup()
{  
  
    dht.begin();
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("hello, world!");

    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("temp  : ");
    lcd.setCursor(0, 1);
    lcd.print("humid : ");
    lcd.display();

    Serial.println(F("Initialize serial"));
    
    Serial.begin(9600);
    
    Serial.println(F("In Setup function, Creating Binary Semaphore"));
    vSemaphoreCreateBinary(binSemaphore_A);  /* Create binary semaphore */
    if(binSemaphore_A == NULL)
    {
        Serial.println(F("Failed to create Semaphore"));
    }
    
    Serial.println(F("Creating  task"));
//    xTaskCreate(LCDDisplay_Task, "LCDDisplay_Task", 100, NULL, 4, &LCDDisplay_TaskHandle);
    xTaskCreate(TempSenor_Task, "LCDDisplay_Task", 100, NULL, 3, &TempSenor_TaskHandle);
    xTaskCreate(HumiditySensor_Task, "LCDDisplay_Task", 100, NULL, 3, &HumiditySensor_TaskHandle);
    
}


void loop()
{ // Hooked to Idle Task, will run when CPU is Idle
    Serial.println(F("Loop function"));
    delay(5000);
}


/*LPT: Low priority task*/
void LCDDisplay_Task(void* pvParameters)
{



    
}


/*MPT: Medium priority task*/
void TempSenor_Task(void* pvParameters)
{
    int temp_count = 0;
    int tempC = 0;
    float voltage = 0;

    while(1)
    {
        temp_count = analogRead(TEMP_PIN);
        voltage = (temp_count / 1024.0) * 5000;
        tempC = voltage / 10;
        
        printMsg(TempSenor_Task,"TempSenor_Task Acquiring semaphore \n"); 
        xSemaphoreTake(binSemaphore_A,portMAX_DELAY);
    
        lcd.setCursor(sizeof("temp  : "), 0);
        lcd.print(tempC);
        
        printMsg(TempSenor_Task,"TempSenor_Task Releasing Semaphore \n");
        xSemaphoreGive(binSemaphore_A);
        vTaskDelay(500);
    }
}


/*MPT: High priority task*/
void HumiditySensor_Task(void* pvParameters)
{
    while(1)
    {
        Serial.print("Current humidity = ");
        Serial.print(dht.readHumidity());
        Serial.print("%  ");
        Serial.print("temperature = ");
        Serial.print(dht.readTemperature()); 
        Serial.println("C  ");
        printMsg(HumiditySensor_Task,"HumiditySensor_Task Acquiring semaphore \n"); 
        xSemaphoreTake(binSemaphore_A,portMAX_DELAY);
        
        lcd.setCursor(sizeof("humid : "), 1);
        lcd.print(dht.readHumidity());
        
        printMsg(HumiditySensor_Task,"HumiditySensor_Task Releasing Semaphore \n");
        xSemaphoreGive(binSemaphore_A);
        vTaskDelay(500);
    }
}
