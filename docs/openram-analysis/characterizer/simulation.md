# `simulation` 类

## `__init__`

````python
def __init__(self, sram, spfile, corner):
    self.sram = sram

    self.name = self.sram.name
    self.word_size = self.sram.word_size
    self.bank_addr_size = self.sram.bank_addr_size
    self.write_size = self.sram.write_size
    self.num_spare_rows = self.sram.num_spare_rows
    if not self.sram.num_spare_cols:
        self.num_spare_cols = 0
    else:
        self.num_spare_cols = self.sram.num_spare_cols
    if not spfile:
        self.sp_file = OPTS.openram_temp + "sram.sp"
    else:
        self.sp_file = spfile

    self.all_ports = self.sram.all_ports
    self.readwrite_ports = self.sram.readwrite_ports
    self.read_ports = self.sram.read_ports
    self.write_ports = self.sram.write_ports
    self.words_per_row = self.sram.words_per_row
    self.num_rows = self.sram.num_rows
    self.num_cols = self.sram.num_cols
    if self.write_size != self.word_size:
        self.num_wmasks = int(math.ceil(self.word_size / self.write_size))
    else:
        self.num_wmasks = 0
````

参数 `sram` 是 `sram_1bank` 类实例化对象。



## `create_measurement_names`

创建测量名称：

```python
def create_measurement_names(self):
    self.delay_meas_names = ["delay_lh", "delay_hl", "slew_lh", "slew_hl"]
    self.power_meas_names = ["read0_power",
                             "read1_power",
                             "write0_power",
                             "write1_power",
                             "disabled_read0_power",
                             "disabled_read1_power",
                             "disabled_write0_power",
                             "disabled_write1_power"]
```

保存到 `self.delay_meas_names` 与 `self.power_meas_names`。



## `set_corner`

````python
def set_corner(self, corner):
    self.corner = corner
    (self.process, self.vdd_voltage, self.temperature) = corner
````

设置工艺角参数，传入一个三个元素的元素 `corner`。赋值给 `self.corner`，并且拆包三个部分。



## `set_spice_constants` 

````python
def set_spice_constants(self):
    self.period = tech.spice["feasible_period"]
    self.slew = tech.spice["rise_time"] * 2
    self.load = tech.spice["dff_in_cap"] * 4

    self.v_high = self.vdd_voltage - tech.spice["nom_threshold"]
    self.v_low = tech.spice["nom_threshold"]
    self.gnd_voltage = 0
````

从工艺库的 `spice` 参数读取信息。例如：feasible period、rise time、diff in cap。会需要计算有效高、低电平。



## `create_signal_names`

获得内存所有的引脚名称列表：

````python
def create_signal_names(self):
    self.addr_name = "a"
    self.din_name = "din"
    self.dout_name = "dout"
    self.pins = self.gen_pin_names(port_signal_names=(self.addr_name, self.din_name, self.dout_name),
                                   port_info=(len(self.all_ports), self.write_ports, self.read_ports),
                                   abits=self.bank_addr_size,
                                   dbits=self.word_size + self.num_spare_cols)
````

