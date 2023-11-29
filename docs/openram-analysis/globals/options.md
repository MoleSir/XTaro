# `options` 类

## Configuration options

- `openram_tech`：工艺库目录路径

- `tech_name`：工艺库名称

- `num_rw_ports`：读写端口数量

- `num_r_ports`：只读端口数量

- `num_w_ports`：只写端口数量

- `local_array_size`：将整个内存矩阵分若干小块（列）；

- `write_size`：每次写入的位数（跟内存位宽不同，还需要 mask）

- **选择工艺角**：

  下面三个对象控制最终生成 .lib 文件使用的工艺角。优先级从高到低：

  1. `use_specified_corners`：一个指定的 PVT 列表，如果不为 None，表示只对这列表中的 PVT 进行 .lib 文件创建；
  2. `only_use_config_corners`：是否使用用户配置的工艺角，即为 `supply_voltages`、`temperatures`、`process_corners` 的三循环组合；
  3. `nominal_corner_only`：只使用 nomial corner 工艺角，tech.py 中定义的 `spice["nom_temperature"]`、`spice["nom_supply_voltage"]`，`"TT"`。

  下面这个对象控制的是 `slew/load` 的选择：

  - `use_specified_load_slew`：
    - 如果其为空，`lib` 类使用 tech 中的 load、slew 乘上 `load_scales` 与 `slew_scales` 得到两个列表，将二者循环组合成最后的 load/slew tuples 列表。
    - 如果不为空，`lib` 类直接使用这个列表；

- **工艺角信息**：

  这个信息表示之后

  - `supply_voltages`：VDD 列表
  - `temperatures`：温度列表
  - `process_corners`：工艺角列表
  - `load_scales`：电容缩放值列表；
  - `slew_scales`：压摆缩放列表；

  如果用户没有配置，这些信息主要在 [`set_default_corner`](../03-init_openram().md) 函数中读取工艺库的值完成配置。

  前三者：`supply_voltages`、`temperatures` 与 `process_corners` 主要作用：

  - 在进行 functional 功能仿真时，使用三个元素的 0 号元素组成一个工艺角进行仿真。而实际上在功能仿真的时候只有电压被使用：作为产生波形的高电平电压值；

    ````python
    corner = (OPTS.process_corners[0], OPTS.supply_voltages[0], OPTS.temperatures[0])

  - 在 lib 类执行 `create_corners` 方法，创建不同的工艺角时会使用。只有配置了 `only_use_config_corners` ，并且不配置 `use_specified_corners` （即为：不使用特定工艺角，并且使用配置的工艺角）。就使用这三个列表的三次循环组合，对每个组合都会输出一个 .lib，计算延时、setup、hold、power。

    `simulation`、`delay`、`stimuli` 类构造时都需要确定一个工艺角。

    电压用来生成高电平电压、温度用于某些 spice 仿真器的 .TEMP 中指定温度、process corner 用于在 `stimuli` 类构造时选择 tech  文件中的 MOS 管模型：

    ```python
    self.device_models += tech.spice["fet_models"][self.process]
    ```

  后二者：`load_scales`、`slew_scales`。用来在不使用指定 load/slew 时，取得 tech.py 文件中的 load、slew 值，通过×这两个列表，得到多个 loads、slews，最后循环组成需要测试的所有 load/slew 组合。

- `num_banks`：bank 数量。源码很乱，看不出来。

- `words_per_row`：

- `num_spare_rows`、`num_spare_cols`：除了必要的一些 memory cells 外，增加空闲的行、列（损坏时使用？）；



## ROM configuration options

- `rom_endian`：大小端
- `rom_data`：
- `data_type`：
- `strap_spacing`：
- `scramble_bits`：



## Control logic options

- `rbl_delay_percentage`
- `multi_delay_chain_pinouts`
- `delay_chain_stages`：控制单元的 delay chain 阶数；
- `accuracy_requirement`：



## Debug options

- `openram_temp`：存在临时文件目录
- `verbose_level`：
- `debug`：是否输出 pds 失败信息；
- `sim_data_path`：
- `spice_raw_file`：



## Run-time vs accuracy options.

- `netlist_only`：只需要网标文件，不生产 gds
- `route_supplies`：是否进行最后的 power routing
- `supply_pin_type`：
- `check_lvsdrc`：是否进行 DRC/LVS
- `inline_lvsdrc`：是否对每个子模块进行 DRC/LVS
- `trim_netlist`：是否删除非关键存储单元以加快 characterization 速度
- `use_pex`：是否进行 PEX（技术参数提取）
- `output_extended_config`：是否输出全部的配置信息文件
- `output_datasheet_info`：是否输出 .html 手册
- `model_name`：决定使用哪个分析模型，允许：elmore，linear_regression；
- `write_graph`：



## Tool options

- `top_process`：

- `use_conda`：是否使用 conda 下载工具

- `spice_name`：spice 工具的名称

- `spice_exe`：可执行的 spice 工具路径

- `drc_name`：DRC 工具

- `lvs_name`：LVS 工具

- `pex_name`：PEX 工具

- `drc_exe`：DRC 路径；

- `lvs_exe`：LVS 路径；

- `pex_exe`：PEX 路径；

- `magic_exe`：For sky130, we need magic for filtering.

- `num_threads`：使用的线程数量

- `num_sim_threads`：运行 ngspice/hsipce 使用的线程数量

- `hier_seperator`：Some tools (e.g. Xyce) use other separators like ":"

- `print_banner`：是否在初始化时输出 banner

- `output_path`：文件输出目录

- `output_name`：输出文件名称；

- `analytical_delay`：Use analytical delay models by default

- 其他是一些默认的模块名称：

- ````python
  # Bitline, s_en and cell names used in characterizer
  bl_format = "X{name}{hier_sep}xbank0{hier_sep}bl_{row}_{col}"
  br_format = "X{name}{hier_sep}xbank0{hier_sep}br_{row}_{col}"
  sen_format = "X{name}{hier_sep}xbank0{hier_sep}s_en"
  cell_format = "X{name}{hier_sep}xbank0{hier_sep}xbitcell_array{hier_sep}xreplica_bitcell_array{hier_sep}xbitcell_array{hier_sep}xbit_r{row}_c{col}"
  
  # Random seed for functional simulation
  functional_seed = None
  
  # Route the input/output pins to the perimeter
  perimeter_pins = True
  
  # Detailed or abstract LEF view
  detailed_lef = False
  
  keep_temp = False
  
  # These are the default modules that can be over-riden
  bank_select = "bank_select"
  bitcell_array = "bitcell_array"
  bitcell = "bitcell"
  buf_dec = "pbuf"
  column_decoder = "column_decoder"
  column_mux_array = "column_mux_array"
  control_logic = "control_logic"
  decoder = "hierarchical_decoder"
  delay_chain = "delay_chain"
  dff_array = "dff_array"
  dff = "dff"
  inv_dec = "pinv"
  nand2_dec = "pnand2"
  nand3_dec = "pnand3"
  nand4_dec = "pnand4" # Not available right now
  precharge = "precharge"
  precharge_array = "precharge_array"
  ptx = "ptx"
  replica_bitline = "replica_bitline"
  sense_amp_array = "sense_amp_array"
  sense_amp = "sense_amp"
  tri_gate_array = "tri_gate_array"
  tri_gate = "tri_gate"
  wordline_driver = "wordline_driver"
  write_driver_array = "write_driver_array"
  write_driver = "write_driver"
  write_mask_and_array = "write_mask_and_array"