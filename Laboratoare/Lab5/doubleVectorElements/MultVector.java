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
        int start   = tid * (int)Math.ceil(N / numThreads);
        int end     = Math.min((int)N, (tid + 1) * (int)Math.ceil(N / numThreads));

        for (int i = start; i < end; ++i) {
            v[i] *= 2;
        }
    }
}
