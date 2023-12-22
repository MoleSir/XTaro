# TSMC 生成的 Spice 文件分析

## 模块

### LEAFCELL

- MCB：6T SRAM 基本单元；
- MCBM2：由四个 MCB 组成的，两条 WL，两条 BL 的 Bitcell 阵列；
- XDEL_SAE：
- DIOA：二极管；
- IO_PULLDOWN：

### LOGIC

- AINV：反相器；
- INV_BULK：反相器；
- INV_BULK_SVT：反相器；
- NOR3_BULK_SVT：三输入或非门；
- NOR_BULK：两输入或非门；
- NOR_BULK_SVT：两输入或非门；