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
 * @file i2c.c
 * @brief Examples for I2C module use i2c0 send data to eeprom and receive data from eeprom.
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-10
 *
 * @details In I2C_Master2Eeprom_Example, we first use gpio mode to sent 9 clk on scl line to release peripherals.
 * Then we init Data & Buffer. 0-31 for transferred data and master receive buffer is all 0.
 * With i2c0 as master sending 32 bytes data and sending data address bytes to to eeprom,
 * i2c0 will receive 32 bytes data from eeprom begin at address = 0x0.
 * Finally the 32 bytes data in receive buffer will be compared to the data sent to eeprom to check if i2c0 works fine.
 *
 * @platform Fullmask evb board
 *
 * @pins
 * | function name | port number | function number | board connector |
 * |:------------- |:------------|:----------------|:----------------|
 *      I2C0 SCL   |    PORTC25  |      ALT3       |    72 pin
 *      I2C0 SDA   |    PORTC26  |      ALT3       |    73 pin
 * @connections
 *
 * @attention
 * EEPROM on peripherals board is 24LC128-I_SN. \n
 * EEPROM's slave address is 0x50 for A0-A2 Pins are set to low. \n
 * EEPROM need to send data Address High Byte and Address Low Byte before reading data.
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "../board/sdk_project_config.h"
#include "testcase.h"

#define DATA_SIZE                            (32U)
#define BUFF_SIZE                            (34U)
#define EEPROM_DATE_ADDRESS_SIZE             (2U)
#define OSIF_WAIT_FOREVER                    (0xFFFFFFFFU)
#define NUM_OF_I2C_CONFIGURED_PINS0_FOR_GPIO (4U)

static uint8_t g_masterRxBuffer[BUFF_SIZE];
static uint8_t g_eepromRxStartDataAddress[2] = {0, 0};
i2c_master_state_t g_stI2C0MasterState;
typedef struct eeprom {
    uint8_t highAddr;
    uint8_t lowAddr;
    uint8_t data[DATA_SIZE];
} EEPROM;

/**
 * @brief PINMUX configuration for I2C0 and I2C1 pins for GPIO mode.
 */
static pin_settings_config_t g_stI2cPinmuxForGpioInitConfigArr0[NUM_OF_I2C_CONFIGURED_PINS0_FOR_GPIO] = {
    {
        .base           = PORTC,
        .pinPortIdx     = 25U, // FullMask evb 72 pin
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR0_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = false,
        .debounceEnable = true,
    },
    {
        .base           = PORTC,
        .pinPortIdx     = 26U, // FullMask evb 73 pin
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR0_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = false,
        .debounceEnable = true,
    },
    {
        .base           = PORTB,
        .pinPortIdx     = 4U, // FullMask evb 45 pin
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR0_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = false,
        .debounceEnable = true,
    },
    {
        .base           = PORTB,
        .pinPortIdx     = 5U, // FullMask evb 46 pin
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR0_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = false,
        .debounceEnable = true,
    },
};

/**
 * @brief Use gpio mode to sent 9 clk on scl line to release peripherals.
 * Because used 24LC128-I/SN eeprom will lock scl when end transform data with error.
 * Use I2C0 and I2C1 pins in gpio mode and send 9 scl to eeprom to release scl lock.
 */
static void ReleasePeripherals(void)
{
    PINS_DRV_Init(NUM_OF_I2C_CONFIGURED_PINS0_FOR_GPIO, g_stI2cPinmuxForGpioInitConfigArr0);
    int i = 9;
    while (i) {
        PINS_DRV_WritePin(PORTC, 25, 0);
        PINS_DRV_WritePin(PORTB, 04, 0);
        OS_DelayMs(10);
        PINS_DRV_WritePin(PORTC, 25, 1);
        PINS_DRV_WritePin(PORTB, 04, 1);
        OS_DelayMs(10);
        i--;
    }
}

/**
 * @brief Compared src0 and src1 arrays.
 *
 * @param  src0 Array0 pointer to be compared.
 * @param  src1 Array1 pointer to be compared.
 * @param  len Length of Arrays.
 * @return true: Array0 and Array1 are same.
 * @return false: Array0 and Array1 are different
 */
static bool MsgCompareI2c(uint8_t *src0, uint8_t *src1, uint32_t len)
{
    uint8_t Len = 0U;
    for (Len = 0U; Len < len; Len++) {
        if (src0[Len] != src1[Len]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief I2C main module
 *
 * @return bool
 *         true if run succeed
 *         false if run failed
 */
int I2C_Master2Eeprom_Example(uint8_t ucAuto)
{
    bool res = true;
    /* Agreement requirement for eeprom data address. */
    EEPROM txdata;
    txdata.highAddr = 0;
    txdata.lowAddr  = 0;

    /* Release eeprom. */
    ReleasePeripherals();

    /* Data & Buffer init. */
    for (int Cnt = 0U; Cnt < DATA_SIZE; Cnt++) {
        txdata.data[Cnt]      = (uint8_t)(DATA_SIZE - Cnt - 1);
        g_masterRxBuffer[Cnt] = 0;
    }

    /* Initialize PDMA. */
    PDMA_DRV_Init(&g_stPdmaState0, &g_stPdma0UserConfig0, g_stPdma0ChnStateArray,
                  g_stPdma0ChannelConfigArray, PDMA_CHANNEL_CONFIG_COUNT);
    /* Init PINMUX for PORTC 25 SCL - PORTC 26 SDA. */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);
    /* Initialize I2C Master configuration. */
    I2C_DRV_MasterInit(INST_I2C_0, &g_stI2c0MasterUserConfig0, &g_stI2C0MasterState);

    /* Send a packet of data to the bus slave */
    I2C_DRV_MasterSendDataBlocking(INST_I2C_0, (uint8_t *)&txdata, BUFF_SIZE, true, OSIF_WAIT_FOREVER);
    /* Delay 5 Ms for eeprom to complete. */
    OS_DelayMs(5);
    /* Send data address to the bus slave. */
    I2C_DRV_MasterSendDataBlocking(INST_I2C_0, (uint8_t *)&g_eepromRxStartDataAddress, EEPROM_DATE_ADDRESS_SIZE,
                                   true, OSIF_WAIT_FOREVER);
    /* Delay 10 Ms for eeprom to complete. */
    OS_DelayMs(10);
    /* Request data from the bus slave. */
    I2C_DRV_MasterReceiveDataBlocking(INST_I2C_0, &g_masterRxBuffer[0], DATA_SIZE, true, OSIF_WAIT_FOREVER);

    /* Verify res. */
    res &= MsgCompareI2c(&txdata.data[0], &g_masterRxBuffer[0], DATA_SIZE);

    /* Deinit I2C. */
    I2C_DRV_MasterDeinit(INST_I2C_0);
    /* Release eeprom. */
    ReleasePeripherals();

    if (res) return 0;
    else return -1;
}
