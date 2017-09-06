/*
 * Copyright 2010-2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <ti/drivers/GPIO.h>

#include "Board.h"

#include <ti/sysbios/knl/Task.h>
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"
#include "aws_iot_config.h"
#include "aws_iot_shadow_json_data.h"
#include "aws_iot_shadow_interface.h"
#include "aws_iot_mqtt_client.h"
/*---------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/WiFi.h>
#include "wificonfig.h"
#include <simplelink.h>
#include <osi.h>

extern void startNTP(void);

/*---------------------------------------------------------------------------------------------------*/

char HostAddress[255] = AWS_IOT_MQTT_HOST;
uint32_t port = AWS_IOT_MQTT_PORT;
uint32_t publishCount = 0;
/* ----------------------------------------For ADC----------------------- */
#define NO_OF_SAMPLES       128
unsigned long pulAdcSamples[4096];

// Driverlib includes
#include "utils.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_types.h"
#include "hw_adc.h"
#include "hw_ints.h"
#include "hw_gprcm.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "pin.h"
#include "adc.h"

#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

#define MAX_LENGTH_OF_UPDATE_JSON_BUFFER 200

int len ;
char AWSData[100];  // for storing the data from AWS topic
char buffer[80]; // forstoring the time stamp
/*---------------------------------Declarations for AC current measurement--------------------------------------------*/

//int mVperAmp = 185;
//int mVperAmp = 100;

/*--------------------------------------------------------------------------------------------------------------------*/
static void BoardInit(void);

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}
/*---------------------------------------------------------------------------------*/
//-----------------------float to string code---------
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
/*-----------------------------Float to String------------------------------------------------------*/
 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *AdcValue, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, AdcValue, 0);

    // check for display option after point
    if (afterpoint != 0)
        AdcValue[i] = '.';  // add dot
    {

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, AdcValue + i + 1, afterpoint);
    }
}

/*-----------------------------------------------------------------------*/

void MQTTcallbackHandler(AWS_IoT_Client *pClient, char *topicName1,
        uint16_t topicNameLen1, IoT_Publish_Message_Params *params, void *pData)
{
    int i = 0;
    //IOT_INFO("Subscribe callback");
    len = (int)params->payloadLen; // message length
    /*-------------------------------------------------------------For guidance----(Guru)----------------
     char* ch;  // created new pointer to store data from topicname1 variable
     for (i = 0; i < len; i++){
        *(ch + i) = *(topicName1 + i + 3);
        //*(v + i) = *(topicName1 + i + 3);
        //*(ch + i) = (char)(*(params->payload + i));
    }-------------------------------------------------------------------------------------*/
    for (i = 0; i < len; i++){
        AWSData[i] = *(topicName1 + i + 3);
        //*(ch + i) = (char)(*(params->payload + i)); -----(Guru)
    }
    AWSData[len]='\0';
    //---------------------------Sanity check (Guru)--------------------------
    //v = ch;
    /*for (i = 0; i < len; i++){
        System_printf("****  %c  **** \n", *(ch + i));
    }*/
    //System_printf("****  %s yede  **** \n", data);
    //------------------------------------------------------------------
    //IOT_INFO("%.*s\t%.*s",topicNameLen1, topicName1, (int)params->payloadLen,
          //(char *)params->payload);
    /*-----------------------------------------------------------------*/
    //System_printf("%.*s\n",(int)params->payloadLen, (char *)params->payload);
    /*------------------------------------------------------------------------*/
    if(((char*)params->payload)[0] == 'a')
    {
        // Board_GPIO1 = PIN 60 on development board
        GPIO_write(Board_LED0, Board_LED_ON);
        GPIO_write(Board_GPIO1, Board_LED_ON);
        }
    else if(((char*)params->payload)[0] == 'b')
    {
        GPIO_write(Board_LED0, Board_LED_OFF);
        GPIO_write(Board_GPIO1, Board_LED_OFF);
        }
    else if(((char*)params->payload)[0] == 'c')
    {
        // Board_GPIO2 = PIN 61 on development board
        GPIO_write(Board_LED1, Board_LED_ON);
        GPIO_write(Board_GPIO2, Board_LED_ON);
        }
    else if(((char*)params->payload)[0] == 'd')
    {
        GPIO_write(Board_LED1, Board_LED_OFF);
        GPIO_write(Board_GPIO2, Board_LED_OFF);
        }
    else if(((char*)params->payload)[0] == 'e')
    {
        // Board_GPIO3 = PIN 62 on development board
        GPIO_write(Board_LED2, Board_LED_ON);
        GPIO_write(Board_GPIO3, Board_LED_ON);
        }
    else if(((char*)params->payload)[0] == 'f')
    {
        GPIO_write(Board_LED2, Board_LED_OFF);
        GPIO_write(Board_GPIO3, Board_LED_OFF);
        }
    /*else if(((char*)params->payload)[0] == 'g')
    {
        //---Publish this ADC value to topic---
        GPIO_write(Board_LED2, Board_LED_ON);
    }*/
}

