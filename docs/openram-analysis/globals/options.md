# `options` 类

## Configuration options

- `openram_tech`：工艺库目录路径
- `tech_name`：工艺库名称
- `num_rw_ports`：读写端口数量
- `num_r_ports`：只读端口数量
- `num_w_ports`：只写端口数量
- `local_array_size`：将整个内存矩阵分若干小块（列）；
- `write_size`：每次写入的位数（跟内存位宽不同，还需要 mask）
- `nominal_corner_only`：
- `supply_voltages`：VDD 列表；
- `temperatures`：温度列表
- `process_corners`：工艺角列表；
- `load_scales`
- `slew_scales`
- `num_banks`：
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
- `only_use_config_corners`：是否只使用用户配置的工艺角；
- `use_specified_corners`：PVT 列表，只有这个列表中的 PVT 会被 characterized；
- `sim_data_path`：
- `use_specified_load_slew`：load/slew tuples 列表。load => 电容，slew => 传播延时的倒数
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
- 