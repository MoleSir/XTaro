# `s = sram()`

导入 `sram`，并且创建实例化：

````python
from openram import sram
s = sram()
````



## `sram` 类

````python
class sram():
    """
    This is not a design module, but contains an SRAM design instance.
    It could later try options of number of banks and organization to compare
    results.
    We can later add visualizer and other high-level functions as needed.
    """
````

这个类仅仅是一个接口，其中会包含一个 SRAM 设计对象，由它完成工作。

这个类主要完成：根据配置信息，产生输出文件、输出日志、实例化各类对象完成工作。



## 构造函数

### 设置用户配置信息

````python
````

### 创建 `sram` 模型

`sram` 类构造函数最重要的部分就是创建出一个 `sram_1bank` 模型对象：

````python
from openram.modules.sram_1bank import sram_1bank as sram

self.s = sram(name, sram_config)

self.s.create_netlist()
if not OPTS.netlist_only:
    self.s.create_layout()
````

创建对象后将其保存在 `self.s` 中。再调用 `s.create_netlist` 创建网标、`s.create_layout` 创建版图，将需要的 SRAM 结构保存在 python 对象中。
