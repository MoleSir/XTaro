# `ptx` 类

MOS 管子的表示原理不复杂，需要确定基本的参数：

````python
    def __init__(self,
                 name="",
                 width=drc("minwidth_tx"),
                 mults=1,
                 tx_type="nmos",
                 add_source_contact=None,
                 add_drain_contact=None,
                 series_devices=False,
                 connect_drain_active=False,
                 connect_source_active=False,
                 connect_poly=False,
                 num_contacts=None,
                 ):
````



## 网表

对 .sp 文件的表示，由于 `ptx` 也是一个 `design` 类。但由于其没有调用其他子模块，所以创建的时候只需要先设置好引脚，之后就只要设置必要的参数，不需要创建模块、实体。

当其他 .SUBCKT 需要使用到 MOS 管时，也需要先创建 `ptx` 类实例化对象，再创建 `instance` 类实例化对象表示实体。

而不同的是，当写入存在 MOS 子模块的 .SUBCKT 时，由于 MOS 管并没有 .SUBCKT 定义，根据 .sp 文件格式，只需要以 M 开头就可以表示这是 MOS，并且之后按顺序指定引脚，指出 MOS 类型即可。

这在 `sp_write_file` 中就对实体是 MOS 的进行单独的处理，不会再继续递归。



## 版图

`ptx` 也是 `design`。由于没有其他子模块（只创建了 contant），其实际版图都是由引脚、矩形对象构成。其创建过程就是根据参数绘制出 MOS 版图：衬底、注入区、有源区、栅、引脚、接触孔（连接到指定金属层）。

实际上都是给 `objs` 增加 `rectangle` 对象完成。

而当其调用 `gds_write_file` 时，也与正常的模块一样（区别于自带 .gds 问的模块），也需要动态赋值 `self.gds`，只不过其中没有 `sref`，基本都是 `boundary`。
