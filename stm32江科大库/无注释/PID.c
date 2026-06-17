#include "main.h"
#include "PID.h"
#include <math.h>

#define EPSILON 1e-6f 

void PID_Fun(int16_t *Actual, float *Target, float *Output, 
             float *Kp, float *Ki, float *Kd, PID_TypeDefVar *PIDStructure)
{

    uint8_t i;

    for (i = 0; i < 2; i++)
    {
        PIDStructure[i].Error1 = PIDStructure[i].Error0;
        PIDStructure[i].Error0 = Target[i] - Actual[i];
        
        /*是否开启积分项*/
        if (fabsf(Ki[i]) > EPSILON)
        {
            PIDStructure[i].ErrorInt += PIDStructure[i].Error0;
        }
        else
        {
            PIDStructure[i].ErrorInt = 0;
        }

        /*积分限幅*/
        if (PIDStructure[i].ErrorInt * Ki[i] > 100) {PIDStructure[i].ErrorInt = 100 / Ki[i];}
        if (PIDStructure[i].ErrorInt * Ki[i] < -100) {PIDStructure[i].ErrorInt = -100 / Ki[i];}
        
        Output[i] = Kp[i] * PIDStructure[i].Error0 + Ki[i] * PIDStructure[i].ErrorInt + Kd[i] * (PIDStructure[i].Error0 - PIDStructure[i].Error1);
        
        if (Output[i] > 100) {Output[i] = 100;}
        if (Output[i] < -100) {Output[i] = -100;}
    }
}

