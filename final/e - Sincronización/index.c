// Para sincronizar los 3 hilos/procesos no pueden emplearse
// flags o semaforos/mutex sino unicamente señales.
// Para ello manejaremos un maximo de 3 signal signal1 signal2