void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data)
{
    IOT_WARN("MQTT Disconnect");
    IoT_Error_t rc = SUCCESS;

    if (NULL == data) {
        return;
    }

    AWS_IoT_Client *client = (AWS_IoT_Client *)data;
    if (aws_iot_is_autoreconnect_enabled(client)) {
        IOT_INFO("Auto Reconnect is enabled, Reconnecting attempt will start now");
    }
    else {
        IOT_WARN("Auto Reconnect not enabled. Starting manual reconnect...");
        rc = aws_iot_mqtt_attempt_reconnect(client);
        if (NETWORK_RECONNECTED == rc) {
            IOT_WARN("Manual Reconnect Successful");
            IOT_WARN("Manual Reconnect Failed - %d", rc);
        }
        else {
            IOT_WARN("Manual Reconnect Failed - %d", rc);
        }
    }
}

/*---------------------------------------------------------------------------*/
void ShadowUpdateStatusCallback(const char *pThingName, ShadowActions_t action,
        Shadow_Ack_Status_t status, const char *pReceivedJsonDocument,
        void *pContextData)
{
    if (status == SHADOW_ACK_TIMEOUT) {
        IOT_INFO("Update Timeout--");
    } else if (status == SHADOW_ACK_REJECTED) {
        IOT_INFO("Update RejectedXX");
    } else if (status == SHADOW_ACK_ACCEPTED) {
        IOT_INFO("Update Accepted !!");
    }
}
/*---------------------------------------------------------------------------*/
/*---------------------------------ADC function------------------------------*/
float result = 0.0;
int readValue = 0;             //value read from the sensor
//------------------
int mVperAmp = 66;
float Voltage = 0;
float VRMS = 0;
float AmpsRMS, AmpsRMS1, AmpsRMS2, an ;
int a_value = 0;
int anj = 0;
//------------------

float getVPP()
{

    /*------------------------------ADC----------------------------------------*/

    unsigned int  uiIndex = 0;
    unsigned long ulSample;

    //-----------------------------------------------------------------------------------
    // Initialize Board configurations
    //

             //
             // Configure ADC timer which is used to timestamp the ADC data samples
             //
             MAP_ADCTimerConfig(ADC_BASE,2^17);

             //
             // Enable ADC timer which is used to timestamp the ADC data samples
             //
             MAP_ADCTimerEnable(ADC_BASE);

             //
             // Enable ADC module
             //
             MAP_ADCEnable(ADC_BASE);
             //
             // Enable ADC channel
             //

             MAP_ADCChannelEnable(ADC_BASE, ADC_CH_1); // ADC pin 58 = ADC_CH_1; pin 59 = ADC_CH_2; pin 60 = ADC_CH_3.

             while(uiIndex < NO_OF_SAMPLES + 4)
             {
                 if(MAP_ADCFIFOLvlGet(ADC_BASE, ADC_CH_1))
                 {
                     ulSample = MAP_ADCFIFORead(ADC_BASE, ADC_CH_1);
                     //pulAdcSamples[uiIndex++] = ulSample;
                     readValue = ((((ulSample >> 2 ) & 0x0FFF)));
                     anj = readValue-1920;
                     //a_value+= (readValue*readValue);
                     a_value+= (anj*anj);
                     //readValue = readValue - 1920 ;
                     /*if (readValue <= 0)
                     {
                         readValue = 0;
                     }
                     else
                     {
                         a_value+= (readValue*readValue);
                     }*/
                     uiIndex++;

                 }
             }

             MAP_ADCChannelDisable(ADC_BASE, ADC_CH_1);
             result = (sqrt((a_value)/(NO_OF_SAMPLES))) *(1.4/4096);
             //result = result - 0.66 ;
             /*if(result <= 0.01)
             {
                 result = 0;
             }*/
             VRMS = (result/2.0) *0.707;
             AmpsRMS = (VRMS * 1000)/mVperAmp ;
             AmpsRMS1 = (VRMS * 1000)/185 ;
             AmpsRMS1 = AmpsRMS1 - 0.08;
             AmpsRMS1 = AmpsRMS1 - 0.14;
             AmpsRMS2 = (VRMS * 1000)/100 ;
             //AmpsRMS = AmpsRMS - 0.014;
             if(AmpsRMS <=0.01)
             {
                 AmpsRMS = 0;
             }
             if(AmpsRMS1 <=0.01)
             {
                 AmpsRMS1 = 0;
             }
             if(AmpsRMS2 <=0.01)
             {
                 AmpsRMS2 = 0;
             }
             return AmpsRMS;

}
/*---------------------------------------------------------------------------*/
int sum(int a, int b);
int sum(int a, int b)
{
    int ab = a + b;
    return ab;
}

