# `timing_graph` 类

> Implements a directed graph. Nodes are currently just Strings.



## `__init__`

````python
def __init__(self):
    self.graph = defaultdict(set)
    self.all_paths = []
    self.edge_mods = {}
````

- `graph`：指定值为 `set`。key 是 node 名称，value 作为集合保存这个 node 的输出 nodes；
- `all_paths`：
- `edge_mods`：key 是一个元组：`(src_node, dest_node)`，表示指定 node 到指定 node；value 是 连接二者的边对象；



## `get_all_paths`

> Traverse all paths from source to destination

````python
def get_all_paths(self, src_node, dest_node, remove_rail_nodes=True, reduce_paths=True):
````

- `src_node`：起始节点名称；
- `dest_node`：中止节点名称；
- `remove_rail_nodes`：是否不计 vdd 与 gnd 路径；
- `reduce_paths`：

### 创建数据结构

````python
# Mark all the vertices as not visited
visited = set()

# Create an array to store paths
path = []
self.all_paths = []
````

- `visited`：保存处理过的节点；
- `path`：保存当前经过的路径（从起点到终点会有很多路径，`path` 保存当前正遍历的一条；
- `self.all_paths`：保存最终的路径，每个元素就是 `path`；

### `get_all_paths_util`

调用 `get_all_paths_util` 完成真正的工作： 

````python
self.get_all_paths_util(src_node, dest_node, visited, path)
````

函数定义如下：

````python
def get_all_paths_util(self, cur_node, dest_node, visited, path):
    visited.add(cur_node)
    path.append(cur_node)

    if cur_node == dest_node:
        self.all_paths.append(copy.deepcopy(path))
    else:
        for node in self.graph[cur_node]:
            if node not in visited:
                self.get_all_paths_util(node, dest_node, visited, path)

    path.pop()
    visited.remove(cur_node)
````

1. 向 `visited` 集合添加 `cur_node`，表示这个节点被处理过；

2. 向 `path` 列表添加 `cur_node`，表示给路径添加一个新的节点；

3. 判断是否达到终点：`if cur_node == dest_node:`

   1. 如果是：`path` 就保存了一条完整的路径，添加到 `self.all_paths` 中；

   2. 如果不是：遍历 `cur_node` 的所有下一个节点，如果节点没有被处理过，递归调用：

      ````python
      self.get_all_paths_util(node, dest_node, visited, path)
      ````

4. 处理完这个节点后，将节点从 `path` 弹出（这样 `path` 可以继续处理其他路径，很巧妙的设计）；

5. 同理，弹出 `visited` 中的 `cur_node`，这样其他路径又可能遍历到这个节点；



## `get_timing`

> Returns the analytical delays in the input path

```python
def get_timing(self, path, corner, slew, load, params):
```

- `path`：列表形式给定的路径，每个元素为线的名称，两个元素（两个线名称）确定一条 timing arch；
- `corner`：指定工艺角；
- `slew`：指定输入 `slew`；
- `load`：指定输出 `load`；
- `params`：计算的参数，例如使用什么延时模型；

### 创建保持数据对象

````python
delays = []
cur_slew = slew
````

- `delays`：
- `cur_slew`：

### 遍历给定路径

````python
for i in range(len(path) - 1):
	path_edge_mod = self.edge_mods[(path[i], path[i + 1])]
````

获取相邻两个 `path`，到 `self.edge_mods` 中查找，得到了两个线之间的 timing arch 模块 `path_edge_mod`；

### 获得输出线的输出负载电容

````python
cout = 0
for node in self.graph[path[i + 1]]:
    output_edge_mod = self.edge_mods[(path[i + 1], node)]
    if params["model_name"] == "cacti":
        cout+=output_edge_mod.get_input_capacitance()
    elif params["model_name"] == "elmore":
        cout+=output_edge_mod.get_cin()
    else:
        debug.error
````

1. 创建 `cout` 对象；
2. 以本次 timing arch 的 终点线为 起点，遍历其所有的 timing arch 的电路模型，根据 `params["model_name"]` 所指定的延时模型，调用 电路模型的 不同方法，得到电容，并且累加到 `cout` 上；

并且如果本次为最后一条需要计算的路径，增加 `load` 到 电容值上：

````python
if i == len(path) - 2:
    cout += load
````

### 计算延时

````python
if params["model_name"] == "cacti":
    delays.append(path_edge_mod.cacti_delay(corner, cur_slew, cout, params))
    cur_slew = delays[-1].slew
elif params["model_name"] == "elmore":
    delays.append(path_edge_mod.analytical_delay(corner, cur_slew, cout))
else:
    debug.error
````

根据 `params["model_name"]` 所指定的延时模型，调用 本次计算的 timing arch 所在模块 `path_edge_mod` 的不同方法计算得到延时：

- cacti 模型：
  1. 调用 [`cacti_delay`](./spice.md/###`cacti_delay`)，传入：工艺角、当前的输入传播时间 `slew`，输出电容值 `cout`，相关参数 `params`，得到 delay 后加入到 `dalyes` 列表中，表示本次计算得到的延时。
  2. 更新 `slew`：每次都使用上一个路径输入 slew，很合理！
- elmore 模型：
  1. 调用 `analytical_delay`，传入：工艺角、当前的输入传播时间 `slew`，输出电容值 `cout`，得到 delay 后加入到 `dalyes` 列表中，表示本次计算得到的延时。

### 返回延时

最后返回 `delays`，其中的每个元素对应输入 `path` 路径中两两导线的延时。

````python
return dalyes
````

返回的是延时对象 [`delay_data`](./delay_data.md) ，其中包含 slew 与 delay 信息；

