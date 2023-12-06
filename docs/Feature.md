#  Compiler Feature

> 先完成 spice 网表 + .lib 时序信息。完成后增加 .lef、.gds 等版图信息。

输入用户配置信息文件，配合工艺库，输出 SRAM 的 spice 网表文件，以及不同 PVT 信息下的时序 .lib 文件。



## 一、程序部分

程序需要读取用户配置文件，配合以及工艺库完成：

- 根据标准单元生成 SRAM 整体的 spice 网表文件。
- 产生不同 PVT 条件下的时序库 .lib 文件。

### 1.1 生成 SRAM 的具体电路

输出 SRAM 的结构（TODO）：

软件如何产生电路（TODO）：

### 1.2 .lib 文件的生成

目前需要仿真的 .lib 文件信息：

- 输出引脚相对 CLK 的 delay（传播延时）与 transition （输出传播时间）：

  - cell_rise：输出引脚从 0 到 1 的 delay time
  - cell_fall：输出引脚从 1 到 0 的 delay time
  - rise_transition：输出引脚从 0 到 1 的 transition time
  - fall_transition：输出引脚从 1 到 0 的  transition time

  这些信息可以直接通过施加合适的 spice 激励 + 调用 .meas（spice 测量命令） 测量。例如指定测量从 CLK 上升沿中点开始，到输出信号上升沿中点结束的时间差，得到 cell_rise。

- 输入引脚相对 CLK 的 setup/hold time （建立/保持时间）信息。每个输入引脚都需要四个信息：

  - setup rise：输入引脚上升沿的 setup time
  - setup fall：输入引脚下降沿的 setup time
  - hold rise：输入引脚上升沿的 hold time
  - hold fall：输入引脚下降沿的 hold time

  这些信息只需要对标准单元的 dff（寄存器） 模块测量即可。因为输入信号总是会先经过一级寄存器缓存。

  不同与 delay 信息，setup/hold 无法直接测量。这里参考 OpenRAM 的实现方式：例如对 setup rise，通过不断缩短 输入引脚上升沿（数据到达） 到 CLK 上升沿（采样时刻）的时间差，计算得到某个时间间隔：使得信号恰好可以被采样。



## 二、工艺库相关

生成一个具体的 SRAM 必须提供特定的工艺库，目前需要工艺库的以下信息：

### 2.1 model 文件

不同工艺角下（TT、SS、FF）的 model 的 .sp 描述。

在 SRAM 的 spice 网表中，不需要指定出具体的 MOS 管参数，仅需要提供 MOS 管名称。但进行 spice 仿真时，就需要确定的 MOS 管信息，即 model 文件，使用 .sp 格式给出。

### 2.2 基本单元

软件采用模块化的设计，使用基本单元，搭建出整个 SRAM 电路。所以工艺库需要给出一些基本单元的 .sp 描述，以下为 OpenRAM 工艺库中提供的基本单元：

- bitcell
- dff
- sense amp
- tri gate
- writ driver

### 2.3 工艺信息

除了以上的 spice 文件，工艺库还需要提供其他额外的信息，主要用于仿真：

- PVT：用来选择 model、指定 spice 仿真时给定的电压、温度。
- 输入引脚的过渡时间 slew：用于 spice 仿真，以及生成 .lib 文件中的 NLDM 表格查找。
- 输出引脚的负载电容 load：用于 spice 仿真，以及生成 .lib 文件中的 NLDM 表格查找。
- 时钟频率：用于 spice 仿真。

 

## 三、TODO

- 用户输入配置信息使用何种格式。
- 工艺库信息使用何种格式。
- 需要给定哪些基本单元。
- SRAM 中的具体电路实现；