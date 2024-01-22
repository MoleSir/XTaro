# Framework

## [app](./app/README.md)

### xtaro

利用其他各模块，解析参数，运行程序。

### main.cc

程序入口。



## [global](./global/README.md)

### allocator

堆内存分配器，除了 std 中所用到的堆内存之外，其他都使用自定义的分配器进行内存的分配与释放。

### config

- Option：解析用户给定是选项文件；
- Tech：解析指定工艺库信息；

### exception

程序中会使用到的基本异常，继承自 `std::exception`。程序中可以直接使用 `std::exception` 类接受所有的异常类型。

### stringpool

由于 spice 网标中经常出现重复的端口或网线名称。实现 StringPool 类保存 std::string，当需要获得一个字符串时，向 StringPool 申请，如果已经存在，返回已经存在的 std::string 的 c 风格指针（使用一个简单的 String 类经过包装）。

### debug

- Logger：日志；
- Console：命令行输出；

### util

存在常用的一些工具函数。例如字符串格式化、文件相关操作等。



## [circuit](./circuit/README.md)

### base

存放表示各个电路模块所需要的类：Circuit、Instance、Net、Port；

### module

存放各个具体的模块类，继承自 Circuit

### factory

总是使用 CircuitFactory 创建各个模块类的对象



## [parse](./parse/README.md)

各种文件的解析模块：gdsii、json、lib 等；



## [character](./character/README.md)

### command

SPICE 的 .meas 命令的表示，方便产生 .meas 测量与获取结果。

### simulator

- Simulator：产生 spice 激励与测量命令；
- SRAMSimulator：产生针对 SRAM 的 spice  激励与测量命令；

### character

- FunctionSimulator：使用 SRAMSimulator 对 SRAM 的功能进行验证；
- DelaySimulator：使用 SRAMSimulator 测量 SRAM 的 delay 信息；
- SetupHoldSimulator：使用 SRAMSimulator 测量 SRAM 的 setup 与 hold 信息；

