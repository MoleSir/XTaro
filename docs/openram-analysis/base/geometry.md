# `geometry` 类

> A specific path, shape, or text geometry. Base class for shared items.

特定的路径、形状或文本几何图形。共享项的基类。



## `__init__`

````python
def __init__(self, lpp=None):
    self.width = 0
    self.height = 0
    if lpp:
        self.lpp = lpp
        self.layerNumber = lpp[0]
        self.layerPurpose = lpp[1]
````

- `lpp` ：layer 序号与 purpose。详见 [tech.py](../tech/tech.md) 中的 layer 字典。

很简单的构造函数，默认图形的宽、高为 0，如果指定 `lpp`，赋值 `self.lpp`、layer 序号与 purpose。



## `transform_coords`

```python
def transform_coords(self, coords, offset, mirr, angle):
```

- `coords`：坐标列表；
- `offset`：偏移；
- `mirr`：镜像类型；
- `angle`：旋转弧度；

函数取出 `coords` 中的每一个坐标，计算其经过 旋转、偏移后的新坐标放到新列表，最后返回：

````python
for item in coords:
    x = item[0] * math.cos(angle) - item[1] * mirr * math.sin(angle) + offset[0]
    y = item[0] * math.sin(angle) + item[1] * mirr * math.cos(angle) + offset[1]
    coordinate += [[x, y]]
````



## `normalize`

````python
def normalize(self):
````

`self.boundary` 使用一对坐标描述矩形的左下、右上角坐标：`[(llx, lly), (urx, ury)]`。但经过变换后这些坐标可能不再是左下、右上。调用这个函数，可以重新组织这四个值，使得 `boundary` 再次满足结构：

````python
def normalize(self):
    """ Re-find the LL and UR points after a transform """
    (first, second) = self.boundary
    ll = vector(min(first[0], second[0]),
                min(first[1], second[1])).snap_to_grid()
    ur = vector(max(first[0], second[0]),
                max(first[1], second[1])).snap_to_grid()
    self.boundary = [ll, ur]
````



## `compute_boundary`

````python
def compute_boundary(self, offset=vector(0, 0), mirror="", rotate=0):
````

- `offset`：偏移量；
- `mirror`：镜像类型；
- `rotate`：旋转角度；

调用这个函数，根据输入三个参数，计算得到这个图形的边界 `boundary`。

### 默认的边界

````python
(ll, ur) = [vector(0, 0), vector(self.width, self.height)]
````

默认以原点为左下角，长、宽为右上创建一个 `boundary`。

### 镜像

````python
if mirror == "MX":
    ll = ll.scale(1, -1)
    ur = ur.scale(1, -1)
elif mirror == "MY":
    ll = ll.scale(-1, 1)
    ur = ur.scale(-1, 1)
elif mirror == "XY":
    ll = ll.scale(-1, -1)
    ur = ur.scale(-1, -1)
elif mirror == "" or mirror == "R0":
    pass
````

根据 `mirror` 的类型，调用 [`vector`](./vector.md) 的 `scale` 方法。

### 旋转

````python
if rotate == 0:
    pass
elif rotate == 90:
    ll = ll.rotate_scale(-1, 1)
    ur = ur.rotate_scale(-1, 1)
elif rotate == 180:
    ll = ll.scale(-1, -1)
    ur = ur.scale(-1, -1)
elif rotate == 270:
    ll = ll.rotate_scale(1, -1)
    ur = ur.rotate_scale(1, -1)
````

### 偏置

````python
self.boundary = [offset + ll, offset + ur]
````

因为经过了旋转等处理，最后得到的两个位置不一定是左下、右上，调用：

````python
self.normalize()
````



## 属性方法

- `ll`：返回 `boundary` 左下角；
- `ur`：返回 `boundary` 右上角；
- `lr`：返回 `boundary` 右下角；
- `ul`：返回 `boundary` 左上角；
- `uy`：返回 `boundary` 最大 `y`；
- `by`：返回 `boundary` 最小 `y`；
- `ux`：返回 `boundary` 最大 `x`；
- `bx`：返回 `boundary` 最小 `x`；
- `cx`，`cy`：返回 `boundary` 中心 x，y；
- `center`：返回 `boundary` 中心坐标 vector 对象

