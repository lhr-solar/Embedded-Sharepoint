**********
Glossary
**********

.. glossary::

  Alternate Function
    Microcontroller pins can be used as regular digital GPIO pins but they could be configured to have some alternate function such as SPI, UART, CAN, etc.

  Analog to Digital Converter
    An `ADC <https://www.electronics-tutorials.ws/combination/analogue-to-digital-converter.html>`_ is a way for a microcontroller to read real world data by converting the analog inputs to digital values.

  ARM
    Computer Architect company that creates commonly used ARM microcontrollers/processors

  ARM Cortex-M4F
    Microcontroller core that has DSP instructions plus FPU; this core is commonly used in the electrical systems

  BJT Transistor 
    Bipolar Junction Transistor; current amplifying transistor; the current flow through the transistor can be controlled based off of the base resistor value; more calculations required to use compared to MOSFETs 

  BMS 
    Battery Management System; monitors, protects, and balances the battery pack; prevents the batteries from overheating or being damaged 

  BOM 
    Bill of Materials 

  BPS 
    Battery Protection System; monitors and protects the battery pack; same as BMS minus the balancing; this term is mainly used by Solar Vehicle teams because the American Solar Challenge regulations separates the terms for protection and balancing. 

  Boost Converter 
    Power converter that steps-up the input voltage; 5V -> 12V 

  Buck Converter 
    Power converter that steps-down the input voltage; 12V -> 5V 

  Buck-Boost Converter 
    Power converter that steps-up or steps-down the input voltage, usually applies if your input voltage can be a range of voltages; (2.5V-4.2V) -> 3.3V 

  Bus 
    Electrical data line which systems can connect to for communication 

  CAN Bus 
    Controller Area Network; standard automotive communication protocol that electrical subsystems use to communicate with each other; 2-ewire differential protocol which a 1 and 0 is determined by the difference in voltage between the two wires; noise resistant; dominant and recessive states indicates 0 and 1 

  Contactor 
    High voltage switch that can handle more power than a relay; there is a contactor the BPS controls to power the whole electrical system with the primary battery pack 

  DC-DC Converter 
    Power converter used to get the correct voltage to power a component; the converter takes an input voltage to step-up or step-down the voltage and outputs the desired voltage; internal components consist of a capacitor, inductor, transistor, diode 

  DMA 
    Direct Memory Access; an internal controller that bypasses the CPU to perform memory operations/transfers e.g. instead of having a for loop to copy the data of arrayA to arrayB, the DMA controller can copy the data through hardware without using any CPU instructions 

  Driver 
    Code that handles I/O operations with a certain IC/component/device; there is another definition used in context of transistors (Gate Drivers) 

  DSP 
    Digital Signal Processing; includes signal/data filtering and data compression 

  EEPROM 
    Electrically Erasable Program Read Only Memory; non-volatile memory used to store data 

  EMI 
    Electromagnetic Interference; RF/high frequency noises that can affect signal/power lines; sources can come from the motor, vibrations of the car, radios, and any high frequency switches/data lines 

  Exceptions 
    Software that handles errors in codes; software triggered 

  Ferrite Bead 
    Passive component that suppresses EMI/high frequency noises  

  Galvanically Isolated 
    Two systems that do not touch the same metal contacts; usually means the ground and power connections of system A is completely separate from system B's ground and power connections; isolation should be considered in the design because if every system/electronic/board/circuit shares the same ground and if one shorts, then the whole system can short whereas if each subsystem is isolated and one shorts, then only that part needs to be replaced 

  GCC 
    GNU Compiler Collection; compiles/builds C code to make the program executable/runnable 

  GPIO 
    General Purpose Input/Output; microcontroller pins that can be configured as digital input/outputs 

  Hall effect sensor
    A Hall effect sensor is a device that is used to measure the magnitude of a magnetic field.

  Heap 
    Block of memory used for dynamic memory allocation; microcontrollers usually don't have a heap because an OS is usually required for dynamic memory allocation 

  IC 
    Integrated Circuit; synonym for chips 

  Interrupts 
    Signals that notify the CPU a hardware event has occurred that must be handled; hardware events can be digital pin logic changes, timer time outs, I/O completions 

  Isolated DC-DC Converter 
    Input power/gnd and the output power/gnd connections of a power converter are separate from each other; look at Galvanically Isolated definition for more information on isolation. 

  Isolated SPI 
    Analog Devices/LTC's proprietary communication protocol that converts regular SPI to an isolated differential protocol; 2-wire interface: IP, IM; GND is not connected; uses a transformer for isolation; used by the LTC6811 Battery Monitoring ICs on the BPS 

  Isolator 
    Component that isolates power/gnd (e.g. SI8261); look at Galvanically Isolated definition for more information about isolation 

  ISR 
    Interrupt Service Routine; block of code that runs whenever there is an interrupt 

  I2C 
    Inter-Integrated Communication; one of the standard synchronous communication protocols to communicate with ICs; 3-wire interface: SCL (clock), SDA (data), GND; open drain pins; pull-up resistors required; for more information on how this works there are a lot of resources on Google 

  J-Link 
    SEGGER's ARM programmer; used to flash/program ARM chips 

  JTAG 
    A standard implemented in many chips, allows use of a tool (like a J-Link) to flash and debug programs. Allows visibility into the registers and other state of the system. 

  Kernel 
    Has a variety of meanings, but most often refers to the code at the center of an operating system (what handles threads, memory management, etc.) 

  LIFO 
    Last In First Out (see also: stack) 

  Linear Regulator 
    Power components that only step down voltage. Dissipates heat to step down the voltage whereas the converter does not dissipate energy; maintains a stable voltage, making it less noisy than a switching regulator. It is however less energy-efficient than a switching regulator. 

  Low-Pass Filter 
    Removes high-frequency components from signals. Often used to reduce noise. 

  MOSFET 
    A transistor that is most commonly used for digital applications. It is controlled by the voltage applied to its base, which either makes or breaks a current between its other two terminals. 

  MPPT 
    A maximum power point tracker (MPPT) monitors the power output of a solar array and attempts to maximize that power by adjusting a load (resistance) on that power line. 

  Mutex 
    A signal that is used to indicate that a thread is using a resource. In multithreaded programming, race conditions become issues. To solve these, a flag is raised each time a resource needs to be accessed, and is unset whenever that resource becomes free for other threads to use. These are typically managed by an operating system or a mediator. 

  Negative Logic 
    Logical 1/true is a low voltage/off and a logic 0/false is a high voltage/on. 

  NGM 
    New Generation Motors is a company that makes highly efficient motors. We have used their motors in the past, and often use "the NGM" to refer to the motor that we have in the lab. 

  Open Drain/Collector 
    A configuration of a pin on a microcontroller where typically logical '0' corresponds to '0V' while logical '1' corresponds to a high impedance state (Hi-Z). This is often used to allow multiple devices to share a single wire for communication. This stands in opposition to a "push-pull" configuration. 

  Op-Amp
    An operational amplifier is a circuit element that performs mathematical operations on input voltages. It is most commonly used to increase these inputs by a certain magnitude to make them easier to read by the microcontroller.

  Phase Locked Loop
    The PLL generates an output signal whose phase is related to the input signal. It is used to increase the clock speed of the STM microcontroller.

  Positive Logic 
    Logical 1/true is a high voltage/on and a logic 0/false is a low voltage/false. 

  Printed Circuit Board
    PCB's are copper circuits printed onto non-conductive boards that electrically connect our electronic
    components together and allow us hardware to run our programs on.

  Pull-Down Resistor 
    A resistor to gnd that pulls a wire's voltage to logical '0' when it disconnects from some other source. 

  Pull-Up Resistor 
    A resistor to Vcc that pulls a wire's voltage to logical '1' when it disconnects from some other source. 

  Pulse Width Modulation
    A method of controlling the power usage of a peripheral by quickly turning it on and off and controlling how long it stays on for. Used to control the speed of the fans.

  Push-Pull 
    A configuration of a pin on a microcontroller where typically logical '0' corresponds to 0V, which logical '1' corresponds to the operating voltage (Vcc). Using this configuration, there can only be one source on a wire that drives communication/control. It stands in opposition to an "open drain" configuration. 

  Queue 
    A First-In-First-Out (FIFO) structure, where typically supported operations are to insert elements (queue/push), take elements out (dequeue/pop), and view the front element (peek). Elements are removed in the same order that they were entered. 

  Race Condition 
    An issue that occurs when multiple threads are attempting to access the same resources. For example, if one thread performs an operation on some data that takes the form read-modify-writeback, and another thread writes to that data after the first read, but before it modifies and writes back. This could potentially put the program in an unaccounted-for state. This issue can be addressed with mutexes and semaphores. 

  Register 
    Basic unit of memory. Temporarily holds important data. 

  RTOS 
    A real-time operating system. These typically manage threads in a way that requires each thread to cede control back to the operating system, rather than allowing the OS to interrupt the thread. This ensures that communications and other critical paths are not interrupted partway through completion. These may or may not include other features like file systems, networking stacks, and memory management systems. 

  Semaphore 
    A variable that tracks the availability of a  resource. Threads will look here before attempting to access resources in order to avoid race conditions, and will set/unset or increment/decrement these when they acquire/release the resources. 

  Shunt Resistor
    Resistor used to measure the current through a line. Typically extremely small so power is not wasted. Voltage is measured across resistor(using an :term:`Op-Amp <Op-Amp>`) and divided by resistance to calculate the current.

  Silkscreen 
    Ink layer on a PCB for all the text and graphics 

  SMD 
    A surface mount device (also called surface-mount-component) is a device that is soldered to the surface of a PCB, as opposed to a through-hole component. 

  SOC 
    A system on a chip is a microprocessor with flash memory, ram, and any other necessary system components integrated in the single chip. 

  SPI 
    The serial-peripheral-interface is a de-facto standard interface between a microcontroller and one or several  peripheral devices. This uses allows for one supervisor device, which initiates all communications, and several worker devices, which share data and clock lines, but each require their own chip-select lines, which indicate which is required to listen/respond to the supervisor. 

  Stack 
    A First-In-Last-Out structure, where the supported options are push (add element), pop (remove element), and peek (look at top element). The typical analogy is a stack of paper, where each page is added individually to the top and each piece is individually removed from the top. This may also refer to a region of memory in a computer that is used to store parameters to functions, local variables, and the state of the calling function. 

  Step-Down 
    A reduction in voltage 

  Step-Up 
    An increase in voltage 

  ST-Link 
    STM's interface for programming and debugging their microcontrollers, or the device that uses that interface. 

  STM 
    STMicroelectronics is a company that produces various silicon chips and sensors. We also often refer to various microcontrollers produced by STMicro as STMs, since all their part IDs begin STM... 

  SWD 
    Serial Wire Debug protocol that is tightly coupled with ST-Link, the protocol that STM uses debugging. 

  Switching Regulator 
    A regulator that boosts or bucks the voltage by fast internal switching mechanisms. The switching (which usually a MOSFET toggling) introduces some noise to the system, but is typically more power efficient than a linear regulator and allows for boosting as well as bucking. 

  Thick-Film Resistor 
    A type of resistor that is relatively easy to manufacture, making it very cheap. The 'thick-film' refers to the internal structure that gives it its resistance. 

  Thin-Film Resistor 
    A type of resistor that is a bit harder to manufacture than a thick-film resistor, making it slightly more expensive. It typically has lower tolerances and parasitic capacitance/inductance, making it useful for high-precision circuits. 

  Through-Hole 
    A type of component that has leads that poke through holes on the PCB and are then soldered, usually from the side of the PCB that is opposite the actual component. It contrasts SMD components. 

  Tritium 
    A brand of 3-phase motor controller that our team uses. 

  UART 
    An universal asynchronous communications protocol between two chips. 
    
