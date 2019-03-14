# RSD - Rolling Shutter Display Library 

![Portada](media/Foto_inicio.jpg)

Fotos  
[<img width='20%' src='media/Triangle.jpg'/>](Triangle)
[<img width='20%' src='media/Filling.jpg'/>](Filling)
[<img width='20%' src='media/Animation.jpg'/>](Animation)
[<img width='20%' src='media/CopyBackground.jpg'/>](CopyBackgroung)
[<img width='20%' src='media/Streaming.jpg'/>](Streaming)

Videos  
![RSD Videos]()

## ¿Qué es un Rolling Shutter Display (RSD)?

Un Rolling Shutter Display, abreviado RSD, es un dispositivo capaz de producir imágenes y videos a través del conjunto de una cámara digital (CMOS) y uno o más [LEDs](https://es.wikipedia.org/wiki/Led), haciendo uso para ello, del fenómeno de interferencia entre una señal encargada de encender y apagar el/los LED/s y la frecuencia de escaneo de la cámara ([Rolling Shutter](https://es.wikipedia.org/wiki/Rolling_shutter)).

Rolling Shutter Display Library, es una librería para Arduino UNO y semejantes, *Arduino Nano, Mini, Adafruit METRO 328, LilyPad Simple Board, etc; es decir, arquitecturas basadas en el microcontrolador ATmega328*. Provee métodos y funciones para sintonizar y controlar el/los LEDs, además de una interfaz de programación simple (API), para facilitar el dibujo en pantalla.

Esta librería nos permite crear un RSD, con tan solo un Arduino y un LED!. Así que, antes de entrar en detalles, no hay excusas para que no la pruebes por vos mismo.

## Hardware: Circuitos básicos
Arduino ya trae incorporado un LED y de hecho es posible [construir un RSD con el mismo](youtube), por lo que, en principio, un Arduino es todo lo que se necesita. Sin embargo para poder apreciar el fenómeno de una manera mas efectiva, a través de la cámara de un celular por ejemplo, es conveniente usar un LED de alto brillo y que esté al alcance ***lo mas cercano posible de la cámara***.

### Circuito mínimo
![minimal setup](media/Circuito_minimo_conjunto.png) 

El circuito mínimo es un LED conectado a *cualquiera* de los pins del Arduino con su correspondiente resistencia limitadora de corriente. 
El valor de la resistencia dependerá del color del LED y de la corriente que queramos que circule por el mismo. Hay que tener en cuenta que la máxima corriente que puede suministrar un pin de Arduino es de alrededor de 40 mA, pero se recomienda no superar los 20 mA. Para la mayoría de los casos, una resistencia de 220 Ω será suficiente. Para un cálculo más preciso te recomiendo esta [calculadora de resistencias para leds](https://www.inventable.eu/paginas/LedResCalculatorSp/LedResCalculatorSp.html).  
Otra cuestión a tener en cuenta es la orientación del LED. En el circuito propuesto el cátodo está conectado al común (GND), esta configuración se conoce como cátodo común. En LEDs de 5mm, el catodo se identifica con una marca plana en el encapsulado, y además su *terminal es más corta* que la del ánodo. No habría inconveniente de conectarlo al revés, es decir en la configuración ánodo común; en cualquier caso, a la hora de programar, tendremos que indicarle a la librería en que configuración se encuentra.  
La sintonía del RSD en este esquema la haremos a través del puerto serie.

### Circuito recomendado
![recommended setup](media/Circuito_recomendado_conjunto.png) 

El circuito recomendado, consiste en un LED RGB y un potenciómetro de 5 kΩ ~ 10 kΩ. Un LED RGB nos permitirá obtener además de los 3 colores del mismo (rojo, verde y azul), sus combinaciones; es decir, amarillo (rojo + verde), cian (verde + azul), magenta (azul + rojo) y blanco (rojo + verde + azul). Contando el negro como un color, este modo nos permite representar 8 colores. Las mismas consideraciones respecto al LED tenidas en cuenta en el circuito mínimo valen para este caso: un LED RGB no deja de ser un conjunto de 3 LEDs.   
El potenciómetro, por otro lado, proporciona un control más dinámico de la sintonía del RSD. La diferencia más importante con el circuito mínimo es que no necesitamos tener conectado el Arduino a la PC para sintonizar el RSD.

## Software: Instalación de la librería

- Descargar este repositorio
- En el entorno de desarrollo de Arduino (IDE), vamos a Programa > Incluir Librería > Añadir librería .ZIP
- Buscamos el fichero Zip del repositorio que acabamos de descargar y hacemos click en abrir

![library installation](media/Instalar_libreria.png) 

Una vez instalada la librería, vamos a cargar el primer Sketch para comprobar que todo esté en funcionamiento. Para ello vamos a Archivo > Ejemplos > RSD-master > TestScreen_mono (en el caso del circuito mínimo) o TestScreen_RGB (en el caso del circuito recomendado).  
Una vez que cargamos el Sketch al Arduino, el LED a simple vista debería parpadear. A medida que acercamos más y más la cámara de un celular al LED, deberíamos ser capaces de ver algo similar a los siguientes patrones:

[<img width='50%' src='media/TestScreen_Mono.jpg'/>](TestScreen_Mono "Test Screen Mono")
[<img width='50%' src='media/TestScreen_RGB.jpg'/>](TestScreen_RGB "Test Screen RGB")

## Ajustes

### Sintonía

### Cámara

## Uso de la librería

## FAQ

## Manifiesto del proyecto

---

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Licencia Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br /> © 2018-2019 Facundo Daguerre, esta documentación está bajo <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Licencia Creative Commons Atribución 4.0 Internacional</a>.