void runAWSClient(void)
{
    IoT_Error_t rc = SUCCESS;
    AWS_IoT_Client mqttClient;
    int32_t i = 0;
    bool infinitePublishFlag = true;
    char *topicName = "ES";
    char *topicName1 = "ES1";
    int topicNameLen = strlen(topicName);
    int topicNameLen1 = strlen(topicName1);
    char JsonDocumentBuffer[MAX_LENGTH_OF_UPDATE_JSON_BUFFER];
    size_t sizeOfJsonDocumentBuffer = sizeof(JsonDocumentBuffer)
            / sizeof(JsonDocumentBuffer[0]);
    int SerialNumber = 0; // For Serial Number
    //------------------------------------------------------


    //------------------------------------------------------

    char AdcValue[20];
    /*float offset = 0.640;
    float current_value;*/
    /*int offset = 66.7;
    float a = 65.75 ; // it used for an example code, for rounding off the numbers.*/

    float current_value;
    int con, r_value, mul;

    int w,x,z;
    float y;
    w = 5; x = 4;
    /*-----------------------------------------------------------------------------------------------------------*/

    /*-----------------------------------------------------------------------------------------------------------*/
    // code for NTP
    unsigned int ts;
    struct tm *info;
    /*-----------------------------------------------------------------------------------------------------------*/
    /*--------JsonDocumentBuffer[MAX_LENGTH_OF_UPDATE_JSON_BUFFER] = "{\"name\":\"Jay\",\"age\":23\}" ;----------*/
    /*char val[10] = "Jay";
    jsonStruct_t name;

    name.cb = NULL;
    name.pData = &val;
    name.pKey = "name";
    name.type = SHADOW_JSON_STRING;

    int val1 = 23;
    jsonStruct_t age;

    age.cb = NULL;
    age.pData = &val1;
    age.pKey = "age";
    age.type = SHADOW_JSON_INT8;*/
/*---------------------------------------------------------------------------------------------------------------*/
    //startNTP();
/*---------------------------------------------------------------------------------------------*/
    AWS_IoT_Client client;
    IoT_Client_Init_Params mqttInitParams = iotClientInitParamsDefault;
    IoT_Client_Connect_Params connectParams = iotClientConnectParamsDefault;

    IoT_Publish_Message_Params paramsQOS0;
    IoT_Publish_Message_Params paramsQOS1;

    IOT_INFO("\nAWS IoT SDK Version %d.%d.%d-%s\n", VERSION_MAJOR, VERSION_MINOR,
            VERSION_PATCH, VERSION_TAG);


    mqttInitParams.enableAutoReconnect = false; // We enable this later below
    mqttInitParams.pHostURL = HostAddress;
    mqttInitParams.port = port;
    /*
     *  The following cert file variables are not used in this release. All
     *  cert files must exist in the "/certs" directory and be named "ca.der",
     *  "cert.der" and "key.der", as shown in the certflasher application. The
     *  ability to change this will be added in a future release.
     */
    mqttInitParams.pRootCALocation = AWS_IOT_ROOT_CA_FILENAME;
    mqttInitParams.pDeviceCertLocation = AWS_IOT_CERTIFICATE_FILENAME;
    mqttInitParams.pDevicePrivateKeyLocation = AWS_IOT_PRIVATE_KEY_FILENAME;
    mqttInitParams.mqttCommandTimeout_ms = 500;
    mqttInitParams.tlsHandshakeTimeout_ms = 50;
    mqttInitParams.isSSLHostnameVerify = true;
    mqttInitParams.disconnectHandler = disconnectCallbackHandler;
    mqttInitParams.disconnectHandlerData = (void *)&client;

    rc = aws_iot_mqtt_init(&client, &mqttInitParams);
    if (SUCCESS != rc) {
        IOT_ERROR("aws_iot_mqtt_init returned error : %d ", rc);
    }

    connectParams.keepAliveIntervalInSec = 10;
    connectParams.isCleanSession = true;
    connectParams.MQTTVersion = MQTT_3_1_1;
    connectParams.pClientID = AWS_IOT_MQTT_CLIENT_ID;
    connectParams.clientIDLen = (uint16_t)strlen(AWS_IOT_MQTT_CLIENT_ID);
    connectParams.isWillMsgPresent = false;

    IOT_INFO("Connecting...");
    rc = aws_iot_mqtt_connect(&client, &connectParams);
    if (SUCCESS != rc) {
        IOT_ERROR("Error(%d) connecting to %s:%d", rc, mqttInitParams.pHostURL,
                mqttInitParams.port);
    }

    /*
     *  Enable Auto Reconnect functionality. Minimum and Maximum time of
     *  exponential backoff are set in aws_iot_config.h:
     *  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
     *  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
     */
    rc = aws_iot_mqtt_autoreconnect_set_status(&client, true);
    if (SUCCESS != rc) {
        IOT_ERROR("Unable to set Auto Reconnect to true - %d", rc);
    }

    IOT_INFO("Subscribing...");
    rc = aws_iot_mqtt_subscribe(&client, topicName, topicNameLen, QOS0,
            MQTTcallbackHandler, NULL);
    rc = aws_iot_mqtt_subscribe(&client, topicName1, topicNameLen1, QOS0,
            MQTTcallbackHandler, NULL);
    if (SUCCESS != rc) {
        IOT_ERROR("Error subscribing (%d)", rc);
    }

    char cPayload[500];
    sprintf(cPayload, "%s : %d ", "hello from SDK", i);

    paramsQOS0.qos = QOS0;
    paramsQOS0.payload = (void *)cPayload;
    paramsQOS0.isRetained = 0;

    paramsQOS1.qos = QOS1;
    paramsQOS1.payload = (void *)cPayload; // modified
    //paramsQOS1.payload = (void )cPayload;
    paramsQOS1.isRetained = 0;

    if (publishCount != 0) {
       infinitePublishFlag = false;
    }

    /**/

    /**/
    while ((NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc ||
            SUCCESS == rc) && (publishCount > 0 || infinitePublishFlag)) {
        rc = aws_iot_mqtt_yield(&client, 100);

        if (NETWORK_ATTEMPTING_RECONNECT == rc) {
            /* If the client is attempting to reconnect, skip rest of loop */
            continue;
        }


/*------------------------------------------------------------------------------------*/
        //GPIO_write(Board_GPIO1, Board_LED_ON);
        //GPIO_write(Board_GPIO2, Board_LED_ON);
        //GPIO_write(Board_GPIO3, Board_LED_ON);
        /*Voltage = getVPP();
        VRMS = (Voltage/2.0) *0.707;
        AmpsRMS = (VRMS * 1000)/mVperAmp ;
        AmpsRMS = AmpsRMS - 0.01;*/
        //GPIO_write(Board_LED2, Board_LED_ON);
        an = getVPP();
        a_value = 0;
        //GPIO_write(Board_LED2, Board_LED_OFF);
        //System_printf("ADC result is %f \n", AmpsRMS);
        //System_printf("ADC value is %f\n", a_value);
/*------------------------------------------------------------------------------------*/
        jsonStruct_t SensorData;

        SensorData.cb = NULL;
        //SensorData.pData = &Voltage;
        SensorData.pData = &an;
        SensorData.pKey = "SensorData";
        SensorData.type = SHADOW_JSON_FLOAT;


        jsonStruct_t serialNumber;

        serialNumber.cb = NULL;
        serialNumber.pData = &SerialNumber;
        serialNumber.pKey = "serialNumber";
        serialNumber.type = SHADOW_JSON_INT16;
        /*---------------------------------------------------------------------------------------------*/
        if (rc == SUCCESS) {
                        rc = aws_iot_shadow_add_reported(JsonDocumentBuffer,
                                sizeOfJsonDocumentBuffer, 2, &SensorData,
                                &serialNumber);}
        sprintf(cPayload, JsonDocumentBuffer);
                    //Recalculate string len to avoid truncation in subscribe callback
        paramsQOS0.payloadLen = strlen(cPayload);
        rc = aws_iot_mqtt_publish(&client, topicName, topicNameLen,
                                  &paramsQOS0);
        SerialNumber = SerialNumber + 1;
        Task_sleep(1000);
        //GPIO_write(Board_GPIO1, Board_LED_OFF);
        //-----------------------------------------------------------------------------------------------
        ts = time(NULL)+19800;
        info = localtime( &ts);
        strftime(buffer,80,"%I:%M%p", info); // for storing the time data
        //System_printf("Formatted date & time : |%s|\n", buffer );
        /*---------------------------------------------------------------------------------------------*/
        /*int ret = strcmp(AWSData,buffer);
        if(ret==0)
        {
            System_printf("***** time is matched *****\n");
            // Check the status of the GPIO pin
            ret = GPIO_read(Board_LED1);
            if(ret == 0)
            {
                GPIO_write(Board_LED1, Board_LED_ON);
                sprintf(cPayload,"*");
                // Recalculate string len to avoid truncation in subscribe callback
                paramsQOS0.payloadLen = strlen(cPayload);
                rc = aws_iot_mqtt_publish(&client, topicName1, topicNameLen1,
                                          &paramsQOS0);
                Task_sleep(1000);
            }

        }*/
        /*---------------------------------------------------------------------------------------------*/

        if(GPIO_read(Board_BUTTON0))
        {
            // for any interrupt testing use this function call.
            /*System_printf("Data aquired from AWS is: %s", AWSData);
            System_printf("Current time is: %s", buffer);*/
            /***********************************************************************************/
            /* example for rounding off the numbers
            mul=a*10;
            con=mul%10;
            r_value=mul/10;
            if(con>=5){
            r_value=r_value+1;}
            else {
            r_value=r_value;}

            current_value = ((r_value - offset)/4.0);
            System_printf("The rounded value is %d\n", r_value);
            System_printf("The current value is %f \n", current_value);*/
            /***********************************************************************************/

            //ftoa(Voltage, AdcValue, 2);
            //sprintf(cPayload, AdcValue);
            /* Recalculate string len to avoid truncation in subscribe callback */
            //paramsQOS0.payloadLen = strlen(cPayload);
            //rc = aws_iot_mqtt_publish(&client, topicName1, topicNameLen1,
            //        &paramsQOS0);
            //Task_sleep(1000);
        }
        else if (GPIO_read(Board_LED2))
        {
           /* ftoa(a_value, AdcValue, 4);
            strcat(AdcValue, " ");
            strcat(AdcValue, buffer);
            sprintf(cPayload,AdcValue);
            /* Recalculate string len to avoid truncation in subscribe callback */
            /*paramsQOS0.payloadLen = strlen(cPayload);
            rc = aws_iot_mqtt_publish(&client, topicName1, topicNameLen1,
                                      &paramsQOS0);
            Task_sleep(1000);*/
            //GPIO_toggle((Board_LED2));
        }
    }

    BoardInit();

#ifdef CC3200_ES_1_2_1
        //
        // Enable ADC clocks.###IMPORTANT###Need to be removed for PG 1.32
        //
        HWREG(GPRCM_BASE + GPRCM_O_ADC_CLK_CONFIG) = 0x00000043;
        HWREG(ADC_BASE + ADC_O_ADC_CTRL) = 0x00000004;
        HWREG(ADC_BASE + ADC_O_ADC_SPARE0) = 0x00000100;
        HWREG(ADC_BASE + ADC_O_ADC_SPARE1) = 0x0355AA00;
#endif
    /*-------------------------------------------------------------------------*/
    if (SUCCESS != rc) {
        IOT_ERROR("An error occurred in the loop. Error code = %d\n", rc);
    }
    else {
        IOT_INFO("Publish done\n");
    }


}
