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
 * @file pin_config.c
 * @author abu (abu@eswincomputing.com)
 * @date 2024-01-29
 *
 */

#include "pin_config.h"

/**
 * @brief User configuration structure
 */
pin_settings_config_t g_stPinmuxConfigArr[NUM_OF_CONFIGURED_PINS] = {
    {
        //UART2_RX function, 100pin package, 20pin
        .base        = PORTA,
        .pinPortIdx  = 18U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //UART2_TX function, 100pin package, 19pin
        .base        = PORTA,
        .pinPortIdx  = 16U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //UART3_RX function, 100pin package, 46pin
        .base        = PORTB,
        .pinPortIdx  = 18U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //UART3_TX function, 100pin package, 17pin
        .base        = PORTA,
        .pinPortIdx  = 13U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //SUPERIO_D1 function, 100pin package, 3pin
        .base        = PORTA,
        .pinPortIdx  = 2U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR1_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
    {
        //SPI3_PCS2 function, 100pin package, 51pin
        .base        = PORTB,
        .pinPortIdx  = 29U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //SPI3_SCK function, 100pin package, 55pin
        .base        = PORTC,
        .pinPortIdx  = 3U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //SPI3_SIN function, 100pin package, 49pin
        .base        = PORTB,
        .pinPortIdx  = 25U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //SPI3_SOUT function, 100pin package, 50pin
        .base        = PORTB,
        .pinPortIdx  = 26U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //SPI2_PCS0 function, 100pin package, 99pin
        .base        = PORTD,
        .pinPortIdx  = 30U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI2_SCK function, 100pin package, 45pin
        .base        = PORTB,
        .pinPortIdx  = 16U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI2_SIN function, 100pin package, 40pin
        .base        = PORTB,
        .pinPortIdx  = 10U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI2_SOUT function, 100pin package, 39pin
        .base        = PORTB,
        .pinPortIdx  = 9U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI1_PCS0 function, 100pin package, 93pin
        .base        = PORTD,
        .pinPortIdx  = 24U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //SPI1_SCK function, 100pin package, 66pin
        .base        = PORTC,
        .pinPortIdx  = 17U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI1_SIN function, 100pin package, 65pin
        .base        = PORTC,
        .pinPortIdx  = 16U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI1_SOUT function, 100pin package, 64pin
        .base        = PORTC,
        .pinPortIdx  = 15U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI0_PCS0 function, 100pin package, 54pin
        .base        = PORTC,
        .pinPortIdx  = 1U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI0_SCK function, 100pin package, 48pin
        .base        = PORTB,
        .pinPortIdx  = 23U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI0_SIN function, 100pin package, 47pin
        .base        = PORTB,
        .pinPortIdx  = 20U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SPI0_SOUT function, 100pin package, 53pin
        .base        = PORTC,
        .pinPortIdx  = 0U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //PORTD14 function, 100pin package, 88pin
        .base           = PORTD,
        .pinPortIdx     = 14U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTD2 function, 100pin package, 78pin
        .base           = PORTD,
        .pinPortIdx     = 2U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_INPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTD31 function, 100pin package, 100pin
        .base           = PORTD,
        .pinPortIdx     = 31U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_INPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_LOW_LEVEL,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTD4 function, 100pin package, 79pin
        .base           = PORTD,
        .pinPortIdx     = 4U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_INPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_RISING_EDGE,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTC10 function, 100pin package, 59pin
        .base           = PORTC,
        .pinPortIdx     = 10U,
        .pullConfig     = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTC18 function, 100pin package, 67pin
        .base           = PORTC,
        .pinPortIdx     = 18U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTC19 function, 100pin package, 68pin
        .base           = PORTC,
        .pinPortIdx     = 19U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTC21 function, 100pin package, 69pin
        .base           = PORTC,
        .pinPortIdx     = 21U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTC22 function, 100pin package, 70pin
        .base           = PORTC,
        .pinPortIdx     = 22U,
        .pullConfig     = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 1,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTC28 function, 100pin package, 74pin
        .base           = PORTC,
        .pinPortIdx     = 28U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //I2C0_SCL function, 100pin package, 72pin
        .base        = PORTC,
        .pinPortIdx  = 25U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //I2C0_SDA function, 100pin package, 73pin
        .base        = PORTC,
        .pinPortIdx  = 26U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //CAN0_RX function, 100pin package, 9pin
        .base        = PORTA,
        .pinPortIdx  = 8U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR1_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //CAN0_TX function, 100pin package, 8pin
        .base        = PORTA,
        .pinPortIdx  = 7U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR1_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT5,
        .isGpio      = false,
    },
    {
        //SUPERIO_D2 function, 100pin package, 2pin
        .base        = PORTA,
        .pinPortIdx  = 1U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR1_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
};
