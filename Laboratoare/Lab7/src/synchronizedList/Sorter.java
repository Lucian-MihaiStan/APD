package synchronizedList;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Sorter extends Thread {
    private Semaphore sem;
    private List<Integer> buffer;

    Sorter(Semaphore sem, List<Integer> buffer) {
        this.sem    = sem;
        this.buffer = buffer;
    }

    @Override
    public void run() {
        try {
            sem.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Collections.sort(buffer);
    }
}
