import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import static java.lang.Math.ceil;
import static java.lang.Math.min;

public class MergeSort extends Thread {
    private final int tid;
    private static int[] v;
    private static int[] vNew;
    private final double N;
    private final double numThreads;
    private CyclicBarrier barrier;

    MergeSort(int[] v, int[] vNew, int tid, CyclicBarrier barrier, int N, int P) {
        MergeSort.v     = v;
        this.tid        = tid;
        this.N          = (double)N;
        this.numThreads = (double)P;
        MergeSort.vNew  = vNew;
        this.barrier    = barrier;
    }

    private void merge(int[] source, int start, int mid, int end, int[] destination) {
        int iA = start;
        int iB = mid;

        for (int i = start; i < end; i++) {
            if (end == iB || (iA < mid && source[iA] <= source[iB])) {
                destination[i] = source[iA];
                iA++;
            } else {
                destination[i] = source[iB];
                iB++;
            }
        }
    }

    @Override
    public void run() {
        int start = tid * (int)ceil(N / numThreads);
        int end   = min((int)N, (tid + 1) * (int)ceil(N / numThreads));
        int[] aux;

        for (int width = 1; width < N; width <<= 1) {
            if (end - start == width) {
                if ((end / width) % 2 == 1) {
                    end -= width;
                } else {
                    start -= width;
                }
            }

            for (int i = start; i < end; i += (width << 1)) {
                merge(v, i, i + width, i + (width << 1), vNew);
            }

            try {
                barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

            if (tid == 0) {
                aux = v;
                v = vNew;
                vNew = aux;
            }

            try {
                barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}
