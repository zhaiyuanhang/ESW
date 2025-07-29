
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
 * @file clock_config.c
 * @brief
 * @author abu (abu@eswincomputing.com)
 * @date 2024-01-29
 *
 */

#include "clock_config.h"

/**
 * @brief User peripheral configuration structure
 */
multiplexer_clock_config_t g_stMuxClockConfig[NUM_OF_MUX_CLOCKS_0] = {
    {
        .clkName = SPLL_SRC_CLK,
        .clkSel  = CRG_SPLL_SRC_FROSC_CLK,
    },
    {
        .clkName = WORK_CLK,
        .clkSel  = CRG_WORK_SRC_SPLL_CLK,
    },
    {
        .clkName = WDOG_CLK,
        .clkSel  = CRG_WDOG_SRC_LPSOC_CLK,
    },
    {
        .clkName = AON_CLK,
        .clkSel  = CRG_RTC_SRC_LPOSC_CLK,
    },
    {
        .clkName = SUPERTMR0_SELCLK,
        .clkSel  = CRG_SUPERTMR_FUNC_CLK,
    },
    {
        .clkName = SUPERTMR1_SELCLK,
        .clkSel  = CRG_SUPERTMR_FUNC_CLK,
    },
    {
        .clkName = SUPERTMR2_SELCLK,
        .clkSel  = CRG_SUPERTMR_FUNC_CLK,
    },
    {
        .clkName = SUPERTMR3_SELCLK,
        .clkSel  = CRG_SUPERTMR_FUNC_CLK,
    },
    {
        .clkName = SUPERTMR4_SELCLK,
        .clkSel  = CRG_SUPERTMR_FUNC_CLK,
    },
    {
        .clkName = SUPERTMR5_SELCLK,
        .clkSel  = CRG_SUPERTMR_FUNC_CLK,
    },
};

/**
 * @brief User peripheral configuration structure
 */
divider_clock_config_t g_stDivClockConfig[NUM_OF_DIV_CLOCKS_0] = {
    {
        .clkName = CPU_RTC_CLK,
        .clkDiv  = DIVIDE_BY_4,
    },
    {
        .clkName = CPU_CORE_CLK,
        .clkDiv  = DIVIDE_BY_1,
    },
    {
        .clkName = PERIPH_BUS_CLK,
        .clkDiv  = DIVIDE_BY_2,
    },
    {
        .clkName = MAIN_BUS_MOTOR_CTRL_CLK,
        .clkDiv  = DIVIDE_BY_2,
    },
    {
        .clkName = MAIN_BUS_AUX_CLK,
        .clkDiv  = DIVIDE_BY_4,
    },
    {
        .clkName = SUPERTMR_FUNC_CLK,
        .clkDiv  = DIVIDE_BY_8,
    },
    {
        .clkName = MAIN_BUS_PLL_FUNC_CLK,
        .clkDiv  = DIVIDE_BY_2,
    },
    {
        .clkName = SUPERIO_CLK,
        .clkDiv  = DIVIDE_BY_2,
    },
    {
        .clkName = PERIPH_BUS_PLL_FUNC_CLK,
        .clkDiv  = DIVIDE_BY_2,
    },
    {
        .clkName = PERIPH_BUS_PLL_AUX_FUNC_CLK,
        .clkDiv  = DIVIDE_BY_4,
    },
    {
        .clkName = FAST_AUX_CLK,
        .clkDiv  = DIVIDE_BY_4,
    },
    {
        .clkName = ADC_FUNC_CLK,
        .clkDiv  = DIVIDE_BY_4,
    },
    {
        .clkName = DEB_CLK,
        .clkDiv  = DIVIDE_BY_1,
    },
    {
        .clkName = PCTMR_FUNC_CLK,
        .clkDiv  = DIVIDE_BY_32,
    },
};

/**
 * @brief User Configuration structure clock_managerCfg_0
 */
clock_manager_user_config_t g_stClockManInitConfig = {
    .soscConfig =
        {
            .freq          = 24000000.00,
            .extRef        = CRG_SOSC_REF_OSC,
            .current       = XTALMF_OPS_CURRENT_120uA,
            .resistor      = XTALRF_1M_OHM_RESISTOR,
            .xtalAGC       = XTAL_AGC_GAIN_1,
            .xtalAGCBypass = true,
            .enable        = true,
        },
    .spllConfig =
        {
            .refDiv           = 0x02,
            .posDiv0          = 0x01,
            .divSetInteger    = 0x0a,
            .divSetFractional = 0,
        },
    .extClkConfig =
        {
            .rtcoscFreq = 32768.00,
            .tclkFreq =
                {
                    0.00,
                    0.00,
                    0.00,
                },
        },
    .muxConfig =
        {
            .count     = NUM_OF_MUX_CLOCKS_0,
            .muxClocks = g_stMuxClockConfig,
        },
    .divConfig =
        {
            .count     = NUM_OF_DIV_CLOCKS_0,
            .divClocks = g_stDivClockConfig,
        },
};

/**
 * @brief Array of pointers to User configuration structures
 */
clock_manager_user_config_t const *g_pstClockManConfigsArr[] = {
    &g_stClockManInitConfig,
};
