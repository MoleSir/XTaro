# `functional` 类

> Functions to write random data values to a random address then read them back and check for successful SRAM operation.



## `__init__`

````python
def __init__(self, sram, spfile=None, corner=None, cycles=15, period=None, output_path=None):
````

### 调用 `simulation` 构造

````python
super().__init__(sram, spfile, corner)
````

`functional` 类基础自 [`simulation`](./simulation.md) 类，主要使用其中添加测试数据的方法。

### 设置随机数 seed

因为会使用随机生成函数产生随机的激励序列：

```python
if OPTS.is_unit_test:
    random.seed(12345)
elif OPTS.functional_seed:
    random.seed(OPTS.functional_seed)
else:
    seed = time.time_ns()
    random.seed(seed)
```

### 拷贝 .sp 文件

````python
if not spfile:
    # self.sp_file is assigned in base class
    sram.sp_write(self.sp_file, trim=OPTS.trim_netlist)
# Copy sp file to temp dir
self.temp_spice = path.join(OPTS.openram_temp, "sram.sp")
shutil.copy(self.sp_file, self.temp_spice)
````

### 设置基本参数

```python
if not corner:
    corner = (OPTS.process_corners[0], OPTS.supply_voltages[0], OPTS.temperatures[0])

if not output_path:
    self.output_path = OPTS.openram_temp
else:
    self.output_path = output_path

if self.write_size != self.word_size:
    self.num_wmasks = int(math.ceil(self.word_size / self.write_size))
else:
    self.num_wmasks = 0

if not self.num_spare_cols:
    self.num_spare_cols = 0

self.max_data = 2 ** self.word_size - 1
self.max_col_data = 2 ** self.num_spare_cols - 1
if self.words_per_row > 1:
    self.addr_spare_index = -int(math.log(self.words_per_row) / math.log(2))
else:
    self.addr_spare_index = self.bank_addr_size
self.valid_addresses = set()
self.max_address = self.num_rows * self.words_per_row - 1
if OPTS.trim_netlist:
    for i in range(self.words_per_row):
        self.valid_addresses.add(i)
        self.valid_addresses.add(self.max_address - i - 1)
self.probe_address, self.probe_data = '0' * self.bank_addr_size, 0
self.set_corner(corner)
self.set_spice_constants()
self.set_stimulus_variables()
```

### 生成数据结构

```python
if period:
    self.period = period

self.wordline_row = 0
self.bitline_column = 0
self.create_signal_names()
self.bl_name = "xsram:xbank0:bl_0_{}"
self.br_name = "xsram:xbank0:br_0_{}"
self.sen_name = "xsram:s_en"
self.q_name, self.qbar_name = self.get_bit_name()

self.num_cycles = cycles
self.stored_words = collections.OrderedDict()
self.stored_spares = collections.OrderedDict()
self.read_check = []
self.read_results = []
```

最重要的是：

- `stored_words`：key 为地址，value 为内存对应地址的值。保存产生所有测试序列后，内存最终的值映射关系；
- `read_check`：
- `read_results`：

### 生成测试数据

````python
self.create_random_memory_sequence()
````

