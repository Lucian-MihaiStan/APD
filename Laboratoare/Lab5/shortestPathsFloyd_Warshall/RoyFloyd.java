import static java.lang.Math.min;
import static java.lang.Math.ceil;

public class RoyFloyd extends Thread {
    private int[][] graph;
    private final int tid;
    private final double N;
    private final double numThreads;

    RoyFloyd(int[][] graph, int tid, int N, int numThreads) {
        this.graph      = graph;
        this.tid        = tid;
        this.N          = (double)N;
        this.numThreads = (double)numThreads;
    }

    @Override
    public void run() {
        final int start   = tid * (int)ceil(N / numThreads);
        final int end     = min((int)N, (tid + 1) * (int)ceil(N / numThreads));

        for (int k = start; k < end; ++k) {
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    synchronized (Main.class) {
                        graph[i][j] = min(graph[i][k] + graph[k][j], graph[i][j]);
                    }
                }
            }
        }
    }
}
