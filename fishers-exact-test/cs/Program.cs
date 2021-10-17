using System;
using System.Linq;

namespace cs {
    class Program {
        private static readonly int[] DATA = new int[] { 1982, 3018, 2056, 2944 };
        private static readonly int GRAND_TOTAL = DATA.Sum();
        private static readonly double[] FACTORIALS = logFactorial(GRAND_TOTAL);
        private static readonly double PVAL_THRESHOLD = logHyperGeometricProbability(DATA, FACTORIALS);
        static void Main(string[] args)  {
            if (args.Length < 1) {
                Console.WriteLine("Specify number of iterations!");
                System.Environment.Exit(1);
            }

            double pvalue = 0.0;
            for (int i = 0; i < int.Parse(args[0]); i++) {
                pvalue = fisherExact(DATA);
            }

            Console.WriteLine("pvalue = " + pvalue);
        }

        static double fisherExact(int[] data) {
            double pvalFraction = 0;

            for (int i = 0; i <= GRAND_TOTAL; i++) {
                int one = data[0] + data[1] - i;
                int two = data[0] + data[2] - i;
                int three = data[3] - data[0] + i;
                if (one >= 0 && two >= 0 && three >= 0) {
                    double lhgp = logHyperGeometricProbability(
                        new int[] { i, one, two, three },
                        FACTORIALS 
                    );

                    if (lhgp <= PVAL_THRESHOLD) {
                        pvalFraction += Math.Exp(lhgp - PVAL_THRESHOLD);
                    }
                }
            }

            return Math.Exp(PVAL_THRESHOLD) * pvalFraction;
        }

        static double[] logFactorial(int n) {
            int len = n + 1;
            double[] fs = new double[len];
            fs[0] = 0;

            for (int i = 1; i < len; i++) {
                fs[i] = fs[i - 1] + Math.Log(i);
            }

            return fs;
        }

        static double logHyperGeometricProbability(int[] data, double[] fs) {
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
    }
}
