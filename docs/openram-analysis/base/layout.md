# `layout` 类

> Class consisting of a set of objs and instances for a module. This provides a set of useful generic types for hierarchy management. 
>
> If a module is a custom designed cell, it will read from the GDS and spice files and perform LVS/DRC. If it is dynamically generated, it should implement a constructor to create the layout/netlist and perform LVS/DRC.



## `__init__`

````python
def __init__(self, name, cell_name):
````

- `name`：电路名称，版图中 `structure` 的名称；
- `cell_name`：电路类型名称；

````python
self.name = name
self.cell_name = cell_name

self.gds_file = OPTS.openram_tech + "gds_lib/" + cell_name + ".gds"
self.is_library_cell = os.path.isfile(self.gds_file)

self.width = None
self.height = None
self.bounding_box = None # The rectangle shape
self.bbox = None # The ll, ur coords
# Holds module/cell layout instances
self.insts = []
# Set of names to check for duplicates
self.inst_names = set()
# Holds all other objects (labels, geometries, etc)
self.objs = []
# This is a mapping of internal pin names to cell pin names
# If the key is not found, the internal pin names is assumed
self.pin_names = {}
# Holds name->pin_layout map for all pins
self.pin_map = {}
# List of modules we have already visited
self.visited = []

self.gds_read()
````

1. 赋值 `self.name` 与 `self.cell_name`；
2. 赋值并且判断是不是一个带有 .gds 文件的电路；
3. 宽度、长度、边界、bbox 暂时为 None
4. 创建 `self.insts = []` 保存本电路使用到的实体电路；
5. 创建 `self.inst_names = set()` 保存实体名称；
6. 创建 `self.objs = []` 保存本电路使用到的对象（例如 `labels`、`rectangle` 等）；
7. 创建 `self.pin_names = {}` 保存内部引脚名称与模块引脚的映射；
8. 创建 `self.pin_map = {}` 保存引脚名称与 `pin_layout` 对象的映射；
9. 创建 `self.visited = []` 保存已经被遍历的电路（输出 GDS 文件使用）；
10. 调用 `self.gds_read()`：如果电路有对应的版图文件，读取信息到 [`VlsiLayout`](../gdsMill/VlsiLayout.md) 对象中，保存在 `self.gds`；如果没有，创建一个空的 [`VlsiLayout`](../gdsMill/VlsiLayout.md) 对象到 `self.gds`。



## `gds_write`





## 坐标类方法

### `find_lowest_coords`

````python
def find_lowest_coords(self):
````

在这个版图中找到最左下角的坐标（遍历 `self.objs`、`self.insts` 与 `self.pin_map`）。

### `find_highest_coords`

````python
def find_highest_coords(self):
````

在这个版图中找到最右上角的坐标（遍历 `self.objs`、`self.insts` 与 `self.pin_map`）。

### `find_highest_layer_coords`

````python
def find_highest_layer_coords(self, layer):
````

在这个版图中找到指定 layer 层次的最右上角的坐标（遍历 `self.objs`、`self.insts` 与 `self.pin_map`）。

### `find_lowest_layer_coords`

````python
def find_lowest_layer_coords(self, layer):
````

在这个版图中找到指定 layer 层次的最左下角的坐标（遍历 `self.objs`、`self.insts` 与 `self.pin_map`）。

### `translate_all`

````python
def translate_all(self, offset):
````

将这个版图中所有对象的 `offset` 都减去参数中指定的 `offset`。（遍历 `self.objs`、`self.insts` 与 `self.pin_map`）。

### `offset_all_coordinates`

```python
def offset_all_coordinates(self, offset=None):
```

函数将先调用 `find_lowest_coords` 找到最左下坐标，调用 `translate_all` 让所有对象减去这个最左下坐标，使得版图最左下角对应到 原点。

之后再调用 `translate_all` 减去参数 `offset`。

### `offset_x_coordinates`

````python
def offset_x_coordinates(self, offset=None):
````

函数将先调用 `find_lowest_coords` 找到最左下坐标，调用 `translate_all` 让所有对象减去这个最左下坐标的 x 坐标，y 不变，使得版图最左下角的 x 坐标对应到 0。

之后再调用 `translate_all` 减去参数 `offset`。

### `get_gate_offset`

````python
def get_gate_offset(self, x_offset, height, inv_num):
````



## `insts`  相关方法

### `add_inst`

````python
def add_inst(self, name, mod, offset=[0, 0], mirror="R0", rotate=0):
    self.mods.add(mod)
    self.inst_names.add(name)
    self.insts.append(geometry.instance(name, mod, offset, mirror, rotate))
    return self.insts[-1]
````

给版图增加一个指定名称、`mod`（在 .sp 中相当于另一个 .SUBCKT，.gds 相当于另一个 strucutrue）、偏移、镜像类型、旋转角度的 实体。

