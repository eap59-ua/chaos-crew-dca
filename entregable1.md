1.- 27,05s 2.- 26,932 3.- 16,416 4.- 9,603 5.- 8,023 6.- 7,083 7.- 6,458 8.- 7,463 9.- 5, 722 10.- 6,398 11.- 5,898
1.- Comprobad tiempos de ejecución de la compilación con diversos valores para N en -jN. ¿A partir de qué valores de N ya no supone una mejora sustancial el incremento en el número de trabajos en paralelo?

A partir de entre -j2 y -j3 encontramos la mayor mejora de diferencia de tiempo entre t-j3 = 9,603 frente a t -j2 = 16,416 una mejora (2,804 - 1,641)/1,641*100 = 71% frente a la mejora poco sustancial entre t -j3 = 9,603 y t -j4 = 8,023 (3,357-2,804)/2,804*100 = 19,7% a partir de aquí no encontramos una mayor mejora que la de -j3 con respecto a -j2

2.- ¿Hay fallos de compilación con ejecuciones en paralelo? Si es así tratad de ver por qué se producen y procurad solucionarlos.
No hemos encontrado fallos



| -j (hilos) | Tiempo (s) | Speedup T_ref / Tn              | Eficiencia E = S / N |
|-----------:|-----------:|--------------------------------:|-----------------------------------:|---------------------:|
| 1          | 26.932     | 1.000                           | 100%              |
| 2          | 16.416     | 1.641                           | 82%               |
| 3          | 9.603      | 2.804                           | 93 %               |
| 4          | 8.023      | 3.357                           | 84 %               |
| 5          | 7.083      | 3.803                           | 76 %               |
| 6          | 6.458      | 4.171                           | 70 %               |
| 7          | 7.463      | 3.609                           | 52 %               |
| 8          | 5.722      | 4.707                           | 59 %               |
| 9         | 6.398      | 4.209                            | 47 %               |
| 10         | 5.898      | 4.566                           | 46 %               |

3.-Aplicad ccache en la compilación del proyecto y compara el tiempo de ejecución de la compilación con y sin esta herramienta.
Realizamos una comparativa de los tiempos de compilación con y sin ccache utilizando -j4 para paralelización
    sin ccache : 9,449s
    con ccache primera compilación : 9,678s
    con ccache segunda compilación : 0,286

    ccache proporciona una muy buena ganancia de 33.04x en recompilaciones cuando los archivos no han cambiado. En la primera compilación, el overhead de ccache es mínimo , añadiendo solo [0.229s] al tiempo total debido al cálculo de hashes y almacenamiento en caché.
    En la segunda compilación, al reutilizar los objetos ya compilados y cacheados, el tiempo se reduce drásticamente de 9,449s a 0.2860s.
    ccache es altamente efectivo para acelerar recompilaciones durante el desarrollo, especialmente cuando se cambian solo algunos archivos del proyecto. 


4.-Borrad las estadísticas de ccache y limpiadla. Después, ejecutad dos veces la compilación con esta herramienta y analizad las estadísticas que proporciona la herramienta.
    1ªcompilación 9,678s
  Estadísticas:
Cacheable calls:     15 /  16 (93.75%)
  Hits:               0 /  15 ( 0.00%)
    Direct:           0
    Preprocessed:     0
  Misses:            15 /  15 (100.0%)
Uncacheable calls:    1 /  16 ( 6.25%)
Local storage:
  Cache size (GiB): 0.0 / 5.0 ( 0.01%)
  Hits:               0 /  15 ( 0.00%)
  Misses:            15 /  15 (100.0%)

  En la primera compilación, casi todos los archivos .cpp generan cache miss ya que la caché estaba vacía ccache compila cada archivo normalmente pero almacena los resultados en la caché, ocupando 50 MiB] de espacio 0,01% de 5.0 MiB

Segunda compilación 0,286s
Cacheable calls:     30 /  32 (93.75%)
  Hits:              15 /  30 (50.00%)
    Direct:          15 /  15 (100.0%)
    Preprocessed:     0 /  15 ( 0.00%)
  Misses:            15 /  30 (50.00%)
Uncacheable calls:    2 /  32 ( 6.25%)
Local storage:
  Cache size (GiB): 0.0 / 5.0 ( 0.01%)
  Hits:              15 /  30 (50.00%)

  En la segunda compilación, todos los archivos cacheables se reutilizan directamente de la caché (15 cache hits), evitando por completo la recompilación.
  Los ddirect hits al 100% indican que ccache pudo verificar que los archivos no habían cambiado usando solo los hashes, sin necesidad de ejecutar el preprocesador.
  ccache funciona almacenando el resultado de compilaciones previas y reutilizándolos cuando detecta que el código fuente y sus dependencias no han cambiado.El tiempo se redujo de 9.678s a 0.286s, un speedup de 33.84x, aquí podemos observar la efectividad de ccache en recompilaciones sin cambios.
    
