/**
 * Copyright Statement:
 * This software and related documentation (ESWIN SOFTWARE) are protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * Beijing ESWIN Computing Technology Co., Ltd.(ESWIN)and/or its licensors.
 * Without the prior written permission of ESWIN and/or its licensors, any reproduction, modification,
 * use or disclosure Software, and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * Copyright ©[2023] [Beijing ESWIN Computing Technology Co., Ltd.]. All rights reserved.
 *
 * RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES THAT THE SOFTWARE
 * AND ITS DOCUMENTATIONS (ESWIN SOFTWARE) RECEIVED FROM ESWIN AND / OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. ESWIN EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON INFRINGEMENT.
 * NEITHER DOES ESWIN PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
 * WHICH MAY BE USED BY,INCORPORATED IN, OR SUPPLIED WITH THE ESWIN SOFTWARE,
 * AND RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ESWIN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file uart1_pta.c
 * @brief
 * @author
 * @date 2024-01-10
 *
 * @details
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "../board/sdk_project_config.h"
#include "testcase.h"

#include <stdio.h>
#include <string.h>

volatile static int g_exitCode = 0;

/** Welcome message displayed at the console */
#define WELCOM_MSG \
    "This example is an simple echo using UART\r\n\
it will send back any character you send to it.\r\n\
The board will greet you if you send 'Hello Board'\r\
\nNow you can begin typing:\r\n"

/** Error message displayed at the console, in case data is received erroneously */
#define ERROR_MSG "An error occurred! The application will stop!\r\n"

/** Timeout in ms for blocking operations */
#define TIMEOUT 500U

/** Receive buffer size */
#define BUFFER_SIZE 256U

/** Buffer used to receive data from the console */
static uint8_t g_buffer[BUFFER_SIZE];
static uint8_t g_bufferldx;

/** uart sample */
#define INST_UART_SAMPLE 1u

static uart_state_t g_stUartState_1;

static uart_user_config_t g_stUart1UserConfig0 = {
    .baudRate        = 115200,
    .parityMode      = UART_PARITY_DISABLED,
    .stopBitCount    = UART_ONE_STOP_BIT,
    .bitCountPerChar = UART_8_BITS_PER_CHAR,
    .transferType    = UART_USING_INTERRUPTS,
    .fifoType        = UART_FIFO_DEPTH_1,
    .rxDMAChannel    = 0,
    .txDMAChannel    = 1,
};

static pin_settings_config_t g_stPinmuxConfigArr_uart1_pta[NUM_OF_CONFIGURED_PINS] = {
    {
        .base        = PORTA,
        .pinPortIdx  = 26U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        .base        = PORTA,
        .pinPortIdx  = 25U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        .base        = PORTD,
        .pinPortIdx  = 10U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
    {
        .base        = PORTD,
        .pinPortIdx  = 11U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
};

/**
 * @brief UART rx callback for continuous reception, byte by byte
 *
 * @param  driverState
 * @param  event uart event
 * @param  userData user data
 */
static void RxCallback(void *driverState, uart_event_t event, void *userData)
{
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL) {
        /* The reception stops when newline is received or the buffer is full */
        if ((g_buffer[g_bufferldx] != '\n') && (g_buffer[g_bufferldx] != '\r') && (g_bufferldx != (BUFFER_SIZE - 2U))) {
            /* Update the buffer index and the rx buffer */
            g_bufferldx++;
            UART_DRV_SetRxBuffer(INST_UART_SAMPLE, &g_buffer[g_bufferldx], 1U);
        } else {
            g_buffer[g_bufferldx++] = '\r';
            g_buffer[g_bufferldx++] = '\n';
            g_buffer[g_bufferldx]   = '\0';
        }
    }
}

int Uart1_Example(uint8_t ucAuto)
{

    /* Write your local variable definition here */
    status_t status;
    /* Declare a buffer used to store the received data */
    uint32_t bytesRemaining;

    /* Write your code here */

    /** Initialize pins
     *    -    See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr_uart1_pta);

    /* Initialize UART instance */
    UART_DRV_Init(INST_UART_SAMPLE, &g_stUartState_1, &g_stUart1UserConfig0);
    /* Install the callback for rx events */
    UART_DRV_InstallRxCallback(INST_UART_SAMPLE, RxCallback, NULL);
    /* Send a welcome message */
    UART_DRV_SendDataBlocking(INST_UART_SAMPLE, (uint8_t *)WELCOM_MSG, strlen(WELCOM_MSG), TIMEOUT);

    /* Infinite loop:
     *     - Receive data from user
     *     - Echo the received data back
     */
    while (1) {
        /* Receive and store data byte by byte until new line character is received,
         * or the buffer becomes full (256 characters received)
         */
        UART_DRV_ReceiveData(INST_UART_SAMPLE, g_buffer, 1U);
        /* Wait for transfer to be completed */
        while (UART_DRV_GetReceiveStatus(INST_UART_SAMPLE, &bytesRemaining) == STATUS_BUSY)
            ;

        /* Check the status */
        status = UART_DRV_GetReceiveStatus(INST_UART_SAMPLE, &bytesRemaining);

        if (status != STATUS_SUCCESS) {
            /* If an error occurred, send the error message and exit the loop */
            UART_DRV_SendDataBlocking(INST_UART_SAMPLE, (uint8_t *)ERROR_MSG, strlen(ERROR_MSG), TIMEOUT);
            break;
        }

        /* Append string terminator to the received data */
        g_bufferldx++;
        g_buffer[g_bufferldx] = 0U;

        /* If the received string is "Hello Board", send back "Hello World" */
        if (strncmp((char *)g_buffer, "Hello Board", strlen("Hello Board")) == 0) {
            strcpy((char *)g_buffer, "Hello World\n");
        }

        /* Send the received data back */
        UART_DRV_SendDataBlocking(INST_UART_SAMPLE, g_buffer, g_bufferldx, TIMEOUT);
        /* Reset the buffer index to start a new reception */
        g_bufferldx = 0U;
        if (strncmp((char *)g_buffer, "quit", strlen("quit")) == 0) {
        	g_exitCode = 0;
            break;
        }
    }
    UART_DRV_Deinit(INST_UART_SAMPLE);
    return g_exitCode;
}
