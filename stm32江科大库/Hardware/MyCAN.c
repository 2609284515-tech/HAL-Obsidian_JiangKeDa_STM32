#include "main.h"

// 声明外部定义的 CAN 句柄，通常在 main.c 或 can.c 中由 CubeMX 生成
extern CAN_HandleTypeDef hcan;

/**
  * @brief  初始化 CAN 过滤器并启动 CAN 外设
  * @note   配置过滤器以接收所有消息 (掩码全0)，并启用 FIFO0
  * @retval 无
  */
void MyCAN_Init(void)
{
	CAN_FilterTypeDef CAN_FilterInitStructure;

	// 配置 CAN 过滤器
	CAN_FilterInitStructure.FilterBank = 0;				// 使用过滤器组 0
	CAN_FilterInitStructure.FilterIdHigh = 0x0000;		// 过滤器 ID 高 16 位
	CAN_FilterInitStructure.FilterIdLow = 0x0000;		// 过滤器 ID 低 16 位
	CAN_FilterInitStructure.FilterMaskIdHigh = 0x0000;	// 过滤器掩码高 16 位 (0x0000 表示不关心任何位，即接收所有 ID)
	CAN_FilterInitStructure.FilterMaskIdLow = 0x0000;	// 过滤器掩码低 16 位
	CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT; // 设置过滤器规模为 32 位
	CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;  // 设置过滤器模式为 标识符掩码模式
	CAN_FilterInitStructure.FilterFIFOAssignment = CAN_RX_FIFO0; // 将过滤后的消息存入 FIFO0
	CAN_FilterInitStructure.FilterActivation = ENABLE;	// 激活过滤器
	
	// 应用过滤器配置
	HAL_CAN_ConfigFilter(&hcan, &CAN_FilterInitStructure);

	// 启动 CAN 外设，开始参与总线通信
	HAL_CAN_Start(&hcan);
}

/**
  * @brief  发送 CAN 消息
  * @param  TxMessage: 指向发送消息头结构的指针 (包含 ID, DLC, RTR 等信息)
  * @param  Data: 指向要发送的数据数组的指针 (最多 8 字节)
  * @retval 无
  * @note   此函数为阻塞式发送，会等待直到消息成功放入发送邮箱或超时
  */
void MyCAN_Transmit(CAN_TxHeaderTypeDef *TxMessage, uint8_t *Data)
{
    uint32_t TxMailbox;

    // 将消息添加到发送邮箱，准备发送
    // TxMailbox 将返回被使用的邮箱编号 (0, 1, 或 2)
    HAL_CAN_AddTxMessage(&hcan, TxMessage, Data, &TxMailbox);

    // 等待消息发送完成
    uint32_t timeout = 100000;
    // 检查指定邮箱中的消息是否仍在等待发送
    while (HAL_CAN_IsTxMessagePending(&hcan, TxMailbox))
    {
        if(timeout-- == 0) 
        {
            break; // 超时退出，防止死锁
        }
    }
}

/**
  * @brief  检查 CAN 接收 FIFO 中是否有数据
  * @retval 1: FIFO 中有数据, 0: FIFO 为空
  */
uint8_t MyCAN_ReceiveFlag(void)
{
    // 获取 RX FIFO0 中的挂起消息数量
    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0)
    {
        return 1; // 有数据
    }
    return 0;     // 无数据
}

/**
  * @brief  从 CAN 接收 FIFO 中读取消息
  * @param  RxMessage: 指向接收消息头结构的指针 (用于存储 ID, DLC, RTR 等信息)
  * @param  Data: 指向数据缓冲区的指针 (用于存储接收到的数据)
  * @retval 无
  * @note   调用此函数前建议先调用 MyCAN_ReceiveFlag 确认有数据，否则可能阻塞或返回错误
  */
void MyCAN_Receive(CAN_RxHeaderTypeDef *RxMessage, uint8_t *Data)
{
	// 从 RX FIFO0 获取消息
	HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, RxMessage, Data);
}