# Spectrum Analyzer on LCD2x16

This repository has been created for Microprocessor Systems project. This is a simple __*LCD2x16 Spectrum Analyzer*__ based on Kinetis KL25Z128 development board which is based on Cortex-M0+ microcontroller. 

<p align="center">
<img src="https://github.com/JZimnol/Spec_Analyz_LCD2x16/blob/main/img/components_1.png" width="1000">
</p>
<p align="center">
(screenshot taken from presentation video) 
</p>

## DSP-Note
This project uses CMSIS DSP library which has not been included in project files in the repo. It's an open-source library which can be easily found on the ARM websites.

## Keyboard
To be able to control the 4x4 matrix keyboard, I have used interrupts combined with multiplexing. Interrupts are row-activated and uC cheks which row has been pressed. Then selected row is set as output and columns as inputs. If uC knows which row and column has been detected, it also knows which button has ben pressed. 
The keyboard is used to select modes, for example SW1 == Mode1, which is frequency in range 0-20 kHz, SW2 == Mode2, which is frequency in range 0-2500 Hz, SW3 == Mode3, which is frequency in range 2500-5000 Hz etc. 

<p align="center">
<img src="https://github.com/JZimnol/Spec_Analyz_LCD2x16/blob/main/img/modes_example.png" width="500">
</p>
<p align="center">
Example mode (screenshot taken from presentation video) 
</p>

## 2x16 LCD Display
Display used in this project is a __HD44780U__ connected through an expander __PCF8574__. The use of an expander allow us to communicate with LCD using I2C protocol (only 2 wires), but it also slows down the column printing. However, a special column display function has been implemented that allows things to be displayed efficiently on the display.

## Transistor audio amplifier
To be able to sample audio-jack voltage, a simple audio amplifier has been made using __BC547B__. This amp adds DC component to the signal so it can be sampled using A/D converter (0-3.3 V reference voltages). There is also possibility to connect any audio device to be able to listen to sampled audio.  

<p align="center">
<img src="https://github.com/JZimnol/Spec_Analyz_LCD2x16/blob/main/img/amp_scheme.png" width="1000">
</p>
<p align="center">
Amp scheme (made in LTspice)
</p>
