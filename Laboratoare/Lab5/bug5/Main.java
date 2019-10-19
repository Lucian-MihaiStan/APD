/**
 * @author cristian.chilipirea
 *
 * DO NOT MODIFY
 */
public class Main {
    static final int N = 1000000; // Try a smaller value for N
    static final Object lockA = new Object();
    static final Object lockB = new Object();
    static int valueA = 0;
    static int valueB = 0;

    public static void main(String[] args) {
        Thread[] threads = new Thread[2];

        threads[0] = new Thread(new MyThreadA());
        threads[1] = new Thread(new MyThreadB());

		for (Thread thread : threads) {
			thread.start();
		}

		for (Thread thread : threads) {
			try {
				thread.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

        System.out.println("I finished. valueA = " + valueA + "; valueB = " + valueB);
    }
}
