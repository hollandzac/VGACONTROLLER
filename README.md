# VGA CONTROLLER

A VGA contoller for a ARM Cortex-MF4 micro controller.

### VGA Controller
The controller use the GPIO pins on the micro controller to send the 8bit colour as well as the vsync and hsync signals. The clock is not fast enough for the 25MHz standard frequency required for 640 x 480 so the resolution is halved to make it possible to use the same timings.

- Resolution 320 x 240
- Pixel Clock 12.5MHz 

### PONG
Simple pong game using keyboard as input.
