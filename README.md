
# IoT

Projects created mainly using Microchip Studio.

---

## Purpose

This repo contains projects used for learning and praticing embedded programming. 
These projects are mainly not written using the Arduino IDE, but rather C through the Microchip Studio IDE.

---

## Projects

This repo contains multiple projects, there are: 
 - FirstProject, a basic project for introduction into the IDE and Embedded.
 - ATMega2560Bootloader, the bootloader that is needed for Arduino Uno to work. Do note that it does contain some program code that is not needed. 

---

## Notes

It will optimise unused values away, so need it change the level under Toolchain -> Optimisation and change it to 'None' from 'Optimise debugging experience'. 'None' is good for development, but can cause problems since it permits not needing Volentile and such.




