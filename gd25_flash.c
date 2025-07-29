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
 * | function name | port number | function number |
 * |:------------- |:------------|:----------------|
 *      SPI2 SCK   |    PORTB16  |      ALT3       |
 *      SPI2 PCS0  |    PORTD30  |      ALT3       |
 *      SPI2 SOUT  |    PORTB9   |      ALT3       |
 *      SPI2 SIN   |    PORTB10  |      ALT3       |
 * @connections
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "../board/sdk_project_config.h"
#include "gd25qxx.h"
#include "testcase.h"
/**
 * @brief Read flash through SPI
 *
 * @return bool
 *         true if run succeed
 *         false if run failed
 */

int Spi_Flash_Example(uint8_t ucAuto)
{
	int gd_result = -1;
    uint8_t id[6] = {0};

    /* Call the init function for  SPI driver */
    SPI_DRV_MasterInit(INST_SPI_2, &g_stSpiState_2, &g_stSpi2MasterConfig0);

    /* Init GD25Qxxx */
    Flash_GD25Qxx_Init();
    /* Read Manufacture/Device ID. */
    Flash_GD25Qxx_Read_ID(id, 0x90);

    if ((id[4] != 0) && (id[5] != 0)) {
    	gd_result = 0;
    } else {
    	gd_result = -1;
    }

    SPI_DRV_MasterDeinit(INST_SPI_2);
    return gd_result;
}
