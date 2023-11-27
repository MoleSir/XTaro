# `wire_path` 类

> Object metal wire_path; given the layer type. 
>
> Add a wire_path of minimium metal width between a set of points. The points should be rectilinear to control the bend points. If not, it will always go down first. 
>
> The points are the center of the wire_path. If width is not given, it uses minimum layer width.

金属线类；给定层类型。在一组点之间添加最小金属宽度的 wire_path。这些点应该是直线的，以控制弯曲点。如果没有，它总是会先下降。这些点是wire_path的中心。

如果未给定宽度，则使用最小层宽度。

这个类在 `layout` 对象调用 `add_path` 时被创建，主要功能是处理传入的一系列点，转为若干段矩形，对每个矩形，再调用 `layout` 对象的 `add_rect`，在这个函数中创建出 `geometry.rectangle` 实例表示一个矩形。

这样创建一个 `wire_path` 类，`layout` 对象会增加若干的 `geometry.rectangle` 实例，完成 `add_path` 的任务。



## `create_rectilinear_route` 辅助函数

> Add intermediate nodes if it isn't rectilinear. Also skip repeated nodes. Also, convert to vector if the aren't.

如果不是直线的，请添加中间节点。还要跳过重复的节点。此外，如果不是 `vector`，则转换为 `vector` 表示。

```python
def create_rectilinear_route(my_list):
```

- `my_list`：一个列表，每个元素表示一个坐标点，相邻两个点连接形成一段路径，表示导线路径。

### 坐标对齐

使得所有坐标对 `grid` 对齐：

````python
pl = [snap_to_grid(x) for x in my_list]
````

### 处理相邻坐标

````python
my_list = []
for index in range(len(pl) - 1):
    if pl[index] != pl[index + 1]:
        my_list.append(vector(pl[index]))
    if (pl[index][0] != pl[index + 1][0]) and (pl[index][1] != pl[index + 1][1]):
        my_list.append(vector(pl[index][0], pl[index + 1][1]))
my_list.append(vector(pl[-1]))
return my_list
````

遍历每相邻两个坐标：`pl[index]` 与 `pl[index + 1]`：

- 如果二者不是一个点，二者可以组成有效连线，先把第一个点 `pl[index]` 加入到 `my_list`；
- 如果两个点 x，y 坐标都不相同：说明二者连接不是直线，就需要创建一个新的坐标：x = 第一个坐标的 x，y = 第二个坐标的 y 加入到 `my_list` 中。这样就会在两个点中插入一个新坐标，使得非直线转为两条直线。



## `__init__`

````python
def __init__(self, obj, layer, position_list, width=None):
````

- `obj`：一个 `layout` 实例对象，需要加导线的对象；
- `layer`：加入版图层次名称；
- `position_list`：路径点列表；
- `width`：线宽度；

### 赋值基本属性

````python
self.obj = obj
self.layer_name = layer
self.layer_id = techlayer[layer]
if width == None:
    self.layer_width = drc["minwidth_{0}".format(layer)]
else:
    self.layer_width = width
self.position_list = position_list
self.pins = [] # used for matching parm lengths
self.switch_pos_list = []
````

如果 `width` 采用缺省，尝试从 `drc` 查找规则，得到这层的最小宽度。详见 [drc](../tech/drc.md)

还创建了 `pins` 与 `switch_pos_list` 空列表。

### `create_layout`

````python
self.create_layout()

    def create_layout(self):
        self.create_rectilinear()
        self.connect_corner()
        self.create_rectangles()
````

