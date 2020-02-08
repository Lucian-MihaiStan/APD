import java.util.concurrent.CyclicBarrier;

public class Main {
    public static void main(String[] args) {
        final int P = 8;
        final int N = (1 << 20);
        int num     = N;
        boolean ok  = true;

        int[] v             = new int[N];
        int[] vNew          = new int[N];
        MergeSort[] threads = new MergeSort[P];

        CyclicBarrier barrier = new CyclicBarrier(P);

        for (int i = 0; i != N; ++i, --num) {
            v[i] = num;
        }

        for (int i = 0; i != P; ++i) {
            threads[i] = new MergeSort(v, vNew, i, barrier, N, P);
            threads[i].start();
        }

        for (int i = 0; i != P; ++i) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 1; i != N; ++i) {
            if (v[i] < v[i - 1]) {
                System.out.println("Wrong: " + v[i - 1] + " > " + v[i] );
                ok = false;
            }
        }

        if (ok) {
            System.out.println("Sorted correctly!");
        }
    }
}
