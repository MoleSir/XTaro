# `instance`

> An instance of a module with a specified location, rotation, spice pins, and spice nets

具有指定位置、旋转、spice 引脚和网络的模块实例。继承自 [geometry](./geometry.md)。因为实体电路应该是有图形的。



## `__init__`

`````python
def __init__(self, name, mod, offset=[0, 0], mirror="R0", rotate=0):
`````

- `name`：实体名称。.sp 文件中 .SUBCKT 中使用子模块时候的名称（Xname ...）；
- `mod`：模型类实例对象，指向一个 特定的 .SUBCKT 或者说 structure；
- `offset`：实体位置偏移；
- `mirror`：实体的镜像类型；
- `rotate`：实体的旋转角度。

### 基类 `geometry` 构造

````python
super().__init__()
````

其中默认 `self.width` 与 `self.height` 为 0。

### 赋值基本信息

````python
self.name = name
self.mod = mod
self.gds = mod.gds
self.rotate = rotate
self.offset = vector(offset).snap_to_grid()
self.mirror = mirror
self.connected = False
````

其中 `self.offset` 是 [`vector`](./vector.md) 实例，需要先 `snap_to_grid` 转为标准网格的整数倍。

### 赋值引脚与网标

````python
self.spice_pins = copy.deepcopy(self.mod.pins)
self.spice_nets = copy.deepcopy(self.mod.nets)
for pin in self.spice_pins.values():
    pin.set_inst(self)
for net in self.spice_nets.values():
    net.set_inst(self)
````

从 `mod` 中深拷贝得到 `self.spice_pins` 与 `self.spice_nets`。详见[`spice_pin` 与 `spice_net`](./pin_spice&net_spice.md)。

因为实体的连接关系需要修改，所以深拷贝，再把引脚、网格对象的 `inst` 指向自身。

### 赋值长度、宽度

````python
if OPTS.netlist_only:
    self.width = 0
    self.height = 0
else:
    if mirror in ["R90", "R270"] or rotate in [90, 270]:
        self.width = round_to_grid(mod.height)
        self.height = round_to_grid(mod.width)
    else:
        self.width = round_to_grid(mod.width)
        self.height = round_to_grid(mod.height)
self.compute_boundary(offset, mirror, rotate)
````

根据镜像或者旋转类型，将 `mod` 的长、宽赋值给实体的 `width`，`height`（继承自 `geometry`）。`mod` 的长、宽继承自 `layout` 基类，在 `mod` 构造过程中被赋值。

最后调用 `compute_boundary` 根据实体自身长、宽，以及位置、旋转信息，确定自身的边界 `boundary`。



## `get_blockages`

> Retrieve blockages of all modules in this instance.
>
> Apply the transform of the instance placement to give absolute blockages.

检索此实体中所有模块的 blockage（可以理解为矩形边界？） 。

一个 实体 中的 `mod` 指向一个具体的模块，这个模块其自己的图形对象（例如 `rectangle`、`label`  等），而模块自己还有若干实体，这些实体也重复这个过程，一直找到最小的子模块（例如管子、图形等））

这样树形递归调用下去

### 计算旋转参数

根据实体本身的信息计算偏移、旋转信息。因为从实体的 `mod` 得到的图形信息都是相对坐标，但一旦有了实体，模块图形的坐标都需要作处理，才得到这个实体相对实体所在模块的坐标：

````python
def get_blockages(self, lpp, top=False):
    angle = math.radians(float(self.rotate))
    mirr = 1
    if self.mirror == "R90":
        angle += math.radians(90.0)
    elif self.mirror == "R180":
        angle += math.radians(180.0)
    elif self.mirror == "R270":
        angle += math.radians(270.0)
    elif self.mirror == "MX":
        mirr = -1
    elif self.mirror == "MY":
        mirr = -1
        angle += math.radians(180.0)
    elif self.mirror == "XY":
        mirr = 1
        angle += math.radians(180.0)
    ...
````

### 获取模块的 `blockages` 并处理

