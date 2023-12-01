# LOG

## 2023/11/21

- `timing_graph` 类

- 两种延时模型：cacti、elmore

- 输出 `.sp` 的区别，调用 `sp_write` 时，`lvs` 参数与 `trim` 参数不同：
  - `.sp`：`lvs` 与 `trim` 都为 `False`；
  - `trimmed.sp`：`lvs` 为 `False`，`trim` 为 `True`；
  - `.lvs.sp`：`lvs` 为 `True`，`trim` 为 `False`；
  
- `sram.sp` 与 `trimmed.sp` 区别：调用 `sp_write` 时，后者 `trim` 参数为 `True`；

- `delay_means.sp` 与 `delay_stim.sp` 文件由 `delay` 类写入；

- `fuctional_means.sp` 与 `functional_stim.sp` 由 `functional` 类写入；

- `design` 类中包含 `readwrite_ports`、`write_ports`、`read_ports` 等属性，都是保存内存端口的序号（不是端口引脚数量！）。

  三种类似：读写、只写、只读。分别根据配置中的 读写、只写、只读端口数量从 0 开始分配端口序号（每个端口表示一组 地址、输入（写需要）、输出）。

  这些数据结构保存端口的序号。例如 读写、只写、只读 端口数量分别为：2、1、1。那么一共需要 4 个端口，序号为 0、1、2、3。0、1 为读写、2 为只写、3 为只读。那么三个列表为：

  - `readwrite_ports = [0, 1]`
  -  `write_ports = [0, 1, 2]`
  - `read_ports = [0, 1, 3]`

- `simulation` 类：传入内存信息（端口数量、数据地址位宽），可以向其中添加若干次仿真引脚输入信息。

- `lib` 类将 .lib 文件需要的信息保存到 `self.times` 中（setup、hold）与 `self.char_port_results`（delay）中；

- `lib` 在构造函数中：调用 `characterize_corners`，对每个工艺角：

  1. 调用 `compute_delay` 计算延时
  2. 调用 `compute_setup_hold` 计算 setup 与 hold，
  3. 再将创建 .lib 写入。



## 2023/11/22

- `sram.sp` 文件在 `lib ` 构造函数中，由 `sram_16x2.sp` 拷贝而来；
- `functional` 类：随机生成测试（读、写）、检测，再写入 .sp 文件：
  - functional_meas.sp：测量信息；
  - functional_stim.sp：激励信息（指定时刻，指定端口的电压）；



## 2023/11/23

- spice 命令：.meas：
  - `.meas tran vdout0_0ck1980 FIND v(dout0_0) AT=1990.05n`  ==>  在 1990.05ns 时，测量 dout0_0 的电压值，标记为 vdout0_0ck1980。
  - `.meas tran read1_power0 avg par('(-1*v(vdd)*I(vvdd))') from=0n to=0n` ==> 测量 0n - 0n 之间 `(-1*v(vdd)*I(vvdd))` 的平均值，即为瞬时功率。
  - `.meas tran delay_lh0 TRIG v(clk0) VAL=2.5 FALL=1 TD=0n TARG v(dout0_1) VAL=2.5 RISE=1 TD=0n`：==> 测量时间差 delay_lh0。起始时间规定为：在 0n 之后，clk0 电压下降沿首次达到 2.5v；终止时间规定为：在 0n 之后，dout0_1 电压上升沿首次达到 2.5v；
  
- spice 命令：创建电源：
  - `Vclk0 clk0 0 PULSE (0 3.3 9.95n 0.1n 0.1n 4.9n 10n)`：定义 CLK0 为方波，`()` 中的量分布代表：Vinitial：初始电压、Von：高电平电压、Tdelay：延时时间、Trise：波形的上升时间、Tfall：波形的下降时间、Ton：导通时间、Tperiod：周期。
  - `Vdin0_1  din0_1  0 PWL (0n 0v 0.0n 0v 0.0n 5v 0.0n 5v 0.0n 0v 0.0n 0v...)`：Piece-wise linear 电源，`()` 为电源信息，以 时刻、电压 组合出现；

