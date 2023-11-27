# `delay_data` 类

> This is the delay class to represent the delay information
>
> Time is 50% of the signal to 50% of reference signal delay.
>
> Slew is the 10% of the signal to 90% of signal

代表延时信息的类。延时信息是以 50% 划分。传输时间以 10% - 90% 划分。

```python
class delay_data():

    def __init__(self, delay=0.0, slew=0.0):
        self.delay = delay
        self.slew = slew

    def __str__(self):
        return "Delta Data: Delay {} Slew {}".format(self.delay, self.slew)

    def __add__(self, other):
        assert isinstance(other, delay_data)
        return delay_data(other.delay + self.delay,
                          other.slew)

    def __radd__(self, other):
        assert isinstance(other, delay_data)
        return delay_data(other.delay + self.delay,
                          self.slew)
```

很简单的类，就是保存 `delay` 与 `slew` 数据！