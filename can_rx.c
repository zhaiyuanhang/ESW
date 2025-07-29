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

#define MASTER_RX_MAILBOX    (0UL)
#define MASTER_RX_MSG_ID     (2UL)

#define SLAVE_TX_MAILBOX     (0UL)
#define SLAVE_TX_MSG_ID      (2UL)

#define SLAVE_RX_MAILBOX     (1UL)
#define SLAVE_RX_MSG_ID      (1UL)
#define SLAVE_RX_MAILBOX_CNT (10UL)

#define FIFO_EN_TX_MB_ID     (15UL)
#define FIFO_START_MB_ID     (0UL)

#define TX_CNT               (64UL)
#define TX_8_BYTE_CNT        (8UL)
#define IS_FD                true
#define NOT_FD               false

#define STD_FILTER_NUM       (2UL)
#define EXT_FILTER_NUM       (1UL)

/**
 * @brief Wait for receive completed
 *
 * @param  index Can instance number
 * @param  messageId  message id
 */
static void WaitReceiveComplete(uint32_t index, uint32_t messageId)
{
    uint32_t timeout = 0;

    /* Wait until the previous FlexCAN receive is completed */
    while ((FLEXCAN_DRV_GetTransferStatus(index, messageId) == STATUS_BUSY) && timeout < 10) {
        OS_DelayMs(1);
        timeout++;
    }
}

/**
 * @brief Prepare the enhance Rxfifo to receive data.
 *
 * @param  instance    CAN instance number.
 * @param  id_filter_table Thetable of Specify id filter scheme.
 * @param  data the data to send.
 */
static void PrepareEnhancedRxFIFO(uint8_t instance, const flexcan_enhancedIdTableType_t *id_filter_table,
                                  flexcan_msgbuff_t *data)
{
    FLEXCAN_DRV_ConfigEnhancedRxFifo(instance, id_filter_table);
    FLEXCAN_DRV_RxFifo(instance, data);
}

static bool MsgCompare(uint8_t *src0, uint8_t *src1, uint32_t len)
{
    uint32_t Len = 0U;

    for (Len = 0U; Len < len; Len++) {
        if (src0[Len] != src1[Len]) {
            return -1;
        }
    }

    return 0;
}

static void BufferZero(uint8_t *buf, uint32_t len)
{
    for (uint32_t Cnt = 0U; Cnt < len; Cnt++) {
        buf[Cnt] = 0;
    }
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
 * @param  CanRxIndex Rx node index.
 *
 * @return true if transfer successful.
 * @return false if transfer failed.
 */
int CanFD_EnhancedRxFifoDma_Example(uint8_t ucAuto)
{
    int res = -1;
    uint8_t txbuf[TX_CNT];
    flexcan_msgbuff_t recvBuff;

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);
    BufferInit(&txbuf[0], TX_CNT);
    BufferZero((uint8_t *)&recvBuff, sizeof(flexcan_msgbuff_t));

    const flexcan_enhancedIdTableType_t id_filter_table[STD_FILTER_NUM + EXT_FILTER_NUM - 1] = {
        {FLEXCAN_ENHANCED_RX_FIFO_ONE_ID_FILTER, false, false, false, 1, 0},
        {FLEXCAN_ENHANCED_RX_FIFO_ONE_ID_FILTER, false, false, false, 2, 0},
    };

    /** Initialize PDMA */
    PDMA_DRV_Init(&g_stPdmaState0, &g_stPdma0UserConfig0, g_stPdma0ChnStateArray,
                  g_stPdma0ChannelConfigArray, PDMA_CHN_STATE_COUNT);

    /** Init flexcan0  */
    FLEXCAN_DRV_Init(0, &g_stFlexcanState_0, &g_stFlexcan0UserConfig0);

    PrepareEnhancedRxFIFO(0, &id_filter_table[0], &recvBuff);

    while (1) {
        WaitReceiveComplete(0, FIFO_START_MB_ID);

        /* Check the received message ID and payload */
        if (recvBuff.msgId == MASTER_TX_MSG_ID) {
            res = MsgCompare(&recvBuff.data[0], &txbuf[0], TX_CNT);
        } else {
            res = -1;
        }

        if (res == 0)
            break;
    }

    FLEXCAN_DRV_Deinit(0);
    PDMA_DRV_Deinit();

    return res;
}