- function_stim.sp 与 function_meas.sp 由 `functional` 类输出，前者给定 sram 各个引脚的输入电压（何时给何值），后者要求 spice 仿真器在何时对何端口进行电压测量（.meas 命令）。

  这两个文件以及 sram.sp 给到 spice 仿真器，可以验证 sram 的写入、读取功能是否正常；

- delay_stim.sp 与 delay_meas.sp 由 `delay` 类输出，前面给定 sram 各个引脚的输入电压（何时给何值），后者要求 spice 仿真器对电压、延时等信息进行测量（.meas 命令）。

  这两个文件以及 sram.sp 给到 spice 仿真器，可以计算 sram 的 slew、delay、power；

- `lib` 类对象的 `compute_delay` 调用 `self.d.analyze` ==> `simulate_loads_and_slews` 进行 spice 仿真，得到 delay 信息写入 .lib 文件；

  `simulate_loads_and_slews` ==> `run_delay_simulation` ==> 先 `write_delay_stimulus` 创建仿真文件，再 `self.stim.run_sim` 运行仿真，结果保存在 openram_temp 目录下的临时文件。

  调用 `self.check_measurements` ==调用 `spice_measurement` 的 `retrieve_measure ` ==> `charutils` 模块的 `parse_spice_list` 解析 spice 运行的临时文件，得到 delay 数据。

- 每个 `spice_measurement` 类对象对应一条 .sp 的 .meas 命令！

- `spice_measurement` 类族功能：

  1. 根据自身测量类型（delay、slew、power）与仿真参数生成对应的 .spice 仿真命令；
  2. 解析仿真结果；

- `functional` 只验证读写功能，所以只用到了 `voltage_at_measure` 类；

- `lib` 类对象的 `compute_setup_hold` 创建 `setup_hold` 类对象，调用其 `analytical_setuphold` 或者 `analyze` 计算 setup 与 hold（四种时间），每 `related_input_slew` 和  `constrained_input_slew` 都要算一组：

  ```python
          for self.related_input_slew in related_slews:
              for self.constrained_input_slew in constrained_slews:
  ```

  - `analytical_setuphold`： 直接使用 `tech.spice["dff_setup"]` 作为计算的值；
  - `analyze`：四个时间，每个分别以不同参数调用 `bidir_search` 计算。而 `bidir_search`：
    1. `write_stimulus`：创建 `stimuli` 类对象，`self.stim`，创建激励、测量的 .sp 文件：sh_stim.sp、sh_meas.sp；
    2. `run_sim`：运行仿真文件；
    3. `parse_spice_list`：解析仿真输出文件，得到数据；
    4. `while True:` 以不同的参数循环 写仿真文件、运行仿真、解析结果。直到。



## 2023/11/24

- trimmed.sp：相对正常的 sram.sp，去掉了部分非必要的 memory cells；

- sram16x2.lvs.sp：相对正常的 sram.sp，对 MOS 管的描述少了一部分：

  ````spice
  Mpinv_pmos Z A vdd vdd p m=1 w=3.2u l=0.4u pd=7.20u ps=7.20u as=3.20p ad=3.20p
  Mpinv_pmos Z A vdd vdd p m=1 w=3.2u l=0.4u 

- `delay` 类对象根据 `OPTS.trim_netlist` 来确定仿真使用哪个 sram 的 spice 描述。如果为真，使用 trimmed.sp（去掉非必要的 memory cells 以加速仿真），否则使用 sram.sp；

- SRAM 频率由工艺库指定：`spice["feasible_period"] = 10`；



## 2023/11/25

- `elmore` 计算延时 => `elmore.get_lib_values` => 
- `hierarchy_design` 类实现了默认的 `build_graph` 函数，其他模块可以重载。在 `elmore` 计算延时，会先创建 `timing_graph` 对象。



## 2023/11/26

- `lib` 读取 tech.py 文件中的所有 slew 与 load 组合保存到 `self.load_slews` 中（`self.load_slews.append((load, slew))`）；

