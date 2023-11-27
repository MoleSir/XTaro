# `vector` 类

> This is the vector class to represent the coordinate vector. It makes the coordinate operations easy and short so the code is concise.
>
> It needs to override several operators to support concise vector operations, output, and other more complex data structures like lists.

这是表示坐标向量的向量类。它使坐标操作简单而简短，因此代码简洁。

它需要重载几个运算符，以支持简洁的矢量运算、输出和其他更复杂的数据结构（如列表）。



## `__init__`

````python
def __init__(self, x, y=0):
    if isinstance(x, (list,tuple,vector)):
        self.x = float(x[0])
        self.y = float(x[1])
    else:
        self.x = float(x)
        self.y = float(y)
    self._hash = hash((self.x,self.y))
````

构造函数支持直接传入两个浮点数 `x`，`y` 来创建，或者直接传入一个包含两个浮点数的数据结构（直接传递给 `x`，这样 `y` 就默认为 0 不起作用）。

### 传入数据结构的创建

````python
    if isinstance(x, (list,tuple,vector)):
        self.x = float(x[0])
        self.y = float(x[1])
````

如果 `x` 是 `list`、`tuple` 或者另一个 `vector`，取出其中的两个数作为这个 `vector` 的 `x` 与 `y`。

### 直接传入数值

````python
    else:
        self.x = float(x)
        self.y = float(y)
````



## `__setitem__`

````python
def __setitem__(self, index, value):
    if index==0:
        self.x=float(value)
    elif index==1:
        self.y=float(value)
    else:
        self.x=float(value[0])
        self.y=float(value[1])
    self._hash = hash((self.x,self.y))
````

重写 `__setitem__` 来实现直接对 `vector` 类对象设置其 `x`、`y`。



## `__getitem__`

````python
def __getitem__(self, index):
    if index==0:
        return self.x
    elif index==1:
        return self.y
    else:
        return self
````

通过重写 `__getitem__` 来实现直接通过  `[]` 获取 `x`、`y`。



## `__add__` 与 `__sub__`

两个 `vector` 类相加减，即为二者的 `x`、`y` 分别加、减后创建一个新的 `vector`。



## `snap_to_grid`

> Changes the coodrinate to match the grid settings

更改 coodrinate 以匹配网格设置。

根据 tech.py 中定义的 `drc["grid"]` 值，将 vector 中的 `x`、`y` 设置到距离其原来的值最接近的 `drc["grid"]` 整数倍：

````python
def snap_to_grid(self):
    self.x = self.snap_offset_to_grid(self.x)
    self.y = self.snap_offset_to_grid(self.y)
    self._hash = hash((self.x,self.y))
    return self

def snap_offset_to_grid(self, offset):
    grid = tech.drc["grid"]
    # this gets the nearest integer value
    off_in_grid = int(round(round((offset / grid), 2), 0))
    offset = off_in_grid * grid
    return offset
````

其中调用了 `snap_offset_to_grid` 方法分别对 `x`、`y` 进行处理。`int(round(round((offset / grid), 2), 0))` 是将 `offset / grid` 后得到的值先取两位小数，再保留 0 位小数字，最后转为整数。



## `rotate`

````python
def rotate(self):
    return vector(self.y,self.x)
````

将 `vector` 的 `x`、`y` 颠倒，创建一个新的 `vector` 返回。不会改变原来的 `vector`。



## `scale`

````python
def scale(self, x_factor, y_factor=None):
    if y_factor==None:
        y_factor=x_factor[1]
        x_factor=x_factor[0]
    return vector(self.x*x_factor,self.y*y_factor)
````

将自身的 `x`、`y` 分别乘上一个倍数，创建一个新的 `vector` 返回。不会改变原来的 `vector`。

同样支持两种方式：

- 分别传入 `x` 与 `y` 的倍数；
- 直接传入一个数据结构保存两个倍数，那么 `y_factor` 默认为 None。



## `rotate_scale`

````python
def rotate_scale(self, x_factor, y_factor=None):
    if y_factor==None:
        y_factor=x_factor[1]
        x_factor=x_factor[0]
    return vector(self.y*x_factor,self.x*y_factor)
````

基本与 `scale` 一致，不过相当于先进行 `ratate`，再 `scale`。注意最后创建 `vector` 时候 `x`、`y` 的顺序。



## `floor`、`ceil` 与 `round`

将 `vector` 的 `x`、`y` 分别向下、向上取整后、四舍五入创建新 `vector` 返回：

```python
def floor(self):
    return vector(int(math.floor(self.x)),int(math.floor(self.y)))

def ceil(self):
    return vector(int(math.ceil(self.x)),int(math.ceil(self.y)))

def round(self):
    return vector(int(round(self.x)),int(round(self.y)))
```



## 比较符号重载

通过比较 `self.__dict__` 判断两个 `vector` 是否相同。



## `max` 与 `min`

````python
    def max(self, other):
        """ Max of both values """
        return vector(max(self.x,other.x),max(self.y,other.y))

    def min(self, other):
        """ Min of both values """
        return vector(min(self.x,other.x),min(self.y,other.y))
````

Show Code!

