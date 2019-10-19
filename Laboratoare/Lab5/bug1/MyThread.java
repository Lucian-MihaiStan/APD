
/**
 * @author cristian.chilipirea
 * 
 *	Why is value set correct even though we do not use
 *  locks/synchronized?
 *  DO NOT MODIFY
 *
 *  Daca se apeleaza `run()`, codul ruleaza secvential
 *  Trebuie apelat `start()`
 */
public class MyThread implements Runnable {
	static int value = 0;

	@Override
	public void run() {
		for (int i = 0; i < Main.N; ++i) {
			synchronized (Main.class) {
				value += 3;
			}
		}
	}
}
