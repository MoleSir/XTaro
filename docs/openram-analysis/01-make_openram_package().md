# `make_openram_package()`

````python
def make_openram_package():
    """ Make sure that OpenRAM can be used as a Python package. """

    import importlib.util

    # Find the package loader from python/site-packages
    openram_loader = importlib.util.find_spec("openram")

    # If openram library isn't found as a python package, import it from
    # the $OPENRAM_HOME path.
    if openram_loader is None:
        OPENRAM_HOME = os.getenv("OPENRAM_HOME")
        # Import using spec since the directory can be named something other
        # than "openram".
        spec = importlib.util.spec_from_file_location(
            "openram", 
            "{}/../__init__.py".format(OPENRAM_HOME))
        module = importlib.util.module_from_spec(spec)
        sys.modules["openram"] = module
        spec.loader.exec_module(module)
````

1. 查找 python/site-packages 中是否存在 openram 的包，如果已经存在，函数直接返回；
2. 如果 openram 不是系统中的一个包，就需要从文件系统中创建：
   1. 得到环境变量 `$OPENRAM_HOME` 的值；
   2. 调用 `importlib.util.spec_from_file_location` 函数调用 `$OPENRAM_HOME/../__init__.py`；
   3. 将 openram 包加入 `sys.modules` 中；
   4. 执行模块。

函数执行后保证系统中存在 openram 的包。