package bugConcurrentHashMap;

import java.util.concurrent.ConcurrentHashMap;

/**
 * @author cristian.chilipirea
 * 
 */
public class MyThread implements Runnable {
    static ConcurrentHashMap<Integer, Integer> hashMap;
    private final int id;

    MyThread(int id) {
        this.id = id;
    }

    private void addValue(int key, int value) {
        Integer prevVal = hashMap.putIfAbsent(key, value);

        if (prevVal != null) {
            hashMap.put(key, value + prevVal);
        }
    }

    @Override
    public void run() {
        if (id == 0) {
            for (int i = 0; i < Main.N; i++) {
                addValue(i, 2 * i);
            }
        } else {
            for (int i = 0; i < Main.N; i++) {
                addValue(i, 3 * i);
            }
        }
    }
}
