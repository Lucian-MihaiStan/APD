package parallelBinarySearch;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import static java.lang.Math.min;
import static java.lang.Math.ceil;


public class BinarySearchThread extends Thread {
    private static int position;
    private static int leftLim;
    private static int rightLim;
    private int left;
    private int right;
    private final int numThreads;
    private final int tid;
    private final int[] v;
    private CyclicBarrier barrier;
    private final int target;

    BinarySearchThread(int[] v, int N, int P, CyclicBarrier barrier, int id, int target) {
        this.v          = v;
        numThreads      = P;
        tid             = id;
        this.barrier    = barrier;
        leftLim         = 0;
        rightLim        = N - 1;
        position        = -1;
        this.target     = target;

        left    = tid * (int)ceil((double)N / numThreads);
        right   = min(rightLim, (tid + 1) * (int)ceil((double)N / numThreads));
    }

    static int getPosition() {
        return position;
    }

    @Override
    public void run() {
        int pos = -1;

        if (v[leftLim] > target || v[rightLim] < target) {
            position = -2;
        }

        while (position == -1) {
            if (v[left] == target) {
                pos = left;
            } else if (v[right] == target) {
                pos = right;
            } else if (v[left] < target && v[right] > target) {
                leftLim = left;
                rightLim = right;
            }

            try {
                barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

            final double intervalLen = rightLim - leftLim;

            left    = tid * (int)ceil(intervalLen / numThreads) + leftLim;
            right   = min(rightLim, (tid + 1) * (int)ceil(intervalLen / numThreads)) + leftLim;

            if (pos != -1)
            {
                position = pos;
            } else if (left == leftLim && right == rightLim) {
                position = -2;
            }

            try {
                barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}