````python
new_blockages = []
if self.mod.is_library_cell:
    blockages = self.mod.gds.getBlockages(lpp)
    for b in blockages:
        new_blockages.append(self.transform_coords(b, self.offset, mirr, angle))
else:
    blockages = self.mod.get_blockages(lpp)
    for b in blockages:
        new_blockages.append(self.transform_coords(b, self.offset, mirr, angle))
return new_blockages
````

会调用 `self.mod` 的 [`getBlockages`](./layout.md) 得到这个实体对应模块的所有图形，再根据实体的位置信息进行处理，最后返回。

而如果 `mod` 是一个 有 .gds 文件的电路。那么在其构造函数中会读取得到一个[版图对象](../gdsMill/VlsiLayout.md) 到 `self.gds` 中，保持版图信息。如果是这样就调用 `self.mod.gds` 的 `getBlockages`。

返回值是：一个列表，每个元素是使用 `[ll_pos, ur_pos]` 表示的图形。



## `gds_write_file`

````python
def gds_write_file(self, new_layout):
    self.mod.gds_write_file(self.gds)
    new_layout.addInstance(self.gds,
                           self.mod.cell_name,
                           offsetInMicrons=self.offset,
                           mirror=self.mirror,
                           rotate=self.rotate)
````

递归写 gds 文件。

实际上是递归处理 `new_layout`，对这个[版图对象](../gdsMill/VlsiLayout.md) 不断添加信息，最后一次性写入。

对一个实体的版图写入，首先需要调用其 `self.mod` 的 `gds_write_file`，将创建一个 `GDSstructure` 到 `new_layout` 中，表示新电路。（类比，.sp 文件，需要使用实体时，需要保证定义实体电路的 .SUBCKT 先存在！）。

最后因为实体只是引用，所以调用 `new_layout.addInstance` 给当前 `structure` 创建出一个 `sref` record，表示 structure 引用。



## `place`

> This updates the placement of an instance

更新实体的位置信息。这个方法很重要，创建一个 strcuture 时，会根据算法得到其中每个实体（子电路）拜访的位置。使用的就是 `place` 函数。

````python
def place(self, offset, mirror="R0", rotate=0):
    self.offset = vector(offset).snap_to_grid()
    self.mirror = mirror
    self.rotate = rotate
    self.update_boundary()
````

确定一个实体的位置需要：实体长、宽，坐标偏移、镜像、旋转角度信息。函数首先更新这些信息（实体长、宽在构造时确定），然后调用 `update_boundary`，即为：`self.compute_boundary(self.offset, self.mirror, self.rotate)`，得到新的 `boundary`。



## 获取引脚

### `get_pin`

```python
def get_pin(self, name, index=-1):
    if index == -1:
        pin = copy.deepcopy(self.mod.get_pin(name))
        pin.transform(self.offset, self.mirror, self.rotate)
        return pin
    else:
        pins = copy.deepcopy(self.mod.get_pin(name))
        pins.transform(self.offset, self.mirror, self.rotate)
        return pin[index]
```

- `name`：引脚名称；





## `connect_spice_pins`

````python
def connect_spice_pins(self, nets_list):
````

- `nets_list`：`spice_net` 对象列表，包含要连接的线对象，连接关系与 `self.spice_pins` 顺序一一对应。

````python
def connect_spice_pins(self, nets_list):
    if len(nets_list) == 0 and len(self.spice_pins) == 0:
        self.connected = True
        return

    for pin in self.spice_pins.values():
        net = nets_list.pop(0)
        pin.set_inst_net(net)
        net.connect_pin(pin)
    self.connected = True
````

1. 检测 线 与 引脚是不是长度为 0，如果是，无需连接，设置 `self.connected = True` 后返回；
2. 依次对其中的 线 与 引脚，建立连接关系：`pin` 的 实体 net 设置为  `net`，`net` 的连接引脚添加一个 `pin`。设置 `self.connected = True` 后返回。



## `get_connecttions`

```python
def get_connections(self):
    conns = []
    for pin in self.spice_pins.values():
        conns.append(pin.inst_net.name)
    return conns
```

将引脚名称形成列表后返回。



