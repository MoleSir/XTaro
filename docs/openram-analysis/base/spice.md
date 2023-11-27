# `spice` 类

````python
class spice():
    """
    This provides a set of useful generic types for hierarchy
    management. If a module is a custom designed cell, it will read from
    the GDS and spice files and perform LVS/DRC. If it is dynamically
    generated, it should implement a constructor to create the
    layout/netlist and perform LVS/DRC.
    Class consisting of a set of modules and instances of these modules
    """
````

很重要的基础类。



## 构造函数

````python
def __init__(self, name, cell_name):
````

传递两个参数：

- `name`：表示对象实例化后的名称（每个电路模块对象各自的名称，例如 `and2_dec_2`）；
- `cell_name`：表示这个模块类的名称（相同的电路模块有相同的模块类名称，例如 `bitcell_2rw`）；

### 赋值构造函数参数

````python
self.name = name
self.cell_name = cell_name
self.sp_file = OPTS.openram_tech + "sp_lib/" + cell_name + ".sp"
````

构造函数中，将两个名称赋值到类的字段中。并且创建 `sp_file`，在工艺库目录下找到 sp_lib 目录，再找到 `cell_name + '.sp'` 文件。（例如 `./technology/scn4m_subm/sp_lib/dff.sp`）

接下来尝试寻找 `lvs_lib` 目录，如果没有的话，`self.lvs_file = self.sp_file`。

### 创建字段

最后创建一些字段：

````python
# Holds subckts/mods for this module
self.mods = set()
# Holds the pins for this module (in order)
# on Python3.7+ regular dictionaries guarantee order too, but we allow use of v3.5+
self.pins = OrderedDict()
# An (optional) list of indices to reorder the pins to match the spice.
self.pin_indices = []
# THE CONNECTIONS MUST MATCH THE ORDER OF THE PINS (restriction imposed by the
# Spice format)
# internal nets, which may or may not be connected to pins of the same name
self.nets = {}
# If this is set, it will not output subckt or instances of this (for row/col caps etc.)
self.no_instances = False
# If we are doing a trimmed netlist, these are the instance that will be filtered
self.trim_insts = set()
````

- `mods`：每个模块对象中还会有其他的模块对象（组合模式）
- `pins`：保存此模块的引脚对象；
- `nets`：保存模块的导线对象；

### `sp_read()`

接着调用 `sp_read()` ：

````python
self.sp_read()
````

这个函数尝试读取 `self.sp_file`，如果文件存在，说明这个模块是一个基本模块，已经有 spice 模型定义。`sp_read` 步骤如下：

````python
if self.sp_file and os.path.isfile(self.sp_file):
    f = open(self.sp_file)
    self.spice = f.readlines()
    for i in range(len(self.spice)):
        self.spice[i] = self.spice[i].rstrip(" \n")
    f.close()

    # find the correct subckt line in the file
    subckt = re.compile("^.subckt {}".format(self.cell_name), re.IGNORECASE)
    subckt_line = list(filter(subckt.search, self.spice))[0]
    # parses line into ports and remove subckt
    self.add_pin_list(subckt_line.split(" ")[2:])
else:
    debug.info(4, "no spfile {0}".format(self.sp_file))
    self.spice = []
````

如果 `sp_file` 存在：

