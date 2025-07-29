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
 * @brief Examples for FlexCAN module, one node send frame and another node receive frame.
 * @author abu (abu@eswincomputing.com)
 * @date 2023-02-02
 *
 * @details Standard frame transmission between two canFd node. Can2 node prepared enhanced rxfifo to receive, and
 *  waiting can0 node sending frame data. By comparing received frame data and send frame data, check the result.
 *
 * @platform Fullmask evb board
 *
 * @pins
 * | function name | port number | function number | board connector |
 * |:------------- |:------------|:----------------|:----------------|
 *      CAN0_TX    |    PORTA07  |      ALT5       |    J7
 *      CAN0_RX    |    PORTA08  |      ALT5       |    J7
 * @connections
   @verbatim
               J7 connect another board J7

   @endverbatim
 *
 * @attention Remember to turn on the 5V power of can transceiver.
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "../board/sdk_project_config.h"
#include <stdbool.h>
#include <stdint.h>
#include "testcase.h"

#define MASTER_TX_MAILBOX    (1UL)
#define MASTER_TX_MSG_ID     (1UL)
#define SLAVE_TX_MAILBOX     (0UL)
#define SLAVE_TX_MSG_ID      (2UL)
#define TX_CNT               (64UL)
#define TX_8_BYTE_CNT        (8UL)
#define IS_FD                true
#define NOT_FD               false

/**
 * @brief Send data via CAN to the specified mailbox with the specified message id
 *
 * @param  index   Can instance number
 * @param  fd      Can transfer mode (Classic or Fd)
 * @param  mailbox  Which mailbox
 * @param  messageId  message id
 * @param  data       data to send
 * @param  len        data length
 * @return status_t
 *         STATUS_SUCCESS if transfer successful.
 *         STATUS_ERROR   if transfer failed.
 */
static status_t SendCanStandardData(uint8_t index, bool fd, uint32_t mailbox, uint32_t messageId, uint8_t *data,
                                    uint32_t len)
{
    flexcan_data_info_t dataInfo = {
        .data_length = len,
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .enable_brs  = fd,
        .fd_enable   = fd,
        .fd_padding  = 0U,
    };

    /* Configure TX message buffer with index TX_MSG_ID and TX_MAILBOX */
    FLEXCAN_DRV_ConfigTxMb(index, mailbox, &dataInfo, messageId);

    /* Execute send non-blocking */
    return FLEXCAN_DRV_Send(index, mailbox, &dataInfo, messageId, data);
}

static void BufferInit(uint8_t *buf, uint32_t len)
{
    uint32_t Cnt      = 0U;
    static uint32_t i = 0U;

    for (Cnt = 0U; Cnt < len; Cnt++) {
        buf[Cnt] = Cnt + i;
    }
    i++;
}

/**
 * @brief  CanFd node Tx send data to CanFd node Rx with enhanced rxfifo and PDMA
 *
 * @param  CanTxIndex Tx node index..
 *
 * @return true if transfer successful.
 * @return false if transfer failed.
 */
int CanFD_EnhancedTxFifoDma_Example(uint8_t ucAuto)
{
    int can_result = 0;
    uint8_t txbuf[TX_CNT];

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);
    BufferInit(&txbuf[0], TX_CNT);

    /** Initialize PDMA */
    PDMA_DRV_Init(&g_stPdmaState0, &g_stPdma0UserConfig0, g_stPdma0ChnStateArray,
                  g_stPdma0ChannelConfigArray, PDMA_CHN_STATE_COUNT);

    /** Init flexcan0  */
    FLEXCAN_DRV_Init(0, &g_stFlexcanState_0, &g_stFlexcan0UserConfig0);

    /* Send the msg via CAN0, until another board can0 receive data */
    printf("continue send data, until another board can0 receive data.\r\n");
    SendCanStandardData(0, IS_FD, MASTER_TX_MAILBOX, MASTER_TX_MSG_ID, txbuf, TX_CNT);

    return can_result;
}
