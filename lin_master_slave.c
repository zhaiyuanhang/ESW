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
 * @brief lin_slave demo
 * @author abu (abu@eswincomputing.com)
 * @date 2023-02-06
 *
 * @details
 * This example demonstrates the LIN communication between EAM2011 EVB Master and Slave using LIN driver without LIN
 * Stack
 *
 * -    A frame contains header and data. The Master node can send header and data, but Slave node only can send data.
 *      Base on header, Master node or Slave node will take corresponding action.
 * -    If Slave node receives FRAME_MASTER_RECEIVE_DATA header, Slave node will respond by sending data (txBuff2).
 * -    If Slave node receives FRAME_SLAVE_RECEIVE_DATA header, Slave node will receive and check data.
 *
 * @platform Fullmask evb board
 *
 * @pins
 * | function name | port number | function number | board connector      |
 * |:------------- |:------------|:----------------|:---------------------|
 *   GND           |  GND        |   /             |  J9-1(Slave GND)
 *   LIN-RX        |  PTB18      |  ALT5           |  J9-2(Slave LIN BUS)
 *   LIN-TX        |  PTA13      |  ALT5           |  J9-2(Slave LIN BUS)
 * @connections
 *      connect lin adapter to lin bus(J9)
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "../board/sdk_project_config.h"

volatile int g_exitCode = 0;

#define UART_PORT_NAME            (PORTB)
#define UART_Rx_INDEX             (18U)
#define UART_PORT_IRQn            (GPIOC_18_IRQn)
#define LIN_INSTANCE              (INST_LIN_3)

/** (CLK (MHz)* timer period (us) / Prescaler) */
#define TIMER_COMPARE_VAL         (uint16_t)(2000U)
#define TIMER_TICKS_1US           (uint16_t)(4U)
#define DATA_SIZE                 (8U)
#define FRAME_SLAVE_RECEIVE_DATA  (1U)
#define FRAME_MASTER_RECEIVE_DATA (2U)
#define FRAME_GO_TO_SLEEP         (3U)
#define TIMEOUT                   (500U)

#define PORT_LED0_INDEX (28U) // PTC28
#define PORT_LED0_MASK  (0x1U << PORT_LED0_INDEX)
#define LED_GPIO_PORT   (PORTC)
#define LED_ON          (1U)
#define LED_OFF         (0U)

uint16_t g_timerOverflowInterruptCount = 0U;
volatile bool g_wakeupSignalFlag       = false;
volatile uint16_t g_capturedValue      = 0U;
volatile bool g_linEnabled             = false;

/** Define for DATA buffer transmit */
uint8_t g_txBuff1[DATA_SIZE] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};

