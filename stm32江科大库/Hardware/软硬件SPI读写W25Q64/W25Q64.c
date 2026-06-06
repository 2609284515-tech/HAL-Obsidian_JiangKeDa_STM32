#include "main.h"
#include "MySPI.h"
#include "W25Q64_Ins.h"

/**
  * @brief  读取 W25Q64 的制造商 ID (MID) 和设备 ID (DID)
  * @param  MID: 用于存储制造商 ID 的指针 (通常为 0xEF)
  * @param  DID: 用于存储设备 ID 的指针 (W25Q64 通常为 0x4017 或类似值)
  * @retval 无
  * @note   使用 JEDEC ID 指令 (0x9F)
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();					// 拉低片选，开始通信
	MySPI_SwapByte(W25Q64_JEDEC_ID);	// 发送读 JEDEC ID 指令 (0x9F)
	
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 读取制造商 ID (Manufacturer ID)
	
	// 读取设备 ID (Device ID)，共 2 字节，高位在前
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 读取高 8 位
	*DID <<= 8;									// 左移 8 位，为低 8 位腾出空间
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// 读取低 8 位并合并
	
	MySPI_Stop();					// 拉高片选，结束通信
}

/**
  * @brief  发送写使能命令
  * @note   在执行写入或擦除操作前，必须先调用此函数设置写使能锁存器 (WEL)
  * @retval 无
  */
void W25Q64_WriteEnable(void)
{
	MySPI_Start();						// 拉低片选
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);	// 发送写使能指令 (0x06)
	MySPI_Stop();						// 拉高片选，指令立即生效
}

/**
  * @brief  等待 W25Q64 空闲 (Busy 位变为 0)
  * @note   写入和擦除操作需要较长时间，必须等待操作完成才能进行下一次操作
  * @retval 无
  */
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	
	MySPI_Start();								// 拉低片选
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);	// 发送读状态寄存器 1 指令 (0x05)
	
	Timeout = 100000; // 设置超时计数，防止死循环
	// 状态寄存器最低位 (Bit 0) 为 Busy 位
	// 1 表示忙碌，0 表示空闲
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break; // 超时退出，避免程序卡死
		}
	}
	
	MySPI_Stop(); // 拉高片选，结束查询
}

/**
  * @brief  页编程 (Page Program)，即写入数据
  * @param  Address: 24 位起始地址
  * @param  DataArray: 指向要写入的数据数组的指针
  * @param  Count: 要写入的数据字节数
  * @retval 无
  * @note   W25Q64 一页大小为 256 字节。如果跨页写入，地址会自动回滚到页首，导致数据错误。
  *         因此，确保 Address + Count 不超过当前页的边界 (256 字节对齐)。
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable(); // 1. 发送写使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM); // 2. 发送页编程指令 (0x02)
	
	// 3. 发送 24 位地址 (高位先发)
	MySPI_SwapByte(Address >> 16); // 高 8 位
	MySPI_SwapByte(Address >> 8);  // 中 8 位
	MySPI_SwapByte(Address);       // 低 8 位
	
	// 4. 连续发送数据
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	
	MySPI_Stop(); // 5. 拉高片选，触发内部写入操作
	
	W25Q64_WaitBusy(); // 6. 等待写入完成
}

/**
  * @brief  扇区擦除 (Sector Erase)
  * @param  Address: 24 位起始地址 (任意位于该扇区内的地址均可)
  * @retval 无
  * @note   W25Q64 一个扇区大小为 4KB (4096 字节)。
  *         擦除会将整个扇区的所有位设置为 1 (0xFF)。
  *         写入数据前，必须确保目标区域已擦除。
  */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable(); // 1. 发送写使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB); // 2. 发送扇区擦除指令 (0x20)
	
	// 3. 发送 24 位地址 (高位先发)
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	
	MySPI_Stop(); // 4. 拉高片选，触发内部擦除操作
	
	W25Q64_WaitBusy(); // 5. 等待擦除完成 (擦除通常比写入慢得多)
}

/**
  * @brief  读取数据
  * @param  Address: 24 位起始地址
  * @param  DataArray: 用于存储读取数据的数组指针
  * @param  Count: 要读取的数据字节数
  * @retval 无
  * @note   读取操作不需要写使能，也不受页边界限制，可以连续读取任意长度。
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA); // 1. 发送读数据指令 (0x03)
	
	// 2. 发送 24 位地址 (高位先发)
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	
	// 3. 连续读取数据
	// 发送 dummy byte (0x00 或任意值) 以产生时钟信号，从而从 MISO 读取数据
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	
	MySPI_Stop(); // 4. 拉高片选，结束通信
}