# `lef` 类

> SRAM LEF Class open GDS file, read pins information, obstruction and write them to LEF file. This is inherited by the sram_1bank class.

SRAM LEF 类，打开 GDS 文件，读取引脚信息，写入 LEF 文件。



## `__init__`

````python
def __init__(self, layers):
    # LEF db units per micron
    self.lef_units = 2000
    # These are the layers of the obstructions
    self.lef_layers = layers
    # Round to ensure float values are divisible by 0.0025 (the manufacturing grid)
    self.round_grid = 4
````

