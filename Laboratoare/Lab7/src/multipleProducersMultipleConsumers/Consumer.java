package multipleProducersMultipleConsumers;

/**
 * @author cristian.chilipirea
 * 
 * DO NOT MODIFY
 */
public class Consumer implements Runnable {
    private Buffer buffer;
    private final int id;
    private static int i = 0;

    Consumer(Buffer buffer, int id) {
        this.buffer = buffer;
        this.id 	= id;
    }

    private int getNextI() {
        final int value;

        synchronized (Consumer.class) {
            value = i;
            i++;
        }

        return value;
    }

    @Override
    public void run() {
        while (true) {
            final int i = getNextI();

            if (i >= Main.N_PRODUCERS * Main.N) {
                break;
            }

            synchronized (Main.results) {
                Main.results[buffer.get()]++;
            }
        }
        System.out.println("Consumer " + id + " finished Correctly");
    }
}
/* DO NOT MODIFY */