package multipleProducersMultipleConsumers;
import java.util.concurrent.Semaphore;

/**
 * @author cristian.chilipirea
 *
 */
class Buffer {
    private static int BUFFER_CAPACITY = 5;
    private int[] v;
    private Semaphore empty;
    private Semaphore full;
    private int pos;

    Buffer() {
        v       = new int[BUFFER_CAPACITY];
        pos     = 0;
        empty   = new Semaphore(BUFFER_CAPACITY);
        full 	= new Semaphore(0);
    }

    void put(int value) {
        try {
            empty.acquire();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }

        synchronized (Main.class) {
            pos     = (pos + 1) % BUFFER_CAPACITY;
            v[pos]  = value;
        }

        full.release();
    }

    int get() {
        try {
            full.acquire();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }

        int val;

        synchronized (Main.class) {
            val = v[pos];

            pos = pos - 1;
            if (pos < 0) {
                pos = BUFFER_CAPACITY - 1;
            }
        }

        empty.release();

        return val;
    }
}
