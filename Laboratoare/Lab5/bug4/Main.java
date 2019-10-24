/**
 * @author cristian.chilipirea
 *
 * Why doesn't this program end? (Hint: volatile)
 *
 * volatile nu poate fi cacheuit; este tinut doar in RAM
 */
public class Main extends Thread {
	private volatile boolean keepRunning = true;

	public void run() {
		long count = 0;

		while (keepRunning) {
			count++;
		}

		System.out.println("Thread terminated: " + count);
	}

	public static void main(String[] args) throws InterruptedException {
		Main t = new Main();
		t.start();

		Thread.sleep(1000);

		t.keepRunning = false;
		System.out.println("keepRunning set to false.");
	}
}
