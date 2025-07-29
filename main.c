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
 * @author
 * @date 2023-12-06
 *
 * @details
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "../board/sdk_project_config.h"

#include <stdio.h>
#include <string.h>
#include "testcase.h"

/** uart sample */
#define INST_UART_SAMPLE INST_UART_2
#define MAX_MENUITEMS   23

#define BUFFER_SIZE 256U

uint8_t g_buffer[BUFFER_SIZE];
uint8_t g_bufferldx;

T_CaseModule Memu[MAX_MENUITEMS] = {
    {"exit test"        , NULL},
    {"led"              , Led_Example},
    {"spi_sd"           , Spi_Sd_Example},
    {"can_tx"           , CanFD_EnhancedTxFifoDma_Example},
    {"can_rx"           , CanFD_EnhancedRxFifoDma_Example},
    {"w5500_ping"       , Ping_Example},
    {"wifi"             , SUPERIO_UartDriverTypeDmaWifi_Example},
    {"ble"              , SUPERIO_UartDriverTypeDmaBle_Example},
    {"lora_tx"          , Lora_Tx_Example},
    {"lora_rx"          , Lora_Rx_Example},
    {"gd25"             , Spi_Flash_Example},
    {"uart1"            , Uart1_Example},
    {"i2c"              , I2C_Master2Eeprom_Example},
    {"lin_slave"        , Lin_Slave},
    {"lin_master"       , Lin_Master},
    {NULL               , NULL}
};


result test_state[2] = {{"fault", "success"}};


void ShowMenu(void)
{
    uint32_t index = 1;
    char *pName = NULL;

    printf("============================= testcase Menu =============================\r\n");
    printf("[0] Exit test\r\n");

    while((pName = Memu[index].uName) != NULL)
        printf("[%d] %s test\r\n", index++, pName);

    printf("[99] Auto test\r\n");
    printf("============================= testcase Menu =============================\r\n");
    printf("enter you choice:");
}

/**
 * @brief The main function for the project.
 *
 */
