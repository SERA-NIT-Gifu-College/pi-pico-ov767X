# pi-pico-ov767X

OV767X camera driver library for RP series microcontrollers.

The registers are taken from ***public*** datasheet of OV7670.
So we don't handle too much private/closed source registers
which complicates the project.
We also ommited debug registers from the list.

OV7675 *should* be supported out of the box.

> Although we provide full VGA(640x480) resolution setting,
> it is resonable to use QVGA or QQVGA due to RAM limitation.

This library is expected to have at least 2 PIO state machines and
one NVIC targetable IRQs available.
