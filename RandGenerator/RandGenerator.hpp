#include "RandGeneratorBase.hpp"

class UniformRandGenerator : public RandBase, public URandGenerator {
private:
	double lower, upper;
	void InverseGenerator() {
		double length = upper - lower;
		try {
			if (length <= 0) {
				throw 1;
			}
		}
		catch (...) {
			cout << "lower bound should be smaller than upper bound" << endl;
			exit(0);
		}
		vector<double> temp(RandBase::N);
		for (int i = 0; i < RandBase::N; i++) {
			temp[i] = lower + USample[i] * length;
		}
		RandBase::Sample = temp;
	}
public:
	UniformRandGenerator(int N_in, double tol_in, string opt_in,double lower_in=0, double upper_in=1) : URandGenerator(N_in, tol_in, opt_in), RandBase(N_in, "Uniform") {
		lower = lower_in;
		upper = upper_in;
		InverseGenerator();
	}

	void testDistribution() {  //chi-square test
		vector<int> testData(10);
		int T;
		for (int i = 0; i < RandBase::N; i++) {
			T = (int)(USample[i] * 10);
			testData[T] += 1;
		}
		double E = RandBase::N / 10.0, chi = 0.0;
		for (int j = 0; j < 10; j++) {
			chi += pow(testData[j] - E, 2) / E;
		}
		if (chi < 16.92) {
			cout << "By Chi-Square test, under 0.05 level, this sample follows uniform distribution, and test statistic is " << chi << endl;
		}
		else {
			cout << "By Chi-Square test, under 0.05 level, this sample does not follows uniform distribution, and test statistic is " << chi << endl;
		}
	}
};

class NormalRandGenerator : public RandBase, public URandGenerator {
private:
	double mu;
	double sigma;
	void InverseGenerator() { //Box-Muller
		int i = 0;
		const double pi2 = 2.0*3.141592653589;
		vector<double> temp(URandGenerator::N);
		while (i < URandGenerator::N) {  
			temp[i] = sqrt(-2.0 * log(USample[i])) * cos(pi2 * USample[i + 1])*sigma + mu;
			temp[i + 1] = sqrt(-2.0 * log(USample[i])) * sin(pi2 * USample[i + 1])*sigma + mu;
			i += 2;
		}
		temp.resize(RandBase::N);
		RandBase::Sample = temp;
	}
public:
	NormalRandGenerator(int N_in, double tol_in, string opt_in, double mu_in=0, double sigma_in=1) : URandGenerator(int((N_in + 1) / 2) * 2, tol_in, opt_in), RandBase(N_in, "Normal") {
		mu = mu_in;
		sigma = sigma_in;
		try {
			if (sigma <= 0) {
				throw 1;
			}
		}
		catch (...)
		{
			cout << "sigma of Normal distribution should be positive" << endl;
		}
		InverseGenerator();
	}

	void testDistribution() {  //JB test
		double mean = 0, avg2 = 0, avg3 = 0, avg4 = 0, S, C, JB;
		for (int i = 0; i < RandBase::N; i++) {
			mean += Sample[i];
		}
		mean /= RandBase::N;
		for (int i = 0; i < RandBase::N; i++) {
			avg2 += pow(Sample[i] - mean, 2);
			avg3 += pow(Sample[i] - mean, 3);
			avg4 += pow(Sample[i] - mean, 4);
		}
		avg2 /= RandBase::N;
		avg3 /= RandBase::N;
		avg4 /= RandBase::N;

		S = avg3 / pow(avg2, 1.5);
		C = avg4 / pow(avg2, 2);

		JB = RandBase::N / 6 * (pow(S, 2) + 0.25*pow(C - 3, 2));
		if (JB < 5.99) {
			cout << "By JB test, under 0.05 level, this sample follows Normal distribution, and test statistic is " << JB << endl;
		}
		else {
			cout << "By JB test, under 0.05 level, this sample does not follows Normal distribution, and test statistic is " << JB << endl;
		}
	}
};
