# `rectangle` 类

> Represents a rectangular shape

代表一个矩形图形，继承自 [`geometry`](./geometry.md)

一个 `retangle` 类对象典型的应用是表示一段连线。会在 `layout` 类实例调用 `add_rect` 时被创建并加入到 `layout` 实例的 `objs` 中。



## `__init__`

```python
def __init__(self, lpp, offset, width, height):
```

- `lpp`：layer 的层次序号与 purpose；
- `offset`：矩形的坐标偏移量；
- `width`、`height`：矩形的长、宽。

````python
def __init__(self, lpp, offset, width, height):
    super().__init__(lpp)
    self.name = "rect"
    self.offset = vector(offset).snap_to_grid()
    self.size = vector(width, height).snap_to_grid()
    self.width = round_to_grid(self.size.x)
    self.height = round_to_grid(self.size.y)
    self.compute_boundary(offset, "", 0)
````

- `geometry` 构造，赋值 `lpp` 信息；
- 赋值矩形名称；
- 赋值偏移 `self.offset`，一个 `vector` 对象，使用 `offset` 参数创建，需要对齐；
- 赋值尺寸：`self.size = vector(width, height).snap_to_grid()`；
- 赋值长、宽：需要对齐；
- 最后调用 `compute_boundary` 获得 `boundary`（左下、右上角坐标确定边界）。



## `get_blockages`

````python
def get_blockages(self, layer):
    if self.layerNumber == layer:
        return [[self.offset,
                 vector(self.offset.x + self.width,
                        self.offset.y + self.height)]]
    else:
        return []
````

如果需要的层次不是当前矩形的层次，返回空列表。否则返回：

````python
[[self.offset, vector(self.offset.x + self.width, self.offset.y + self.height)]]
````

因为矩形只有一个图形，所以返回的 `blockages` 列表只有一个元素：`[self.offset, vector(self.offset.x + self.width, self.offset.y + self.height)]`。

元素也是一个列表，使用 `[ll_pos, ur_pos]` 表示矩形。



## `gds_write_file`

````python
def gds_write_file(self, new_layout):
    new_layout.addBox(layerNumber=self.layerNumber,
                      purposeNumber=self.layerPurpose,
                      offsetInMicrons=self.offset,
                      width=self.width,
                      height=self.height,
                      center=False)
````

矩形的 `gds_write_file` 很简单，只需要调用版图对象 `new_layout` 的 `addBox` 方法，根据矩形的信息，加入一个 `boundary` 对象即可。

