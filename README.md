# Indice-ICC

# Least-Recently-Used-LRU-Cache





La “Least Recently Used” Cache se basa en el principio de que el dato que fue

accedido más recientemente es más probable que vuelva a ser accedido en el corto tiempo.

Esto a su vez conlleva a que cuando la memoria (caché) de este TDA se llene (se define un

tamaño limitado para ella), se descarte el dato menos usado recientemente (de aquí su

nombre), asegurando que los datos más relevantes se encuentren siempre disponibles.

Las primitivas del TDA deben ser:

\- crear\_lrucache: debe recibir por parámetro una capacidad máxima y una función de

comparación de claves, y definir los valores necesarios para inicializar la caché. La

función de comparación debe devolver un valor negativo, cero o positivo según la

primera clave sea menor, igual o mayor que la segunda.

\- agregar\_lrucache: agrega un nuevo elemento a la caché. El elemento es agregado

al inicio del set de datos, y si, con el agregado de este nuevo elemento, la caché se

desborda, debe eliminar el último elemento. Si el dato ya existía (misma clave), es

reemplazado y queda al principio del set de datos.

\- obtener\_lrucache: busca un elemento en la caché por clave. Si lo encuentra, debe

moverlo al principio del set de datos y devolver el valor del mismo. Si no lo encuentra

devuelve que no existe.

\- borrar\_lrucache: Elimina un elemento de la caché buscando por clave y lo borra del

set de datos.

\- vaciar\_lrucache: Vacía el set de datos y libera toda la memoria.



