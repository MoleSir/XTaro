# `pin_layout` 类

>   A class to represent a rectangular design pin. It is limited to a  single shape.

表示矩形设计引脚的类。它仅限于一个形状。

需要使用：

- [vector](./vector.md)



## `__init__`

````python
def __init__(self, name, rect, layer_name_pp):
````

- `name` 表示引脚名称；
- `rect` 包含两个 `vector` 对象（或者可以构造两个 `vector` 对象）。用两个 `vector` 分别确定这个引脚矩形的 左下、右上 点坐标；
- `layer_name_pp`：可以是字符串，表示引脚所在版图层次的名称（如 `poly`、`m1` ）。或者传入一个元组，对应了某个版图层次的序号（GDS 文件使用的就是序号表示 layer。

### 赋值名称

```python
self.name = name
```

### 赋值 `rect`

````python
if type(rect[0]) == vector:
    self._rect = rect
else:
    self._rect = [vector(rect[0]), vector(rect[1])]
# snap the rect to the grid
self._rect = [x.snap_to_grid() for x in self.rect]
````

- 如果 `rect` 参数是一个 `vector` 容器，说明 `rect` 直接包含了两个 `vector`，赋值给 `self._rect`；
- 否则，通过 `rect` 的第一个、第二个元素（肯定都是包含两个数值的数据结构，见 [vector](./vector.md) 的构造函数）来构建 `self._rect`。

最后将传入的两个 `vector` 调用 `snap_to_grid` 使得其对工艺尺寸对齐。

### 赋值 layer 名称与 layer 序号

先创建层次与其序号的映射：

````python
valid_layers = {x: layer[x] for x in layer_indices.keys()}
````

