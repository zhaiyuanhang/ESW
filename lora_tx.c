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
 * @brief
 * @author abu (abu@eswincomputing.com)
 * @date 2023-02-02
 *
 * @details Lora_Example test communication between spi1 and lora.
 *
 * @platform Fullmask evb board
 *
 * @pins
 * | function name | port number | function number | board connector |
 * |:------------- |:------------|:----------------|:----------------|
 *      SPI1 PCS0  |    PORTD24  |      ALT3       |    J2-Lora
 *      SPI1 SOUT  |    PORTC15  |      ALT3       |    J2-Lora
 *      SPI1 SIN   |    PORTC16  |      ALT3       |    J2-Lora
 *      SPI1 SCK   |    PORTC17  |      ALT3       |    J2-Lora
 *      SW CTL1    |    PORTC19  |      ALT1       |    J2-Lora
 *      SW CTL2    |    PORTC21  |      ALT1       |    J2-Lora
 *      BUSY       |    PORTD02  |      ALT1       |    J2-Lora
 *      DIO1       |    PORTD04  |      ALT1       |    J2-Lora
 *      NRESET     |    PORTC10  |      ALT1       |    J2-Lora
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "../board/sdk_project_config.h"
#include "lora_driver.h"
#define INST_UART_SAMPLE INST_UART_2

extern uint8_t tx_cnt;
uint8_t lora_send_buffer[5];

int Lora_Tx_Example(uint8_t ucAuto)
{
    int lora_tx_result = -1;
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);
    SPI_DRV_MasterInit(INST_SPI_1, &g_stSpiState_1, &g_stSpi1MasterConfig0);

    LoRa_DRV_Init();

    memcpy(lora_send_buffer, "eswin", 5);

    do {
        LoRa_DRV_Send(lora_send_buffer, 5);
        OS_DelayMs(1000);
    } while (tx_cnt <= 4);

    if (tx_cnt == 5) {
        lora_tx_result = 0;
    } else {
        lora_tx_result = -1;
    }

    SPI_DRV_MasterDeinit(INST_SPI_1);
    return lora_tx_result;
}
