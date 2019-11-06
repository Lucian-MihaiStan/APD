package synchronizedList;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Main {
    private static final int NUM_REPS       = 1000;
    private static final int NUM_THREADS    = 4;

    public static void main(String[] args) {
        Thread[] threads    = new Thread[NUM_THREADS];
        boolean sorted      = true;

        List<Integer> buffer;
        Semaphore sem;

        for (int i = 0; i < NUM_REPS; ++i) {
            sem     = new Semaphore(-2);
            buffer  = Collections.synchronizedList(new ArrayList<>());

            for (int j = 0; j < NUM_THREADS - 1; ++j) {
                threads[j] = new Writer(j + 1, sem, buffer);
                threads[j].start();
            }

            threads[3] = new Sorter(sem, buffer);
            threads[3].start();

            for (Thread thread : threads) {
                try {
                    thread.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            int len = buffer.size();

            for (int j = 1; j < len; ++j) {
                if (buffer.get(j) < buffer.get(j - 1)) {
                    System.out.println("Sorted incorrectly: "
                            + buffer.get(j - 1) + " > " + buffer.get(j) + "!");
                    sorted = false;
                    break;
                }
            }
        }

        if (sorted) {
            System.out.println("Sorted correctly!");
        }
    }
}
