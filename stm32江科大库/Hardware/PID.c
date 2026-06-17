#include "main.h"
#include "PID.h"
#include <math.h>

//定义一个极小值，用于浮点数比较，避免直接比较0带来的精度问题
#define EPSILON 1e-6f 

/**
  * @brief  PID控制计算函数
  * @param  Actual: 实际值数组指针 (例如当前速度或位置)
  * @param  Target: 目标值数组指针
  * @param  Output: 输出值数组指针 (计算后的PWM占空比或控制量)
  * @param  Kp, Ki, Kd: P、I、D系数数组指针
  * @param  PIDStructure: PID结构体数组指针，用于存储历史误差和积分累计值
  * @note   该函数同时处理2个通道的PID计算
  */
void PID_Fun(int16_t *Actual, float *Target, float *Output, 
             float *Kp, float *Ki, float *Kd, PID_TypeDefVar *PIDStructure)
{
    uint8_t i;

    // 循环处理2个独立的PID通道 (例如左轮和右轮，或X轴和Y轴)
    for (i = 0; i < 2; i++)
    {
        // 1. 更新误差历史
        PIDStructure[i].Error1 = PIDStructure[i].Error0; // 将上一次的误差保存为前一次误差 (用于微分项)
        
        // 计算当前误差：目标值 - 实际值
        PIDStructure[i].Error0 = Target[i] - Actual[i];
        
        /* 2. 积分项处理 */
        // 判断是否启用了积分作用 (Ki不为0)
        if (fabsf(Ki[i]) > EPSILON)
        {
            // 累加当前误差到积分项
            PIDStructure[i].ErrorInt += PIDStructure[i].Error0;
        }
        else
        {
            // 如果Ki为0，则清零积分项，防止残留值影响
            PIDStructure[i].ErrorInt = 0;
        }

        /* 3. 积分限幅 (Anti-windup) */
        // 限制积分项产生的贡献值在 [-100, 100] 之间，防止积分饱和导致系统响应变慢或超调过大
        // 注意：这里限制的是 Ki * ErrorInt 的结果范围
        if (PIDStructure[i].ErrorInt * Ki[i] > 100) 
        {
            PIDStructure[i].ErrorInt = 100 / Ki[i];
        }
        if (PIDStructure[i].ErrorInt * Ki[i] < -100) 
        {
            PIDStructure[i].ErrorInt = -100 / Ki[i];
        }
        
        /* 4. PID 公式计算 */
        // Output = Kp*当前误差 + Ki*积分误差 + Kd*(当前误差 - 上次误差)
        Output[i] = Kp[i] * PIDStructure[i].Error0 + 
                    Ki[i] * PIDStructure[i].ErrorInt + 
                    Kd[i] * (PIDStructure[i].Error0 - PIDStructure[i].Error1);
        
        /* 5. 输出限幅 */
        // 限制最终输出值在 [-100, 100] 范围内，通常对应PWM的占空比百分比或其他执行器的物理极限
        if (Output[i] > 100) 
        {
            Output[i] = 100;
        }
        if (Output[i] < -100) 
        {
            Output[i] = -100;
        }
    }
}