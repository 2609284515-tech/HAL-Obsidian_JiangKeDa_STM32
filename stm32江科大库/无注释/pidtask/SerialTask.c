#include "main.h"
#include "cmsis_os2.h"
#include "Serial.h"
#include "OLED.h"

float Target[2], 
                 Kp[3] = {4.8, 0.02, 0.4},
                 Ki[3] = {0.8, 0, 0.1},
                 Kd[3] = {1.1, -0.04, 0},
                 Offset[3] = {0, 0, 0};
float *PID_Para[5] = {Target, Kp, Ki, Kd, Offset};

void StartSerialTask(void *argument)
{
    OLED_ShowString(0, 17, "Ta:", OLED_6X8);
    OLED_ShowString(0, 25, "P1:", OLED_6X8);
    OLED_ShowString(0, 33, "P2:", OLED_6X8);

    OLED_ShowFloatNum(19, 17, Target[0], 3, 0, OLED_6X8);
    OLED_ShowFloatNum(43, 17, Target[1], 3, 0, OLED_6X8);

    OLED_ShowFloatNum(19, 25, Kp[0], 1, 2, OLED_6X8);
    OLED_ShowFloatNum(51, 25, Ki[0], 1, 2, OLED_6X8);
    OLED_ShowFloatNum(83, 25, Kd[0], 1, 2, OLED_6X8);

    OLED_ShowFloatNum(19, 33, Kp[1], 1, 2, OLED_6X8);
    OLED_ShowFloatNum(51, 33, Ki[1], 1, 2, OLED_6X8);
    OLED_ShowFloatNum(83, 33, Kd[1], 1, 2, OLED_6X8);       
    
    // OLED_ShowFloatNum(19, 41, Kp[2], 1, 2, OLED_6X8);
    // OLED_ShowFloatNum(51, 41, Ki[2], 1, 2, OLED_6X8);
    // OLED_ShowFloatNum(83, 41, Kd[2], 1, 2, OLED_6X8);

    for(;;)
    {
        if (Serial_GetRxFlag() == 1)
        {
            /*步长调整*/
            double k;
            switch (Serial_RxPacket[0])
            {
                case 0:
                    k = 100;
                    break;
                default:
                    k = 0.01;
                    break;
            }

            /*4位分别表示：要修改的数，轮子号码，变化大小，变化方向*/
            if (Serial_RxPacket[3] == 0)
            {
                PID_Para[Serial_RxPacket[0]][Serial_RxPacket[1]] += Serial_RxPacket[2] * k;
            }
            else if (Serial_RxPacket[3] == 1)
            {
                PID_Para[Serial_RxPacket[0]][Serial_RxPacket[1]] -= Serial_RxPacket[2] * k;
            }
            

            OLED_ShowFloatNum(19, 17, Target[0], 3, 0, OLED_6X8);
            OLED_ShowFloatNum(43, 17, Target[1], 3, 0, OLED_6X8);

            OLED_ShowFloatNum(19, 25, Kp[0], 1, 2, OLED_6X8);
            OLED_ShowFloatNum(51, 25, Ki[0], 1, 2, OLED_6X8);
            OLED_ShowFloatNum(83, 25, Kd[0], 1, 2, OLED_6X8);

            OLED_ShowFloatNum(19, 33, Kp[1], 1, 2, OLED_6X8);
            OLED_ShowFloatNum(51, 33, Ki[1], 1, 2, OLED_6X8);
            OLED_ShowFloatNum(83, 33, Kd[1], 1, 2, OLED_6X8);           
        
            OLED_ShowFloatNum(19, 41, Kp[2], 1, 2, OLED_6X8);
            OLED_ShowFloatNum(51, 41, Ki[2], 1, 2, OLED_6X8);
            OLED_ShowFloatNum(83, 41, Kd[2], 1, 2, OLED_6X8);

            OLED_ShowFloatNum(19, 57, Offset[0], 1, 2, OLED_6X8);
        }
        else
        {
            osDelay(10);
        }
    }
}