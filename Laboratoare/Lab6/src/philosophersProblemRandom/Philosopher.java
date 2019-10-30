package philosophersProblemRandom;

import java.util.Random;
import static java.lang.Math.abs;

/**
 * @author cristian.chilipirea
 * 
 */
public class Philosopher implements Runnable {
    private final Object leftFork, rightFork;
    private final int id;
    private final Random rand;

    Philosopher(int id, Object leftFork, Object rightFork) {
        this.leftFork 	= leftFork;
        this.rightFork 	= rightFork;
        this.id 		= id;
        rand = new Random();
    }

    private void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        sleep(abs(rand.nextInt()) % 500);

        synchronized (leftFork) {
            sleep(100); // delay added to make sure the dead-lock is visible
            synchronized (rightFork) {
                System.out.println("Philosopher " + id + " is eating");
            }
        }
    }
}
