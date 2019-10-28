package parallelBinarySearch;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import static java.lang.Math.min;
import static java.lang.Math.ceil;


public class BinarySearchThread extends Thread {
    private static int position;
    private static int leftLim;
    private static int rightLim;
    private static boolean finished;
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
        finished        = false;

        left    = tid * (int)ceil((double)N / numThreads);
        right   = min(rightLim, (tid + 1) * (int)ceil((double)N / numThreads));

        if (v[leftLim] > target || v[rightLim] < target) {
            position = -2;
        }
    }

    static int getPosition() {
        return position;
    }

    @Override
    public void run() {
        while (position == -1 && !finished) {
            if (v[left] == target) {
                position = left;
            } else if (v[right] == target) {
                position = right;
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

            if (left == leftLim && right == rightLim) {
                finished = true;
            }

            try {
                barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}