#include <iostream>
#include "asian_call.h"
#include <random>

// Destructor
asian_call::~asian_call() {};

// Member function
double asian_call::price() const {
	// to valuate an asian (arithmetic) call, we need to compute the average of the price (S_mean) of the underlying asset

	double V = 0;
	std::default_random_engine generator(time(0));
	std::normal_distribution<double> distribution(0.0, 1.0);

	int N = 10000; //number of price paths
	int n = 1000; //number of simulation of the borwnian motion for one price paths (from 0 till maturity T)


	for (int i = 0; i < N; i++) {
		double S_mean = S/n;
		double W = 0; //brownian motion

		for (int j = 0; j < n; j++) {
			double delta_W = distribution(generator) * sqrt(T / n); // because W_{t+h}-W_{t} follows N(0,h)

			W += delta_W;
			S_mean += S * exp((r - pow(sigma, 2) / 2) * T + sigma * W)/ n;
		};
		
		if (S_mean > K) {
			V += S_mean - K;
		}
		
	};

	return V/N;
};