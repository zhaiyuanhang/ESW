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
 * @file w5500_ping.c
 * @brief This file contains user's application code.
 * @author
 * @date
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

/* Including necessary configuration files. */
#include "w5500_driver.h"
#include "string.h"
#include "sdk_project_config.h"
/**
 * @brief   transfer data between InstMaster and InstSlave.
 *
 * @return true if test run succeed
 * @return false if test run failed.
 */

int Ping_Example(uint8_t ucAuto)
{
    uint8_t ping_result = 1;
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);
    SPI_DRV_MasterInit(INST_SPI_3, &g_stSpiState_3, &g_stSpi3MasterConfig0);
    printf("W5500 ping test.\r\n");
    W5500_DRV_Reset();
    W5500_DRV_SetMAC();
    W5500_DRV_SetIP();
    W5500_DRV_InitSocketBuffer(txsize, rxsize);
    printf("Start............ \r\n");

    req = 0;
    rep = 0;
    do{
        ping_result = W5500_DRV_DoPing();
        if (req >= 3) break;
    } while (ping_result == 0);

    SPI_DRV_MasterDeinit(INST_SPI_3);
    if(ping_result == 0) {
        return 0;
    } else {
        return -1;
    }
}


