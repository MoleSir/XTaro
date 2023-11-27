# `VlsiLayout` 类

一个 `VlsiLayout` 类对象描述了一个 .gds 文件内容，可以由 `gds2writer` 写入到指定文件中。



## 静态方法

### `labelInRectangle`

````python
def labelInRectangle(self,coordinate,rectangle):
````

- `coordinate`：表示一个坐标；
- `retangle`：使用四个坐标表示一个矩形：` [leftx, bottomy, rightx, topy]`。

函数返回 `coordinate` 位于 `rectangle` 内部:

### `transformCoordinate`

```python
def transformCoordinate(self,coordinate,uVector,vVector):
```

- `coordinate`：表示一个坐标；
- `uVector`、`vVector`：一个包含两个坐标的 `list`，包含坐标变换信息。

函数将 `coordinate` 旋转后返回一个新的坐标：

````python
x=coordinate[0]*uVector[0][0]+coordinate[1]*vVector[0][0]
y=coordinate[0]*uVector[1][0]+coordinate[1]*vVector[1][0]
transformCoordinate=[x,y]
````

### `transformRectangle`

```python
def transformRectangle(self,originalRectangle,uVector,vVector):
```

- `originalRectangle`：四个值的列表，分别表示：最左 x，最下 y，最右 x，最上 y；
- `uVector`、`vVector`：一个包含两个坐标的 `list`，包含坐标变换信息。

函数形成左下、右上两个点，分别调用 `transformCoordinate`。得到两个新的坐标表示矩形，再重新找到矩形的左下、右上点返回。

### `transformPolygon`

````python
def transformPolygon(self,originalPolygon,uVector,vVector):
````

- `originalPolygon`：一个 `float` 列表，相邻两个元素表示一个点，最后由这些点组成一个 polygon；
- `uVector`、`vVector`：一个包含两个坐标的 `list`，包含坐标变换信息。

对每两个元素（一个坐标）调用 `transformCoordinate`。得到的新位置也用 `float` 列表形式返回。

### `rotatedCoordinates`

````python
def rotatedCoordinates(self,coordinatesToRotate,rotateAngle):
````

- `coordinatesToRotate`：需要旋转的坐标列表；
- `rotateAngle`：旋转的角度（使用角度制）。

将列表中的每一个坐标取出，进行坐标旋转：

````python
newX = coordinate[0]*math.cos(angle) - coordinate[1]*math.sin(angle)
newY = coordinate[0]*math.sin(angle) + coordinate[1]*math.cos(angle)
````

其中 `angle` 是参数 `rotateAngle` 转为弧度制的值：`angle = math.radians(float(rotateAngle))`。

将新坐标放到列表中返回。

### `isBounded`

````python
def isBounded(self,testPoint,startPoint,endPoint):
````

- `testPoint`：一个坐标；
- `startPoint`：一个坐标；
- `endPoint`：一个坐标。

返回 `testPoint` 是否位于由 `startPoint` 和 `endPoint` 两个点形成的对角线的矩形内部。

### `intersectionPoint`

```python
def intersectionPoint(self,startPoint1,endPoint1,startPoint2,endPoint2):
```

- 四个参数都表示坐标。代表两条线。

函数返回两条线段的交点（注意是线段，即使不平行也可能没有）。如果没有交点，返回 None。

### `isCollinear`

````python
def isCollinear(self,testPoint,point1,point2):
````

- 三个参数都表示坐标。

函数返回：`testPoint` 是否位于 `point1` 与 `point2` 形成的直线上。

### `doShapesIntersect`

````python
def doShapesIntersect(self,shape1Coordinates, shape2Coordinates):
````

- 两个参数都是坐标列表，这些坐标依次连接形成图形。

函数返回：这两个图形是否存在交点，即是否存在重叠部分。

### `isPointInsideOfBox`

````python
def isPointInsideOfBox(self,pointCoordinates,boxCoordinates):
````

- `pointCoordinates`：坐标点；
- `boxCoordinates`：使用四个坐标表示一个 Box。

函数返回：`point` 是否位于这个 Box 内。

### `isShapeInsideOfBox`

```python
def isShapeInsideOfBox(self,shapeCoordinates, boxCoordinates):
```

- `shapeCoordinates`：坐标列表表示一个图形；
- `boxCoordinates`：使用四个坐标表示一个 Box。

函数返回：这个图形是不是被 Box 包含。判断的方法是对图形的所有的点调用 `isPointInsideOfBox`。







## `__init__`

````python
def __init__(self, name=None, units=(0.001,1e-9), libraryName="DEFAULT.DB", gdsVersion=5):
````

- `name`：版图描述的结构名称；
- `uints`：版图中 UNITS record 需要的两个信息；
- `libraryName` 与 `gdsVersion` 不太重要；

### 创建结构字典

````python
self.structures=dict()
self.layerNumbersInUse = []
````

### 赋值根结构

````python
if name:
    # take the root structure and copy it to a new structure with the new name
    self.rootStructureName=self.padText(name)
    # create the ROOT structure
    self.structures[self.rootStructureName] = GdsStructure()
    self.structures[self.rootStructureName].name = name
    self.structures[self.rootStructureName].createDate = (modDate.year,
                                                          modDate.month,
                                                          modDate.day,
                                                          modDate.hour,
                                                          modDate.minute,
                                                          modDate.second)
    self.structures[self.rootStructureName].modDate = (modDate.year,
                                                       modDate.month,
                                                       modDate.day,
                                                       modDate.hour,
                                                       modDate.minute,
                                                       modDate.second)