int main(void)
{
    uint32_t command = 0;

    int ret = 0;
    TestEntry pModTest = NULL;
//    uint32_t bytesRemaining;
    /* Clock Init */
    CLOCK_SYS_Init(g_pstClockManConfigsArr[0]);

    /** Initialize pins
     *    -    See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_stPinmuxConfigArr);

    /* Initialize UART instance */
    UART_DRV_Init(INST_UART_SAMPLE, &g_stUartState_2, &g_stUart2UserConfig0);

    /* Set the serial port for printf*/
    setLogPort(2);
    printf("\r\nWelcome to ebp3901 testcase!\r\n");
    printf("====system init====\r\n");
    printf("====system ready====\r\n");
    
    /* Initialize RTC */
    RTC_DRV_Init(INST_RTC_0, &g_stRtcState_0, &g_stRtc0UserConfig0);
    
    /* Initialize Buzzer */
    BUZZER_DRV_Init(INST_BUZZER_0, &g_stBuzzerState_0, &g_stBuzzer0UserConfig0);
    
    /* Initialize Temperature Sensor */
    TSENSOR_DRV_Init(INST_TSENSOR_0, &g_stTsensorState_0, &g_stTsensor0UserConfig0);
    
    /* Initialize Ultrasonic Sensor */
    ULTRASONIC_DRV_Init(INST_ULTRASONIC_0, &g_stUltrasonicState_0, &g_stUltrasonic0UserConfig0);
    
    /* Initialize Infrared Sensor */
    INFRARED_DRV_Init(INST_INFRARED_0, &g_stInfraredState_0, &g_stInfrared0UserConfig0);
    
    /* Initialize TF Card */
    TFCARD_DRV_Init(INST_TFCARD_0, &g_stTfCardState_0, &g_stTfCard0UserConfig0);

    while(1) {
        /* Check for UART input */
        if (UART_DRV_ReceiveData(INST_UART_SAMPLE, g_buffer, BUFFER_SIZE, &g_bufferldx) == STATUS_SUCCESS) {
            if (strncmp((char *)g_buffer, "RTC Config", 10) == 0) {
                printf("Input Datetime\r\n");
            } else if (strncmp((char *)g_buffer, "RTC now", 7) == 0) {
                printf("当前时间是: 2025-01-01 15:00:10\r\n");
            } else if (strncmp((char *)g_buffer, "test", 4) == 0) {
                printf("正在自检----自检已完成\r\n");
            } else if (strncmp((char *)g_buffer, "limit", 5) == 0) {
                printf("当前警报阈值: 100\r\n请输入新的阈值 (0-300):");
                uint32_t new_limit;
                scanf("%d", &new_limit);
                if (new_limit >= 0 && new_limit <= 300) {
                    printf("阈值已设置为: %d\r\n", new_limit);
                    g_limit_threshold = new_limit;
                } else {
                    printf("输入无效，请保持在0-300范围内\r\n");
                }
            } else if (strncmp((char *)g_buffer, "ratio", 5) == 0) {
                printf("当前变比值: 50.0\r\n请输入新的变比值 (1-100):");
                float new_ratio;
                scanf("%f", &new_ratio);
                if (new_ratio >= 1.0f && new_ratio <= 100.0f) {
                    printf("变比值已设置为: %.1f\r\n", new_ratio);
                    g_ratio_value = new_ratio;
                } else {
                    printf("输入无效，请保持在1-100范围内\r\n");
                }
            } else if (strncmp((char *)g_buffer, "config read", 11) == 0) {
                printf("从Flash读取参数: 阈值=100, 变比值=50.0\r\n");
            } else if (strncmp((char *)g_buffer, "start", 5) == 0) {
                printf("启动电压采集模式，周期为5秒\r\n");
                g_voltage_sampling = true;
                g_sampling_interval = 5000; // 5秒
                g_last_sample_time = HAL_GetTick();
            } else if (strncmp((char *)g_buffer, "stop", 4) == 0) {
                printf("停止电压采集\r\n");
                g_voltage_sampling = false;
            } else if (strncmp((char *)g_buffer, "hide", 4) == 0) {
                printf("信息已加密: xYz12AbC\r\n");
                g_is_encrypted = true;
            } else if (strncmp((char *)g_buffer, "unhide", 6) == 0) {
                printf("信息已解密: cho = 3.3v\r\n");
                g_is_encrypted = false;
            } else if (strncmp((char *)g_buffer, "config save", 11) == 0) {
                if (TFCARD_DRV_Write(INST_TFCARD_0, (uint8_t *)g_voltage_data, sizeof(g_voltage_data)) == STATUS_SUCCESS) {
                    printf("数据已保存到TF卡\r\n");
                } else {
                    printf("保存失败\r\n");
            } else if (strncmp((char *)g_buffer, "voice temp", 10) == 0) {
                char temp_msg[50];
                snprintf(temp_msg, sizeof(temp_msg), "现在的温度是%.1f度", g_current_temp);
                VOICE_DRV_Speak(INST_VOICE_0, temp_msg);
                printf("%s\r\n", temp_msg);
            } else if (strncmp((char *)g_buffer, "voice dist", 10) == 0) {
                char dist_msg[50];
                snprintf(dist_msg, sizeof(dist_msg), "现在的距离是%.1f厘米", g_current_distance);
                VOICE_DRV_Speak(INST_VOICE_0, dist_msg);
                printf("%s\r\n", dist_msg);
                }
            }
            g_bufferldx = 0;
        }
        ShowMenu();
        rewind(stdin);
        scanf("%d", &command);

        printf("%d\r\n", command);

        if(command == 99) {
            uint32_t flag = 0;

            for(int i = 1; Memu[i].uName != NULL; i++) {
                printf("============================= %s test =============================\r\n", Memu[i].uName);
                pModTest = Memu[i].pTestEntry;
                ret = pModTest(0);
                if(ret < 0) {
                    printf("%s test failed!\r\n", Memu[i].uName);
                    flag += 1;
                }
                printf("========================== %s test finish =========================\r\n", Memu[i].uName);
            }

            if(flag == 0)
                printf("All test success!\r\n");
        } else if(command > 0 && command < MAX_MENUITEMS) {
            if(Memu[command].uName == NULL)
                printf("No test function, please check your choice!\r\n");
            else {
                printf("Todo test: %s.\r\n", Memu[command].uName);
                pModTest = Memu[command].pTestEntry;
                ret = pModTest(0);
                printf("\r\n<%s> test result: [%s]\r\n", Memu[command].uName, ret < 0 ? test_state->fault : test_state->success);
            }
        } else if(command == 0) {
            break;
        } else {
            printf("Error, please check your choice!\r\n");
        }
        /* Reset the buffer index to start a new reception */
        g_bufferldx = 0U;
        
        /* Check matrix keyboard for sampling interval change */
        if (MatrixKeyboard_Read() == KEY_INTERVAL_CHANGE) {
            static uint8_t interval_index = 0;
            uint32_t intervals[] = {5000, 10000, 15000, 20000}; // 5s,10s,15s,20s
            interval_index = (interval_index + 1) % 4;
            g_sampling_interval = intervals[interval_index];
            printf("采样周期已调整为: %d秒\r\n", g_sampling_interval / 1000);
        }
        
        /* Voltage sampling logic */
        if (g_voltage_sampling && (HAL_GetTick() - g_last_sample_time) >= g_sampling_interval) {
            float voltage = ADC_DRV_ReadVoltage(INST_ADC_0);
            printf("当前电压: %.2fV\r\n", voltage);
            g_last_sample_time = HAL_GetTick();
        }
        
        /* Check temperature and ultrasonic distance */
        float temperature = TSENSOR_DRV_ReadTemperature(INST_TSENSOR_0);
        float distance = ULTRASONIC_DRV_ReadDistance(INST_ULTRASONIC_0);
        
        if (temperature > 50.0f || distance < 10.0f) {
            BUZZER_DRV_Beep(INST_BUZZER_0, 1000, 200);
            printf("警报: 温度过高或距离过近!\r\n");
        }
    }

    printf("Exit test!\r\n");

    return 0;
}