1. 首先定义地址、输入、输出引脚名称前缀；
2. 调用 [`gen_pin_names`](##`gen_pin_names`) 创建引脚名称列表保存到 `self.pins`。其中的引脚信息来自构造函数从 `self.sram` 拷贝而来的信息。

  

## `set_stimulus_variables`

````python
def set_stimulus_variables(self):
````

设置 stimulus 的变量。执行流程为：

> `self.all_ports = list(range(total_ports))`

### 时钟信息

````python
self.cycle_times = []
self.t_current = 0
````

### 控制端

````python
self.csb_values = {port: [] for port in self.all_ports}
self.web_values = {port: [] for port in self.readwrite_ports}
````

- 所有端口都有 `csb`，遍历所有端口序号作为 key，一个空列表作为 value。
- 同理对 `web`。

### 地址与数据

````python
self.addr_value = {port: [] for port in self.all_ports}
self.data_value = {port: [] for port in self.write_ports}
self.wmask_value = {port: [] for port in self.write_ports}
self.spare_wen_value = {port: [] for port in self.write_ports}
````

跟 控制端 的赋值类似。

### 地址与数据（三维？）

````python
self.addr_values = {port: [[] for bit in range(self.bank_addr_size)] for port in self.all_ports}
self.data_values = {port: [[] for bit in range(self.word_size + self.num_spare_cols)] for port in self.write_ports}
self.wmask_values = {port: [[] for bit in range(self.num_wmasks)] for port in self.write_ports}
self.spare_wen_values = {port: [[] for bit in range(self.num_spare_cols)] for port in self.write_ports}
````

之前创为地址与数据创建的字典是针对端口的，不是引脚。这里创建的就针对每个引脚都有一个列表。

key 是端口序号，value 是列表的列表，每个元素是空列表，对应一个引脚。

### 注释

```python
self.cycle_comments = []
self.fn_cycle_comments = []
```



## `set_probe`

````python
def set_probe(self, probe_address, probe_data):
````

- `probe_address`：以二进制字符串形式给定的输入地址；
- `probe_address`：以整数形式给定的数据；

函数执行流程为：

### 保存数据

````python
self.probe_address = probe_addresse
self.probe_data = probe_data
````

保存输入参数。

### 计算 bitline_column 与 wordline_row 

```python
    self.bitline_column = self.get_data_bit_column_number(probe_address, probe_data)
    self.wordline_row = self.get_address_row_number(probe_address)

def get_data_bit_column_number(self, probe_address, probe_data):
    """Calculates bitline column number of data bit under test using bit position and mux size"""

    if self.sram.col_addr_size>0:
        col_address = int(probe_address[0:self.sram.col_addr_size], 2)
    else:
        col_address = 0
    bl_column = int(self.sram.words_per_row * probe_data + col_address)
    return bl_column

def get_address_row_number(self, probe_address):
    """Calculates wordline row number of data bit under test using address and column mux size"""

    return int(probe_address[self.sram.col_addr_size:], 2)
```



## `add_contorl_one_port`

````python
def add_control_one_port(self, port, op):
````

- `port`：指定端口序号；
- `op`：指定操作类型（字符串）：`"read", "write", "noop"`

### 可知端口求值

````python
web_val = 1
csb_val = 1
if op == "read":
    csb_val = 0
elif op == "write":
    csb_val = 0
    web_val = 0
elif op != "noop":
    # error!!!
````

- 读操作：`web_val = 1`，`csb_val = 0 `；
- 写操作：`web_val = 0`，`csb_val = 0 `；
- noop 操作：`web_val = 1`，`csb_val = 1 `；

### 保存值

```python
self.csb_values[port].append(csb_val)
if port in self.readwrite_ports:
    self.web_values[port].append(web_val)
```

计算得到的值添加到 `csb_values` 与 `web_values` 中。这两个结构在 [`set_stimulus_variables`](##`set_stimulus_variables`) 中创建。



## `add_data`

 ````python
 def add_data(self, data, port):
 ````

- `data`：二进制字符串给定的输入值；
- `port`：端口序号；

### 添加 `data_value`

````python
self.data_value[port].append(data)
````

### 添加 `data_values`

````python
bit = self.word_size + self.num_spare_cols - 1
for c in data:
    if c=="0":
        self.data_values[port][bit].append(0)
    elif c=="1":
        self.data_values[port][bit].append(1)
    else:
        debug.error("Non-binary data string", 1)
    bit -= 1
````

> key 都是端口号，`data_value` 的 value 为列表，保存不同的输入值（使用字符串二进制表示），`data_values` 的 value 为列表的列表，每个列表对应一个引脚，使用 0 或 1 表示不同的输入。
>
> 所以可以保存很多组的输入数据。第一次输入保存在 `data_value[port][0]`，以此类推。。。



## `add_address`

```python
def add_address(self, address, port):
    self.addr_value[port].append(address)
    bit = self.bank_addr_size - 1
    for c in address:
        if c=="0":
            self.addr_values[port][bit].append(0)
        elif c=="1":
            self.addr_values[port][bit].append(1)
        else:
            debug.error("Non-binary address string", 1)
        bit -= 1
```

与 `add_data` 方法相同。



## `add_write`

````python
def add_write(self, comment, address, data, wmask, port):
````

- `comment`：字符串，注释信息；
- `address`：二进制字符串形式给定的地址；
- `data`：二进制字符串形式给地的收入数据；
- `port`：端口序号；

### 处理注释

```python
self.fn_cycle_comments.append(comment)
self.append_cycle_comment(port, comment)
```

首先添加 `comment` 到 `fn_cycle_comments` 列表。进入 `append_cycle_comment` 函数，也是增加注释，只不过多了一些时序的信息。

### 更新时间

````python
self.cycle_times.append(self.t_current)
self.t_current += self.period
````

### 设置端口信息

```python
self.add_control_one_port(port, "write")
self.add_data(data, port)
self.add_address(address, port)
```

调用之前分析的几个函数，把控制端值、地址、数据加入到对应数据结构中。

### 处理其他端口

````python
for unselected_port in self.all_ports:
    if unselected_port != port:
        self.add_noop_one_port(unselected_port)
````

对除了  `port` 之外的所有端口调用 `add_noop_one_port`，添加 noop 输入信息。（表示无效输入）



## `add_write_one_port`

````python
def add_write_one_port(self, comment, address, data, wmask, port):
    self.fn_cycle_comments.append(comment)

    self.add_control_one_port(port, "write")
    self.add_data(data, port)
    self.add_address(address, port)
    self.add_wmask(wmask, port)
    self.add_spare_wen("0" * self.num_spare_cols, port)
````

根据 `add_write` 相比，只少了：添加带时间信息的注释、不更新时间戳、不给其他引脚添加无效仿真信息。



## `add_read`

```python
def add_read(self, comment, address, port):
```

- `comment`：字符串，注释信息；
- `address`：二进制字符串形式给定的地址；
- `port`：端口序号；

### 处理注释

```python
self.fn_cycle_comments.append(comment)
self.append_cycle_comment(port, comment)
```

首先添加 `comment` 到 `fn_cycle_comments` 列表。进入 `append_cycle_comment` 函数，也是增加注释，只不过多了一些时序的信息。

### 更新时间

````python
self.cycle_times.append(self.t_current)
self.t_current += self.period
````

### 设置端口信息

````python
self.add_control_one_port(port, "read")
self.add_address(address, port)
````

如果对允许读写的端口进行写入，还需要设置一次 data：

````python
if port in self.write_ports:
    try:
        self.add_data(self.data_value[port][-1], port)
    except:
        self.add_data("0" * (self.word_size + self.num_spare_cols), port)
````

### 处理其他端口

````python
for unselected_port in self.all_ports:
    if unselected_port != port:
        self.add_noop_one_port(unselected_port)
````

对除了  `port` 之外的所有端口调用 `add_noop_one_port`，添加 noop 输入信息。（表示无效输入）



## `add_read_one_port`

````python
def add_read_one_port(self, comment, address, port):
    self.fn_cycle_comments.append(comment)

    self.add_control_one_port(port, "read")
    self.add_address(address, port)

    if port in self.write_ports:
        try:
            self.add_data(self.data_value[port][-1], port)
        except:
            self.add_data("0" * (self.word_size + self.num_spare_cols), port)
        try:
            self.add_wmask(self.wmask_value[port][-1], port)
        except:
            self.add_wmask("0" * self.num_wmasks, port)
        self.add_spare_wen("0" * self.num_spare_cols, port)
````

根据 `add_read` 相比，只少了：添加带时间信息的注释、不更新时间戳、不给其他引脚添加无效仿真信息。



## `gen_pin_names`

函数根据传入的参数，返回一个字符串列表，包含这个内存所有的引脚（注意是引脚）名称。

> Creates the pins names of the SRAM based on the no. of ports.
>
> ````python
> # This may seem redundant as the pin names are already defined in the sram. However, it is difficult
> # to extract the functionality from the names, so they are recreated. As the order is static, changing
> # the order of the pin names will cause issues here.
> ````

```python
def gen_pin_names(self, port_signal_names, port_info, abits, dbits):
```

- `port_signal_names`：一个字符串列表，保存内存最需要的三个引脚的名称前缀：地址、数据、输出；
- `prot_info`：元组，保存三个信息：总端口数量、允许写端口的序号列表、允许读端口的序号列表；
- `abits`：地址线宽度；
- `wbits`：数据线宽度；

### 拆包

根据 `port_signal_names` 与 `port_info` 格式拆包得到信息：

````python
pin_names = []
(addr_name, din_name, dout_name) = port_signal_names
(total_ports, write_index, read_index) = port_info
````

### 添加读引脚名称

````python
for write_input in write_index:
    for i in range(dbits):
        pin_names.append("{0}{1}_{2}".format(din_name, write_input, i))
````

两次循环：

1. 遍历 `write_index`，即所有允许写数据的端口序号；
2. 遍历 `range(dbits)`，即输入数据的每个 bits。

给 `pin_names` 增加名称：`"{0}{1}_{2}".format(din_name, write_input, i)`。例如：`"[d0_0", "d0_1", "d2_0", "d2_1"]`。

### 添加地址引脚名称

````python
for port in range(total_ports):
    for i in range(abits):
        pin_names.append("{0}{1}_{2}".format(addr_name, port, i))
````

与增加 读引脚 类似，只不过，因为不管什么类似的端口都需要地址，所以第一层循环直接遍历 `range(total_ports)`。

例如：`a0_0, a0_1, a0_2, a1_0, a1_1, a1_2`。

### 添加每个端口的片选与写使得

````python
for port in range(total_ports):
    pin_names.append("CSB{0}".format(port))
for port in range(total_ports):
    if (port in read_index) and (port in write_index):
        pin_names.append("WEB{0}".format(port))
````

注意：写使能只有允许写的端口有，所以增加：

```python
if (port in read_index) and (port in write_index):
```

> ？？？为什么要判断是否在 `read_index` 里面？

### 添加时钟引脚

````python
for port in range(total_ports):
    pin_names.append("{0}{1}".format("clk", port))
````

### 增加读端口的输出引脚

```python
for read_output in read_index:
    for i in range(dbits):
        pin_names.append("{0}{1}_{2}".format(dout_name, read_output, i))
```

### 增加电源、地

````python
pin_names.append("{0}".format("vdd"))
pin_names.append("{0}".format("gnd"))
return pin_names
````



## `append_cycle_comment`

````python
def append_cycle_comment(self, port, comment):
    time = "{0:.2f} ns:".format(self.t_current)
    time_spacing = len(time) + 6
    self.cycle_comments.append("Cycle {0:<6d} Port {1:<6} {2:<{3}}: {4}".format(len(self.cycle_times),
                                                                                port,
                                                                                time,
                                                                                time_spacing,
                                                                                comment))

````

给指定端口序号 `port` 添加一条注释 `comment`。但会添加一些时序信息。



## `add_noop_one_port`

````python
def add_noop_one_port(self, port):
````

- `port`：指定端口序号；

### 控制引脚信息

```python
self.add_control_one_port(port, "noop")
```

调用 [`add_control_one_port`](##`add_contorl_one_port`)，传入 `"noop"` 写入非使能位到 web 与 scb。

### 地址引脚

````python
try:
    self.add_address(self.addr_value[port][-1], port)
except:
    self.add_address("0" * self.bank_addr_size, port)
````

先尝试写入上一次地址，如果没有上一次地址，则写入 0 地址。（[`add_address`](##`add_address`))

### 写入数据引脚

````python
if port in self.write_ports:
    try:
        self.add_data(self.data_value[port][-1], port)
    except:
        self.add_data("0" * (self.word_size + self.num_spare_cols), port)
````

首先要判断 `port` 是不是允许写入端口。先尝试写入上一次值，如果没有上一次值，则写入 0。（[`add_data`](##`add_data`)）



## `add_noop_clock_one_port`

````python
def add_noop_clock_one_port(self, port):
    self.fn_cycle_comments.append('Clock only on port {}'.format(port))
    self.append_cycle_comment(port, 'Clock only on port {}'.format(port))

    self.cycle_times.append(self.t_current)
    self.t_current += self.period

    self.add_noop_one_port(port)

    # Add noops to all other ports.
    for unselected_port in self.all_ports:
        if unselected_port != port:
            self.add_noop_one_port(unselected_port)
````

给所有端口都增加一次无效仿真信息。并且添加一条关于 `port` 的注释。跟 `add_noop_all_ports` 差不多~



## `add_noop_all_ports`

```python
def add_noop_all_ports(self, comment):
```

给所有端口写入一次无效仿真信息：

````python
def add_noop_all_ports(self, comment):
    self.fn_cycle_comments.append(comment)
    self.append_cycle_comment("All", comment)

    self.cycle_times.append(self.t_current)
    self.t_current += self.period

    for port in self.all_ports:
        self.add_noop_one_port(port)
````



## `format_value`

### `combine_word`

````python
def combine_word(self, spare, word):
    if len(spare) > 0:
        return spare + "+" + word
    return word
````

### `delineate`

````python
def delineate(word):
    # Create list of chars in reverse order
    split_word = list(reversed([x for x in word]))
    # Add underscore every 4th char
    split_word2 = [x + '_' * (n != 0 and n % 4 == 0) for n, x in enumerate(split_word)]
    # Join the word unreversed back together
    new_word = ''.join(reversed(split_word2))
    return (new_word)
````

函数输入一个字符串，将字符串每个 4 个字符添加 `_` 后返回：

cdefghijklmnopqrstuvwxyz ==> cdef_ghij_klmn_opqr_stuv_wxyz

abcdefghijklmnopqrstuvwxyz  ==> ab_cdef_ghij_klmn_opqr_stuv_wxyz

### 处理字符串

```python
def format_value(self, value):
    vals = delineate(vals)
    spare_vals = delineate(spare_vals)

    return self.combine_word(spare_vals, vals)
```



## `gen_cycle_comment`

````python
def gen_cycle_comment(self, op, word, addr, wmask, port, t_current):
````

- `op`：内存操作：noop、write、partial_write，其他；
- `word`：二进制格式字符串，作为数据输入；
- `addr`：二进制格式字符串，作为地址输入；
- `port`：指定端口序号；
- `t_current`：当前时刻

函数根据操作类型，使用参数生成注释：

````
if op == "noop":
    str = "\tIdle during cycle {0} ({1}ns - {2}ns)"
    comment = str.format(int(t_current / self.period),
                         t_current,
                         t_current + self.period)
elif op == "write":
    comment = "\tWriting {0}  to  address {1} (from port {2}) during cycle {3} ({4}ns - {5}ns)".format(word,
                                                                                                       addr,
                                                                                                       port,
                                                                                                       int(t_current / self.period),
                                                                                                       t_current,
                                                                                                       t_current + self.period)
elif op == "partial_write":
    str = "\tWriting (partial) {0}  to  address {1} with mask bit {2} (from port {3}) during cycle {4} ({5}ns - {6}ns)"
    comment = str.format(word,
                         addr,
                         wmask,
                         port,
                         int(t_current / self.period),
                         t_current,
                         t_current + self.period)
else:
    str = "\tReading {0} from address {1} (from port {2}) during cycle {3} ({4}ns - {5}ns)"
    comment = str.format(word,
                         addr,
                         port,
                         int(t_current / self.period),
                         t_current,
                         t_current + self.period)

return comment
````



## `set_internal_spice_names`

