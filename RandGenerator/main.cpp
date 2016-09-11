#include "RandGenerator.hpp"
#include <algorithm>
#include <time.h>  

int main() {
	time_t t = clock();
	string Name,type;
	int N;
	double tol;
	cout << "Please input total number of random number, number should be a positive integer: ";
	cin >> N;
	cout << "Please input target distribution, only take \"Uniform\" and \"Normal\": ";
	cin >> Name;
	cout << "Please input target tolerance, format should be 1e-X: ";
	cin >> tol;
	cout << "Please input method to generate base uniform distribution, only take \"binary\" and \"map\": ";
	cin >> type;
	RandBase *Rd;
	if (!Name.compare("Uniform")) {
		double low, up;
		cout << "Please input lower bound of uniform distribution: ";
		cin >> low;
		cout << "Please input upper bound of uniform distribution: ";
		cin >> up;
		Rd = new UniformRandGenerator(N, tol, type, low, up);
	}
	else if (!Name.compare("Normal")) {
		double mean, sigma;
		cout << "Please input mean of normal distribution: ";
		cin >> mean;
		cout << "Please input stand deviation of normal distribution: ";
		cin >> sigma;
		Rd = new NormalRandGenerator(N, tol, type, mean, sigma);
	}
	else {
		cout << "illegal distribution type input";
		return(0);
	}
	t = clock() - t;
	Rd->printSample();
	Rd->saveSample();
	Rd->testDistribution();
	cout << t << endl;
	delete Rd;
}