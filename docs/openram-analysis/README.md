# Memory Compiler

## Done

### Report 1

- 阅读 *Embedded Memories for Nano-Scale VLSIs*，学习内存相关知识、SRAM、DRAM 基本原理与组成结构；
- 下载 OpenRAM 源码，阅读官方文档，使用 OpenRAM 输出 SRAM；
- 了解 OpenRAM [输出文件](../../reference/OpenRAM/ouput)：.sp、.gds、.lef、.lib、.v 的基本内容；
- 观察 OpenRAM 源码基本结构，绘制[基本类关系](./base/类图.drawio)，了解 OpenRAM 执行的基本流程；
- 学习 .sp 文件格式，使用 LTSpice 绘制 OpenRAM 输出得到的 sram.sp 文件，观察 OpenRAM 产生的 SRAM 电路结构；
- 阅读 OpenRAM 源码，学习其[如何构造](./base/网标结构类关系图.drawio) spice 网格关系，[如何输出](./base/网标结构与版图构造.md) .sp 文件；
- b 站学习版图与数字 IC 后端知识；
- 学习 [.gds 文件格式](https://www.rulabinsky.com/cavd/text/chapc.html)，使用 python 编写简单的 gds 文件读取、写入模块；
- 阅读 OpenRAM  源码，学习其如何构造版图结构，[如何输出](./base/网标结构与版图构造.md) .gds 文件；
- .lef 与 .v 文件的创建原理与过程；
- MOS 管本身如何[构建与表示](./base/ptx.md)（版图 & 网标）；

### Report 2

-  输出文件[详细分析](./09-s.save().md)；
-  characterizer 模型[类图](./characterizer/类图.drawio)；
-  .lib 文件内容；
-  `lib` 类[创建 .lib 文件](./characterizer/lib工作流程.drawio)的流程与大致原理；
-  OPTS 所有[参数](./globals/options.md)；



## Doing

- B 站学习 *VLSI CAD*；
- 阅读 *Static Timing Analysis for Nanometer Designs:A Practical Approach*，静态时序分析；
- 阅读 characterizer 源码，如何仿真得到 delay 等参数；
