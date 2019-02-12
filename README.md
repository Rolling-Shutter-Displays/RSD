## RSD - Rolling Shutter Display Library 

![RSD Images]()

![RSD Videos]()

### ¿Qué es un Rolling Shutter Display (RSD)?

Un Rolling Shutter Display, abreviado RSD, es un dispositivo capaz de producir imágenes y videos a través del conjunto de una cámara digital (CMOS) y uno o más [LEDs](https://es.wikipedia.org/wiki/Led), haciendo uso para ello, del fenómeno de interferencia entre una señal encargada de encender y apagar el/los LED/s y la frecuencia de escaneo de la cámara ([Rolling Shutter](https://es.wikipedia.org/wiki/Rolling_shutter)).

Rolling Shutter Display Library, es una librería para Arduino UNO y semejantes, *Arduino Nano, Mini, Adafruit METRO 328, LilyPad Simple Board, etc; es decir, arquitecturas basadas en el microcontrolador ATmega328*. Provee métodos y funciones para sintonizar y controlar el/los LEDs, ademas de una interfaz de programación simple (API), para facilitar el dibujo en pantalla.

Esta librería nos permite crear un RSD, con tan solo un Arduino y un LED!. Así que, antes de entrar en detalles, no hay excusas para que no la pruebes por vos mismo.

### Hardware: Circuitos básicos
Por lo general, Arduino ya trae incorporado un LED en la placa (LED_BUILTIN). Sin embargo para poder apreciar el fénomeno de una manera mas efectiva, a través de la cámara de un celular por ejemplo, es conveniente usar un LED de alto brillo y que esté al alcance ***lo mas cercano posible de la cámara***.

#### Circuito mínimo
![minimal setup](/media/Circuito_minimo_conjunto.png) 

El circuito mínimo es un LED conectado a *cualquiera* de los pins del Arduino con su correspondiente resistencia limitadora de corriente. 
El valor de la resistencia dependerá del color del LED y de la corriente que querramos que circule por el mismo. Hay que tener en cuenta que la máxima corriente que puede suministrar un pin de Arduino es de alrededor de 40 mA, pero se recomienda no superar los 20 mA. Para la mayoría de los casos, una resistencia de 220 Ω será suficiente. Para un cálculo más preciso te recomiendo esta [calculadora de resistencias para leds](https://www.inventable.eu/paginas/LedResCalculatorSp/LedResCalculatorSp.html).
Otra cuestión a tener en cuenta es la orientación del LED. En el circuito propuesto el catodo está conectado al común (GND), esta configuración se conoce como catodo común. En LEDs de 5mm, el catodo se identifica con una marca plana en el encapsulado, y además su *terminal es mas corta* que la del anodo. No habría inconveniente de conectarlo al revés, es decir en la configuración anodo común; en cualquier caso, a la hora de programar, tendremos que indicarle a la librería en que configuración se encuentra.

#### Circuito recomendado
![recommended setup](/media/Circuito_recomendado_conjunto.png) 

### Instalación de la librería

### Cámara

### Uso de la librería

### FAQ

### Manifiesto del proyecto

---

Copyright (C) 2018 Facundo Daguerre
