# RSD - Rolling Shutter Display Library 

![Portada](media/Foto_inicio.jpg)

Fotos  
[<img width='15%' src='media/Triangle.jpg'/>](media/Triangle.jpg)
[<img width='15%' src='media/Filling.jpg'/>](media/Filling.jpg)
[<img width='15%' src='media/Animation.jpg'/>](media/Animation.jpg)
[<img width='15%' src='media/CopyBackground.jpg'/>](media/CopyBackground.jpg)
[<img width='15%' src='media/Streaming.jpg'/>](media/Streaming.jpg)

YouTube Videos  

[<img width='20%' src='https://i.ytimg.com/vi/spv1qrix324/0.jpg'/>](https://youtu.be/spv1qrix324)
[<img width='20%' src='https://i.ytimg.com/vi/mH6nSG8UHA0/0.jpg'/>](https://youtu.be/mH6nSG8UHA0)
[<img width='20%' src='https://i.ytimg.com/vi/pxOb_LTTh-k/0.jpg'/>](https://youtu.be/pxOb_LTTh-k)
[<img width='20%' src='https://i.ytimg.com/vi/9zj89XnVqRI/0.jpg'/>](https://youtu.be/9zj89XnVqRI)

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

Lista de componentes:

- 1 Arduino
- 1 LED RGB 5mm cátodo común (en lo posible difuso, si no añadiremos un difursor)
- 3 resistencias 220 Ω
- 1 Potenciómetro de entre 5 kΩ ~ 10 kΩ

El LED RGB nos permitirá obtener además de los 3 colores del mismo (rojo, verde y azul), sus combinaciones; es decir, amarillo (rojo + verde), cian (verde + azul), magenta (azul + rojo) y blanco (rojo + verde + azul). Contando el negro como un color, este modo nos permite representar 8 colores. Las mismas consideraciones respecto al LED tenidas en cuenta en el circuito mínimo valen para este caso: un LED RGB no deja de ser un conjunto de 3 LEDs.   
El potenciómetro, por otro lado, proporciona un control más dinámico de la sintonía del RSD.

## Software: Instalación de la librería

- Descargar este repositorio
- En el entorno de desarrollo de Arduino (IDE), vamos a Programa > Incluir Librería > Añadir librería .ZIP
- Buscamos el fichero Zip del repositorio que acabamos de descargar y hacemos click en abrir

![library installation](media/Instalar_libreria.png) 

Una vez instalada la librería, vamos a cargar el primer Sketch para comprobar que todo esté en funcionamiento. Para ello vamos a Archivo > Ejemplos > RSD-master > TestScreen_mono (en el caso del circuito mínimo) o TestScreen_RGB (en el caso del circuito recomendado).  
Una vez que cargamos el Sketch al Arduino, el LED a simple vista debería parpadear. A medida que acercamos más y más la cámara de un celular al LED, deberíamos ser capaces de ver algo similar a los siguientes patrones:

[<img width='20%' src='media/TestScreen_Mono.jpg'/>](media/TestScreen_Mono.jpg "Test Screen Mono")
[<img width='20%' src='media/TestScreen_RGB.jpg'/>](media/TestScreen_RGB.jpg "Test Screen RGB")

## Ajustes

### Cámara

[<img width='15%' src='media/ajustes/Acercamiento1.png'/>](media/ajustes/Acercamiento1.png)
[<img width='15%' src='media/ajustes/Acercamiento2.png'/>](media/media/ajustes/Acercamiento2.png)
[<img width='15%' src='media/ajustes/Acercamiento3.png'/>](media/ajustes/Acercamiento3.png)
[<img width='15%' src='media/ajustes/ISO.png'/>](media/ajustes/ISO.png)
[<img width='15%' src='media/ajustes/Final.png'/>](media/ajustes/Final.png)

La clave para obtener una buena imágen del RSD es **acercar lo más posible** la cámara al LED, de ser posible que estén en contacto. Para mejorar la nitidez de la imágen debemos aumentar el [ISO](https://es.wikipedia.org/wiki/Escala_de_sensibilidad_fotogr%C3%A1fica) o aumentar la potencia del LED. Cada cámara es distinta y permite o no modificar distintos parámetros, no hay una regla universal por lo que deberás experimentar hasta conseguir el resultado deseado.  

Difusor

[<img width='15%' src='media/ajustes/SinDifusor.png'/>](media/ajustes/SinDifusor.png)
[<img width='15%' src='media/ajustes/Final.png'/>](media/ajustes/Final.png)  
Para conseguir que los colores se mezclen de manera uniforme, es necesario un buen difusor. Las imagenes anteriores muestran un LED RGB sin y con difusor, como se puede apreciar la diferencia es notable. En este caso, se le agregó un papel de cocina que resulta ser excelente para el efecto buscado. Otro difusor común es la silicona. Hay que tener en cuenta el compromiso entre un buen difuminado y la pérdida de potencia luminosa.

### Sintonía  
Para conseguir una imágen lo más estática posible debemos sintonizar el RSD. De nuevo, cada cámara tiene distinta frecuencia y las variaciones pueden ser mínimas.  
En el caso del ejemplo TestScreen_Mono, el ajuste lo realizaremos a tráves del puerto serie, para ello en el IDE de Arduino vamos a Herramientas > Monitor serie. Una vez allí ingresando valores de entre 1 y 255, podremos modificar la frecuencia entre +-1 Hz.
En el caso del ejemplo TestScreen_RGB este ajuste lo realizaremos a través del potenciomentro.  
En los dos ejemplos anteriores, la variación se produce sobre una frecuencia base de 30 Hz que es la más común en las cámaras, sin embargo también es frecuente encontrarnos con cámaras a 24 Hz. Esta frecuencia la podemos modificar, desde el método `begin(  f_cam , BWIDTH )` en el setup() del scketch.  
Aún con esta sintonía es muy probable con notes que la pantalla sigue con un mínimo movimiento. Para un ajuste todavía más fino, abrí el ejemplo FineTuning. En este caso realizaremos el ajuste grueso a través del puerto serie y un ajuste fine con el potenciometro.

---

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licencia Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />© 2018-2019 Facundo Daguerre, esta documentación está bajo <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Licencia Creative Commons Atribución-CompartirIgual 4.0 Internacional</a>.