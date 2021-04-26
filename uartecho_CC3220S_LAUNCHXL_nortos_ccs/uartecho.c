/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== uartecho.c ========
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Driver configuration */
#include "ti_drivers_config.h"

// Define macros for the states.
#define SM_INIT 0
#define SM_O 1
#define SM_N 2
#define SM_F 3
#define SM_F_OFF 4

UART_Handle uart; // This is global since I don't want to have to pass it to the functions.

void changeState(unsigned char*,unsigned char*);
void performAction(unsigned char*);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    //char        input; // Moved declaration elsewhere.
    const char  echoPrompt[] = "Echoing characters:\r\n";
    //UART_Handle uart; // Changed to global decleration.
    UART_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);



    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    // Disabled so the LED starts off.
    /* Turn on user LED to indicate successful initialization */
    //GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    UART_write(uart, echoPrompt, sizeof(echoPrompt));

    unsigned char state = SM_INIT; // The state of the state machine.
    unsigned char input; // The byte received from the last call of UART_read().

    /* Loop forever echoing */
    while (1) {
        UART_read(uart, &input, 1);
        changeState(&state, &input);
        performAction(&state);
        UART_write(uart, &input, 1);
    }
}

// Function to trigger a state transition based on the last byte received by the UART.
void changeState(unsigned char* state,unsigned char* input)
{
    switch (*state)
    {
        // When in state SM_INIT, if the last key pressed was O transition to state SM_O. Otherwise, stay in SM_INIT.
        case SM_INIT:
            if (*input == 79) {*state = SM_O;}
            else {*state = SM_INIT;}
            break;

        // When in state SM_O, if the last key pressed was F go to state SM_F. If the key was N, go to SM_N.
        // for any other key go back to SM_INIT.
        case SM_O:
            if (*input == 70) {*state = SM_F;}
            else if (*input == 78) {*state = SM_N;}
            else {*state = SM_INIT;}
            break;

        case SM_N:
            break;

        // When in state SM_F, if the last key pressed was F again go to SM_OFF. Otherwise, go back to SM_INIT.
        case SM_F:
            if (*input == 70) {*state = SM_F_OFF;}
            else {*state = SM_INIT;}
            break;

        case SM_F_OFF:
            break;

        default:
            *state = SM_INIT;
            break;
    }
}

// Function to perform an action based on the current state.
void performAction(unsigned char* state)
{
    switch (*state)
    {
        case SM_INIT:
            break;

        case SM_O:
            break;

        case SM_N:
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON); // Turn on the red LED.
            *state = SM_INIT; // State transition is happening here since otherwise it would have to wait for another key press.
            break;

        case SM_F:
            break;

        case SM_F_OFF:
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Turn off the red LED.
            *state = SM_INIT; // State transition is happening here since otherwise it would have to wait for another key press.
            break;

        default:
            break;
    }
}
