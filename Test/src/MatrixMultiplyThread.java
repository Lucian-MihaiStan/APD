import static java.lang.Math.min;
import static java.lang.Math.ceil;

public class MatrixMultiplyThread extends Thread {
    private static final int MASTER = 0;

    private static int[][] parRes;
    private static int[][] mat0;
    private static int[][] mat1;
    private static double P;
    private static double N;
    private final int id;

    MatrixMultiplyThread(
            final int id,
            final int N,
            final int P,
            final int[][] mat0,
            final int[][] mat1,
            final int[][] parRes) {
        this.id = id;

        if (id == MASTER) {
            MatrixMultiplyThread.N      = (double)N;
            MatrixMultiplyThread.P      = (double)P;
            MatrixMultiplyThread.parRes = parRes;
            MatrixMultiplyThread.mat0   = mat0;
            MatrixMultiplyThread.mat1   = mat1;
        }
    }

    @Override
    public void run() {
        int start   = (int)ceil(N / P) * id;
        int end     = (int)min(N, (int)ceil(N / P) * (id + 1));

        int tmp;

        for (int i = 0; i != N; ++i) {
            for (int j = start; j != end; ++j) {
                tmp = 0;

                for (int k = 0; k != N; ++k) {
                    tmp += mat0[i][k] * mat1[k][j];
                }

                parRes[i][j] = tmp;
            }
        }
    }
}
