# Arduino Robot Arm

Uses ANET A8 Mainboard

## Uploading using Arduino
Get this board for board manager in arduino
```
https://github.com/benlye/anet-board/raw/master/package_anet_board_index.json
```

## Wiring
https://github.com/ralf-e/ANET-3D-Board-V1.0/blob/master/ANET3D_Board_Schematic.png

## End Stops
```
x-stop -> pin 16
```
Will pull low when triggered

## Controlling Stepper Motors
https://www.makerguides.com/a4988-stepper-motor-driver-arduino-tutorial/

## Stepper Motor Driver
A4988
Datasheet:
https://www.pololu.com/file/0J450/a4988_DMOS_microstepping_driver_with_translator.pdf

## LCD Screen
Uses HD44780 LCD
Wired using https://github.com/ralf-e/ANET-3D-Board-V1.0/blob/master/ANET3D_LCD2004_board.png

Can control using 
```
LiquidCrystal lcd(LCD_PINS_RS,
                  LCD_PINS_ENABLE,
                  LCD_PINS_D4,
                  LCD_PINS_D5,
                  LCD_PINS_D6,
                  LCD_PINS_D7);
```
