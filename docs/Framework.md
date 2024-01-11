# Framework

## app

存放 main 函数的模块



## global

### allocator

堆内存分配器，除了 std 中所用到的堆内存之外，其他都使用自定义的分配器进行内存的分配与释放。

### config

存放配置 Memory Compiler 的配置信息

### exception

程序中会使用到的基本异常，继承自 `std::exception`。程序中可以直接使用 `std::exception` 类接受所有的异常类型。

### log

日志模块

### tech

工艺库模块，将从 config 中指定的文件夹中导入工艺库信息到程序中。

### util

存在常用的一些工具函数。例如字符串格式化等。



## circuit

### base

存放表示各个电路模块所需要的类：Circuit、Instance、Net、Port；

### module

存放各个具体的模块类，继承自 Circuit

### factory

总是使用 CircuitFactory 创建各个模块类的对象



## parse

各种文件的解析模块：gdsii、json、lib 等；



## character

### simulator

spice 的 .meas 命令的表示、产生 spice 激励与测量文件

### delay

利用 simulate 模块测量电路的 delay 信息

### setuphold

利用 simulate 模块测量电路的 setup 与 hold 信息

### function

利用 simulate 模块堆电路的功能进行验证

