#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double calcularPi(int iteraciones, double pi, double signo, double denominador) {
    int i;
    
    for (i = 0; i < iteraciones; i++) {
        pi += signo / denominador;
        signo *= -1;
        denominador += 2.0;
    }
    pi *= 4;
    return pi;
}

double calcularPiparalel(int N, int num_hilos) {
	double pi;
    double sum = 0.0;

	omp_set_num_threads(num_hilos);
	
    #pragma omp parallel
    {
    	int id, Hilo;
        double sum_parcial = 0.0;
        
        id = omp_get_thread_num();
		Hilo = omp_get_num_threads();
		
    	for (int i = id; i < N; i += Hilo) {
         double signo = (i % 2 == 0) ? 1.0 : -1.0;// establece el singo del numerador en cada iteracion
         double termino = signo / (2.0 * i + 1.0);
         sum_parcial += termino;
    	}
    
    	#pragma omp critical
    	sum += sum_parcial;
	}
    
 	pi = 4.0 * sum;
 	
    return pi;
}

int main() {
    char opc;
    clock_t start, end, start2, end2;
    double cpu_time_used, cpu_time_used2, pisecuncial, piparalelo, pi;
    double signo = 1.0, denominador = 1.0 ;
    
    do {
        printf("\t\tMENU\n"); 
        printf("1.- Secuencial\n");
        printf("2.- Paralelo\n");
        printf("3.- SALIR\n");

        printf("      \nINGRESE OPCION : ");
        scanf(" %c", &opc);
        system("cls");

        if (opc == '1') {
            int iteraciones;
            
            printf("Ingrese el numero de iteraciones para aproximar Pi: ");
            scanf("%d", &iteraciones);
            
            start = clock();
            pisecuncial = calcularPi(iteraciones, pi, signo, denominador);
            end = clock();
            
            printf("El valor aproximado de Pi es: %.10lf\n", pisecuncial);
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("Tiempo de ejecucion (Secuencial): %.10f segundos\n", cpu_time_used);
            system("pause");
        } else if (opc == '2') {
            int N;
            int numHilos;

            printf("Ingrese el numero de iteraciones para aproximar Pi: ");
            scanf("%d", &N);

            printf("Ingrese el numero de hilos: ");
            scanf("%d", &numHilos);
            
			start2 = clock();
            piparalelo = calcularPiparalel(N, numHilos);
            end2 = clock();
          
            printf("El valor aproximado de Pi es: %.10lf\n", piparalelo);
            cpu_time_used2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;
            printf("Tiempo de ejecucion (Paralelo): %f segundos\n", cpu_time_used2);
            system("pause");
        }

    } while (opc != '3');

    return 0;
}

