# Cherry Pi

Inspired by the song "Cherry Pie" released by American glam metal band Warrant in 1990, Cherry Pi is my very own Raspberry Pi-based Guitar Pedal.

<iframe width="560" height="315" src="https://www.youtube.com/embed/OjyZKfdwlng" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## The Story

As a guitarist and Computer Engineering candidate, I've always been fascinated by Guitar Pedals, and how they work. Plugging in a 1/4" jack into a mysterious box and expecting it to make magical effects always seemed surreal. It's always been a goal of mine to make one of my own.

## Overview

The schematics for the hardware of this project were sourced from https://www.electrosmash.com/pedal-pi, however the sourcing of the parts, and assembly was done manually by myself. I did this both for educational purposes and practical ones too. All parts were sourced from DigiKey.

[Image of Schematics]

Though my pedal has different switch configurations, I/O behaviour, CPU model (the Raspberry Pi Zero houses a BCM2835, while the Pi 4 Model B houses a BCM2711), the core parts are essentially the same. The pedal consists of three main parts: The Input Stage, the Power Supply Stage and the Output Stage.

[Image of Breadboard Diagram]

The above displays my particular breadboard layout (at the time of prototyping), as well as a picture of the prototyped circuit.

One significant difference that the CPU change makes is that the BCM2835.h library could not be used. Rather, I decided to take an approach utilizing the sysfs interface for SPI communications with the MCP3202 ADC, and WiringPi's API functions for PWM analog output control. In particular I used the native linux `linux/spi/spidev.h` library for SPI control, and the `wiringPi.h` library for wiringPi functions.

I decided to use WiringPi's functions over the sysfs approach as I found that the sysfs approach natively supported by linux was a bit too involved for a simple prototype (see Appendix). This is due to change, of course, as I expand the functionality.

## Roadmap

Housing a guitar pedal on a Raspberry Pi means a lot for expandability, and most definitely aim to expand functionality beyond a clean effects pedal, which takes in analog readings and outputs them.

#### Goal 1: MVP (Reached)

I have declared my minimum viable product to be to create the input, output and power supply stage for the circuit, and to have a simple SPI read, PWM out c file to do the signal processing.

#### Goal 2: FX

My next step would be to look into both cleaning up the circuitry, the c file, and to start writing some guitar pedal effects.

#### Future Goals

At the time of writing this, next steps are unknown. However the possible ideas (which will require a research spike) will be:
 - Find a way to treat the Raspberry Pi as an audio source to an external computer, and use the Cherry Pi as an audio interface
 - Use wireless interface (Bluetooth, WiFi) to transmit audio data, and use the Cherry Pi as a wireless Guitar connector
 - Take advantage of the SPI protocol and power of the Raspberry Pi to allow for multiple inputs, and treat the pedal as a effects center for multiple instruments simultaneously.
 - Source Hi-Fi parts for clearer audiophile tone (faster ADC, invest in housing to prevent EMI)

## Appendix

Note about the PWM sysfs approach:

Taking the sysfs approach for PWM would involve enabling pwm 2 channel support in the linux kernel device tree overlay. This, upon reboot would expose the folder `/sys/class/pwm/pwmchip0`. Then, the determined pins would need to be written into `/sys/class/pwm/pwmchip0/export` which would expose a folder in the format `pwm[n]` where n represents the pwm pin (pwm0, pwm1, etc.). This is not to be confused by the GPIO pin number. Then, based on the pwm pin, the period and duty cycle required for PWM (in nanoseconds) should be written into `/sys/class/pwm/pwmchip0/pwm[n]/period` and `/sys/class/pwm/pwmchip0/pwm[n]/duty_cycle`. Once that is set, the pwm pin needs to be enabled by writing `1` into `/sys/class/pwm/pwmchip0/pwm[n]/enable`. Once the operation performed is finished, the `0` needs to be written into `/sys/class/pwm/pwmchip0/pwm[n]/enable` to disable it. Then, the exposed pins need to be written into `/sys/class/pwm/pwmchip0/unexport` to hide them again. This is discussed in http://blog.oddbit.com/post/2017-09-26-some-notes-on-pwm-on-the-raspberry-pi/ and https://www.ics.com/blog/how-control-gpio-hardware-c-or-c

I have attempted to do all this in a single file, however since PWM will need to be changed several thousand times per second (as defined by the refresh rate of the ADC), this does not seem like a clean process to work on. This, again, is subject to change as the project progresses.