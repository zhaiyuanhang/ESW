
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
 * @file peripherals_flexcan_1_config.c
 * @author abu (abu@eswincomputing.com)
 * @date 2024-01-29
 *
 */

#include "peripherals_flexcan_1_config.h"

flexcan_state_t g_stFlexcanState_0;

flexcan_user_config_t g_stFlexcan0UserConfig0 = {
    .max_num_mb                  = 21,
    .num_id_filters              = FLEXCAN_RX_FIFO_ID_FILTERS_8,
    .is_rx_fifo_needed           = false,
    .num_enhanced_std_id_filters = 2,
    .num_enhanced_ext_id_filters = 0,
    .num_enhanced_watermark      = 0,
    .is_enhanced_rx_fifo_needed  = true,
    .flexcanMode                 = FLEXCAN_NORMAL_MODE,
    .payload                     = {
                            .blockR0 = FLEXCAN_PAYLOAD_SIZE_64,
                            .blockR1 = FLEXCAN_PAYLOAD_SIZE_64,
                            .blockR2 = FLEXCAN_PAYLOAD_SIZE_64,
                            .blockR3 = FLEXCAN_PAYLOAD_SIZE_64,
    },
    .fd_enable = true,
    .pe_clock  = FLEXCAN_CLK_SOURCE_PERIPH,
    .bitrate   = {
          .propSeg    = 5,
          .phaseSeg1  = 7,
          .phaseSeg2  = 4,
          .preDivider = 3,
          .rJumpwidth = 3,
    },
    .bitrate_cbt = {
        .propSeg    = 6,
        .phaseSeg1  = 7,
        .phaseSeg2  = 4,
        .preDivider = 1,
        .rJumpwidth = 3,
    },
    .transfer_type    = FLEXCAN_RXFIFO_USING_INTERRUPTS,
    .rxFifoDMAChannel = 0,
    .time_stamp       = {
              .timeStampSource      = FLEXCAN_CAN_CLK_TIMESTAMP_SRC,
              .msgBuffTimeStampType = FLEXCAN_MSGBUFFTIMESTAMP_TIMER,
              .hrConfigType         = FLEXCAN_TIMESTAMPCAPTURE_DISABLE,
    },
};
