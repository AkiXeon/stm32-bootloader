# Git-Learn 仓库说明

本仓库主要用于学习使用 Git 工具，同时存放了一个用于 STM32F103C8 的 Bootloader 程序。

##  📁 仓库目录结构
```bash
├── main/                     # Bootloader 主程序
├── program in A sector/      # 应用程序示例
├── 串口示例程序/             # 串口通信独立示例
├── README.md                 # 项目说明文档
└── keilkill.bat              # 编译中间文件清理脚本
```

## 🧩 Bootloader 程序说明

内部 Flash 划分为四个区域：
```shell
B (20KB) → A (28KB) → C (15KB) → D (1KB)
```
功能实现
B区：存放 Bootloader 程序 
A区：存放用户应用程序 其余分区暂未使用(main.h 中定义)

目前实现的命令行功能：

- [1] A区擦除
- [2] 串口 IAP 下载
- [7] 系统重启

## 💻 应用程序示例
位于 program in A sector/ 目录 已经配置写入地址，可直接烧写
同时本仓库提供可直接烧写的测试用 .bin 文件

## 📡 串口通信示例
位于 串口示例程序/ 目录：存放独立串口功能供学习
核心功能：
USART 空闲中断处理
DMA 数据搬运
环形缓冲区实现

## ⚙️ 实用工具
keilkill.bat：一键清除编译生成的中间文件

💡 提示：所有程序均针对 STM32F103C8 开发板优化，可直接用于学习或二次开发。
