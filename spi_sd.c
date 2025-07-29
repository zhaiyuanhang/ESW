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
 * @file spi_sd.c
 * @brief
 * @author abu (abu@eswincomputing.com)
 * @date 2023-02-02
 *
 * @details Spi_Sd_Example can test communication between spi0(master) and tf(slave) after connect spi0 to tf.
 *
 * @platform Fullmask evb board
 *
 * @pins
 * | function name | port number | function number | board connector |
 * |:------------- |:------------|:----------------|:----------------|
 *      SPI0 SCK   |    PORTB23  |      ALT3       |    J1-5
 *      SPI0 PCS0  |    PORTC1   |      ALT3       |    J1-2
 *      SPI0 SOUT  |    PORTC0   |      ALT3       |    J1-7
 *      SPI0 SIN   |    PORTB20  |      ALT3       |    J1-3
 * @connections
 * SPI0 SCK     <-------->      SD SCLK
 * SPI0 PCS0    <-------->      SD CS
 * SPI0 SOUT    <-------->      SD DI
 * SPI0 SIN     <-------->      SD DO
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "../board/sdk_project_config.h"
#include "testcase.h"
#include "spi_sd_driver.h"
/**
 * @brief   transfer data between InstMaster and InstSlave.
 *
 * @return true if test run succeed
 * @return false if test run failed.
 */
int Spi_Sd_Example(uint8_t ucAuto)
{
    uint32_t capacity;
    uint32_t sectors;
    uint8_t val;
    uint8_t send_buf[512] = {0};
    uint8_t read_buf[512] = {0};
    uint8_t send_buf_multi[1024] = {0};
    uint8_t read_buf_multi[1024] = {0};

    for (int i = 0; i < 512; i++) {
        send_buf[i] = i;
    }

    for (int i = 0; i < 1024; i++) {
        send_buf_multi[i] = i;
    }

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);

    SPI_DRV_MasterInit(INST_SPI_0, &g_stSpiState_0, &g_stSpi0MasterConfig0);

    val = SD_DRV_Init(INST_SPI_0);
    if (val != 0) return -1;

    sectors = SD_DRV_GetSecOrCapCount(INST_SPI_0, 0);
    if (sectors == 0) return -1;

    capacity = SD_DRV_GetSecOrCapCount(INST_SPI_0, 1);
    if (capacity == 0) return -1;

    val = SD_DRV_WriteDisk(INST_SPI_0, send_buf, 0, 1);
    if (val != 0) return -1;

    val = SD_DRV_ReadDisk(INST_SPI_0, read_buf, 0, 1);
    if (val != 0) return -1;

    for (int i = 0; i < 512; i++) {
       if (read_buf[i] != send_buf[i]) {
           return -1;
       }
    }

    val = SD_DRV_WriteDisk(INST_SPI_0, send_buf_multi, 5, 2);
    if (val != 0) return -1;

    val = SD_DRV_ReadDisk(INST_SPI_0, read_buf_multi, 5, 2);
    if (val != 0) return -1;

    for (int i = 0; i < 512; i++) {
        if (read_buf_multi[i] != send_buf_multi[i]) {
            return -1;
        }
    }

    SPI_DRV_MasterDeinit(INST_SPI_0);
    return 0;
}
