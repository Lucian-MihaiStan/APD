package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;

/**
 * @author cristian.chilipirea
 *
 */
class Buffer {
    private static final int BUFFER_CAPACITY = 5;
    private ArrayBlockingQueue<Integer> arr;

    Buffer() {
        arr = new ArrayBlockingQueue<>(BUFFER_CAPACITY, true);
    }

    void put(int value) {
        try {
            arr.put(value);
        } catch(InterruptedException e) {
            e.printStackTrace();
        }
    }

    int get() {
        int retVal = -1;

        try {
            retVal = arr.take();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }

        return retVal;
    }
}
