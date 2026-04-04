# printf

So, you wanna know how to use printf huh? Well you've come to the right place. 

First, make sure to initialize your desired UART handle. Then, call `printf_init` with the UART handle. An example is shown below for a STM32G473xx:

```
    husart3->Init.BaudRate = 115200;
    husart3->Init.WordLength = UART_WORDLENGTH_8B;
    husart3->Init.StopBits = UART_STOPBITS_1;
    husart3->Init.Parity = UART_PARITY_NONE;
    husart3->Init.Mode = UART_MODE_TX_RX;
    husart3->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart3->Init.OverSampling = UART_OVERSAMPLING_16;
    printf_init(husart3);
```

`printf` is a library function provided by newlib, but we've replaced it with [`nanoprintf`](https://github.com/charlesnicholson/nanoprintf), a more lightweight implementation that uses less stack space and probably runs faster. `nanoprintf` handles all the format specifier replacement and puts our final string into a buffer of `MAX_PRINTF_SIZE`, which can be redefined if need be in the Makefile (default value is 256 bytes).

To use our `printf` implementation, *please include "printf.h" rather than <stdio.h>*.

We also have some extra memory management on top of the `nanoprintf` implementation that keeps our memory usage lower. Upon calling `printf`, if your thread does not have a buffer assigned to it, a Task Local Variable will be stored with a pointer to your thread's designated `printf` buffer. This is where your thread will do any necessary `printf` processing and spit out the final strings it needs to `uart_send`. 

The number of these buffers that exist is defined by `NUM_PRINTF_BUFFERS`. If `NUM_PRINTF_BUFFERS` < number of threads using `printf`, threads will start reusing buffers (i.e. two threads can be assigned to one buffer). This is why each buffer has a mutex to prevent two threads from modifying each other's printf data; the downside of this is increased latency as each `printf` call must wait for a previous `printf` call to finish processing. So, if the number of active threads is known, and _if you have enough memory_, it's best to set `NUM_PRINTF_BUFFERS` = `NUM_ACTIVE_THREADS`. However, if low on memory, you can reduce this at the cost of blocking `printf` on other `printf` calls a little more (this balloons as number of threads increases).

You can also change the `MAX_PRINTF_SIZE` if you feel like you need a little more space.

`printf` latency is pretty horrendous the more threads you have. Most of the time using a logic analyzer and a pin toggle will be better for debug, especially if you have access to the base board. A logging thread is another option; having one low-prio thread do all your printfs at some low frequency and just sending stuff there is a good way to reduce contention. For reference, the lowest worst-case latency I got with 15 threads was 118ms.

Finally, never `printf` from an interrupt. Please defer `printf` to a thread; the UART driver is not designed to be called from an interrupt (also it will explode interrupt latency).
