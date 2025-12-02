1.- 27,05s 2.- 26,932 3.- 16,416 4.- 9,603 5.- 8,023 6.- 7,083 7.- 6,458 8.- 7,463 9.- 5, 722 10.- 6,398 11.- 5,898

A partir de -j4 encontramos la mayor mejora de diferencia de tiempo entre t-j3 = 16,416 frente a t -j4 = 9,603 una mejora (16,416-9,603)/16,416100 = 41% frente a la mejora poco sustancial entre t -j4 = 9,603 y t -j5 = 8,023 (9,603-8,023)/9,603100 = 16% a partir de aquí no encontramos una mayor mejora que la de -j4

**Resultados (se omite 27,05 s). Referencia T_ref = 26.932 s (-j2).**

| -j (hilos) | Tiempo (s) | Speedup absoluto S = T_ref / Tn | Speedup relativo S_rel = Tprev / Tn | Eficiencia E = S / N |
|-----------:|-----------:|--------------------------------:|-----------------------------------:|---------------------:|
| 2          | 26.932     | 1.000                           | -                                  | 50.0 %               |
| 3          | 16.416     | 1.641                           | 1.641                              | 54.7 %               |
| 4          | 9.603      | 2.804                           | 1.709                              | 70.1 %               |
| 5          | 8.023      | 3.357                           | 1.197                              | 67.1 %               |
| 6          | 7.083      | 3.803                           | 1.133                              | 63.4 %               |
| 7          | 6.458      | 4.171                           | 1.097                              | 59.6 %               |
| 8          | 7.463      | 3.609                           | 0.866                              | 45.1 %               |
| 9          | 5.722      | 4.707                           | 1.304                              | 52.3 %               |
| 10         | 6.398      | 4.209                           | 0.895                              | 42.1 %               |
| 11         | 5.898      | 4.566                           | 1.085                              | 41.5 %               |



3.- 
31,362s
32,161

4.-
al principio comete en todos los intentos fallos porque no tiene referencias en la caché está vacía hasta que ya se va llenando
29,171
Pero en el segundo tiempo nos
31,503