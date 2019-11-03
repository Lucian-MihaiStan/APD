package lock;

/**
 * @author cristian.chilipirea
 * 
 */
public class MyThread implements Runnable {
    private final int id;
	private MyLock lock;
    static int value = 0;

    MyThread(int id, MyLock lock) {
        this.id 	= id;
        this.lock 	= lock;
    }

    @Override
    public void run() {
        if (id == 0) {
            for (int i = 0; i < Main.N; i++) {
                lock.lock();
                value = value + 3;
                lock.unlock();
            }
        } else {
            for (int i = 0; i < Main.N; i++) {
                lock.lock();
                value = value + 3;
                lock.unlock();
            }
        }
    }
}
