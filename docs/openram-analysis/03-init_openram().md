# `openram.init_openram()`

配置好 OpenRAM 选项后，调用 `init_openram`，第一个参数为命令行中的第一个参数，即为用户自定义的 config 文件。

````python
def init_openram(config_file, is_unit_test=False):
    """ Initialize the technology, paths, simulators, etc. """

    check_versions()

    debug.info(1, "Initializing OpenRAM...")

    setup_paths()

    read_config(config_file, is_unit_test)

    install_conda()

    import_tech()

    set_default_corner()

    init_paths()

    from openram.sram_factory import factory
    factory.reset()

    global OPTS

    # Setup correct bitcell names
    setup_bitcell()

    # Import these to find the executables for checkpointing
    from openram import characterizer
    from openram import verify
````



## `check_versions()`

1. 首先会检测 python 的版本，需要至少为 3.5；
2. 尝试调用 `git --version` 判断系统中是否存在 Git；
3. 判断系统中是否存在 `coverage` 命令，如果有给 OPTS 增加字段 `OPTS.coverage_exe = "coverage run -p " `，否则这个字段为空字符串；
4. 尝试 `import coverage`，如果 `coverage` 模块不存在，`OPTS.coverage = 0`，否则为 `1`。



## `setup_paths()`

1. 判断使用的线程数量是否大于 1 或临时文件目录为 /tmp。

   ````python
   if OPTS.num_threads > 1 or OPTS.openram_temp == "/tmp":
   ````

   如果是的话：根据当前进程 pid 重新设置 `OPTS.openram_temp`；

2. 保证 `OPTS.openram_temp` 结尾是 `/`



## `read_config()`

将用户编写的 config 文件目录作为函数第一个参数。

1. 获取 config 文件的绝对路径为 `config_file`；

2. 如果配置文件以 .py 结尾，去掉 .py 保证在 `config_file`；

3. ？？

4. `OPTS.config_file = config_file + ".py"`

5. 获取 `config_file` 的目录名保存在 `dir_name`；

6. 获取 `config_file` 的文件名保存在 `module_name`；

7. 检测 `module_name` 是否是一个 python 关键字；

8. 将 `dir_name` 加入到系统环境变量中；

9. 尝试使用 `import` 导入 `module_name`；

10. 将从 `module_name` 中获取到的用户配置信息（如果这些配置信息本来就存在于 `OPTS` 中，或者配置信息是 `'tech_name'`）添加到 `OPTS.__dict__` 字典中，并且记录这个选项被 overridden：

    ````python
        OPTS.overridden = {}
        for k, v in config.__dict__.items():
            # The command line will over-ride the config file
            # except in the case of the tech name! This is because the tech name
            # is sometimes used to specify the config file itself (e.g. unit tests)
            # Note that if we re-read a config file, nothing will get read again!
            if k not in OPTS.__dict__ or k == "tech_name":
                OPTS.__dict__[k] = v
                OPTS.overridden[k] = True
    ````

11. 保证 `OPTS.output_path` 是一个合理的目录名称（以 `/` 结尾）；

12. `OPTS.is_unit_test = is_unit_test` 默认为 `False`；

13. 如果只生成 netlist，就不需要 DRC/LVS：

    ````python
        if OPTS.netlist_only:
            OPTS.check_lvsdrc = False
    ````

14. 配置 `OPTS.write_size`

    ````python
        if OPTS.write_size is None:
            OPTS.write_size = OPTS.word_size
    ````



## `install_conda()`

1. 判断是否使用 conda，如果不需要，直接函数返回：

   ````python
       if not OPTS.use_conda or OPTS.is_unit_test:
           return
   ````

2.  调用 /install_conda.sh 下载 conda



## `import_tech()`

导入工艺库：

1. 从系统环境变量中得到 `$OPENRAM_TECH`，并且获得此目录的绝对路径；

2. 将此目录更新到环境变量 `$OPENRAM_TECH` 中；

3. 更新 openram 的静态字段：`openram.OPENRAM_TECH = OPENRAM_TECH`；