uint8_t g_txBuff2[DATA_SIZE] = {0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11};
/** Define for DATA buffer receiver */
uint8_t g_rxBuff[DATA_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/**
 * @brief PCTMR Interrupt Service Routine
 *
 * @param  data Parameter values passed between functions
 */
void PCTMR_ISR(void *data)
{
    (void)data;
    /* Timer Interrupt Handler */
    LIN_DRV_TimeoutService(LIN_INSTANCE);
    /* Increment overflow count */
    g_timerOverflowInterruptCount++;
    /* Clear compare flag */
    PCTMR_DRV_ClearIntFlag(INST_PCTMR_0);
}

/**
 * @brief Callback function to get time interval in nano seconds
 * @param ns - number of nanoseconds passed since the last call of the function
 * @return dummy value
 */
uint32_t Lin1TimerGetTimeIntervalCallback0(uint32_t instance)
{
    (void)instance;
    static uint32_t previousCountValue = 0UL;
    uint32_t counterValue;

    counterValue = PCTMR_DRV_GetCounterValueByCount(INST_PCTMR_0);
    uint32_t ns  = ((uint32_t)(counterValue + g_timerOverflowInterruptCount * TIMER_COMPARE_VAL - previousCountValue)) * 1000U /
                  TIMER_TICKS_1US;
    g_timerOverflowInterruptCount = 0UL;
    previousCountValue            = counterValue;

    return ns;
}

/**
 * @brief  Callback function to get time interval in nano seconds
 *
 * @param  ns number of nanoseconds passed since the last call of the function
 * @return uint32_t dummy value
 */
uint32_t Lin1TimerGetTimeIntervalCallback1(uint32_t instance)
{
    (void)instance;
    static uint32_t previousCountValue = 0UL;
    uint32_t counterValue;

    counterValue = PCTMR_DRV_GetCounterValueByCount(INST_PCTMR_0);
    uint32_t ns  = ((uint32_t)(counterValue + g_timerOverflowInterruptCount * TIMER_COMPARE_VAL - previousCountValue)) * 1000U /
                  TIMER_TICKS_1US;
    g_timerOverflowInterruptCount = 0UL;
    previousCountValue            = counterValue;

    return ns;
}

#ifdef AUTOBAUD_SUPPORT
/**
 * @brief Interrupt handler for UART Rx pin port event
 *
 * @param  data Parameter values passed between functions
 */
void RXPIN_IRQHandler(void *data)
{
    static bool autoBaudComplete = false;

    /* Check if the interrupt is triggered by the UART Rx Pin */
    if (PINS_DRV_GetPortIntFlag(UART_PORT_NAME) & (1UL << UART_Rx_INDEX)) {
        /* Capture transition time stamp */
        g_capturedValue = PCTMR_DRV_GetCounterValueByCount(INST_PCTMR_0);
        /* If the auto baud process is not completed and lin is enabled, call
         * LIN_DRV_AutoBaudCapture
         */
        if ((!autoBaudComplete) && (g_linEnabled)) {
            if (LIN_DRV_AutoBaudCapture(LIN_INSTANCE) == STATUS_SUCCESS) {
                autoBaudComplete = true;
            }
        }

        /* Clear PORT interrupt register */
        PINS_DRV_ClearPortIntFlagCmd(UART_PORT_NAME);
    }
}
#endif

/**
 * @brief  Callback handler function
 *
 * @param  instance Lin instance number
 * @param  lin1_State lin1_State state information
 * @return lin_callback_t
 */
lin_callback_t CallbackHandler(uint32_t instance, lin_state_t *lin1_State)
{
    lin_callback_t callbackCurrent;
    callbackCurrent = lin1_State->Callback;
    (void)instance;

    switch (lin1_State->currentEventId) {
    case LIN_PID_OK:
        /* Set timeout */
        LIN_DRV_SetTimeoutCounter(LIN_INSTANCE, TIMEOUT);

        /* If PID is 0x01, salve node will receive data from master node */
        if (FRAME_SLAVE_RECEIVE_DATA == lin1_State->currentId) {
            /* Call to Receive Frame DATA Function */
            LIN_DRV_ReceiveFrameData(LIN_INSTANCE, g_rxBuff, sizeof(g_rxBuff));
        }

        /* If PID is 0x02, salve node will send data */
        if (FRAME_MASTER_RECEIVE_DATA == lin1_State->currentId) {
            /* Call to Send Frame DATA Function */
            LIN_DRV_SendFrameData(LIN_INSTANCE, g_txBuff2, sizeof(g_txBuff2));
        }

        /* If PID is 0x03, salve node will go to sleep mode */
        if (FRAME_GO_TO_SLEEP == lin1_State->currentId) {
            /* Go to sleep mode */
            LIN_DRV_GoToSleepMode(LIN_INSTANCE);
        }
        break;
    case LIN_PID_ERROR:
        /* Go to idle mode */
        LIN_DRV_GoToSleepMode(LIN_INSTANCE);
        break;
    case LIN_TX_COMPLETED:
    case LIN_RX_COMPLETED:
        /* Go to idle mode */
        LIN_DRV_GotoIdleState(LIN_INSTANCE);
        break;
    case LIN_CHECKSUM_ERROR:
    case LIN_READBACK_ERROR:
    case LIN_FRAME_ERROR:
    case LIN_RECV_BREAK_FIELD_OK:
        /* Set timeout */
        LIN_DRV_SetTimeoutCounter(LIN_INSTANCE, TIMEOUT);
        break;
    case LIN_WAKEUP_SIGNAL:
        /* Set wakeup signal flag */
        g_wakeupSignalFlag = true;
        break;
    case LIN_SYNC_ERROR:
    case LIN_BAUDRATE_ADJUSTED:
    case LIN_NO_EVENT:
    case LIN_SYNC_OK:
    default:
        /* do nothing */
        break;
    }

    return callbackCurrent;
}

/**
 * @brief Declare Callback handler function
 *
 * @param  instance
 * @param  lin1_State
 * @return lin_callback_t
 */
lin_callback_t CallbackHandler1(uint32_t instance, lin_state_t *lin1_State)
{
    lin_callback_t callbackCurrent;
    callbackCurrent = lin1_State->Callback;
    (void)instance;

    switch (lin1_State->currentEventId) {
    case LIN_PID_OK:

        /* Set timeout */
        LIN_DRV_SetTimeoutCounter(LIN_INSTANCE, TIMEOUT);

        /* If PID is FRAME_SLAVE_RECEIVE_DATA, salve node will receive data from master node */
        if (FRAME_SLAVE_RECEIVE_DATA == lin1_State->currentId) {
            /* Call to Send Frame DATA Function */
            LIN_DRV_SendFrameData(LIN_INSTANCE, g_txBuff1, sizeof(g_txBuff1));
        }

        /* If PID is FRAME_MASTER_RECEIVE_DATA, master node will receive data */
        if (FRAME_MASTER_RECEIVE_DATA == lin1_State->currentId) {
            /* Call to Receive Frame DATA Function */
            LIN_DRV_ReceiveFrameData(LIN_INSTANCE, g_rxBuff, sizeof(g_rxBuff));
        }

        /* If PID is FRAME_GO_TO_SLEEP, salve node will go to sleep mode */
        if (FRAME_GO_TO_SLEEP == lin1_State->currentId) {
            /* Go to sleep mode */
            LIN_DRV_GoToSleepMode(LIN_INSTANCE);
        }
        break;
    case LIN_PID_ERROR:
        /* Go to idle mode */
        LIN_DRV_GoToSleepMode(LIN_INSTANCE);
        break;
    case LIN_TX_COMPLETED:
    case LIN_RX_COMPLETED:
        /* Go to idle mode */
        LIN_DRV_GotoIdleState(LIN_INSTANCE);
        break;
    case LIN_CHECKSUM_ERROR:
    case LIN_READBACK_ERROR:
    case LIN_FRAME_ERROR:
    case LIN_RECV_BREAK_FIELD_OK:
        /* Set timeout */
        LIN_DRV_SetTimeoutCounter(LIN_INSTANCE, TIMEOUT);
        break;
    case LIN_WAKEUP_SIGNAL:
        /* Set wakeup signal flag */
        g_wakeupSignalFlag = true;
        break;
    case LIN_SYNC_ERROR:
    case LIN_BAUDRATE_ADJUSTED:
    case LIN_NO_EVENT:
    case LIN_SYNC_OK:
    default:
        /* do nothing */
        break;
    }
    return callbackCurrent;
}

/**
 * @brief  Main LIN master task
 */
bool LIN_SlaveBaremetal_Example(void)
{

    g_linEnabled = true;

    uint8_t index;
    uint8_t byteRemain = 1U;
    bool receiveFlag   = false;
    status_t status    = STATUS_ERROR;

    g_stLin3UserConfig0.timerGetTimeIntervalCallback = Lin1TimerGetTimeIntervalCallback1;
    g_stLin3UserConfig0.nodeFunction = LIN_SLAVE;
    /* Initialize LIN network interface */
    LIN_DRV_Init(LIN_INSTANCE, &g_stLin3UserConfig0, &g_stLinState_3);

    /* Install callback function */
    LIN_DRV_InstallCallback(LIN_INSTANCE, (lin_callback_t)CallbackHandler);

    /* Infinite loop */
    for (;;) {
        status = LIN_DRV_GetReceiveStatus(LIN_INSTANCE, &byteRemain);

        if ((status == STATUS_SUCCESS) && (0U == byteRemain) && (FRAME_SLAVE_RECEIVE_DATA == g_stLinState_3.currentId)) {
            receiveFlag = true;
            for (index = 0; index < DATA_SIZE; index++) {
                /* Check data receiving */
                if (g_rxBuff[index] != g_txBuff1[index]) {
                    receiveFlag = false;
                }

                /* Clear Rx buffer */
                g_rxBuff[index] = 0x00;
            }

            /* Check data receiving */
            if (false == receiveFlag) {
                /* Receive data failed */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, LED_OFF);
            } else {
                /* Receive data success */
                /* Toggle LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, LED_ON);
                break;
            }

            g_stLinState_3.currentId = 0;
        }

        /* Check wakeup signal flag */
        if (true == g_wakeupSignalFlag) {
            /* Clear wakeup signal flag */
            g_wakeupSignalFlag = false;
        }

        /* Check node state */
        if (LIN_NODE_STATE_SLEEP_MODE == LIN_DRV_GetCurrentNodeState(LIN_INSTANCE)) {
        }
    }
    return receiveFlag;
}

/**
 * @brief The lin slave example.
 *
 * @return int exit code.
 */
int Lin_Slave_Example(void)
{
	bool res = false;
    /* Initialize PCTMR */
    lin_pctmr_config_t linPctmrConf = {0};
    linPctmrConf.linInstance        = LIN_INSTANCE;
    linPctmrConf.pctmrInstance      = INST_PCTMR_0;

    g_stPctmr0Config0.callBack  = PCTMR_ISR;
    g_stPctmr0Config0.parameter = (void *)&linPctmrConf;

    PCTMR_DRV_Init(INST_PCTMR_0, &g_stPctmr0Config0);

    /** Start PCTMR */
    PCTMR_DRV_StartCounter(INST_PCTMR_0);

    /* Start LIN master task */
    res = LIN_SlaveBaremetal_Example();
    LIN_DRV_Deinit(LIN_INSTANCE);
    if (res)
    	return 0;
    else
    	return -1;
}

/**
 * @brief Main LIN master task
 * This function will initialize the LIN interface and set the scheme
 * to the normal table.
 * After the LIN network is initialized, the function will monitor
 * the Motor temperature and will turn LEDs on and off depending
 * on the retrieved temperature.
 *
 */
bool LIN_MasterBaremetal_Example(void)
{
    uint8_t index;
    uint8_t byteRemain = 1U;
    bool receiveFlag   = false;
    status_t status    = STATUS_ERROR;

    g_stLin3UserConfig0.timerGetTimeIntervalCallback = Lin1TimerGetTimeIntervalCallback0;
    g_stLin3UserConfig0.nodeFunction = LIN_MASTER;
    /* Initialize LIN network interface */
    LIN_DRV_Init(LIN_INSTANCE, &g_stLin3UserConfig0, &g_stLinState_3);

    /* Install callback function */
    LIN_DRV_InstallCallback(LIN_INSTANCE, (lin_callback_t)CallbackHandler1);

    /* Infinite loop */
    for (int i = 0;i < 3000; i++) {
        /* Send a frame to slave */
        LIN_DRV_MasterSendHeader(LIN_INSTANCE, FRAME_SLAVE_RECEIVE_DATA);
        OS_DelayMs(10);
        status = LIN_DRV_GetReceiveStatus(LIN_INSTANCE, &byteRemain);

        if ((status == STATUS_SUCCESS) && (0U == byteRemain) &&
            (FRAME_MASTER_RECEIVE_DATA == g_stLinState_3.currentId)) {
            for (index = 0; index < DATA_SIZE; index++) {
                /* Check data receiving */
                if (g_rxBuff[index] == g_txBuff2[index]) {
                    receiveFlag = true;
                }

                /* Clear Rx buffer */
                g_rxBuff[index] = 0x00;
            }

            /* Check data receiving */
            if (false == receiveFlag) {
                /* Receive failed */
            } else {
                /* Receive success */
            }
        }

        /* Check wakeup signal flag */
        if (true == g_wakeupSignalFlag) {
            /* Clear wakeup signal flag */
            g_wakeupSignalFlag = false;
        }

        /* Check node state */
        if (LIN_NODE_STATE_SLEEP_MODE == LIN_DRV_GetCurrentNodeState(LIN_INSTANCE)) {
        }
    }
    return receiveFlag;
}

/**
 * @brief The lin master example.
 *
 * @return int exit code.
 */
int Lin_Master_Example(void)
{
	bool res = false;
    /* Initialize PCTMR */
    lin_pctmr_config_t linPctmrConf = {0};
    linPctmrConf.linInstance        = LIN_INSTANCE;
    linPctmrConf.pctmrInstance      = INST_PCTMR_0;

    g_stPctmr0Config0.callBack  = PCTMR_ISR;
    g_stPctmr0Config0.parameter = (void *)&linPctmrConf;
    PCTMR_DRV_Init(INST_PCTMR_0, &g_stPctmr0Config0);

    /** Start PCTMR */
    PCTMR_DRV_StartCounter(INST_PCTMR_0);

    /* Start LIN master task */
    res = LIN_MasterBaremetal_Example();
    LIN_DRV_Deinit(LIN_INSTANCE);

    if (res)
    	return 0;
    else
    	return -1;
}

int Lin_Slave(uint8_t ucAuto){
	int res_a = -1;
	res_a = Lin_Slave_Example();
    OS_DelayMs(30000);
    Lin_Master_Example();

    return res_a;
}

int Lin_Master(uint8_t ucAuto){
	int res_a = -1;

    Lin_Master_Example();

    res_a = Lin_Slave_Example();

    return res_a;
}
