
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
 * @file main.c
 * @brief Examples for SuperIO uart module. Data transmission between uart and ble in dma mode.
 * @author abu (abu@eswincomputing.com)
 * @date 2023-02-02
 *
 * @details SuperIO configured uart, uart sent/receive data, and ble receive/sent data, which used in dma mode.
 *
 * @platform Fullmask evb board
 *
 * @pins
 * | function name | port number | function number | board connector |
 * |:------------- |:------------|:----------------|:----------------|
 *        D1       |    PORTA02  |      ALT6       |    J4-BLE
 *        D3       |    PORTA00  |      ALT6       |    J4-BLE
 * @connections
 *      BLE Board is plugged into SuperIO PORT J4
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "../board/sdk_project_config.h"

#include "clock_driver.h"

#include <stdint.h>
#include <string.h>

#define TRANSFER_TIMEOUT 200UL
#define INST_UART_SAMPLE INST_UART_2
static superio_device_state_t g_stSuperIODeviceState0;
static superio_uart_state_t g_stSuperioUartState_0;
static superio_uart_state_t g_stSuperioUartState_1;

/* set according to your needs */
#define RX_BUFFER_SIZE   (256U)

/* command: AT\r\n */
static uint8_t at_command[4] =
{0x41, 0x54, 0xd, 0xa};

/* command: AT+BLEADDR\r\n */
static uint8_t mac_command[12] = {
        0x41, 0x54, 0x2b, 0x42, 0x4c, 0x45, 0x41, 0x44, 0x44, 0x52, 0xd, 0xa};

/* command: AT+BLENAME\r\n */
static uint8_t name_command[12] = {
        0x41, 0x54, 0x2b, 0x42, 0x4c, 0x45, 0x4e, 0x41, 0x4d, 0x45, 0xd, 0xa};

/* command: AT+BLEADVMOD=0\r\n */
static uint8_t mode_command[16] = {
        0x41, 0x54, 0x2b, 0x42, 0x4c, 0x45, 0x41, 0x44, 0x56, 0x4d, 0x4f, 0x44, 0x3d, 0x30, 0xd, 0xa};

/* command: AT+BLEROLE=0\r\n */
static uint8_t role_command[14] = {
        0x41, 0x54, 0x2b, 0x42, 0x4c, 0x45, 0x52, 0x4f, 0x4c, 0x45, 0x3d, 0x30, 0xd, 0xa};

/* command: AT+BLEADVEN=1\r\n */
static uint8_t start_command[15] = {
        0x41, 0x54, 0x2b, 0x42, 0x4c, 0x45, 0x41, 0x44, 0x56, 0x45, 0x4e, 0x3d, 0x31, 0xd, 0xa};

/* command: AT+BLEADVEN=0\r\n */
static uint8_t close_command[15] = {
        0x41, 0x54, 0x2b, 0x42, 0x4c, 0x45, 0x41, 0x44, 0x56, 0x45, 0x4e, 0x3d, 0x30, 0xd, 0xa};

/* receive buffer */
static uint8_t rxBuffer[RX_BUFFER_SIZE];
/* command buffer */
static uint8_t commandBuffer[RX_BUFFER_SIZE];

/* the number of response values obtained after each command is sent */
static uint32_t index_buffer = 0;

/* clear buffer */
static void ZeroBuffer(uint8_t *buff, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
        buff[i] = 0;
}

/* get response buffer(discard the last two bytes: \r and \n) obtained after each command is sent */
void GetCommandBuffer(uint8_t *rbuffer, uint8_t *cbuffer)
{
    int i;
    for (i = index_buffer; i < RX_BUFFER_SIZE; i++) {
        if ((rbuffer[i] == 0xd) & (rbuffer[i+1] == 0xa) & (rbuffer[i+2] == 0x0) & (rbuffer[i+3] == 0x0)) {
            break;
        } else {
            cbuffer[i - index_buffer] = rbuffer[i];
        }
    }
    index_buffer = i + 2;
}

/* judeg command buffer whether has 'O' and 'K' */
bool JudegCommandBuffer(uint8_t *cbuffer)
{
    bool res = false;
    for (int j = 0; j < RX_BUFFER_SIZE; j++) {
        if ((cbuffer[j] == 0x4f) & (cbuffer[j+1] == 0x4b)) {
            res = true;
            break;
        }
    }
    return res;
}