- [create_rectilinear](##`create_rectilinear`)
- [connect_corner](##`connect_corner`)
- [create_rectangles](##`create_rectangles`)



## `create_rectilinear`

````python
def create_rectilinear(self):
    self.position_list = create_rectilinear_route(self.position_list)
````

调用辅助方法 `create_rectilinear_route`，处理坐标列表。 



## `connect_corner`

### 获取三个列表

````python
from itertools import tee, islice
nwise = lambda g, n=2: zip(*(islice(g, i, None) for i, g in enumerate(tee(g, n))))
threewise=nwise(self.position_list, 3)

for (a, offset, c) in list(threewise):
    ...
````

通过 `tee`，`islice`。把 `position_list` 点分成三份，每次循环 `a` 是 i 个点、`offset` 是 i + 1 个，`c` 是 i + 2 个。 

> 例如，`l = [(0, 0), (0, 10), (10, 10), (10, 0), (0, 0)]`。处理后：
>
> ````python
> for (a, offset, c) in list(threewise):
>     print(a, offset, c)
> ````
>
> 输出：
>
> ````
> (0, 0) (0, 10) (10, 10)
> (0, 10) (10, 10) (10, 0)
> (10, 10) (10, 0) (0, 0)
> ````

### 处理 corner

````python
for (a, offset, c) in list(threewise):
    # add a exceptions to prevent a corner when we retrace back in the same direction
    if a[0] == c[0]:
        continue
    if a[1] == c[1]:
        continue
    corner_offset = [offset[0] - 0.5 * self.layer_width,
                     offset[1] - 0.5 * self.layer_width]
    self.draw_corner_wire(corner_offset)
````

`a`，`offset`，`c` 是连续的三个点，如果 `a` 与 `c` 有一个坐标是相同的，说明三点组成直线（因为要么直角，要么直线）。

如果不是，说明三者形成 corner 直角。将 `offset` x，y 都减小线宽一半，创建一个新的坐标（这个坐标就是线角的左下坐标，想象线有宽度）。调用 `draw_corner_wire`。

### `draw_corner_wire`

````python
def draw_corner_wire(self, offset):
    self.obj.add_rect(layer=self.layer_name,
                      offset=offset,
                      width=self.layer_width,
                      height=self.layer_width)
````

调用 `layout` 实例的 `add_rect`，参数：

- 层次为 `self.layer_name`；
- 偏移为传入参数，即为拐角的左下坐标；
- 宽度、高度都为线宽；

综上，顾名思义，这个 `connect_corner` 函数找到线上所有拐角，将拐角处这个正方形作为 `rectangle` 实例加入到 `layout` 中。

> 线有宽度！可以把线分为拐角的正方形，以及其他直走线。



## `create_rectangles`

函数将遍历 点 列表中所有相邻的点（0，1 处理过了，1，2 也要处理）。

````python
pl = self.position_list  # position list
for index in range(len(pl) - 1):
    if pl[index][0] != pl[index + 1][0]:
        ...
    elif pl[index][1] != pl[index + 1][1]:
        ...
````

并且分为两种情况：

### 二者 `x` 坐标不相等

说明这是一条平行 y 的直线（`create_rectilinear_route` 保证相邻两个坐标x，y 不可以都不同）。

````python
line_length = pl[index + 1][0] - pl[index][0]
offset = [pl[index][0],
          pl[index][1] - 0.5 * self.layer_width]
if line_length < 0:
    offset = [offset[0] + line_length,
              offset[1]]
self.add_line(layer_name=self.layer_name,
              length=abs(line_length),
              offset=offset,
              orientation="horizontal",
              layer_width=self.layer_width)
````

1. 通过 x 坐标的差线长度；

2. 偏移量是 [第一个点的 x，第二个点的 y - 线宽]，即为矩阵的左下角点；

3. 如果长度是负数，说明 x  的大小关系搞反了，更新偏移量，x 偏移减去真实长度。因为有可能前一个点在左，有一个点在右，但构造矩形时，要求偏移 `offset` 是左下角。

4. 调用 `add_line`：

   ````python
   def add_line(self, layer_name, length, offset, orientation, layer_width):
       width = layer_width
       height = length
   
       if orientation == "horizontal":
           width = length
           height = layer_width
       self.obj.add_rect(layer=layer_name,
                         offset=offset,
                         width=width,
                         height=height)
   ````

   把得到的矩形偏移、长、宽调用 `self.obj.add_rect`。注意 `orientation` 参数规定矩形的方向，如果是 `horizontal`，把长、宽对调。因为作为线的时候总是把走线方向作为长，但转为 `rectangle` 对象后，长就是指 x 轴距离，宽是 y。

### 二者 `y` 坐标不相等

与 x 类似：

````python
elif pl[index][1] != pl[index + 1][1]:
    line_length = pl[index + 1][1] - pl[index][1]
    offset = [pl[index][0] - 0.5 * self.layer_width,
              pl[index][1]]
    if line_length < 0:
        offset = [offset[0],
                  offset[1] + line_length]
    self.add_line(layer_name=self.layer_name,
                  length=abs(line_length),
                  offset=offset,
                  orientation="vertical",
                  layer_width=self.layer_width)

````

