# drc 

分析 drc/design_rules、drc/drc_lut、drc/drc_value



## `drc_value` 类

```python
class drc_value():
    def __init__(self, value):
        self.value = value

    def __call__(self, *args):
        return self.value
```

构造函数传入 `value` 被类对象保存。重写 `__call__` 使得在调用 `obj()` 时返回构造时候传入的值。



## `drc_lut` 类

> Implement a lookup table of rules. 
>
> Each element is a tuple with the last value being the rule.
>
> It searches through backwards until all of the key values are met and returns the rule value.
>
> For exampe, the key values can be width and length, and it would return the rule for a wire of at least a given width and length.
>
> A dimension can be ignored by passing inf.

实现规则的查找表。

每个元素都是一个元组，元组的最后一个值是规则。

它向后搜索，直到满足所有键值，并返回规则值。

例如，键值可以是 width 和 length，它将为至少给定宽度和长度的导线返回规则。

通过传递 `inf` 可以忽略维度。

### `__init__`

````python
def __init__(self, table):
    self.table = table
````

传入的是一个字典对象。例如：

````python
drc_lut({(0.00, 0.0): 0.14,
         (0.27, 0.9): 0.27,                       
         (0.50, 1.8): 0.5,
         (0.90, 2.7): 0.9,
         (1.50, 4.0): 1.5})
````

### `match`

````python
def match(self, key1, key2):
    debug.check(len(key1) == len(key2), "Comparing invalid key lengths.")
    for k1, k2 in zip(key1, key2):
        if k1 < k2:
            return False
    return True
````

对两个参数 `key1`、`key2`（都为元组），如果 `key1` 中的所有元素都大于或者等于 `key2` 中对应的元素，返回 `True` 否则为 `False`。

### `__call__`

````python
def __call__(self, *key):
    if len(key)==0:
        first_key = list(sorted(self.table.keys()))[0]
        return self.table[first_key]

    for table_key in sorted(self.table.keys(), reverse=True):
        if self.match(key, table_key):
            return self.table[table_key]
````

参数是要要需要的规则名称列表。

- 如果没有传入任何参数。将 `self.table` 键进行排序，取出第一个（最小值）赋值给 `first_key`，之后返回这个`key` 对应的值；
- 如果有参数，从表的最大键开始判断，使用 `match` 比较 `key` 列表中的所有值是不是都大于 `table_key`，如果找到就返回这个 `table_key` 对应的值。也就是找到第一个比 `key` 小的键值。



## `design_rules` 类

> This is a class that implements the design rules structures.

### `__init__`

````python
def __init__(self, name):
    self.tech_name = name
    self.rules = {}
````

传入工艺卡名称保存到 `tech_name`，创建空字典 `rules` 放置规则。

### `add`

````python
def add(self, name, value):
    self.rules[name] = value
````

给 `self.rules` 增加指定的键值对。

### `__call__`

````python
def __call__(self, name, *args):
    rule = self.rules[name]
    if callable(rule):
        return rule(*args)
    else:
        return rule
````

传入需要的规则名称 `name` 与参数列表 `args`。

1. 通过 `name` 从 `self.rules` 获取对应的 `rule`；
2. 如果 `rule` 重写了 `__call__`，调用 `rule(*args)`；
3. 否则直接返回 `rule`。

### 模仿字典

重写 `__setitem__`、`__contains__` 与 `__getitem__` 等使得类对象可以完成字典的操作。实际上都是对内部的 `self.rules` 字典操作：

````python
def __setitem__(self, b, c):
    self.rules[b] = c

def __contains__(self, b):
    return b in self.rules

def __getitem__(self, b):
    rule = self.rules[b]
    if not callable(rule):
        return rule
    else:
        debug.error("Must call complex DRC rule {} with arguments.".format(b),-1)

def keys(self):
    return self.rules.keys()
````

### `add_layer`

````python
def add_layer(self, name, width, spacing, area=0):
    # Minimum width
    self.add("minwidth_{}".format(name), width)
    # Minimum spacing (could be a table too)
    self.add("{0}_to_{0}".format(name), spacing)
    # Minimum area
    self.add("minarea_{}".format(name), area)
````

增加某 layer 的规则信息：`width`、`spacing`、`area`。

实际上就是调用三个 `add`，规则名称如上，规则值为 `width`、`spacing`、`area`。

### `add_enclosure`

````python
def add_enclosure(self, name, layer, enclosure, extension=None):
    self.add("{0}_enclose_{1}".format(name, layer), enclosure)
    # Reserved for asymmetric enclosures
    if extension:
        self.add("{0}_extend_{1}".format(name, layer), extension)
    else:
        self.add("{0}_extend_{1}".format(name, layer), enclosure)
````

增加 enclosure 规则。

