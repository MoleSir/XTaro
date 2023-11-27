# tech.py 文件分析

以 scn4m_subm 为例。首先会导入 drc 模块的一些类。



## `tech_modules`

````python
tech_modules = d.module_type()
````

创建 `drc.module_type` 实例对象 ``tech_modules`，这个类很简单地封装了一个字典。所以 `tech_modules` 可以理解为就是一个字典。



## `cell_properties`

````python
cell_properties = d.cell_properties()
cell_properties.bitcell_1port.gnd_layer = "m2"
cell_properties.bitcell_1port.gnd_dir = "V"
````

创建 `drc.cell_properties` 类实例对象 `cell_propertices`。其中包含了 OpenRAM 将会使用到的一些模块的名称，模块的信息（引脚名称、引脚类型，各个引脚所在的版图层次、方向等）。

模块的名称使用一个 字典 `names` 来保存。而各个模块的信息使用 drc/custom_cell_proertes.py 中定义的一些类描述。这些类都比较简单，保存模块的信息。

在这个 tech.py 中，就把 `cell_properties` 的 `bitcell_1port` 模块的 `gnd_layer`（GND 所在的版图层次设置到 m2），方向为垂直。



## `layer_properties`

 ````python
 layer_properties = d.layer_properties()
 ````

与 `cell_properties` 类似，包含布线时候的信息。这里就全部采用默认（默认使用 `__init__` 赋值）。不做修改。



## `GDS`

```python
GDS={}
GDS["unit"]=(0.001,1e-6)
GDS["zoom"] = 0.5
```

创建一个保存 GDS 文件信息的字典 `GDS`，其中的 `uint` 对应两个值：

- the size of a database unit in user units
- the size of a database unit in meters

也就是版图文件 .gds 中 UNITS record 保存的两个值。

而 `zoom` 对应的了标签的缩放。



## 连接关系

### 各层次连接关系

````python
poly_stack = ("poly", "poly_contact", "m1")
active_stack = ("active", "active_contact", "m1")
m1_stack = ("m1", "via1", "m2")
m2_stack = ("m2", "via2", "m3")
m3_stack = ("m3", "via3", "m4")
````

例如 `poly_stack = ("poly", "poly_contact", "m1") `  表示 poly 层通过 poly_contact 与 m1 层连接。其他各个连接关系类似。

###  层次索引

````python
layer_indices = {"poly": 0,
                 "active": 0,
                 "m1": 1,
                 "m2": 2,
                 "m3": 3,
                 "m4": 4}
````

poly 与 active 为最低层次，m1、m2、m3、m4 依次增高。

### 前端与后端连接关系

````python
# The FEOL stacks get us up to m1
feol_stacks = [poly_stack,
               active_stack]

# The BEOL stacks are m1 and up
beol_stacks = [m1_stack,
               m2_stack,
               m3_stack]

layer_stacks = feol_stacks + beol_stacks
````

FEOL 表示前端、BEOL 表示表示后端。

### 各层导线方向

数电版图每层的导线方向需要尽可能一致：

```python
preferred_directions = {"poly": "V",
                        "active": "V",
                        "m1": "H",
                        "m2": "V",
                        "m3": "H",
                        "m4": "V"}
```



## Power grid

使用 M3/<4 作为 power_grid：

````python
power_grid = m3_stack
````



## GDS 文件层次映射

在 GDS 文件中，LAYER record 提供的信息只是层次序号，定义一个字典保存层次名称与层次序号的映射关系：

````python
layer={}
layer["pwell"]          = (41, 0)
layer["nwell"]          = (42, 0)
layer["active"]         = (43, 0)
layer["pimplant"]       = (44, 0)
layer["nimplant"]       = (45, 0)
layer["poly"]           = (46, 0)
layer["poly_contact"]   = (47, 0)
layer["active_contact"] = (48, 0)
layer["m1"]         = (49, 0)
layer["via1"]           = (50, 0)
layer["m2"]         = (51, 0)
layer["via2"]           = (61, 0)
layer["m3"]         = (62, 0)
layer["via3"]           = (30, 0)
layer["m4"]         = (31, 0)
layer["text"]           = (63, 0)
layer["boundary"]       = (63, 0)
````

再创建名称映射关系，给其他的 PDKs：

````python
layer_names = {}
layer_names["active"]  = "active"
layer_names["pwell"]   = "pwell"
layer_names["nwell"]   = "nwell"
layer_names["nimplant"]= "nimplant"
layer_names["pimplant"]= "pimplant"
layer_names["poly"]    = "poly"
layer_names["poly_contact"] = "poly_contact"
layer_names["active_contact"] = "active_contact"
layer_names["m1"]      = "metal1"
layer_names["via1"]    = "via1"
layer_names["m2"]      = "metal2"
layer_names["via2"]    = "via2"
layer_names["m3"]      = "metal3"
layer_names["via3"]    = "via3"
layer_names["m4"]      = "metal4"
layer_names["text"]    = "text"
layer_names["boundary"]= "boundary"
````



## DRC/LVS 规则

OpenRAM 使用 [`drc.design_rules`](./drc.md) 类对象确定一个工艺库的 DRC。

````python
drc = d.design_rules("scn4me_sub")
````

之后不断添加各种规则。规则名称是字符串，规则的值可以是数字、函数等。



## Spice 仿真参数



## 工艺工具

