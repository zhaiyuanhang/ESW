
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
 * @brief Examples for SuperIO uart module. Data transmission between uart and wifi in dma mode.
 * @author abu (abu@eswincomputing.com)
 * @date 2023-02-02
 *
 * @details SuperIO configured uart, uart sent/receive data, and wifi receive/sent data, which used in dma mode.
 *
 * @platform Fullmask evb board
 *
 * @pins
 * | function name | port number | function number | board connector |
 * |:------------- |:------------|:----------------|:----------------|
 *        D1       |    PORTA02  |      ALT6       |    J4-Wifi
 *        D3       |    PORTA00  |      ALT6       |    J4-Wifi
 * @connections
 *      WIFI Board is plugged into SuperIO PORT J4
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "../board/sdk_project_config.h"
#include <stdint.h>
#include <string.h>
#include "testcase.h"

#define TRANSFER_TIMEOUT 200UL
#define RX_BUFFER_SIZE   (256U)

#define AT_CMD 		"AT\r\n"
#define CWMODE_CMD 	"AT+CWMODE=1,\"s.y\"\r\n"
#define CWQAP_CMD 	"AT+CWQAP\r\n"
#define CWJAP_CMD 	"AT+CWJAP=\"xxx\",\"12345678\",\"s.y\"\r\n"

static superio_device_state_t g_stSuperIODeviceState0;
static superio_uart_state_t g_stSuperioUartState_0;
static superio_uart_state_t g_stSuperioUartState_1;

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
static void GetCommandBuffer(uint8_t *rbuffer, uint8_t *cbuffer)
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
static bool JudegCommandBuffer(uint8_t *cbuffer)
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
bool SUPERIO_UartDriverSend_WifiCommand(const char* wifi_com, uint32_t time)
{
    ZeroBuffer(rxBuffer, RX_BUFFER_SIZE);
    ZeroBuffer(commandBuffer, RX_BUFFER_SIZE);

    SUPERIO_UART_DRV_ReceiveData(&g_stSuperioUartState_0, rxBuffer, RX_BUFFER_SIZE);
    int len = strlen(wifi_com);
    uint8_t wifi_arr[len];
    strcpy((char *)wifi_arr, wifi_com);
    SUPERIO_UART_DRV_SendDataBlocking(&g_stSuperioUartState_1, wifi_arr, sizeof(wifi_arr), TRANSFER_TIMEOUT);
    OS_DelayMs(time);

    GetCommandBuffer(rxBuffer, commandBuffer);

    return JudegCommandBuffer(commandBuffer);
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
int SUPERIO_UartDriverTypeDmaWifi_Example(uint8_t ucAuto)
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
    SUPERIO_UART_DRV_Init(INST_SUPERIO_UART_0, &g_stSuperioUart0UserConfig0, &g_stSuperioUartState_0);

    /* Init the SuperIO UART driver
     *  - 115200UL baudrate
     *  - TX only
     *  - 8 bit per frame
     *  - Interrupt based
     */
    SUPERIO_UART_DRV_Init(INST_SUPERIO_UART_0, &g_stSuperioUart0UserConfig1, &g_stSuperioUartState_1);

    /* send at command */
    printf("send AT command:\r\n");
    res = SUPERIO_UartDriverSend_WifiCommand(AT_CMD, 20);
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send AT command success.\r\n");
    } else {
        printf("send AT command fail!\r\n");
        goto __exit;
    }

    /* send cwmode command */
    printf("send CWMODE command:\r\n");
    res = SUPERIO_UartDriverSend_WifiCommand(CWMODE_CMD, 20);
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send CWMODE command success.\r\n");
    } else {
        printf("send CWMODE command fail!\r\n");
        goto __exit;
    }

    /* send cwjap command */
    printf("send CWJAP command:\r\n");
    res = SUPERIO_UartDriverSend_WifiCommand(CWJAP_CMD, 20000);
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send CWJAP command success.\r\n");
    } else {
        printf("send CWJAP command fail!\r\n");
        goto __exit;
    }

    /* wifi is turned on before, then it is turned off */
    OS_DelayMs(1000 * 30);

    /* send close command */
    printf("send CWQAP command:\r\n");
    res = SUPERIO_UartDriverSend_WifiCommand(CWQAP_CMD, 20);
    if (res == true) {
        printf("response command:\r\n%s\r\n", commandBuffer);
        printf("\r\n");
        printf("send CWQAP command success.\r\n");
    } else {
        printf("send CWQAP command fail!\r\n");
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

