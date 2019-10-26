package philosophersProblem;

/**
 * @author cristian.chilipirea
 * 
 * DO NOT MODIFY
 */
public class Main {
    private static final int N = 6;

    public static void main(String[] args) {
        Thread[] threads 	= new Thread[N];
        Object[] forks 		= new Object[N];

        System.out.println("Philosophers problem!");

        for (int i = 0; i < N; i++) {
            forks[i] = new Object();
        }

        for (int i = 0; i < N; i++) {
            threads[i] = new Thread(new Philosopher(i, forks[i], forks[(i + 1) % N]));
            threads[i].start();
        }

        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

}
/* DO NOT MODIFY */