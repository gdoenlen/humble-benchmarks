import java.util.stream.IntStream;

public class Fet {
    private static final int[] DATA = new int[] { 1982, 3018, 2056, 2944 };
    private static final int GRAND_TOTAL = IntStream.of(DATA).sum();
    private static final double[] FACTORIALS = logFactorial(GRAND_TOTAL);
    private static final double PVAL_THRESHOLD = logHyperGeometricProbability(DATA, FACTORIALS);

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Specify number of iterations!");
            System.exit(1);
        }

        double pvalue = 0.0;
        for (int i = 0; i < Integer.parseInt(args[0]); i++) {
            pvalue = fisherExact(DATA);
        }

        System.out.println("pvalue = " + pvalue);
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
                    pvalFraction += Math.exp(lhgp - PVAL_THRESHOLD);
                }
            }
        }

        return Math.exp(PVAL_THRESHOLD) * pvalFraction;
    }

    static double[] logFactorial(int n) {
        int len = n + 1;
        double[] fs = new double[len];
        fs[0] = 0;

        for (int i = 1; i < len; i++) {
            fs[i] = fs[i - 1] + Math.log(i);
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