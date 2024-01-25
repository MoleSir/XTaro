# Memory Compiler

## Report 1

- 阅读 *Embedded Memories for Nano-Scale VLSIs*，学习内存相关知识、SRAM、DRAM 基本原理与组成结构；
- 下载 OpenRAM 源码，阅读官方文档，使用 OpenRAM 输出 SRAM；
- 了解 OpenRAM [输出文件](../reference/OpenRAM/ouput)：.sp、.gds、.lef、.lib、.v 的基本内容；
- 观察 OpenRAM 源码基本结构，绘制[基本类关系](./openram-analysis/base/类图.drawio)，了解 OpenRAM 执行的基本流程；
- 学习 .sp 文件格式，使用 LTSpice 绘制 OpenRAM 输出得到的 sram.sp 文件，观察 OpenRAM 产生的 [SRAM 电路结构](./openram-analysis/spice-file)；
- 阅读 OpenRAM 源码，学习其[如何构造](./openram-analysis/base/网标结构类关系图.drawio) spice 网格关系，[如何输出](./openram-analysis/base/网标结构与版图构造.md) .sp 文件；
- b 站学习版图与数字 IC 后端知识；
- 学习 [.gds 文件格式](https://www.rulabinsky.com/cavd/text/chapc.html)，使用 python 编写简单的 gds 文件读取、写入模块；
- 阅读 OpenRAM  源码，学习其如何构造版图结构，[如何输出](./openram-analysis/base/网标结构与版图构造.md) .gds 文件；
- .lef 与 .v 文件的创建原理与过程；
- MOS 管本身如何[构建与表示](./openram-analysis/base/ptx.md)（版图 & 网标）；



## Report 2

-  输出文件[详细分析](./openram-analysis/09-s.save().md)；
-  OpenRAM  characterizer 模型[类图](./openram-analysis/characterizer/类图.drawio)；
-  .lib 文件内容；
-  OpenRAM  `lib` 类[创建 .lib 文件](./openram-analysis/characterizer/lib工作流程.drawio)的流程与大致原理；
-  OpenRAM  OPTS [参数](./openram-analysis/globals/options.md)；



## Report 3

- [*Static Timing Analysis for Nanometer Designs:A Practical Approach*](https://zhuanlan.zhihu.com/p/345536827)：静态时序分析基本概念、计算延时原理、.lib 文件内容（功率）；
- 深入 OpenRAM characterizer 模块：setup_hold 计算原理；
- tsmc 简单使用；
- 基本代码框架：log、gdsparser、ngspice 仿真、setup hold 计算测试；



## Report 4

- 初步 [Feature](./Feature.md)
- json 解析模块；
- OpenRAM 的 [SRAM 电路实现](./openram-analysis/电路实现.md)：时序、dummy、replica
- 简化 OpenRAM 电路，测试。



## Report 5

- tsmc 产生的 SRAM 电路；
- *CMOS SRAM Circuit Design and Parametric Test in Nano-Scaled Technologies*，学习 SRAM 具体电路实现，特别是 replica 逻辑控制原理；
- 使用 replica 方法进行读写控制的 16x4 [SRAM](../circuit/ref-openram)，[文档](./sram-circuit/SRAM电路实现.md)；



## Report 6

- 修复 BLB、BL 需要外加电容的问题；
- 学习 MBIST，实现 March C- 算法；
- DAC review；
- 更新 SRAM [逻辑控制电路结构](../circuit/ref-tsmc)，[文档](./sram-circuit/SRAM电路实现.md)；



## Report 7

- tsmc SRAM 网标
- tsmc SRAM verilog



## Report 8

- 软件[框架](./Framework.md)
- 电路模块[类图](./framework/circuit/电路类图.drawio) 
- 若干模块



## Report 9

- 生成 SRAM 网表（调试中）
- 生成 Verilog
- Function 功能验证
- MBIST ？
- 获取 SPICE 仿真结果 ？