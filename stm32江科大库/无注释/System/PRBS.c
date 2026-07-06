#include "main.h"

// 定义 LFSR 相关变量
static uint8_t lfsr = 0x7F; // 初始种子
static uint8_t prbs_bit;

// 这个函数每次调用就产生一个PRBS的位 (0或1)
uint8_t get_prbs_bit(void) {
    // 7阶本原多项式：x^7 + x^6 + 1
    // 反馈位 = bit7 XOR bit6  (注意位编号，从1开始或从0开始，这里用从LSB为bit0)
    uint8_t feedback = ((lfsr >> 6) ^ (lfsr >> 5)) & 0x01; // 取bit6和bit5 (对应7位寄存器的第7和第6位)
    
    lfsr = (lfsr << 1) | feedback;  // 左移一位，最低位补入反馈
    lfsr &= 0x7F;                   // 保持7位掩码

    return feedback; // 返回移出的最高位(即原来的bit6)，作为随机位
}