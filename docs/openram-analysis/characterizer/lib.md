# `lib` 类

`lib` 类用于完成 .lib 文件的创建：

````python
lib(out_dir=OPTS.output_path, sram=self.s, sp_file=sp_file)
````



## `prepare_tables`

### 1、获取 load/slew 列表

根据 `OPTS.use_specified_load_slew` 判断是否使用指定的值：

#### 1.1 使用指定的 load/slew

```python
self.load_scales = np.array(OPTS.load_scales)
self.load = tech.spice["dff_in_cap"]
self.loads = self.load_scales * self.load

self.slew_scales = np.array(OPTS.slew_scales)
self.slew = tech.spice["rise_time"]
self.slews = self.slew_scales * self.slew
self.load_slews = []
for slew in self.slews:
    for load in self.loads:
        self.load_slews.append((load, slew))
```

#### 1.2 不使用指定的 load/slew

````python
self.load_slews = OPTS.use_specified_load_slew
self.loads = []
self.slews = []
for load,slew in self.load_slews:
    self.loads.append(load)
    self.slews.append(slew)
````

### 2、转为 np.array

```python
self.loads = np.array(self.loads)
self.slews = np.array(self.slews)
```



## `create_corners`

### 1、获取配置选择的工艺角、电压、温度

````python
self.temperatures = OPTS.temperatures
self.supply_voltages = OPTS.supply_voltages
self.process_corners = OPTS.process_corners
````

### 2、获取特殊工艺值

````python
min_temperature = min(self.temperatures)
nom_temperature = tech.spice["nom_temperature"]
max_temperature = max(self.temperatures)
min_supply = min(self.supply_voltages)
nom_supply = tech.spice["nom_supply_voltage"]
max_supply = max(self.supply_voltages)
min_process = "FF"
nom_process = "TT"
max_process = "SS"
````

获得最大、最小与 nominal corner 的 温度、电压、工艺；

### 3、创建工艺角

创建数据结构来保持工艺角：

```python
self.corners = []
self.lib_files = []
```

需要根据配置中是否指定工艺角进行赋值：

#### 3.1 不使用指定的工艺角

先创建临时对象：

```python
corner_tuples = set()
```

再根据 `OPTS.only_use_config_corners`  是否使用配置中的工艺角进行赋值：

##### 3.1.1 使用配置的工艺角

那么就需要用到 [第一步配置的三个列表](###1、获取配置选择的工艺角、电压、温度)：

````python
for p in self.process_corners:
    for v in self.supply_voltages:
        for t in self.temperatures:
            corner_tuples.add((p, v, t))
````

##### 3.1.2 不使用配置的工艺角

````python
nom_corner = (nom_process, nom_supply, nom_temperature)
corner_tuples.add(nom_corner)
if not OPTS.nominal_corner_only:
    # Temperature corners
    corner_tuples.add((nom_process, nom_supply, min_temperature))
    corner_tuples.add((nom_process, nom_supply, max_temperature))
    # Supply corners
    corner_tuples.add((nom_process, min_supply, nom_temperature))
    corner_tuples.add((nom_process, max_supply, nom_temperature))
    # Process corners
    corner_tuples.add((min_process, nom_supply, nom_temperature))
    corner_tuples.add((max_process, nom_supply, nom_temperature))
````

那么就使用 nominal 的组合作为第一个工艺角。之后如果选项中配置的：`OPTS.nominal_corner_only` 不只使用  nominal，就需要再添加一些工艺角；

#### 3.2 使用指定的工艺角

````python
corner_tuples = OPTS.use_specified_corners
````

直接使用 `OPTS.use_specified_corners`。

### 4、添加工艺角

````python
for corner_tuple in corner_tuples:
    self.add_corner(*corner_tuple)
````

将第三步得到的 `corner_tuple`：一个列表，其中每个元素为：`(process, supply, temperature)`。

的每个元素调用 `self.add_corner`：

```python
def add_corner(self, proc, volt, temp):
    self.corner_name = "{0}_{1}_{2}V_{3}C".format(self.sram.name,
                                                  proc,
                                                  volt,
                                                  temp)
    self.corner_name = self.corner_name.replace(".","p") # Remove decimals
    lib_name = self.out_dir+"{}.lib".format(self.corner_name)

    # A corner is a tuple of PVT
    self.corners.append((proc, volt, temp))
    self.lib_files.append(lib_name)
```

函数首先根据传入的三个值，创建一个 .lib 文件每次。再向 `self.corners` 添加一个工艺角、`self.lib_files` 添加一个文件。

