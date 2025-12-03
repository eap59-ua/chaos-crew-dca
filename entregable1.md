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

31,362s
32,161

4.-Borrad las estadísticas de ccache y limpiadla. Después, ejecutad dos veces la compilación con esta herramienta y analizad las estadísticas que proporciona la herramienta.

Al principio comete en todos los intentos fallos porque no tiene referencias en la caché está vacía hasta que ya se va llenando y el tiempo era de 29,171s
Pero en el segundo tiempo nos
31,503