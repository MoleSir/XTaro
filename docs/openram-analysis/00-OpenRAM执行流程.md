# OpenRAM 执行流程

## OpenRAM 的使用

配置环境后，创建一个 python 文件 myconfig.py 文件，写入本次需要创建的 SRAM 内存的参数，例如：

````python
# Data word size
word_size = 2
# Number of words in the memory
num_words = 16

# Technology to use in $OPENRAM_TECH
tech_name = "scn4m_subm"
# Process corners to characterize
process_corners = [ "TT" ]
# Voltage corners to characterize
supply_voltages = [ 3.3 ]
# Temperature corners to characterize
temperatures = [ 25 ]

# Output directory for the results
output_path = "temp"
# Output file base name
output_name = "sram_16x2"

# Disable analytical models for full characterization (WARNING: slow!)
# analytical_delay = False

# To force this to use magic and netgen for DRC/LVS/PEX
# Could be calibre for FreePDK45
drc_name = "magic"
lvs_name = "netgen"
pex_name = "magic"
````

其中指定了内存的大小信息、工艺库为 scn4m_subm、电压、温度、输出文件路径、输出文件名称以及执行 DRC/LVS/PEX 所使用的命令。

最后执行：

````bash
python3 $OPENRAM_HOME/../sram_compiler.py myconfig.py
````

等待一段时间，创建出 temp 目录，输出若干文件。例如 verilog 文件、spice 网表与仿真文件、gds 版图文件等。



## sram_compiler.py

````python
"""
SRAM Compiler

The output files append the given suffixes to the output name:
a spice (.sp) file for circuit simulation
a GDS2 (.gds) file containing the layout
a LEF (.lef) file for preliminary P&R (real one should be from layout)
a Liberty (.lib) file for timing analysis/optimization
"""

import sys
import os
import datetime

# You don't need the next two lines if you're sure that openram package is installed
from common import *
make_openram_package()
import openram

(OPTS, args) = openram.parse_args()

# Check that we are left with a single configuration file as argument.
if len(args) != 1:
    print(openram.USAGE)
    sys.exit(2)

# Set top process to openram
OPTS.top_process = 'openram'

# These depend on arguments, so don't load them until now.
from openram import debug

# Parse config file and set up all the options
openram.init_openram(config_file=args[0])

# Ensure that the right bitcell exists or use the parameterised one
openram.setup_bitcell()

# Only print banner here so it's not in unit tests
openram.print_banner()

# Keep track of running stats
start_time = datetime.datetime.now()
openram.print_time("Start", start_time)

# Output info about this run
openram.report_status()

debug.print_raw("Words per row: {}".format(OPTS.words_per_row))

output_extensions = ["lvs", "sp", "v", "lib", "py", "html", "log"]
# Only output lef/gds if back-end
if not OPTS.netlist_only:
    output_extensions.extend(["lef", "gds"])

output_files = ["{0}{1}.{2}".format(OPTS.output_path,
                                    OPTS.output_name, x)
                for x in output_extensions]
debug.print_raw("Output files are: ")
for path in output_files:
    debug.print_raw(path)

# Create an SRAM (we can also pass sram_config, see documentation/tutorials for details)
from openram import sram
s = sram()

# Output the files for the resulting SRAM
s.save()

# Delete temp files etc.
openram.end_openram()
openram.print_time("End", datetime.datetime.now(), start_time)
````

### 备注

````python
"""
SRAM Compiler

The output files append the given suffixes to the output name:
a spice (.sp) file for circuit simulation
a GDS2 (.gds) file containing the layout
a LEF (.lef) file for preliminary P&R (real one should be from layout)
a Liberty (.lib) file for timing analysis/optimization
"""
````

### 创建 openram 包

[make_openram_package()](./01-make_openram_package().md)

### 解析命令行参数

[(OPTS, args) = openram.parse_args()](./02-parse_args().md)

### 初始化 openram

[openram.init_openram(config_file=args[0])](./03-init_openram().md)

### `setup_bitcell`

[openram.setup_bitcell()](./04-setup_bitcell().md)

### `print_banner`

[openram.report_status()](./05-print_banner().md)

### `report_status`

[openram.report_status()](./06-report_status().md)

### 创建输出文件列表

[output_files](./07-创建输出文件列表.md)

### 创建 `sram` 对象

[sram()](./08-sram().md)

### `s.save()`

[s.save()](./09-s.save().md)

### 释放 `openram`

[openram.end_openram()](./10-end_openram())