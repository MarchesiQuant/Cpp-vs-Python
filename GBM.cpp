#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <iomanip> 
#include <chrono>

using namespace std;


// Funciones para el calculo de los payoffs al vencimiento
double CallPayoff(double S_T, double K){

    if (S_T > K){return S_T - K;}
    else{return 0;}
}

double PutPayoff(double S_T, double K){

    if (S_T < K){return K - S_T;}
    else{return 0;}
}

// Funcion para calcular la media de los payoffs descontados 
double avgDiscPayoffs(vector<double> P, double T, double r){

    double sum = 0;
    for(int i = 0; i < P.size(); ++i){
        sum = sum + P[i];
    }
    double mean = sum/P.size();
    return mean * exp(-T*r);
}


int main() {

    // Parámetros del modelo

    double S0;      // Precio inicial del activo
    double mu;      // Tasa de retorno esperada
    double sigma;   // Volatilidad
    double T;       // Tiempo total (en años)
    double dt;      // Tamaño del paso de tiempo
    double K;       // Strike 
    string optType; // Tipo de opción (CALL o PUT) 
    int iter;       // Numero de simulaciones 
    double r;       // Tasa de descuento


    // Solicitar al usuario los parámetros
    cout << "Cálculo del precio de una opción a través de la simulación del MBG\n";
    cout << "-----------------------------------------------\n";
    cout << "Ingrese el precio inicial del activo (S0): ";
    cin >> S0;
    cout << "Ingrese la tasa de retorno esperada (mu, por ejemplo 0.05 para 5%): ";
    cin >> mu;
    cout << "Ingrese la volatilidad (sigma, por ejemplo 0.2 para 20%): ";
    cin >> sigma;
    cout << "Ingrese el tiempo total de simulación (T, en años, por ejemplo 1): ";
    cin >> T;
    cout << "Ingrese el tipo de opcion (CALL o PUT): ";
    cin >> optType;
    cout << "Ingrese el strike (K): ";
    cin >> K;
    cout<< "Ingrese el número de simulaciones (iter): ";
    cin >> iter;
    cout << "Ingrese la tasa de descuento (r, por ejemplo 0.05 para 5%): ";
    cin >> r; 

    auto start = chrono::high_resolution_clock::now();

    int N = 252;
    dt = T / N; 

    // Configurar el generador de números aleatorios
    int seed = 200;
    mt19937 gen(seed); 
    normal_distribution<> d(0.0, 1.0); 

    // Vector para almacenar los payoffs
    vector<double> P(iter);

    for(int k = 0; k < iter; ++k){
        // Vector para almacenar los precios simulados
        vector<double> S(N + 1);
        S[0] = S0;

        // Simulación del GBM
        for(int i = 1; i <= N; ++i) {
            double Z = d(gen); // Variable aleatoria normal estándar
            S[i] = S[i-1] * exp( (mu - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * Z );
        }

        // Calculo del payoff
        if (optType == "CALL"){
            P[k] = CallPayoff(S.back(), K);

        } else if(optType == "PUT"){
            P[k] = PutPayoff(S.back(), K);
        }
    }

    double V = avgDiscPayoffs(P, T, r);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duracion = end - start;

    cout << "El valor de la opcion es de: " << V << endl;
    cout << "El tiempo de ejecución fue de " << duracion.count() << " segundos";
    return 0;
}