[`create_random_memory_sequence`](##`create_random_memory_sequence`)

### 写 sp 文件

`````python
self.write_functional_stimulus()
`````



## `create_random_memory_sequence`

### 操作类型

````python
if self.write_size != self.word_size:
    rw_ops = ["noop", "write", "partial_write", "read", "read"]
    w_ops = ["noop", "write", "partial_write"]
else:
    rw_ops = ["noop", "write", "read", "read"]
    w_ops = ["noop", "write"]
r_ops = ["noop", "read"]
````

### 第一个周期 idle 

````python
comment = self.gen_cycle_comment("noop", "0" * self.word_size, "0" * self.bank_addr_size, "0" * self.num_wmasks, 0, self.t_current)
self.add_noop_all_ports(comment)
````

### 产生写入测试

````python
for i in range(3):
    for port in self.write_ports:
        addr = self.gen_addr()
        (word, spare) = self.gen_data()
        combined_word = self.combine_word(spare, word)
        comment = self.gen_cycle_comment("write", combined_word, addr, "1" * self.num_wmasks, port, self.t_current)
        self.add_write_one_port(comment, addr, spare + word, "1" * self.num_wmasks, port)
        self.stored_words[addr] = word
        self.stored_spares[addr[:self.addr_spare_index]] = spare

    # All other read-only ports are noops.
    for port in self.read_ports:
        if port not in self.write_ports:
            self.add_noop_one_port(port)
    self.cycle_times.append(self.t_current)
    self.t_current += self.period
    self.check_lengths()
````

### 生成读数据测试

````python
for port in self.all_ports:
    if port in self.write_ports and port not in self.read_ports:
        self.add_noop_one_port(port)
    else:
        (addr, word, spare) = self.get_data()
        combined_word = self.combine_word(spare, word)
        comment = self.gen_cycle_comment("read", combined_word, addr, "0" * self.num_wmasks, port, self.t_current)
        self.add_read_one_port(comment, addr, port)
        self.add_read_check(spare + word, port)
self.cycle_times.append(self.t_current)
self.t_current += self.period
self.check_lengths()
````

### 生成若干随机读写测试

````python
for i in range(self.num_cycles):
    w_addrs = []
    for port in self.all_ports:
        if port in self.readwrite_ports:
            op = random.choice(rw_ops)
        elif port in self.write_ports:
            op = random.choice(w_ops)
        else:
            op = random.choice(r_ops)

        if op == "noop":
            self.add_noop_one_port(port)
        elif op == "write":
            addr = self.gen_addr()
            # two ports cannot write to the same address
            if addr in w_addrs:
                self.add_noop_one_port(port)
            else:
                (word, spare) = self.gen_data()
                combined_word = self.combine_word(spare, word)
                comment = self.gen_cycle_comment("write", combined_word, addr, "1" * self.num_wmasks, port, self.t_current)
                self.add_write_one_port(comment, addr, spare + word, "1" * self.num_wmasks, port)
                self.stored_words[addr] = word
                self.stored_spares[addr[:self.addr_spare_index]] = spare
                w_addrs.append(addr)
        elif op == "partial_write":
            # write only to a word that's been written to
            (addr, old_word, old_spare) = self.get_data()
            # two ports cannot write to the same address
            if addr in w_addrs:
                self.add_noop_one_port(port)
            else:
                (word, spare) = self.gen_data()
                wmask  = self.gen_wmask()
                new_word = self.gen_masked_data(old_word, word, wmask)
                combined_word = self.combine_word(spare, word)
                comment = self.gen_cycle_comment("partial_write", combined_word, addr, wmask, port, self.t_current)
                self.add_write_one_port(comment, addr, spare + word, wmask, port)
                self.stored_words[addr] = new_word
                self.stored_spares[addr[:self.addr_spare_index]] = spare
                w_addrs.append(addr)
        else:
            (addr, word) = random.choice(list(self.stored_words.items()))
            spare = self.stored_spares[addr[:self.addr_spare_index]]
            combined_word = self.combine_word(spare, word)
            # The write driver is not sized sufficiently to drive through the two
            # bitcell access transistors to the read port. So, for now, we do not allow
            # a simultaneous write and read to the same address on different ports. This
            # could be even more difficult with multiple simultaneous read ports.
            if addr in w_addrs:
                self.add_noop_one_port(port)
            else:
                comment = self.gen_cycle_comment("read", combined_word, addr, "0" * self.num_wmasks, port, self.t_current)
                self.add_read_one_port(comment, addr, port)
                self.add_read_check(spare + word, port)
    self.cycle_times.append(self.t_current)
    self.t_current += self.period

# Last cycle idle needed to correctly measure the value on the second to last clock edge
comment = self.gen_cycle_comment("noop", "0" * self.word_size, "0" * self.bank_addr_size, "0" * self.num_wmasks, 0, self.t_current)
self.add_noop_all_ports(comment)
````



## `write_functional_stimulus`