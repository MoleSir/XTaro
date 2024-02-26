# Summary

## Chapter 1 Design & Test of Memories

本书关于：memory 设计、memory 测试、memory 自测。

要理解 Memory self-test 首先需要了解 memory test，要理解 memory test 就先要了解 memory 设计。

- memory：存储计算信息；
- memory test：在一系列环境条件下，在内存中执行特定的 patterns，以检测 defects；
- memory self test：在不需要外部激励的情况下，对嵌入式 memory 进行测试；

由于密度增长，内存设计越来越具有挑战性。

对不同类型的内容的测试也是不同的。

BIST 相对 ATE (Automatic Test Equipment)：

- 在嵌入式内存中，通常只能使用 BIST；
- BIST 的速度快得多；
- BIST 十分优雅；

使用 BIST，测试必须与设计同时进行。



## Chapter 2 Static Random Access Memories

SRAM 发展趋势：速度块、存储数量大。

6T Bitcell 的基本读写流程。

模拟电路是影响 SRAM 设计与测试的关键。

读数据路径：

- 预充电；
- Isolation 电路：隔离位线与敏感放大器；
- 敏感放大器。一般情况下，Isolation 先开启一会，再开启敏感放大器（也开可以同时）；

写驱动

译码电路

REDUNDANCY



## Chapter 8 Memory Faults

逻辑测试中，一般仅关注 stuck-at；

而内存需要考虑很多的错误类型；

一般性错误模型：

- Stuck-at fault model (SAF)
- Transition fault model (TF)
- Coupling fault model (CF)
- Neighborhood pattern sensitive fault model (NPSF)

读错误 Fault (READ DISTURB FAULT MODEL)

预充电 Fault

False write through

Data retention faults

译码器 Falut

- Disconnected address fault
- Mis-directed address fault
- Multiple select address fault
- Multiple selected cell fault



## Chapter 9 Memory Patterns

- ZERO-ONE PATTERN
- EXHAUSTIVE TEST PATTERN
- WALKING, MARCHING,AND GALLOPING
  - WALKING：对同一个地址连续进行例如：R0、W1、R1、W0 的操作，要求是操作前后保持原来的状态。操作完一个地址再到下一个；
  - MARCHING：对同一个地址连续进行例如：R0、W1、R1 的操作，操作前后状态变化。操作完一个地址再到下一个；
  - GALLOPING：ping-pong action？
- 常见的 MARCH PATTERNS
  - March C- Pattern：可以涉及大多 faults：stuck-at、many coupling faults、、transition faults、Many decoder faults；
  - Partial Moving Inversion Pattern
  - Enhanced March C- Pattern：增加 pre-charge defects；
  - March LR Pattern：增加 realistic linked faults；
  - March G Pattern
- PSEUDO-RANDOM PATTERNS：随机，但不是真的完全随机。用的少。



## Chapter 10 BIST Concepts

BIST 的测试更快。

BIST 与 Memory 的边界需要区分清楚。

Memory 内部尽可能少的逻辑。

在制造过程中，BIST 可以用于 wafer test, module test, card test, and initial system test。

虽然 ATE 无法在嵌入式内存中使用，只能使用 BIST，但 ATE 总还是需要的，启动 BIST 也是需要 ATE。

BIST 逻辑本身的运行速度应该比 Memory 快。

BIST 引擎为 Memory 测试提供了 Patterns。

BIST 可以使用 FSM 或者 Mirco-Code 实现。



## Chapter 11 State Machine BIST

状态机 BIST 可以很简单，只能产生基本的 Patterns。

也可以很复杂，产生合适的 Patterns。

因为状态机 BIST 必须十分复杂才可以产生合适的 Patterns。



## Chapter 12 Micro-Code BIST

Micro-Code BIST 的优势在于其可以编程。

设计要 BIST 使用的指令，存储起来，可以灵活修改，以应对不同的 Patterns 需求（产生不同的错误模型时）。而状态机不可以。



## Chapter 13 BIST and Redundancy

Redundancy 对良率十分重要。

是替换而非修复！

虽然可能只有一个 bit 错误，但不可能只替换这个一个。

Redundancy 类型：

- 行替换：
- 列替换：通常是整个列选择器所在的列一起替换；

Redundancy 实现：

- Hard redundancy：使用 fuses 存储替换信息；
- Soft redundancy：每次 Reset，进行 BIST，替换信息被 Latches 存储；

通常二者同时使用。

使用 BIST 进行 Redundancy 的系统称为 Build-in Self-Repair BISR

Redundancy 计算：根据 BIST 得到错误的信息，执行一定算法，进行行、列替换组合，使得替换的效率最高。



## Chapter 15 Conclusions

BIST 是高性能的嵌入式内存的关键，产生好的 BIST 需要理解：

1. Memory 设计，其中的各个模块；
2. Fault Models，以及其对应哪些合适的 Patterns；
3. BIST 的架构，实现方式；
4. Redundancy 算法；

不同的 Memory 有不同的测试方法。

We need the best memory designs, the best fault models, the best tests, and the best BIST.
