/**
 * @author cristian.chilipirea
 * 
 *  Why is value set correct even though we use different locks in
 *  different threads?
 */
public class MyThread implements Runnable {
    private static final String a 	= "LOCKa";
    private static final String b 	= "LOCKb";
    static int value 				= 0;
	private final int id;

	MyThread(int id) {
        this.id = id;
    }

    @Override
    public void run() {
        if (id == 0) {
            synchronized (a) {
                for (int i = 0; i < Main.N; ++i) {
                    value += 3;
                }
            }
        } else {
            synchronized (b) {
				for (int i = 0; i < Main.N; ++i) {
					value += 3;
				}
            }
        }
    }
}