/* sent AT command and get response buffer(discard the last two bytes: \r and \n) */
bool SUPERIO_UartDriverSend_AtCommand(void)
{
    ZeroBuffer(rxBuffer, RX_BUFFER_SIZE);
    ZeroBuffer(commandBuffer, RX_BUFFER_SIZE);

    SUPERIO_UART_DRV_ReceiveData(&g_stSuperioUartState_0, rxBuffer, 256U);
    SUPERIO_UART_DRV_SendDataBlocking(&g_stSuperioUartState_1, at_command, sizeof(at_command) / sizeof(at_command[0]), TRANSFER_TIMEOUT);
    OS_DelayMs(20);

    GetCommandBuffer(rxBuffer, commandBuffer);

    return JudegCommandBuffer(commandBuffer);
}

/* sent check mac command and get response buffer(discard the last two bytes: \r and \n) */
bool SUPERIO_UartDriverSend_MacCommand(void)
{
    ZeroBuffer(rxBuffer, RX_BUFFER_SIZE);
    ZeroBuffer(commandBuffer, RX_BUFFER_SIZE);

    SUPERIO_UART_DRV_ReceiveData(&g_stSuperioUartState_0, rxBuffer, 256U);
    SUPERIO_UART_DRV_SendDataBlocking(&g_stSuperioUartState_1, mac_command, sizeof(mac_command) / sizeof(mac_command[0]), TRANSFER_TIMEOUT);
    OS_DelayMs(20);

    GetCommandBuffer(rxBuffer, commandBuffer);

    return JudegCommandBuffer(commandBuffer);
}

/* sent get name command and get response buffer(discard the last two bytes: \r and \n) */
bool SUPERIO_UartDriverSend_NameCommand(void)
{
    ZeroBuffer(rxBuffer, RX_BUFFER_SIZE);
    ZeroBuffer(commandBuffer, RX_BUFFER_SIZE);

    SUPERIO_UART_DRV_ReceiveData(&g_stSuperioUartState_0, rxBuffer, 256U);
    SUPERIO_UART_DRV_SendDataBlocking(&g_stSuperioUartState_1, name_command, sizeof(name_command) / sizeof(name_command[0]), TRANSFER_TIMEOUT);
    OS_DelayMs(20);

    GetCommandBuffer(rxBuffer, commandBuffer);

    return JudegCommandBuffer(commandBuffer);
}

/* sent set mode command and get response buffer(discard the last two bytes: \r and \n) */
bool SUPERIO_UartDriverSend_SetModeCommand(void)
{
    ZeroBuffer(rxBuffer, RX_BUFFER_SIZE);
    ZeroBuffer(commandBuffer, RX_BUFFER_SIZE);

    SUPERIO_UART_DRV_ReceiveData(&g_stSuperioUartState_0, rxBuffer, 256U);
    SUPERIO_UART_DRV_SendDataBlocking(&g_stSuperioUartState_1, mode_command, sizeof(mode_command) / sizeof(mode_command[0]), TRANSFER_TIMEOUT);
    OS_DelayMs(20);

    GetCommandBuffer(rxBuffer, commandBuffer);

    return JudegCommandBuffer(commandBuffer);
}

/* sent set role command and get response buffer(discard the last two bytes: \r and \n) */
bool SUPERIO_UartDriverSend_SetRoleCommand(void)
{
    ZeroBuffer(rxBuffer, RX_BUFFER_SIZE);
    ZeroBuffer(commandBuffer, RX_BUFFER_SIZE);

    SUPERIO_UART_DRV_ReceiveData(&g_stSuperioUartState_0, rxBuffer, 256U);
    SUPERIO_UART_DRV_SendDataBlocking(&g_stSuperioUartState_1, role_command, sizeof(role_command) / sizeof(role_command[0]), TRANSFER_TIMEOUT);
    OS_DelayMs(20);

    GetCommandBuffer(rxBuffer, commandBuffer);

    return JudegCommandBuffer(commandBuffer);
}

/* sent start command and get response buffer(discard the last two bytes: \r and \n) */
bool SUPERIO_UartDriverSend_StartCommand(void)
{
    ZeroBuffer(rxBuffer, RX_BUFFER_SIZE);
    ZeroBuffer(commandBuffer, RX_BUFFER_SIZE);

    SUPERIO_UART_DRV_ReceiveData(&g_stSuperioUartState_0, rxBuffer, 256U);
    SUPERIO_UART_DRV_SendDataBlocking(&g_stSuperioUartState_1, start_command, sizeof(start_command) / sizeof(start_command[0]), TRANSFER_TIMEOUT);
    OS_DelayMs(20);

    GetCommandBuffer(rxBuffer, commandBuffer);

    return JudegCommandBuffer(commandBuffer);
}