其中 `layer_indices` 与 `layer` 都是从 [tech](../tech/tech.md/##连接关系) 模块中导入。

> `layer_indices` 中只有走线的层次，而 `layer` 有 via、pwell 等没有走线的。

最后得到 `valid_layers` 包含哪些有导线层次的名称与序号映射（一个元组）。

````python
if type(layer_name_pp) == str:
    self._layer = layer_name_pp
else:
    for (layer_name, lpp) in valid_layers.items():
        if not lpp:
            continue
        if self.same_lpp(layer_name_pp, lpp):
            self._layer = layer_name
            break

    else:
        try:
            from openram.tech import layer_override
            from openram.tech import layer_override_name
            if layer_override[name]:
               self.lpp = layer_override[name]
               self.layer = "pwellp"
               self._recompute_hash()
               return
        except:
            debug.error("Layer {} is not a valid routing layer in the tech file.".format(layer_name_pp), -1)

self.lpp = layer[self.layer]
````

- 如果 `layer_name_pp` 参数就是字符串，说明传入的就是 layer 名称，直接赋值；
- 否则，说明 `layer_name_pp` 本身就是序号。那么从 `valid_layers` 依次取出需要与其比较，得到一致的 `layer_name`，赋值给 `self._layer`。

最后总是可以得到 `self._layer`，表示层次名称。再到 `layer` 查找得到序号赋值给 `self.lpp`。



## 比较函数

### `__eq__`

````python
def __eq__(self, other):
    if isinstance(other, self.__class__):
        return (self.lpp == other.lpp and self.rect == other.rect)
    else:
        return False
````

如果两个 `pin_layout` 对象的 `lpp` 与 `rect` 即：层次序号一致，位置一致，二者相同：

### `__lt__`

```python
def __lt__(self, other):
    (ll, ur) = self.rect
    (oll, our) = other.rect
    if ll.x < oll.x and ll.y < oll.y:
        return True
    return False
```

只比较两个 `pin_layout` 的 左下 顶点。



## `bbox`

````python
def bbox(self, pin_list):
    (ll, ur) = self.rect
    min_x = ll.x
    max_x = ur.x
    min_y = ll.y
    max_y = ur.y

    for pin in pin_list:
        min_x = min(min_x, pin.ll().x)
        max_x = max(max_x, pin.ur().x)
        min_y = min(min_y, pin.ll().y)
        max_y = max(max_y, pin.ur().y)

    self.rect = [vector(min_x, min_y), vector(max_x, max_y)]
````

- `pin_list`：一个 `pin_layout` 对象列表。

找到 `self` 与 `pin_layout` 中所有 `x`、`y` 坐标的最大、最小值。将四个值创建一个新的 `rect` 赋值给 `self.rect`。

这样可以使得调用完之后的 `self.rect` 可以覆盖 `pin_list` 已经原来所有的 `rect`。



## `inflate`

````python
def inflate(self, spacing=None, multiple=0.5):
    if not spacing:
        spacing = multiple*drc("{0}_to_{0}".format(self.layer))

    (ll, ur) = self.rect
    spacing = vector(spacing, spacing)
    newll = ll - spacing
    newur = ur + spacing

    return (newll, newur)
````

函数返回将 `self.rect` 向外延申出 `spacing` 距离的新 `rect`。如果调用不提供 `spacing`，会根据 `self.layer`，即层次名称与 `drc` 对象中获取。



## `overlap` 方法

`pin_layout` 提供了一些列关于计算判断 `overlap` 的方法：

### `xoverlaps`

`def xoverlaps(self, other):` 判断 `self` 与 `other` 的 `x` 坐标上是否有重叠：

````python
x_overlaps = False
# check if self is within other x range
if (ll.x >= oll.x and ll.x <= our.x) or (ur.x >= oll.x and ur.x <= our.x):
    x_overlaps = True
# check if other is within self x range
if (oll.x >= ll.x and oll.x <= ur.x) or (our.x >= ll.x and our.x <= ur.x):
    x_overlaps = True
````

### `yoverlaps`

`def yoverlaps(self, other):` 判断 `self` 与 `other` 的 `y` 坐标上是否有重叠；

### `overlaps`

`def overlaps(self, other):` 判断 `self` 与 `other` 的矩形是否重叠。首先判断层次，再分别调用 x 与 y 的 `overlaps`。都满足才有重叠。

### `intersection`

`def intersection(self, other)`：先判断 `self` 与 `other` 是否重叠，如果没有，返回 `None`；如果是，返回将二者包裹起来的最大的矩形的 `pin_layout`：

````python
if not self.overlaps(other):
    return None

(ll, ur) = self.rect
(oll, our) = other.rect

min_x = max(ll.x, oll.x)
max_x = min(ur.x, our.x)
min_y = max(ll.y, oll.y)
max_y = min(ur.y, our.y)

if max_x - min_x == 0 or max_y - min_y == 0:
    return None

return pin_layout("", [vector(min_x, min_y), vector(max_x, max_y)], self.layer)
````

### `compute_overlap`

`def compute_overlap(self, other):` 计算 两个 `pin_layout` 重叠部分的矩形：

````python
def compute_overlap(self, other):
    (r1_ll, r1_ur) = self.rect
    (r2_ll, r2_ur) = other.rect
    
    dy = min(r1_ur.y, r2_ur.y) - max(r1_ll.y, r2_ll.y)
    dx = min(r1_ur.x, r2_ur.x) - max(r1_ll.x, r2_ll.x)

    if dx >= 0 and dy >= 0:
        return [dx, dy]
    else:
        return [0, 0]
````



## `contains` 方法

`pin_layout` 提供了一些列关于计算判断 `contains` 的方法：

-  `def xcontains(self, other):` 判断 `other` 的 `x` 坐标范围是否被 `self` 包含：

  ````python
  (ll, ur) = self.rect
  (oll, our) = other.rect
  
  return (oll.x >= ll.x and our.x <= ur.x)
  ````

-  `def ycontains(self, other):` 判断 `other` 的 `y` 坐标范围是否被 `self` 包含；

- `def contains(self, other):` 判断 `self` 是否完全包含 `other`：

  ```python
  if self == other:
      return True
  if not self.same_lpp(self.lpp, other.lpp):
      return False
  if not self.xcontains(other):
      return False
  if not self.ycontains(other):
      return False
  return True
  ```

- `def contained_by_any(self, shape_list):` 判断 `self` 是否被 `shape_list` 的中任一个 `contains`。



## 面积、高度、宽度

三个属性可以通过 `self.rect` 直接计算得到：

````python
def area(self):
    return self.height()*self.width()

def height(self):
    return abs(self.rect[1].y-self.rect[0].y)

def width(self):
    return abs(self.rect[1].x-self.rect[0].x)
````



## `normalize`

> Re-find the LL and UR points after a transform

在进行变换后，重写得到 左下角 与 右上角点。

````python
(first, second) = self.rect
ll = vector(min(first[0], second[0]), min(first[1], second[1]))
ur = vector(max(first[0], second[0]), max(first[1], second[1]))
self.rect=[ll, ur]
````



## `transform`

````python
def transform(self, offset, mirror, rotate):
````

- `offset`：坐标平移距离；
- `mirror`：镜像类型（关于 x、y 或者 xy）；
- `rotate`：旋转角度。

基本都是利用在 [vector](./vector.md) 中实现的方法进行处理：

### 镜像

````python
        (ll, ur) = self.rect
        if mirror == "MX":
            ll = ll.scale(1, -1)
            ur = ur.scale(1, -1)
        elif mirror == "MY":
            ll = ll.scale(-1, 1)
            ur = ur.scale(-1, 1)
        elif mirror == "XY":
            ll = ll.scale(-1, -1)
            ur = ur.scale(-1, -1)
````

### 角度

````python
        if rotate == 90:
            ll = ll.rotate_scale(-1, 1)
            ur = ur.rotate_scale(-1, 1)
        elif rotate == 180:
            ll = ll.scale(-1, -1)
            ur = ur.scale(-1, -1)
        elif rotate == 270:
            ll = ll.rotate_scale(1, -1)
            ur = ur.rotate_scale(1, -1)
````

### 偏移

````python
        self.rect = [offset + ll, offset + ur]
        self.normalize()
````

进行坐标变换后，需要调用 `self.normalize()`。



## 基本属性

- `center` 矩形中心坐标；
- `cx`：x 中心坐标
- `cy`：y 中心坐标
- `ll`：左下角点
- `ur`：右上角点
- `uy`：右上角 y 值
- `by`：左下角 y 值
- `ux`：右上角 x 值
- `bx`：左下角 x 值
- ……



## `gds_write_file ` 

````python
def gds_write_file(self, newLayout):
````

- `newLayout`：`VlsiLayout` 类对象，表示一个版图 GDSII 文件；

函数将会把 `self` 的基本信息添加到 `newLayout` 中。

### 获取 layer 序号

根据不同工艺库的 `tech` 文件做了不同处理，例如：

````python
try:
    (pin_layer_num, pin_purpose) = layer[self.layer + "p"]
except KeyError:
    (pin_layer_num, pin_purpose) = layer[self.layer]
(layer_num, purpose) = layer[self.layer]
````

对 scn4m_subm 工艺，可以提供 `layer[self.layer + "p"]`，所以 `(pin_layer_num, pin_purpose)` 和 `(layer_num, purpose)` 是一样的。

### 添加 `box`

调用 `newLayout` 的 `addBox` 添加一个 boundary：

````python
newLayout.addBox(layerNumber=layer_num,
                 purposeNumber=purpose,
                 offsetInMicrons=self.ll(),
                 width=self.width(),
                 height=self.height(),
                 center=False)
````

如果 `pin_layer_num` 与 `layer_num` 不匹配，还需要添加 `pin_purpose` 的 boundary。

### 添加 `text` 

给 `newLayout` 添加 `self.name`，表示引脚名称：

```python
newLayout.addText(text=self.name,
                  layerNumber=layer_num,
                  purposeNumber=label_purpose,
                  magnification=zoom,
                  offsetInMicrons=self.center())
```



## `distance`

````python
def distance(self, other):
````

计算两个不交叠的 `pin_layout` 之间的距离（如果交叠，返回 0）。距离指的是最短距离。



## `overlap_length`

计算两个 `pin_layout` 的交叠长度。

### 判断包含

````python
if self.contains(other):
    return math.inf
elif other.contains(self):
    return math.inf
````

二者包含关系，返回无限大。

### 获取交点

````python
intersections = set(self.compute_overlap_segment(other))
````

`compute_overlap_segment` 函数首先得到两个矩形的变，再判断这 2 组四条边是否有交点。最后返回所有交点列表。

### 两个交点

如果 `intersections` 长度为 2，说明两个矩形只有 2 个交点，返回这两点的距离：

````python
if len(intersections) == 2:
    (p1, p2) = intersections
    return math.sqrt(pow(p1[0]-p2[0], 2) + pow(p1[1]-p2[1], 2))
````

### 四个交点

四个交点，那么四个点应该形成一个矩阵，返回这个矩形较长一边的长度：

````python
elif len(intersections) == 4:
    points = intersections
    ll = vector(min(p.x for p in points), min(p.y for p in points))
    ur = vector(max(p.x for p in points), max(p.y for p in points))
    new_shape = pin_layout("", [ll, ur], self.lpp)
    return max(new_shape.height(), new_shape.width())
````

### 其他情况

返回 0：

````python
return 0
````



## `cut`

````python
def cut(self, shape):
````

`self` 与 `shape` 分别得到 4 个 x 坐标、4 个 y 坐标，这些坐标点可以形成一个 9 宫格。为九宫格中的每一个小格子创建出一个 `pin_layout`，形成一个列表返回。（不能包含 `shape`）。

````python
def cut(self, shape):
    # Make the unique coordinates in X and Y directions
    x_offsets = sorted([self.lx(), self.rx(), shape.lx(), shape.rx()])
    y_offsets = sorted([self.by(), self.uy(), shape.by(), shape.uy()])

    new_shapes = []
    # Create all of the shapes
    for x1, x2 in zip(x_offsets[0:], x_offsets[1:]):
        if x1==x2:
            continue
        for y1, y2 in zip(y_offsets[0:], y_offsets[1:]):
            if y1==y2:
                continue
            new_shape = pin_layout("", [vector(x1, y1), vector(x2, y2)], self.lpp)
            # Don't add the existing shape in if it overlaps the pin shape
            if new_shape.contains(shape):
                continue
            # Only add non-zero shapes
            if new_shape.area() > 0:
                new_shapes.append(new_shape)

    return new_shapes
````

