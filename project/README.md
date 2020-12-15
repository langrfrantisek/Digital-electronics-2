# Generátor signálů s využitím R-2R DAC

### Cíle projektu

Cílem projektu je vytvořit digitálně analogový převodník za použití R-2R ladder na AVR ATmega328P. Ovládání zajistí 4x3 klávesnice a zobrazování informací bude přes LCD a UART. 


## Popis harwaru

Použitý mikrokontrolér ATmega328P řídí celý generátor. Maticová klávesnice 4x3 je připojena přes dělič jedním vodičem na ADC vstupní pin PC0. Hodnoty rezistorů v děliči byly vypočítány tak, aby byl rozdíl napětí po stisku sousedních tlačítek co největší. LCD je připojeno čtyřmi datovými vodiči. R-2R leadder je připojený částečně přes PORTD a částečně přes PORTB.
circuit diagram

## Popis kódu a simulace

![alt text](https://github.com/langrfrantisek/Digital-electronics-2/blob/master/project/diagram.pdf)

[main.c](https://github.com/langrfrantisek/Digital-electronics-2/blob/master/project/DAC/DAC/main.c)


## Video/animace

*Write your text here*


## Reference

[Peter Fleury's LCD and UART library](http://www.peterfleury.epizy.com/avr-software.html)
[sinus vzorky](http://aquaticus.info/pwm-sine-wave)
https://github.com/tomas-fryza/Digital-electronics-2
