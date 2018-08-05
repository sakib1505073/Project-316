avr-gcc -g -Os -mmcu=atmega32 -c temp.c
avr-gcc -g -mmcu=atmega32 -o temp.elf temp.o
avr-objcopy -j .text -j .data -O ihex temp.elf temp.hex
avr-size --format=avr --mcu=atmega32 temp.elf
avrdude -c usbasp -p m32 -P /dev/ttyACM0 -B10 -U flash:w:temp.hex -F