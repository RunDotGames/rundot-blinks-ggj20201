PATH=$PATH:/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/
config=~/Documents/Arduino/hardware/Move38-manual/avr/avrdude.conf
avrdude -C $config -B 5 -v -patmega168pb -cusbtiny -Uflash:w:./dist/main.ino.with_bootloader.standard.hex:i -Uefuse:w:0xf8:m -u