
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
 * @file peripherals_pctmr_1_config.c
 * @author abu (abu@eswincomputing.com)
 * @date 2024-01-29
 *
 */

#include "peripherals_pctmr_1_config.h"

pctmr_config_t g_stPctmr0Config0 = {
    .dmaRequest      = false,
    .interruptEnable = true,
    .freeRun         = false,
    .workMode        = PCTMR_WORKMODE_TIMER,
    .clockSelect     = PCTMR_CLOCKSOURCE_ERCLK,
    .prescaler       = PCTMR_PRESCALE_2,
    .bypassPrescaler = false,
    .compareValue    = 1999,
    .counterUnits    = PCTMR_COUNTER_UNITS_TICKS,
    .pinSelect       = PCTMR_PINSELECT_TRGMUX,
    .pinPolarity     = PCTMR_PINPOLARITY_RISING,
    .callBack        = NULL,
    .parameter       = NULL,
};