4. 将工艺库路径添加到系统路径中；

5. 将 `OPTS.tech_name` 导入到 `tech_mod` 变量（`OPTS.tech_name` 在 `parse_args` 被设定，默认使用 scn4m_subm）。即找到 ./technology/scn4m_subm 下的 `__init__.py`。

   执行这个文件会将各个工艺库的 DRC、SPICE 模型添加到系统环境变量中。

   其中每个工艺库文件夹都有一个 tech 文件，其中的 tech.py 文件包含了工艺库关于 DRC、SPICE 等参数，以 `dict[str, Any]` 的形式保存在 `drc`、`spice` 字典中；

6. 更新 `OPTS.opernram_tech` 为工艺库目录；



## `set_default_corner()`

根据 `OPTS` 中的选项，使用导入的工艺库，重新配置一些选项：

````python
    # Set some default options now based on the technology...
    if (OPTS.process_corners == ""):
        if OPTS.nominal_corner_only:
            OPTS.process_corners = ["TT"]
        else:
            OPTS.process_corners = list(tech.spice["fet_models"].keys())

    if (OPTS.supply_voltages == ""):
        if OPTS.nominal_corner_only:
            OPTS.supply_voltages = [tech.spice["supply_voltages"][1]]
        else:
            OPTS.supply_voltages = tech.spice["supply_voltages"]

    if (OPTS.temperatures == ""):
        if OPTS.nominal_corner_only:
            OPTS.temperatures = [tech.spice["temperatures"][1]]
        else:
            OPTS.temperatures = tech.spice["temperatures"]

    # Load scales are fanout multiples of the DFF input cap
    if (OPTS.load_scales == ""):
        OPTS.load_scales = [0.25, 1, 4]

    # Load scales are fanout multiples of the default spice input slew
    if (OPTS.slew_scales == ""):
        OPTS.slew_scales = [0.25, 1, 8]
````



## `init_paths()`

创建输出文件夹：

1. 如果 `OPTS.openram_temp` 已经存在，删除；
2. 不存就创建目录；



## `openram.sram_factory`

接下里从 `sram_factory` 模块导入了一个 `factory` 对象，用于之后创建模块对象使用。这个对象在 sram_factor.py 中定义：

````python
# Make a factory
factory = sram_factory()
````

关于 sram_factory 的分析见：[sram_factory](./globals/sram_factory.md)。



## `setup_bitcell()`

处理 OPTS 中的一些模块名称选项：

````python
def setup_bitcell():
    if OPTS.bitcell == "pbitcell":
        OPTS.bitcell = "pbitcell"
        OPTS.dummy_bitcell = "dummy_pbitcell"
        OPTS.replica_bitcell = "replica_pbitcell"
    else:
        OPTS.num_ports = OPTS.num_rw_ports + OPTS.num_w_ports + OPTS.num_r_ports
        OPTS.bitcell = "bitcell_{}port".format(OPTS.num_ports)
        OPTS.dummy_bitcell = "dummy_" + OPTS.bitcell
        OPTS.replica_bitcell = "replica_" + OPTS.bitcell

    # See if bitcell exists
    try:
        c = importlib.import_module("openram.modules." + OPTS.bitcell)
        mod = getattr(c, OPTS.bitcell)
    except ImportError:
        OPTS.bitcell = "pbitcell"
        OPTS.dummy_bitcell = "dummy_pbitcell"
        OPTS.replica_bitcell = "replica_pbitcell"
        if not OPTS.is_unit_test:
            msg = "Didn't find {0}rw {1}w {2}r port bitcell. ".format(OPTS.num_rw_ports, OPTS.num_w_ports, OPTS.num_r_ports)
            debug.warning("{} Using the parameterized bitcell which may have suboptimal density.".format(msg))
    debug.info(1, "Using bitcell: {}".format(OPTS.bitcell))
````



## 导入 `characterizer` 与 `verify`

````python
    from openram import characterizer
    from openram import verify
````

这两个模块主要用来做 DRC/LVS 等检测。[verify]() & [characterizer]()
