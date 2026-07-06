#include "main.h"
#include "cmsis_os2.h"
#include "Motor.h"
#include "OLED.h"
#include <stdint.h>
#include "Serial.h"
#include "PID.h"
#include "MPU6050.h"
#include "IC.h"
#include "PRBS.h"

float Encoder_Speed[2], Encoder_Position[2];

extern float Target[2], Kp[3], Ki[3], Kd[3], Offset[3];

void StartPWMTask(void *argument)
{
    float PWM[2], PWM_Delta, PWM_Aver, Speed_Delta_Actual, Speed_Aver_Actual;

    float Angle_Target;
    uint8_t PID_StartFlag = 1;

    PID_TypeDefVar PID_TypedefVarStucture_1,
                   PID_TypedefVarStucture_2,
                   PID_TypedefVarStucture_3;

    OLED_ShowString(0, 9, "PW:", OLED_6X8);
    for (;;)
    {
        Encoder_Speed[0] = Encoder_Get_1();
        Encoder_Speed[1] = Encoder_Get_2();
        Encoder_Reset();
        Speed_Delta_Actual = Encoder_Speed[1] - Encoder_Speed[0];
        Speed_Aver_Actual = (Encoder_Speed[1] + Encoder_Speed[0]) / 2.0;


        Encoder_Position[0] += Encoder_Speed[0];
        Encoder_Position[1] += Encoder_Speed[1];

        
        OLED_ShowFloatNum(19, 0, Encoder_Speed[0], 3, 0, OLED_6X8);
        OLED_ShowFloatNum(43, 0, Encoder_Speed[1], 3, 0, OLED_6X8);

        // OLED_ShowFloatNum(19, 41, Encoder_Position[0], 3, 0, OLED_6X8);
        // OLED_ShowFloatNum(43, 41, Encoder_Position[1], 3, 0, OLED_6X8);

        

        if (AngleGyroX > 50 || AngleGyroX < -50)
        {
            PID_StartFlag = 0;
        }

        float PRBS;
        if (PID_StartFlag == 1)
        {
            PID_Fun(Speed_Aver_Actual, Target[0], &Angle_Target,
                 Kp[1], Ki[1], Kd[1],
                  20, -1, Offset[1], &PID_TypedefVarStucture_1);
            PID_Fun(AngleGyroX, Angle_Target, &PWM_Aver,
                 Kp[0], Ki[0], Kd[0],
                  100, 1, Offset[0], &PID_TypedefVarStucture_2);
            PID_Fun(Speed_Delta_Actual, Target[1], &PWM_Delta,
                 Kp[2], Ki[2], Kd[2],
                  100, 1, Offset[2], &PID_TypedefVarStucture_3);

            // //prbs周期
            // static uint8_t clock_counter = 0;
            // if (clock_counter == 0) 
            // {
            //     PRBS = 20.0 * (get_prbs_bit() - 0.5) * 2;
            // }
            
            // clock_counter++;
            // if (clock_counter >= 5)
            // {
            //     clock_counter = 0;
            // }
            
            // PWM_Aver += PRBS; // 添加PRBS扰动信号

            PWM[0] = PWM_Aver - PWM_Delta / 2;
            PWM[1] = PWM_Aver + PWM_Delta / 2;

            if (PWM[0] > 100) {PWM[0] = 100;}
            else if (PWM[0] < -100) {PWM[0] = -100;}
            if (PWM[1] > 100) {PWM[1] = 100;}
            else if (PWM[1] < -100) {PWM[1] = -100;}
        }
        else 
        {
            PWM[0] = 0;
            PWM[1] = 0;
        }        

        Motor_SetSpeed_1(PWM[0]);
        Motor_SetSpeed_2(PWM[1]);

        // OLED_ShowFloatNum(19, 9, PWM[0], 3, 0, OLED_6X8);
        // OLED_ShowFloatNum(43, 9, PWM[1], 3, 0, OLED_6X8);

        // Serial_SendSignedNumber(Speed_Delta_Actual, 3);
        // Serial_SendString(",");
        // Serial_SendSignedNumber(Target[1], 3);
        // Serial_SendString("\n");

        // Serial_SendFloatNumber(AngleGyroX);
        // Serial_SendFloatNumber(PWM_Aver);
        // Serial_SendTail();

        OLED_Update();

        osDelay(10);
    }
}


