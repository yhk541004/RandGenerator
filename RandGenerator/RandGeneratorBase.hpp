#include <vector>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <iterator>
#include <map>
#include <string>


using namespace std;


class RandBase {
private:
	string Name;
	virtual void InverseGenerator() = 0;
public:
	int N;
	vector<double> Sample;
	RandBase(int N_in, string Name_in) {
		N = N_in;
		Name = Name_in;
	}

	void printSample();
	void saveSample();
	vector<double> retSample() {return Sample;}
	virtual void testDistribution() = 0;
};

void RandBase::printSample(){
	for (vector<double>::iterator i = Sample.begin(); i != Sample.end(); ++i)
		cout << *i << ' ';
	cout << endl;
}

void RandBase::saveSample() {
	string fileName = "./" + Name + "Sample.csv";
	ofstream file;
	file.open(fileName, ofstream::trunc);
	file.close(); //remove previous content
	file.open(fileName, ofstream::app);
	for (int i = 0; i<N; i++)
		file << Sample[i] << "\n";
	file.close();
}

class URandGenerator {
private:

	vector<int> BSample;
	double tol;
	int k; //number of decimal to match tolerance
	const map<vector<int>, int> mapB2D = {
		{ { 0,0,0,0 },0 },{ { 0,0,0,1 },1 },{ { 0,0,1,0 },2 },{ { 0,0,1,1 },3 },
		{ { 0,1,0,0 },4 },{ { 0,1,0,1 },5 },{ { 0,1,1,0 },6 },{ { 0,1,1,1 },7 },
		{ { 1,0,0,0 },8 },{ { 1,0,0,1 },9 }
	};

	inline int BGenerator() {return rand() % 2;	}

	vector<int> BGenerator_binary(int n);
	int binary2dec(vector<int> Bin);
	void UGenerator_Binary();
	double Int2Dec(vector<int> DIG);
	void UGenerator_map();


public:
	vector<double> USample;
	int N;	//Number of total random number
	URandGenerator(int N_in, double tol_in, string opt)
	{
		N = N_in;
		try {
			if (N <= 0) {
				throw 1;
			}
		}
		catch (...) {
			cout << "Number of Random number should be positive" << endl;
			exit(0);
		}
		USample.resize(N);
		tol = tol_in;
		k = (int)(-log10(tol_in)+0.5);
		try {
			if (pow(10, -k) != tol) {
				throw 1;
			}
			if (k < 0) {
				throw 2;
			}
		}
		catch(int e){
			switch (e)
			{
			case(1):
				cout << "tol input should be like 1e-X" << endl;
			case(2):
				cout << "power of tolarence should be negative" << endl;
			}
			
			exit(0);
		}
		try {
			if (!opt.compare("binary"))
			{
				
				UGenerator_Binary();
			}
			else if (!opt.compare("map")) {
				UGenerator_map();
			}
			else {
				throw 1;
			}
		}
		catch (...) {
			cout << "Cannot recognize uniform RG method, only take \"binary\" and \"map\"" << endl;
			exit(0);
		}
	}

};

vector<int> URandGenerator::BGenerator_binary(int n)
{
	vector<int> result(n);
	for (int i = 0; i < n; i++)
	{
		result[i] = BGenerator();
	}
	return result;
}

int URandGenerator::binary2dec(vector<int> Bin)
{
	int result = 0;
	for (size_t i = 0; i < Bin.size(); i++)
	{
		result += Bin.at(i) *  (int)((pow(2, i + 1))+0.5);
	}
	return result;
}

void URandGenerator::UGenerator_Binary()
{
	int i = 0;
	int temp;
	int n = (int)(-log(tol) / log(2.0)) + 1; //number of digit to match tolarence
	while (i < N) {
		temp = binary2dec(BGenerator_binary(n));
		int upper = (int)(1.0 / tol + 0.5);  
		if (temp < upper && temp>0 ) {  //control upper and lower bound and ensure the sample space to be open interval
			USample[i] = temp*tol;
			i++;
		}
	}
}

double URandGenerator::Int2Dec(vector<int> DIG) {
	double result = 0.0;
	for (size_t i = 0; i < DIG.size(); i++) {
		result += DIG[i] * pow(0.1, i + 1);
	}
	return(result);
}

void URandGenerator::UGenerator_map() {
	USample.resize(N);
	int i = 0;
	vector<int> Bin;
	vector<int> temp(k);
	double tempD;
	int D, j;
	for (int i = 0; i < N; i++) {
		j = 0;
		while (j < k) {
			try {
				Bin = BGenerator_binary(4);
				D = mapB2D.at(Bin);
				temp[j] = D;
				j++;
			}
			catch (...) {}
		}
		tempD = Int2Dec(temp);
		if (tempD != 0.0) {
			USample[i] = tempD;
		}

	}
}

