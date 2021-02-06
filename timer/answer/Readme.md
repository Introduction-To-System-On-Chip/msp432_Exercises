
## Register inspection in debug

Images can be seen in `debugRegs/` folder to inspect the different registers
updated by the driverlib functions.

## Time measurements

The Master Clock (MCLK) is the one that cadence the CPU execution of
instructions. The different clock sources have different frequencies, hence
giving different times to execute instructions. Clock frequency also impacts
power consumption that can be monitored using the Energy trace tool.

```
    MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_8);
```
                                                    
Time: 1s 884ms
Time Elapsed: 14s 428ms

```
    MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
```
Time: 0s 242ms
Time Elapsed: 2s 305ms

It takes more time when the divider is higher, which makes sense as the final
frequency is lower. However, the operating power consumption, i.e. the
power used to executes instructions is the same if we use the same clock source.

```
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
```
Time: 0s 0ms
Time Elapsed: 0s 6ms

```
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_1_5);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
```
Time: 0s 1ms
Time Elapsed: 0s 37ms

```
    MAP_CS_initClockSignal(CS_MCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_1);
```

Time: 0s 0ms
Time Elapsed: 0s 2ms 

