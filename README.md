# SaitekYokeMod
This is relatively simple mod for improving Logitech Saitek yoke system. I figured it would be nice to have a bunch of switches on the yoke, because most of the time I'm playing VR and it's hard to reach with a controller behind the yoke. 
Disassembling the yoke is just removing couple of screws and I've quickly figured out that there would be enough space for:
- 9x On/Off flip switch - for lights, pitot, and something else
- rotary encoder on the left for magneto/start switch
- two red on/off for alt/batery master
- two black on/off for avionics1/2 switch

At first I've thought I'd use Arduino nano, but it was more practical to get cheap Leonardo clone with HID support. This allows the solution to look like just another game controller with plenty of buttons.

# Parts
- 10x small on of switch
- 1x small rotary encoder with cap
- 4x small red/black switches for master
- arduino micro pro - AT32u4 based clone
- prototype board
- bunch of cables and connectors
- 14x 1N4148 diodes 

# HW 
Buttons are arranged into 5x5 matrix with diodes to avoid ghosting. I have wired diodes directly to the switches, so there are not components on the proto board except for connectors. Google keypad matrix for more information. I'm using only 3 columns now since we only have 14 switches, but there's a potential for more in the future. Rotary encoder with its push button is wired directly as I had a component with +5v, gnd, btn, A, B pins and builtin pullups. I have space for another rotary encoder on the microcontroller. 10 pins are used for 5 rows and cols, 3+3 pins are used for two rotary encoders. It's very simple.

# SW
The software is a simple Arduino sketch. It uses following libraries:
- Keypad
- Joystick
- RotaryEncoder 

One note is that Keypad brings row down to sense buttons. Since I had the diodes soldered the other way around I had to swap rows/columns. Every switch is mapped to two joystick buttons. One for switch being flipped on and anoother for flipped off state. Swith 1 maps to button 1(off) and 2(on), switch 2 to 3(off) and 4(on). And so on.
Rotary encoder is modified to click + and - buttons with totary encoder button being separate joystick button. Read the code, it's very simple.

I was thinking of using Xplane interface directly but then I decided to go with more universal joystick support. This allows me to map this in both xplane and msfs.

# Pictures
![Switches arrived)[01-switches.jpg)
![Placing switches into the case)[02-switches-placed.jpg)
![Proto board - 0 components)[03-proto-board.jpg)
![Look inside)[04-in-the-case.jpg)
![Rotary encoder for magnetos)[05-rotary-encoder.jpg)
![Making hole for master switches)[06-master-hole.jpg)
![Master and avionics)[07-master-btns.jpg)
![Finished mod)[08-finished-yoke-mod.jpg)
