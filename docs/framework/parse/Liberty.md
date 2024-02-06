# Liberty

## 流程

1. `slew` 与 `load`；

2. PVT 信息，一个 PVT 配置应该对应一个 .lib 文件；

3. 对每一个 PVT 执行：

   1. delay 与 setuphold 仿真。（每个 PVT 会尝试使用不同的 slew 与 load 产生 lookup table）

      遍历所有 slew 与 load 组合，得到一组数据。作为 loopup table

   2. 写 lib 文件

   ````python
   self.compute_delay()
   
   self.compute_setup_hold()
   
   self.write_header()
   
   # Loop over all ports.
   for port in self.all_ports:
       # set the read and write port as inputs.
       self.write_data_bus(port)
       self.write_addr_bus(port)
       if self.sram.write_size != self.sram.word_size and \
               port in self.write_ports:
           self.write_wmask_bus(port)
       # need to split this into sram and port control signals
       self.write_control_pins(port)
       self.write_clk_timing_power(port)
   
   self.write_footer()
   ````

   



## 需要写入的数据

### write_header

- `lib_name`：`library (sram_16x2_SS_5p0V_25C_lib)`
- `cell_name`
- `width` & `height`：`area`
- `char_sram_results["leakage_power"]`

#### write_units

- `process`
- `voltage`
- `temperature`

#### write_defaults

- `nom_voltage`
- `nom_voltage`
- `nom_process`
- `voltage_map`：`voltage_map ( VDD, 5.0 );`、`voltage_map ( GND, 0 );`

#### write_LUT_templates

CELL_TABLE 与 CONSTRAINT_TABLE

- slew
- load

#### write_bus

- `word_size`
- `addr_size`

#### write_pg_pin

- `vdd_name`
- `gnd_name`

### write_data_bus

#### write_data_bus_input

- `port_name`
- `capacitance`
- `word_size`

#### write_data_bus_output

- `port_name`
- `max_capacitance`
- `word_size`
- `char_port_results[read_port]["delay_lh"]`
- `char_port_results[read_port]["delay_hl"]`
- `char_port_results[read_port]["slew_lh"]`
- `char_port_results[read_port]["slew_hl"]`

### write_addr_bus

- `dff_in_cap`
- `addr_size`
- `port_name`

#### write_FF_setuphold

- `port_name`
- `self.times["setup_times_HL"]`
- `self.times["setup_times_LH"]`
- `self.times["hold_times_LH"]`
- `self.times["hold_times_HL"]`

### write_control_pins

- `dff_in_cap`

#### write_FF_setuphold

### write_clk_timing_power

- `dff_in_cap`
- `char_sram_results["min_period"]`
- `char_sram_results["min_period"]`

#### add_clk_control_power

- `char_port_results[port]["write1_power"]`
- `char_port_results[port]["disabled_write1_power"]`
- `char_port_results[port]["disabled_write0_power"]`

### write_footer



## lib 数据结构

- lib name
- PVT
- SRAM 对象
- 每一组 slew & load：
  - delay_lh
  - delay_hl
  - slew_lh
  - slew_hl
- 每一组：related_slew & constrained_pin_transition
  - setup_times_HL
  - setup_times_LH
  - hold_times_LH
  - hold_times_HL