/* sent close command and get response buffer(discard the last two bytes: \r and \n) */
bool SUPERIO_UartDriverSend_CloseCommand(void)
{
    ZeroBuffer(rxBuffer, RX_BUFFER_SIZE);
    ZeroBuffer(commandBuffer, RX_BUFFER_SIZE);

    SUPERIO_UART_DRV_ReceiveData(&g_stSuperioUartState_0, rxBuffer, 256U);
    SUPERIO_UART_DRV_SendDataBlocking(&g_stSuperioUartState_1, close_command, sizeof(close_command) / sizeof(close_command[0]), TRANSFER_TIMEOUT);
    OS_DelayMs(20);

    GetCommandBuffer(rxBuffer, commandBuffer);

    return JudegCommandBuffer(commandBuffer);
}


void SuperioUartRx0Callback(void *driverState, uart_event_t event, void *userData)
{
    (void)userData;
    (void)driverState;

    if (event == UART_EVENT_RX_FULL) {
        /* do what you want to do */
    }
}

void SuperioUartTx0Callback(void *driverState, uart_event_t event, void *userData)
{
    (void)userData;
    (void)driverState;

    if (event == UART_EVENT_END_TRANSFER) {
        /* do what you want to do */
    }
}

/**
 * @brief  SuperIO uart driverType is dma
 * One uart Tx send data, and another uart Rx received data.
 * Then compared the data between sent and received.
 *
 * @return bool
 *         true if run succeed
 *         false if run failed
 */
int SUPERIO_UartDriverTypeDmaBle_Example(uint8_t ucAuto)
{
    bool res = false;

    /* Initialize PDMA */
    PDMA_DRV_Init(&g_stPdmaState0, &g_stPdma0UserConfig0,
                  g_stPdma0ChnStateArray, g_stPdma0ChannelConfigArray,
                  PDMA_CHANNEL_CONFIG_COUNT);

    /* Init the SUPERIO device */
    SUPERIO_DRV_InitDevice(INST_SUPERIO_UART_0, &g_stSuperIODeviceState0);

    /* Init the SuperIO UART driver
     *  - 115200UL baudrate
     *  - RX only
     *  - 8 bit per frame
     *  - Interrupt based
     */
    g_stSuperioUart0UserConfig0.callback = SuperioUartRx0Callback;
    SUPERIO_UART_DRV_Init(INST_SUPERIO_UART_0, &g_stSuperioUart0UserConfig0, &g_stSuperioUartState_0);

    /* Init the SuperIO UART driver
     *  - 115200UL baudrate
     *  - TX only
     *  - 8 bit per frame
     *  - Interrupt based
     */
    g_stSuperioUart0UserConfig1.callback = SuperioUartTx0Callback;
    SUPERIO_UART_DRV_Init(INST_SUPERIO_UART_0, &g_stSuperioUart0UserConfig1, &g_stSuperioUartState_1);

    /* send at command */
    printf("send at command:\r\n");
    res = SUPERIO_UartDriverSend_AtCommand();
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send at command success.\r\n");
    } else {
        printf("send at command fail!\r\n");
        goto __exit;
    }

    /* send check mac command */
    printf("send mac command:\r\n");
    res = SUPERIO_UartDriverSend_MacCommand();
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send mac command success.\r\n");
    } else {
        printf("send mac command fail!\r\n");
        goto __exit;
    }

    /* send get name command */
    printf("send get name command:\r\n");
    res = SUPERIO_UartDriverSend_NameCommand();
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send get name command success.\r\n");
    } else {
        printf("send get name command fail!\r\n");
        goto __exit;
    }

    /* send set mode command */
    printf("send set mode command:\r\n");
    res = SUPERIO_UartDriverSend_SetModeCommand();
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send set mode command success.\r\n");
    } else {
        printf("send set mode command fail!\r\n");
        goto __exit;
    }

    /* send set role command */
    printf("send set role command:\r\n");
    res = SUPERIO_UartDriverSend_SetRoleCommand();
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send set role command success.\r\n");
    } else {
        printf("send set role command fail!\r\n");
        goto __exit;
    }

    /* send start command */
    printf("send start command:\r\n");
    res = SUPERIO_UartDriverSend_StartCommand();
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send start command success.\r\n");
    } else {
        printf("send start command fail!\r\n");
        goto __exit;
    }

    /* ble is turned on before, then it is turned off */
    OS_DelayMs(30000);

    /* send close command */
    printf("send close command:\r\n");
    res = SUPERIO_UartDriverSend_CloseCommand();
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send close command success.\r\n");
    } else {
        printf("send close command fail!\r\n");
        goto __exit;
    }

__exit:
    SUPERIO_UART_DRV_Deinit(&g_stSuperioUartState_0);
    SUPERIO_UART_DRV_Deinit(&g_stSuperioUartState_1);
    SUPERIO_DRV_DeinitDevice(INST_SUPERIO_UART_0);
    PDMA_DRV_Deinit();

    if (res) return 0;
    else return -1;
}
