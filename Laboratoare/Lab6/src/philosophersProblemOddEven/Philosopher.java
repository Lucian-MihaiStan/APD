package philosophersProblemOddEven;

/**
 * @author cristian.chilipirea
 * 
 */
public class Philosopher implements Runnable {
    private final Object leftFork, rightFork;
    private final int id;

    Philosopher(int id, Object leftFork, Object rightFork) {
        this.leftFork 	= leftFork;
        this.rightFork 	= rightFork;
        this.id 		= id;
    }

    private void sleep() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        if ((id & 1) == 0) {
            synchronized (rightFork) {
                sleep(); // delay added to make sure the dead-lock is visible
                synchronized (leftFork) {
                    System.out.println("Philosopher " + id + " is eating");
                }
            }
        } else {
            synchronized (leftFork) {
                sleep(); // delay added to make sure the dead-lock is visible
                synchronized (rightFork) {
                    System.out.println("Philosopher " + id + " is eating");
                }
            }
        }
    }
}
