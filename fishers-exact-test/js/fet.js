const fisherExact = data => {
  let pvalFraction = 0;

  for (let i = 0; i <= GRAND_TOTAL; i++) {
    const one = data[0] + data[1] - i;
    const two = data[0] + data[2] - i;
    const three = data[3] - data[0] + i;

    if (one >= 0 && two >= 0 && three >= 0) {
      const lhgp = logHyperGeometricProbability([i, one, two, three], FACTORIALS);
      if (lhgp <= PVAL_THRESHOLD) {
        pvalFraction += Math.exp(lhgp - PVAL_THRESHOLD);
      }
    }
  }

  return Math.exp(PVAL_THRESHOLD) * pvalFraction;
}

const logFactorial = n => {
  const len = n + 1;
  const fs = new Array(len);
  fs[0] = 0;

  for (let i = 1; i < len; i++) {
    fs[i] = fs[i - 1] + Math.log(i);
  }

  return fs;
};

const logHyperGeometricProbability = (data, fs) => {
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
};

const DATA = [1982, 3018, 2056, 2944];
const GRAND_TOTAL = DATA.reduce((a, b) => a + b, 0);
const FACTORIALS = logFactorial(GRAND_TOTAL);
const PVAL_THRESHOLD = logHyperGeometricProbability(DATA, FACTORIALS);

if (process.argv.length < 3) {
	console.log('Specify number of iterations!');
  process.exit(1);
}

let pvalue = 0.0;
for (let i = 0, len = parseInt(process.argv[2]); i < len; i++) {
  pvalue = fisherExact(DATA);
}

console.log(`pvalue = ${pvalue}`);