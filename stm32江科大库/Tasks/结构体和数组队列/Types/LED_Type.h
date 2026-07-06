#ifndef __LED_TYPE_H
#define __LED_TYPE_H

typedef enum
{
    LED_Color_Red = 0,
    LED_Color_Green = 1,
    LED_Color_Blue = 2
} LED_Color_Type;

typedef enum
{
    LED_State_Off = 0,
    LED_State_On = 1
} LED_State_Type;

typedef struct
{
    LED_Color_Type LED_Color;
    LED_State_Type LED_State;
} LED_MsgStructure;

#endif