1. 本电路引用了其他电路（或者是本 `strucure` 引用了其他 `strucuture`），将 `mod` 添加到 `self.mods`；
2. `self.inst_name` 增加一个项 `name`；
3. 创建一个 `instance` 实例对象：`geometry.instance(name, mod, offset, mirror, rotate)`，并且将其添加到 `self.insts` 中；
4. 返回 `instance` 实例对象。

### `get_inst`

````python
def get_inst(self, name):
````

遍历 `self.insts`，判断其名称是否为 `name`，是则返回这个 `instance` 实例，否则返回 `None`；

### `add_flat_inst`



## `objs` 相关方法

### `add_rect`

````python
def add_rect(self, layer, offset, width=None, height=None):
    if not width:
    	width = drc["minwidth_{}".format(layer)]
	if not height:
    	height = drc["minwidth_{}".format(layer)]
	lpp = tech_layer[layer]
	self.objs.append(geometry.rectangle(lpp,
                                    offset,
                                    width,
                                    height))
	return self.objs[-1]
````

给版图增加一个矩形：

- `layer`：指定矩形的层次；
- `offset`：矩形坐标偏移（左下坐标）；
- `width`、`height` 长、宽。

本质上是给 `self.objs` 添加一个 `geometry.rectangle` 实例对象。

### `add_rect_center`

````python
def add_rect_center(self, layer, offset, width=None, height=None):
````

跟 `add_rect` 作用相同，只不过 `add_rect` 的 `offset` 是矩形左下角，而 `add_rect_center` 是矩形中点，需要自己计算得到左下角坐标，再创建 `geometry.rectangle`。

### `add_segment_center`

````python
def add_segment_center(self, layer, start, end, width=None):
````

添加从 `start` 到 `end` 两个坐标的矩形（二者都是）。

函数不允许传入的 `start` 与 `end` 坐标的 x、y 都不同，只允许平行 x、y 的直线：

```python
def add_segment_center(self, layer, start, end, width=None):
    if not width:
        width = drc["minwidth_{}".format(layer)]

    if start.x != end.x and start.y != end.y:
        debug.error("Nonrectilinear center rect!", -1)
    elif start.x != end.x:
        offset = vector(0, 0.5 * width)
        return self.add_rect(layer,
                             start - offset,
                             end.x - start.x,
                             width)
    else:
        offset = vector(0.5 * width, 0)
        return self.add_rect(layer,
                             start - offset,
                             width,
                             end.y - start.y)
```

### `get_tx_insts`



## 引脚相关方法

### `add_layout_pin`

````python
def add_layout_pin(self, text, layer, offset, width=None, height=None):
````

给版图添加指定：名称、layer 层次、偏移、长款的引脚（`pin_layout`）

1. 创建引脚（详见 [`pin_layout`](./pin_layout.md)）：

   ````python
   new_pin = pin_layout(text,
                        [offset, offset + vector(width,height)],
                        layer)
   ````

2. 添加引脚

   ```python
   try:
       if new_pin not in self.pin_map[text]:
           self.pin_map[text].add(new_pin)
   except KeyError:
       self.pin_map[text] = set()
       self.pin_map[text].add(new_pin)
   
   return new_pin
   ```

   > `pin_layout` 重写了 `__eq__`，只要层次和矩形一致就是相等。

   1. 如果 `text` 名称的引脚已经被加入过（同名称、但位置不同？），并且 之前加入的引脚都和 `new_pin` 不一致，那么给 `self.pin_map[text]` 这个集合加入 `new_pin`。
   2. 如果 `text` 名称的引脚还没加入过，给 `self.pin_map[text]` 设置一个空集合，再加入 `new_pin`。

综上，`self.pin_map` 的结构是：

````python
{
	'text1' : {pin_layout_0, pin_layout_1},
    'text2' : {pin_layout_3, pin_layout_4, pin_layout2},
}
````

### `add_layout_pin_rect_center`

```python
def add_layout_pin_rect_center(self, text, layer, offset, width=None, height=None):
```

这里的 `offset` 是引脚中心坐标，先转为左下角再调用 `add_layout_pin`。

### `add_layout_pin_rect_ends`

````python
def add_layout_pin_rect_ends(self, name, layer, start, end, width=None):
````

// TODO

### `remove_layout_pin`

```python
def remove_layout_pin(self, text):
    self.pin_map[text] = set()
```

清空 `name` 对应的引脚集合。

### `remove_layout_pins`

````python
def remove_layout_pins(self):
    self.pin_map = {}
````

清空整个 `self.pin_map`。

### `get_pin_name`

```python
def get_pin_name(self, text):
    if text in self.pin_names:
        return self.pin_names[text]
    else:
        return text
```

返回外部引脚对应名称，没有就返回 `text`

### `get_pin`

````python
def get_pin(self, text):
````

