/**
 * @author cristian.chilipirea
 * 
 *  Why is value set correct even though we use different locks in
 *  different threads?
 *  Pentru ca a si b sunt referinte catre acelasi sir si Java e cretin si le face practic acelasi
 *  obiect. Daca se schimba unul dintre siruri, apar race conditionuri pentru ca threadurile vor
 *  fi sincronizate dupa obiecte diferite.
 */
public class MyThread implements Runnable {
    private static final String a 	= "LOCK";
    private static final String b 	= "LOCK";
    static int value 				= 0;
	private int id;

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