1. 打开文件，将内容逐行读取为 `list[str]` 到 `self.spice`；
2. 对所有行，去掉结尾的 ` \n`；
3. 创建一个正则表达，限定以 `".subckt {}".format(self.cell_name)` 开头，`self.cell_name` 表示模块名称；
4. 在 `self.spice` 找到满足上述正则的字符串，这一行定义了模块名称与引脚名称；
5. 从这一行取出端口名称，保存为一个列表，调用 [`self.add_pin_list`](##`add_pin_list()`)，并且使用默认类型 `INOUT`

如果不存在：

1. `self.spice` 为空列表。

接下来，如果 `self.lvs_file` 不为 `self.sp_file`，并且 `self.lvs_file` 存在，对 `self.lvs_file` 执行上述操作，否则什么也不做。



## 网表构造方法

### `add_pin_list()`

 ````python
 def add_pin(self, name, pin_type="INOUT"):
 ````

- `name`：引脚名称列表；
- `pin_type`：引脚类型，默认 `INOUT`，当然也可以是一个字符串列表，对应引脚名称列表；

````python
def add_pin_list(self, pin_list, pin_type="INOUT"):
    if isinstance(pin_type, str):
        for pin in pin_list:
            self.add_pin(pin, pin_type)

    elif len(pin_type)==len(pin_list):
        for (pin, type) in zip(pin_list, pin_type):
            self.add_pin(pin, type)
````

逻辑很简单。

### `add_pin()`

````python
def add_pin(self, name, pin_type="INOUT"):
    self.pins[name] = pin_spice(name, pin_type, self)
````

给 `self.pins` 列表增加一个键值对，键即为引脚名称，值为 `pin_spice` 对象：名称为 `name`，类型为 `pin_type`，父模块为 `self`，指向本模块，表示 `self` 创建了 `pin_spice` 对象。



## 输出网表文件

### `sp_write()`

将自身 spice 网表信息写入到指定文件中：

````python
def sp_write(self, spname, lvs=False, trim=False):
    """Writes the spice to files"""
````

- `spname`：目标文件路径；
- `lvs`、`trim`：

````python
def sp_write(self, spname, lvs=False, trim=False):
    spfile = open(spname, 'w')
    spfile.write("*FIRST LINE IS A COMMENT\n")
    usedMODS = list()
    self.sp_write_file(spfile, usedMODS, lvs=lvs, trim=trim)
    del usedMODS
    spfile.close()
````

1. 创建 `spname` 文件对象；
2. 写入 `*FIRST LINE IS A COMMENT\n`；
3. 创建空列表 `useMODS`；
4. 调用 [`self.sp_write_file`](##`sp_write_file()`) 完成写入工作；
5. 删除 `useMODS`，关闭文件；

### `sp_write_file()`

````python
def sp_write_file(self, sp, usedMODS, lvs=False, trim=False):
````

- `sp`：写入文件对象；
- `useMODS`：列表，放置已经写过的 mod，放置重复写入；
- `lvs`、`trim`。

函数执行流程为：

#### 不是一个基本模块

##### 递归子模块

即 `self.spice` 是空列表，那么这个模块内部是动态创建的，首先对其中的子模块递归调用 `sp_write_file`：

````python
for mod in self.mods:
    if self.contains(mod, usedMODS):
        continue
    usedMODS.append(mod)
    mod.sp_write_file(sp, usedMODS, lvs, trim)
````

调用之前先判断是不是已经在 `useMODS` 中，如果在跳过，否则加入。

##### 写入第一行

递归完子模块后，处理自身的输出，首先完成第一行的写入，即 `.SUBCKT` 命令

```python
# write out the first spice line (the subcircuit)
wrapped_pins = "\n+ ".join(tr.wrap(" ".join(list(self.pins))))
sp.write("\n.SUBCKT {0}\n+ {1}\n".format(self.cell_name,
                                         wrapped_pins))
```

1. 将所有引脚名称封装在一个列表中；
2. 以空格为分割将引脚名称组成一个大字符串；
3. 如果这个字符串太长，就分割为若干行，再以 `\n+` 为间隔组成大字符串 `wrapped_pins`；
4. 写入 `\n.SUBCKT ` + 模块名称 +  引脚信息 `wrapped_pins` 到文件。

##### 写引脚说明与模块注释

````python
# Also write pins as comments
for pin in self.pins.values():
    sp.write("* {1:6}: {0} \n".format(pin.name, pin.type))
for line in self.comments:
	sp.write("* {}\n".format(line))
````

##### 处理 `inst`

将内部子实体的名称与连接关系输出。那些不是基本模块，但使用了 MOS 管的 .SUBCKT 也在这个部分输出（判断 `lvs_`）

````python
if lvs and hasattr(inst.mod, "lvs_device"):
    sp.write(inst.mod.lvs_device.format(inst.name,
                                        " ".join(inst.get_connections())))
    sp.write("\n")
````

一般的情况就把 inst 信息写入：

````python
if trim and inst.name in self.trim_insts:
    wrapped_connections = "\n*+ ".join(tr.wrap(" ".join(inst.get_connections())))
    sp.write("X{0}\n*+ {1}\n*+ {2}\n".format(inst.name,
                                             wrapped_connections,
                                             inst.mod.cell_name))
else:
    wrapped_connections = "\n+ ".join(tr.wrap(" ".join(inst.get_connections())))
    sp.write("X{0}\n+ {1}\n+ {2}\n".format(inst.name,
                                           wrapped_connections,
                                           inst.mod.cell_name))
````

##### 写入 spice 子模块结尾

````python
sp.write(".ENDS {0}\n".format(self.cell_name))
````

#### 是一个基本模块

`self.spice` 中保存着模块的 spice 网表，直接以 `\n` 间隔组成字符串后写入：

````python
if lvs and hasattr(self, "lvs"):
    sp.write("\n".join(self.lvs))
else:
    sp.write("\n".join(self.spice))

sp.write("\n")
````



## 计算 delay 方法

### `cacti_delay`

````python
def cacti_delay(self, corner, inrisetime, c_load, cacti_params):
````

- `corner`：工艺角；
- `intisetime`：输入传播时间；
- `c_load`：输出负载电容；
- `cacti_params`：相关参数信息；

#### 获取电容、电阻

````python
# Get the r_on the the tx
rd = self.get_on_resistance()
# Calculate the intrinsic capacitance
c_intrinsic = self.get_intrinsic_capacitance()
# Get wire values
c_wire = self.module_wire_c()
r_wire = self.module_wire_r()
````

四个函数都是需要被子类重载的虚函数；

#### 计算延时

````python
tf = rd*(c_intrinsic+c_load+c_wire)+r_wire*(c_load+c_wire/2)
extra_param_dict = {}
extra_param_dict['vdd'] = corner[1] #voltage is second in PVT corner
extra_param_dict['load'] = c_wire+c_intrinsic+c_load #voltage is second in PVT corner
this_delay = self.cacti_rc_delay(inrisetime, tf, 0.5, 0.5, True, extra_param_dict)
inrisetime = this_delay / (1.0 - 0.5)
return delay_data(this_delay, inrisetime)
````

1. 获取输入传播时间 $inrisetime$，输出负载电容 $c_{load}$

1. 获取 $r_d$、$c_{intrinsic}$、$c_{wire}$、$r_{wire}$；

2. 计算 $t_f$：
   $$
   tf = r_d * (c_{intrinsic} + c_{load} + c_{wire}) + r_{wire} * (c_{load} + c_{wire}/2)
   $$
   其中 $r_{wire} * (c_{load} + c_{wire}/2)$ 为互连线延时。
   
4. 计算 $delay$：
   $$
   delay = tf * \sqrt{ln(0.5) * ln(0.5) + (inrisetime/tf) * (0.5)}
   $$

5. 计算 `slew`：
   $$
   slew = 2 * delay
   $$

### `analytical_delay`

```python
def analytical_delay(self, corner, slew, load=0.0):
```

- `corner`：工艺角；
- `intisetime`：输入传播时间；
- `c_load`：输出负载电容；
