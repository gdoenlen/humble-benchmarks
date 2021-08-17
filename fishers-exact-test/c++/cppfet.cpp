#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <numeric>
#include <stdlib.h>

constexpr std::array<long, 4> DATA = { 1982, 3018, 2056, 2944 };
constexpr long GRAND_TOTAL = DATA[0] + DATA[1] + DATA[2] + DATA[3];
constexpr long FACTORIAL_SIZE = GRAND_TOTAL + 1;

std::array<double, FACTORIAL_SIZE> logFactorial() {
    std::array<double, FACTORIAL_SIZE> fs;
	
	fs[0] = 0;
	for(long i = 1; i < FACTORIAL_SIZE; i++) {
		fs[i] = fs[i-1] + log(i);
	}
    
    return fs;
}

double logHypergeometricProbability(const std::array<long, 4>& data, const std::array<double, FACTORIAL_SIZE>& fs) {
	return (
		fs[data[0] + data[1]] +
		fs[data[2] + data[3]] +
		fs[data[0] + data[2]] +
		fs[data[1] + data[3]] -
		fs[data[0]] -
		fs[data[1]] -
		fs[data[2]] -
		fs[data[3]] -
		fs[data[0] + data[1] + data[2] + data[3]]
	);
}

double fisher_exact(const std::array<long, 4>& data, std::array<double, FACTORIAL_SIZE>& factorials) {
    // calculate our rejection threshold
	const double pvalThreshold = logHypergeometricProbability(data, factorials);
	
    double pvalFraction = 0;
	for(long i = 0; i <= GRAND_TOTAL; i++) {
		if((data[0] + data[1] - i >= 0) && (data[0] + data[2] - i >= 0) && (data[3] - data[0] + i >=0)) {
            double lhgp = logHypergeometricProbability({
                i,
                data[0] + data[1] - i,
                data[0] + data[2] - i,
                data[3] - data[0] + i
            }, factorials);

			if(lhgp <= pvalThreshold) {
				pvalFraction += exp(lhgp - pvalThreshold);
			}
		}
	}
    
	return (exp(pvalThreshold) * pvalFraction);
}

int main(int argc, char* argv[]) {
    // checking num of agruments
    if(argc < 2) {
        std::cout << "Specify number of iterations!\n";
        return 0;
    }
    
    double pvalue = 0.0;
    std::array<double, FACTORIAL_SIZE> factorials = logFactorial();
	for(size_t i = 0; i < atoi(argv[1]); i++) {
		pvalue = fisher_exact(DATA, factorials);
	}
    
    std::cout << "pvalue = " << pvalue << "\n";
    
	return 0;
}