- `lib` 每次调用 `characterize`，产生一个特点工艺角的 .lib 文件。每个 .lib 都需要计算 `self.load_slews`  中的每个 load slew 组合的延时信息；（setup、hold 只针对 slews 变化）

- 所以每个 `elmore`、`delay`、`cacti` 类对象创建时，需要指定一个工艺角，计算延时时，需要给定 `self.load_slews` 列表；

- 每次在调用 `get_lib_values` 调用 `self.graph.get_timing` 函数，传入一组确定的 load 与 slew，计算得这个组合下的延时；

- 调用 `self.graph.get_timing`，会指定一条路径，这个路径每两个点直接的延时被计算，最后返回得到延时列表。将列表中的每个元素相加，得到总延时。返回的是 `delay_date` 对象，每个对象保存 `delay` 与 `slew` 信息，将两个 `delay_data` 相加，`delay` 可以直接相加，而 `slew` 直接使用在 + 号右侧的（合理）

- `get_lib_values` 方法中，只会计算第一个可读端口的路径延时信息：

  ````python
  port = self.read_ports[0]
  self.graph.get_all_paths('{}{}'.format("clk", port),
                           '{}{}_{}'.format(self.dout_name, port, self.probe_data))
  ````

  但是会用这个端口的信息填充所有读端口的延时：

  ````python
  for port in self.all_ports:
      for mname in self.delay_meas_names + self.power_meas_names:
          if "power" in mname:
              port_data[port][mname].append(power.dynamic)
          elif "delay" in mname and port in self.read_ports:
              port_data[port][mname].append(total_delay.delay / 1e3)
          elif "slew" in mname and port in self.read_ports:
              port_data[port][mname].append(total_delay.slew / 1e3)
  ````

- `lib` 类调用 `compute_setup_hold` 计算四个值：上升 setup、hold，下降 setup、hold。四个值会用到所有输入引脚与 clk 的描述。



## 2023/11/26

- 在 `get_lib_values` 方法中，会对所有 slew 与 load 组合各计算出一个延时对象，然后把值加入数据结构保存。

  只计算一个 port，但是计算延时对所有的都使用；

  ````python
  for load,slew in load_slews:
      path_delays = self.graph.get_timing(bl_path, self.corner, slew, load, self.params)
  
      total_delay = self.sum_delays(path_delays)
      max_delay = max(max_delay, total_delay.delay)
  
      for port in self.all_ports:
          for mname in self.delay_meas_names + self.power_meas_names:
              if "power" in mname:
                  port_data[port][mname].append(power.dynamic)
              elif "delay" in mname and port in self.read_ports:
                  port_data[port][mname].append(total_delay.delay / 1e3)
              elif "slew" in mname and port in self.read_ports:
                  port_data[port][mname].append(total_delay.slew / 1e3)
  ````

- 计算得到一个延时对象 `delay_data`，即为一个 slew 与一个 delay。一个 slew 表示上升、下降的 slew（rise_transition、fall_transition）；一个 delay 打表示上升、下降的 delay（cell_rise、cell_fall）；



## 2023/11/27

- 每次计算延时得到：`char_results = self.d.analyze(probe_address, probe_data, self.load_slews)` 或 `char_results = m.get_lib_values(self.load_slews)`。

  再拆开：`self.char_sram_results, self.char_port_results = char_results`。

  `self.char_port_results` 的类型是：`dict[int, dict[str, list[float]]]`。

  第一层查找的是端口号，第二层是计算的名称（delay_lh、delay_hl、power 等），再得到一个列表，列表中的每个值对应由一个 slew/load 计算得到的值。

- `setup_hold` 计算建立时间：通过不断仿真一次寄存器的采样过程，观察施加激励与时钟有效边沿的间隔，尝试不断缩小间隔，得到建立时间；

- `setup_hold` 计算建立时间：通过不断仿真一次寄存器的采样过程，观察撤去激励与时钟有效边沿的间隔，尝试不断缩小间隔，得到保持时间；
