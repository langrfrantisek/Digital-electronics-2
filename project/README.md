# Generátor signálů s využitím R-2R DAC

### Cíle projektu

Cílem projektu je vytvořit digitálně analogový převodník za použití R-2R ladder na AVR ATmega328P. Ovládání zajistí 4x3 klávesnice a zobrazování informací bude přes LCD a UART. 


## Popis harwaru

Použitý mikrokontrolér ATmega328P řídí celý generátor. Maticová klávesnice 4x3 je připojena přes dělič jedním vodičem na ADC vstupní pin PC0. Hodnoty rezistorů v děliči byly vypočítány tak, aby byl rozdíl napětí po stisku sousedních tlačítek co největší. LCD je připojeno čtyřmi datovými vodiči. R-2R leadder je připojený částečně přes PORTD a částečně přes PORTB.

![schematic](https://user-images.githubusercontent.com/71698018/102280153-e51e2100-3f2c-11eb-9f09-7c88580527f8.PNG)

## Popis kódu a simulace

![diagram](https://user-images.githubusercontent.com/71698018/102279864-6b863300-3f2c-11eb-9747-47fd06339c5d.png)

Za začátku kódu se provede inicializace, nastavení pinů, převodníku, časovačů a vypíše se na display základní menu. Veškerý program se poté ukrývá v ISR daných časovačů a ISR ADC. ISR časovače 1 spouští ADC převod. Po dokončení ADC převodu se vyhodnotí, jestli došlo ke stisku tlačítka. Vyhodnotí se, které tlačítko bylo stisknuto a na UART a display se zapíší ingormace o aktuální frekvenci a jaký signál se právě generuje. O generaci signálu se stará ISR timeru 0. Generace funkce sinus probíhá pomocí lookup tabulky.

Uživatel je po spuštění aplikace vyzván ke stisku tlačítka a na display se mu zobrazí možnosti. Po stisku tlačítka se začne generovat příslušný signál a menu se změní. Dále zobrazuje jaké tlačítko bylo stisknuto, značku příslušného signálu a aktuální frekvenci. Nabízí při stisku nuly návrat k základnímu menu a vypnutí výstupu. Stiskem * dojde ke snížení frekvence a stiskem # dojde k jejímu zvýšení.

[main.c](https://github.com/langrfrantisek/Digital-electronics-2/blob/master/project/DAC/DAC/main.c)


## Video/animace

[video](https://drive.google.com/file/d/1Q5DAWgp-w9RkpAxS_AesRBgdQx01LGIt/view?usp=sharing)


## Reference

[Peter Fleury's LCD and UART library](http://www.peterfleury.epizy.com/avr-software.html)

[sinus vzorky](http://aquaticus.info/pwm-sine-wave)

https://github.com/tomas-fryza/Digital-electronics-2
