# CircuitFactory

使用 `CircuitFactory` 对所有 `Circuit` 对象的创建与释放进行管理；



## 创建接口

由于各类电路模块构造函数所需要的参数不同，需要对所有电路统一一个创建接口：

```c++
Circuit* create(ModuleType circuitType,
                CircuitArguments* arguments,
                String circuitName = "");
```

### 电路模块类型 `ModuleType`

使用一个枚举标记需要创建的模块

````c++
    enum class ModuleType
    { 
        BITCELL,
		...
        MOS,
        INV,
        NAND,
		...
        BANK,
        CONTROL_LOGIC,
        SRAM,
    };
````

### 电路参数 `CircuitArguments*`

定义一个接口类：`CircuitArguments`，其没有任何字段，仅有一个纯虚函数：

````c++
struct CircuitArguments 
{
    virtual ~CircuitArguments() noexcept = default;
    virtual std::string toString() const = 0; 
};
````

任何新的电路模块，也需要创建一个新的子类来继承 `CircuitArguments`，并且在子类中实现这个电路模块需要的参数。使用工厂模式创建电路时，首先需要创建这个电路对于的 `CircuitArguments` 子类，并且填充其字段，再传递给 `create` 函数。

例如，实现 `AND` 模块，需要一个 `ANDArguments`，其中包含了 AND Gate 需要的参数：`driveCapability` 与 `inputSize`：

````c++
struct ANDArguments : public CircuitArguments
{
    ANDArguments(DriveCapability dc, int is) : 
        driveCapability{dc}, inputSize{is} {}

    virtual ~ANDArguments() noexcept override {}

    virtual std::string toString() const override;

    DriveCapability driveCapability;
    int inputSize;
};
````

同时还需要实现 `toString` 接口，得到在此参数下，返回的格式字符串。这样不同的参数就可以得到不同的格式字符串，可以在 `create` 函数中进行区分。

例如对 `AND` 模块的的接口实现：

````c++
std::string ANDArguments::toString() const
{
    return util::format("dc%d_is%d", 
                        static_cast<int>(this->driveCapability),
                        this->inputSize);
}
````

最后可以这样使用：

```c++
ANDArguments andArgument{this->_driveCapability, this->_inputSize};
this->_and = factory->create(ModuleType::AND, &andArgument);
this->_circuits.insert(this->_and);
```



## 工厂类电路对象管理

对每类电路模块，使用一个 `std::map<std::string, Circuit*>` 进行管理。即 Key 为 `Arguments` 类的 `toString` 接口返回的字符串，Value 为此 `Arguments` 值对应的电路对象。

最后使用 `std::array` 将所有不同电路模块的  `std::map<std::string, Circuit*>` 管理：

````c++
using CircuitMap = std::map<std::string, Circuit*>;
std::array<CircuitMap, MODULESIZE>_circuits{};
````



## `create` 流程

````c++
Circuit* CircuitFactory::create(ModuleType circuitType, 
                                CircuitArguments* arguments, 
                                String circuitName)
````

### 判断目标电路是否已经存在

````c++
    std::string argsList{ arguments == nullptr ? "" : arguments->toString() };
    Circuit* circuit{this->findCircuit(circuitType, argsList)};
    if (circuit != nullptr)
        return circuit;
````

### 获取电路对象名称

````c++
    if (circuitName.empty())
        circuitName = this->getDefaultCircuitName(circuitType);
````

### 创建新电路

````c++
     this->collectCircuit(circuitType, argsList, circuit);
````





