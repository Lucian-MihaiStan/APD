import static java.lang.Math.ceil;
import static java.lang.Math.min;

public class MultVector extends Thread {
    private final int[] v;
    private final int tid;
    private final double N;
    private final double numThreads;

    MultVector(int[] v, int tid, int N, int numThreads) {
        this.v          = v;
        this.tid        = tid;
        this.N          = (double)N;
        this.numThreads = (double)numThreads;
    }

    @Override
    public void run() {
        final int start = tid * (int)ceil(N / numThreads);
        final int end   = min((int)N, (tid + 1) * (int)ceil(N / numThreads));

        for (int i = start; i < end; ++i) {
            v[i] *= 2;
        }
    }
}
