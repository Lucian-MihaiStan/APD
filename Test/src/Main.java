public class Main {
    private static void sequentialMultiply(final int N, int[][] seqRes, final int[][] mat0, final int[][] mat1) {
        for (int i = 0; i != N; ++i) {
            for (int j = 0; j != N; ++j) {
                for (int k = 0; k != N; ++k) {
                    seqRes[i][j] += mat0[i][k] * mat1[k][j];
                }
            }
        }
    }

    public static void main(String[] args) {
        final int P         = Runtime.getRuntime().availableProcessors();
        final int N         = 100;
        boolean ok          = true;

        Thread[] threads    = new Thread[P];
        int[][] mat0        = new int[N][N];
        int[][] mat1        = new int[N][N];
        int[][] seqRes      = new int[N][N];
        int[][] parRes      = new int[N][N];

        for (int i = 0; i < N; ++i) {
            for(int j = 0; j < N; ++j) {
                mat0[i][j]      = i + j;
                mat1[i][j]      = i - j;
                seqRes[i][j]    = 0;
                parRes[i][j]    = 0;
            }
        }

        for (int i = 0; i < P; ++i) {
            threads[i] = new MatrixMultiplyThread(i, N, P, mat0, mat1, parRes);
            threads[i].start();
        }

        for (int i = 0; i != P; ++i) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        sequentialMultiply(N, seqRes, mat0, mat1);

        for (int i = 0; i != N; ++i) {
            for(int j = 0; j != N; ++j) {
                if (seqRes[i][j] != parRes[i][j]) {
                    System.out.println("Wrong! Matrices differ at position (" + i + ", " + j + "): "
                            + seqRes[i][j] + " != " + parRes[i][j]);
                    ok = false;
                }
            }
        }

        if (ok) {
            System.out.println("Correct!");
        }
    }
}