````

如果传入了名称 `name`，那么就这是根结构的名称（通过 `padText` 函数，如果 `name` 长度为单数，补上一个 0）。

创建一个 `GdsStructure` 类对象，保存到 `self.structure`，键为 `self.rootStructureName`。给这个结构命名为 `name`。并且将这个结构的创建、修改时间赋值为当前时间（使用年、月、日、时、分、秒）的元组表示。

### 版图信息

````python
self.info = dict()  # information gathered from the GDSII header
self.info['units']=self.units
self.info['dates']=(modDate.year,
                    modDate.month,
                    modDate.day,
                    modDate.hour,
                    modDate.minute,
                    modDate.second,
                    modDate.year,
                    modDate.month,
                    modDate.day,
                    modDate.hour,
                    modDate.minute,
                    modDate.second)
self.info['libraryName']=libraryName
self.info['gdsVersion']=gdsVersion
````

### `xyTree`

````python
self.xyTree = []
````

创建一个列表，其中的每个元素是一个：（structure 名称、X、Y、偏移量、旋转）元组。

### 引脚字典

````python
self.pins = {}
````



## `uniquify`

````python
def uniquify(self, prefix_name=None):
````

- `prefix_name`：一个前缀字符串。

函数将 `self` 中所有的 `GdsStructure` 对象的 `name` 都加上 `prefix_name` 。除了 `rootStructureName` 之外。

当然 `srefs` 中引用的名称也要修改。



## `rename`

````python
def rename(self,newName)
````

- `newName`：新的根结构名称。

将原来的 `rootStructureName` 修改 `newName`。



## `getAllShapes`

````python
def getAllShapes(self, lpp):
````

- `lpp` 指定的 layer。

### 调用 `getShapesInStructure`

````python
boundaries = set()
for TreeUnit in self.xyTree:
    # print(TreeUnit[0])
    boundaries.update(self.getShapesInStructure(lpp, TreeUnit))
````

对 `self.xyTree` 中的每一个选，调用 `getShapesInStructure`，并且将得到的返回值添加到 `boundaries` 集合中。

`xyTree` 是一个列表，每个元素是：`(结构名称， 结构Origin，u旋转信息，v选项信息)`。

`getShapesInStructure` ：

````python
def getShapesInStructure(self, lpp, structure):
        (structureName, structureOrigin,
         structureuVector, structurevVector) = structure
````

首先取出 `structure` 的信息。得到 `structureName`。到 `self.structures` 中查找，得到一个 GDSStructure 类对象，遍历这个 `structure` 对象的 `boundarys`，并且判断 `boundary` 的层次是不是函数参数规定的，如果不是跳过：

````python
boundaries = []
for boundary in self.structures[str(structureName)].boundaries:
    if sameLPP((boundary.drawingLayer, boundary.purposeLayer),
               lpp):
````

这样就找到了参数中指定 structure 中位于 layer 层次的 boudarys：

接下来就是取出 `boundary` 对象的 `coordinates` 对其进行 `transformPolygon` 变换，放入 `boundaries` 数组（变换后使用值列表的形式表示，变换前是点列表）。

最后将 `boundaries` 数组返回。

这样每次调用 `getShapesInStructure` 将得到一个数组：这个数组的每一个元素使用值列表表示一个图形。每个图形都是由特定 `structure` 的特定层次上的 boundary 对象的路径通过特定变换得到。

> 注意，如果 boundary 是矩形，其实只要 4 个值就可以表示（两个点），即 `[llx, lly, urx, ury]`。而如果是一般图形，就需要把所有点都描述出来。

### 转为 user units 表示

将上面得到的 `boundaries` 的值都转为 user units 表示：

````python
user_boundaries = []
for boundary in boundaries:
    boundaries_list = []
    for i in range(0, len(boundary)):
        boundaries_list.append(boundary[i]*self.units[0])
    user_boundaries.append(boundaries_list)
return user_boundaries
````

> PS：吐槽这个代码写得真烂！



## `getBlockages`

````python
def getBlockages(self, lpp):
````

`getAllShapes` 函数得到的是一个集合，集合中的每一个元素表示一个 `lpp` 层次上的 `boudary` 的路径。使用值列表表示。

所以 `getBlockages` 函数就要把这个集合中的所有元素都转为点列表表示：

````python
def getBlockages(self, lpp):
    blockages = []

    shapes = self.getAllShapes(lpp)
    for boundary in shapes:
        vectors = []
        for i in range(0, len(boundary), 2):
            vectors.append((boundary[i], boundary[i+1]))
        blockages.append(vectors)

    return blockages
````

- 原来：`{[llx, lly, urx, ury], [x1, y1, x2, y2, x3, y3...], ... ,[[llx, lly, urx, ury]]}`
- 转为：`[[(llx, lly), (urx, ury)], [(x1, y1),(x2, y2), (x3, y3)...]]`。



## `getLayoutBorder`

```python
def getLayoutBorder(self, lpp, structure = ""):
```

- `lpp`：指定 layer 层；
- `structure`：没屁用（烂代码！）

首先通过 `getAllShapes` 获得路径，再从中找到面积最大的矩形，返回矩形的右上角 `[urx, ury]`。



