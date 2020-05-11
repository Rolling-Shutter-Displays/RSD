# RSD - Rolling Shutter Display Library 

![Portada](media/Foto_inicio.jpg)

Photos   
[<img width='15%' src='media/Triangle.jpg'/>](media/Triangle.jpg)
[<img width='15%' src='media/Filling.jpg'/>](media/Filling.jpg)
[<img width='15%' src='media/Animation.jpg'/>](media/Animation.jpg)
[<img width='15%' src='media/CopyBackground.jpg'/>](media/CopyBackground.jpg)
[<img width='15%' src='media/Streaming.jpg'/>](media/Streaming.jpg)

YouTube videos  
[<img width='20%' src='https://i.ytimg.com/vi/spv1qrix324/0.jpg'/>](https://youtu.be/spv1qrix324)
[<img width='20%' src='https://i.ytimg.com/vi/mH6nSG8UHA0/0.jpg'/>](https://youtu.be/mH6nSG8UHA0)
[<img width='20%' src='https://i.ytimg.com/vi/pxOb_LTTh-k/0.jpg'/>](https://youtu.be/pxOb_LTTh-k)
[<img width='20%' src='https://i.ytimg.com/vi/9zj89XnVqRI/0.jpg'/>](https://youtu.be/9zj89XnVqRI)   

### [en]    
Tutorial en [español aquí](#es)

## What is a Rolling Shutter Display (RSD)?

A Rolling Shutter Display, abbreviated RSD, is a device capable of producing images and videos through the set of a digital camera (CMOS) and one or more [LEDs](https://en.wikipedia.org/wiki/Light-emitting_diode), making use of the phenomenon of interference between a signal responsible for turning on and off the LED and the scanning frequency of the camera ([Rolling Shutter](https://en.wikipedia.org/wiki/Rolling_shutter)).

Rolling Shutter Display Library is a library for Arduino UNO and the like, *Arduino Nano, Mini, Adafruit METRO 328, LilyPad Simple Board, etc; that is, architectures based on the ATmega328* microcontroller. It provides methods and functions to tune and control the LEDs, in addition to a simple programming interface (API) to facilitate on-screen drawing.

This library allows us to create an RSD with only Arduino and an LED!

## Hardware
Arduino already has a built-in LED and in fact it is possible to build an RSD with it, so in principle, an Arduino is all that is needed. However, in order to appreciate the phenomenon more effectively through a cell phone camera, for example, it is convenient to use a high-brightness LED that is ***as close as possible to the camera***.

### Minimal circuit
![minimal setup](media/Circuito_minimo_conjunto.png) 

The minimal circuit is an LED connected to *any* of the Arduino's pins with its corresponding current limiting resistor.    
The resistance value will depend on the color of the LED and the current that we want to circulate through it. Keep in mind that the maximum current that an Arduino pin can supply is around 40 mA, but it is recommended not to exceed 20 mA. For most cases, a 220 Ω resistor it will be enough. For a more precise calculation we recommend this [resistance calculator for leds](https://www.inventable.eu/paginas/LedResCalculatorSp/LedResCalculatorSp.html).        
Another issue to consider is the orientation of the LED. In the proposed circuit the cathode is connected to the common (GND), this configuration is known as the common cathode. In 5mm LEDs, the cathode is identified by a flat mark on the package, and its *terminal is shorter* than the anode. There would be no problem connecting it the other way around, that is, in the common anode configuration (in this case the anode is connected to positive VCC, and the cathode through the resistance to the Arduino pin); in any case, when programming we will have to tell the library what configuration it is in.    
We will tune the RSD in this scheme through the serial port.

### Recommended circuit
![recommended setup](media/Circuito_recomendado_conjunto.png) 

Component list:

- 1 Arduino
- 1 common anode 5mm RGB LED (if possible diffused, if not we will add a diffuser)
- 3 resistors 220 Ω
- 1 potentiometer between 5 kΩ ~ 10 kΩ

The RGB LED will allow us to obtain, in addition to its 3 colors (red, green and blue), its combinations; that is, yellow (red + green), cyan (green + blue), magenta (blue + red), and white (red + green + blue). Counting black as a color, this mode allows us to represent 8 colors. The same considerations regarding the LED taken into account in the minimal circuit apply in this case: an RGB LED is still a set of 3 LEDs.  
The potentiometer on the other hand, provides more dynamic control of the tuning of the RSD.

## Software: Library installation

- Download this repository
- In the Arduino development environment (IDE), go to Program > Include Library > Add .ZIP Library
- Look for the Zip file of the repository that just downloaded and click on open

![library installation](media/Instalar_libreria.png) 

Once the library is installed, we will load the first Sketch to verify that everything is working. For this go to File > Examples > RSD-master > TestScreen_One_LED (in the case of the minimal circuit) or TestScreen_RGB_LED (in the case of the recommended circuit).    
Once we load the Sketch, the LED with the naked eye should be flickering. As we move the cell phone camera closer and closer to the LED, we should be able to see something similar to the following patterns:

[<img width='20%' src='media/TestScreen_One_LED.jpg'/>](media/TestScreen_One_LED.jpg "Test Screen One")
[<img width='20%' src='media/TestScreen_RGB_LED.jpg'/>](media/TestScreen_RGB_LED.jpg "Test Screen RGB")

To improve the definition and stability of the image we must make some adjustments that are detailed below.

## Adjustments

### Camera

#### Distance

[<img width='20%' src='media/ajustes/Acercamiento1.png'/>](media/ajustes/Acercamiento1.png)
[<img width='20%' src='media/ajustes/Acercamiento2.png'/>](media/media/ajustes/Acercamiento2.png)
[<img width='20%' src='media/ajustes/Acercamiento3.png'/>](media/ajustes/Acercamiento3.png)

The key to getting a good RSD image is **getting the camera as close as possible** to the LED, or vice versa; if possible that they are in contact and centered.

#### Film speed (ISO)

[<img width='20%' src='media/ajustes/Acercamiento3.png'/>](media/ajustes/Acercamiento3.png)
[<img width='20%' src='media/ajustes/ISO.png'/>](media/ajustes/ISO.png)
[<img width='20%' src='media/ajustes/Final.png'/>](media/ajustes/Final.png)

Each camera may or may not modify different parameters in the different modes it has. To improve the definition of the image we must increase the [ISO](https://en.wikipedia.org/wiki/Film_speed) that is generally accessed in professional or manual mode.

### LED

#### Brightness 

It may be the case that the camera does not allow to control the ISO or, for example, it does allow to control it in the professional camera mode but not in the video mode. The solution in those cases is to increase the power of the LED.       
Increasing the power in addition to improving the definition, also allows the use of better diffusers (see below) and increases the area and distance at which the RSD image can be observed. Of course there is a limit, after a certain threshold the camera becomes saturated and any color looks white.  
To increase power, the proposed circuit must be modified, a topic that is beyond the scope of this document.    

#### Diffuser

[<img width='20%' src='media/ajustes/SinDifusor.png'/>](media/ajustes/SinDifusor.png)
[<img width='20%' src='media/ajustes/Final.png'/>](media/ajustes/Final.png)  

To get the colors to blend smoothly we need a good diffuser.    
The images above show an LED (RGB Clear) without and with diffuser, as you can see the difference is noticeable. In this case, a strip of kitchen paper was added, which turns out to be excellent for the desired effect. Another common material is silicone. There are LEDs whose encapsulation is a diffuser itself (RGB Diffused). 
The compromise between a good fade and the loss of light power must be taken into account with the consequent loss of definition in the image.

### RSD - Tuning

To get the most static image possible we must tune the RSD. Cellular cameras generally work at 30 frames per second, that is to say 30 Hz, but they are usually also at 24 frames / second (24 Hz). Each camera has a different frequency and the variations can be minimal on the order of + -0.0001 Hz.   

In the case of the TestScreen_One_LED example, the adjustment will be made through the serial port. For this, in the Arduino IDE go to Tools > Serial Monitor. Once there enter the frequency, for example `30.002` and press enter.   
In the case of the TestScreen_RGB_LED example we will make this adjustment through the potentiometer. The serial monitor shows the frequency at which the RSD is currently operating and its resolution in lines.   

## Explore  

Rolling Shutter Display Library comes with several examples that generate images and animations, these are found in File > Examples > RSD-master > ... 

| CopyBackground       | Filling              | Animation            |
| :------------------: | :------------------: | :------------------: |
| [<img width='100%' src='https://i.ytimg.com/vi/pxOb_LTTh-k/0.jpg'/>](https://youtu.be/pxOb_LTTh-k)    | [<img width='100%' src='https://i.ytimg.com/vi/9zj89XnVqRI/0.jpg'/>](https://youtu.be/9zj89XnVqRI) | [<img width='100%' src='https://i.ytimg.com/vi/mH6nSG8UHA0/0.jpg'/>](https://youtu.be/mH6nSG8UHA0) |    

These examples show different functions and ways of using the library. At the moment a detailed documentation is not available (TODO: [doxygen](http://www.doxygen.nl/)).   
The code and examples are intended to show the structure of the program and several of these methods and functions. Happy Hacking!  

We would like to know if you are using RSD, with which camera and at what frequency, your images and experiences, suggestions and concerns. Contact us on social networks:   
[Instagram @rollingshutterdisplays](https://www.instagram.com/rollingshutterdisplays/)   
[Rolling Shutter Displays Facebook Page](https://www.facebook.com/RollingShutterDisplays)   
[Rolling Shutter Displays Facebook Group](https://www.facebook.com/groups/rollingshutterdisplays/)  
[Rolling Shutter Displays YouTube Channel](https://www.youtube.com/channel/UCgTTSPkXvOK5V8WAWk7dh2A)

---

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licencia Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />© 2018-2020 Made with ❤ by derfaq for Rolling Shutter Displays, this documentation is under <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">License Crative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)</a>.

---
 
 

### [es]    
Tutorial in [English here](#en)

## ¿Qué es un Rolling Shutter Display (RSD)?

Un Rolling Shutter Display, abreviado RSD, es un dispositivo capaz de producir imágenes y videos a través del conjunto de una cámara digital (CMOS) y uno o más [LEDs](https://es.wikipedia.org/wiki/Led), haciendo uso para ello, del fenómeno de interferencia entre una señal encargada de encender y apagar el/los LED/s y la frecuencia de escaneo de la cámara ([Rolling Shutter](https://es.wikipedia.org/wiki/Rolling_shutter)).

Rolling Shutter Display Library es una librería para Arduino UNO y semejantes, *Arduino Nano, Mini, Adafruit METRO 328, LilyPad Simple Board, etc; es decir, arquitecturas basadas en el microcontrolador ATmega328*. Provee métodos y funciones para sintonizar y controlar el/los LEDs, además de una interfaz de programación simple (API) para facilitar el dibujo en pantalla.

Esta librería nos permite crear un RSD con tan solo un Arduino y un LED!

## Hardware

Arduino ya trae incorporado un LED y de hecho es posible construir un RSD con el mismo, por lo que en principio, un Arduino es todo lo que se necesita. Sin embargo, para poder apreciar el fenómeno de una manera mas efectiva a través de la cámara de un celular por ejemplo, es conveniente usar un LED de alto brillo y que esté al alcance ***lo mas cercano posible de la cámara***.

### Circuito mínimo

![minimal setup](media/Circuito_minimo_conjunto.png) 

El circuito mínimo es un LED conectado a *cualquiera* de los pins del Arduino con su correspondiente resistencia limitadora de corriente. 
El valor de la resistencia dependerá del color del LED y de la corriente que queramos que circule por el mismo. Hay que tener en cuenta que la máxima corriente que puede suministrar un pin de Arduino es de alrededor de 40 mA, pero se recomienda no superar los 20 mA. Para la mayoría de los casos, una resistencia de 220 Ω será suficiente. Para un cálculo más preciso recomendamos esta [calculadora de resistencias para leds](https://www.inventable.eu/paginas/LedResCalculatorSp/LedResCalculatorSp.html).  
Otra cuestión a tener en cuenta es la orientación del LED. En el circuito propuesto el cátodo está conectado al común (GND), esta configuración se conoce como cátodo común. En LEDs de 5mm, el catodo se identifica con una marca plana en el encapsulado, y además su *terminal es más corta* que la del ánodo. No habría inconveniente de conectarlo al revés, es decir en la configuración ánodo común (en esté caso el ánodo esta conectado a positivo VCC, y el catodo a través de la resistencia al pin de Arduino); en cualquier caso, a la hora de programar tendremos que indicarle a la librería en que configuración se encuentra.  
La sintonía del RSD en este esquema la haremos a través del puerto serie.

### Circuito recomendado

![recommended setup](media/Circuito_recomendado_conjunto.png) 

Lista de componentes:

- 1 Arduino
- 1 LED RGB 5mm ánodo común (en lo posible difuso, si no añadiremos un difursor)
- 3 resistencias 220 Ω
- 1 potenciómetro de entre 5 kΩ ~ 10 kΩ

El LED RGB nos permitirá obtener además de los 3 colores del mismo (rojo, verde y azul), sus combinaciones; es decir, amarillo (rojo + verde), cian (verde + azul), magenta (azul + rojo) y blanco (rojo + verde + azul). Contando el negro como un color, este modo nos permite representar 8 colores. Las mismas consideraciones respecto al LED tenidas en cuenta en el circuito mínimo valen para este caso: un LED RGB no deja de ser un conjunto de 3 LEDs.   
El potenciómetro por otro lado, proporciona un control más dinámico de la sintonía del RSD.

## Software: Instalación de la librería

- Descargar este repositorio
- En el entorno de desarrollo de Arduino (IDE), vamos a Programa > Incluir Librería > Añadir librería .ZIP
- Buscamos el fichero Zip del repositorio que acabamos de descargar y hacemos click en abrir

![library installation](media/Instalar_libreria.png) 

Una vez instalada la librería, vamos a cargar el primer Sketch para comprobar que todo esté en funcionamiento. Para ello vamos a Archivo > Ejemplos > RSD-master > TestScreen_One_LED (en el caso del circuito mínimo) o TestScreen_RGB_LED (en el caso del circuito recomendado).  
Una vez que cargamos el Sketch, el LED a simple vista debería parpadear. A medida que acercamos más y más la cámara de un celular al LED, deberíamos ser capaces de ver algo similar a los siguientes patrones:

[<img width='20%' src='media/TestScreen_One_LED.jpg'/>](media/TestScreen_One_LED.jpg "Test Screen One")
[<img width='20%' src='media/TestScreen_RGB_LED.jpg'/>](media/TestScreen_RGB_LED.jpg "Test Screen RGB")

Para mejorar la nitidez y la estabilidad de la imágen debemos realizar algunos ajustes que se detallan a continuación.

## Ajustes

### Cámara

#### Distancia

[<img width='20%' src='media/ajustes/Acercamiento1.png'/>](media/ajustes/Acercamiento1.png)
[<img width='20%' src='media/ajustes/Acercamiento2.png'/>](media/media/ajustes/Acercamiento2.png)
[<img width='20%' src='media/ajustes/Acercamiento3.png'/>](media/ajustes/Acercamiento3.png)

La clave para obtener una buena imágen RSD es **acercar lo más posible** la cámara al LED, o viceversa; de ser posible que estén en contacto y centrados.

#### Sensibilidad fotográfica (ISO)

[<img width='20%' src='media/ajustes/Acercamiento3.png'/>](media/ajustes/Acercamiento3.png)
[<img width='20%' src='media/ajustes/ISO.png'/>](media/ajustes/ISO.png)
[<img width='20%' src='media/ajustes/Final.png'/>](media/ajustes/Final.png)

Cada cámara permite o no, modificar distintos parámetros en los distintos modos que posee. Para mejorar la nitidez de la imágen debemos aumentar el [ISO](https://es.wikipedia.org/wiki/Escala_de_sensibilidad_fotogr%C3%A1fica) que generalmente se accede en el modo profesional o manual.

### LED

#### Potencia 

Puede darse el caso que la cámara no permite controlar el ISO o que, por ejemplo, sí permite controlarlo en el modo de cámara profesional pero no en el modo video. La solución en esos casos es aumentar la potencia del LED.  
Aumentar la potencia además de mejorar la nitidez, también permite el uso de mejores difusores (ver a continuación) y aumenta el area y la distancia a la que puede observarse la imágen RSD. Por supuesto tiene un límite, a partir de cierto umbral la cámara se satura y cualquier color se ve blanco.   
Para aumentar la potencia hay que modificar el circuito propuesto, tema que queda fuera del alcance de este documento.

#### Difusor

[<img width='20%' src='media/ajustes/SinDifusor.png'/>](media/ajustes/SinDifusor.png)
[<img width='20%' src='media/ajustes/Final.png'/>](media/ajustes/Final.png)  

Para conseguir que los colores se mezclen de manera uniforme es necesario un buen difusor.  
Las imagenes anteriores muestran un LED (RGB Clear/Transparente) sin y con difusor, como se puede apreciar la diferencia es notable. En este caso, se le agregó una tira de papel de cocina que resulta ser excelente para el efecto buscado. Otro material común es la silicona. Existen LEDs cuyo encapsulado es un difusor en sí (RGB Diffused/Difuso).  
Hay que tener en cuenta el compromiso entre un buen difuminado y la pérdida de potencia luminosa con la consecuente pérdida de nitidez en la imágen.

### RSD - Sintonía

Para conseguir una imágen lo más estática posible debemos sintonizar el RSD. Generalmente las cámaras de celulares trabajan a 30 cuadros por segundo, es decir 30 Hz, pero suelen encontrase también a 24 cuadros/segundo (24 Hz).  
Cada cámara tiene distinta frecuencia y las variaciones pueden ser mínimas del orden de los +-0.0001 Hz.  

En el caso del ejemplo TestScreen_One_LED, el ajuste lo realizaremos a tráves del puerto serie. Para ello en el IDE de Arduino vamos a Herramientas > Monitor serie. Una vez allí ingresamos la frecuencia, por ejemplo `30.002` y pulsamos enter.  
En el caso del ejemplo TestScreen_RGB_LED este ajuste lo realizaremos a través del potenciomentro. En el monitor serie muestra la frecuencia a la que actualmente está operando el RSD y la resolución en líneas del mismo.   

## Explora  

Rolling Shutter Display Library viene con varios ejemplos que generan imágenes y animaciones, estos se encuentran en Archivo > Ejemplos > RSD-master > ...  

| CopyBackground       | Filling              | Animation            |
| :------------------: | :------------------: | :------------------: |
| [<img width='100%' src='https://i.ytimg.com/vi/pxOb_LTTh-k/0.jpg'/>](https://youtu.be/pxOb_LTTh-k)    | [<img width='100%' src='https://i.ytimg.com/vi/9zj89XnVqRI/0.jpg'/>](https://youtu.be/9zj89XnVqRI) | [<img width='100%' src='https://i.ytimg.com/vi/mH6nSG8UHA0/0.jpg'/>](https://youtu.be/mH6nSG8UHA0) |    

Esto ejemplos muestran distintas funciones y modos de empleo de la librería. Por el momento no se encuentra disponible una documentación detallada de la misma (TODO: [doxygen](http://www.doxygen.nl/)).  
El código y los ejemplos estan orientados a mostrar la estructura del programa y varios de estos métodos y funciones. Happy Hacking!

Nos gustaría saber si estas usando RSD, con que cámara y a que frecuencia, tus imagenes y experiencias, sugerencias e inquietudes. Contáctanos en redes sociales:   
[Instagram @rollingshutterdisplays](https://www.instagram.com/rollingshutterdisplays/)   
[Rolling Shutter Displays Pagina Facebook](https://www.facebook.com/RollingShutterDisplays)   
[Rolling Shutter Displays Grupo Facebook](https://www.facebook.com/groups/rollingshutterdisplays/)  
[Rolling Shutter Displays Canal YouTube](https://www.youtube.com/channel/UCgTTSPkXvOK5V8WAWk7dh2A)


---

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licencia Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />© 2018-2020 Hecho con ❤ por derfaq para Rolling Shutter Displays, esta documentación está bajo <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Licencia Creative Commons Atribución-CompartirIgual 4.0 Internacional</a>.

---