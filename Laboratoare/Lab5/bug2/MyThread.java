/**
 * @author cristian.chilipirea
 * 
 * Why does this code not block? We took the same lock twice!
 *
 * Cand un thread detine lockul unui obiect, orice noua incercare a aceluias thread de a obtine
 * respectivul lock este posibila si nu apare un deadlock.
 * Fenomenul se numeste "reentrant synchronization".
 * Pe deasupra, orice clasa are in mod automat lock pe ea insasi.
 */
public class MyThread implements Runnable {
	static int i;

	@Override
	public void run() {
		synchronized (this) {
			synchronized (this) {
				i++;
			}
		}
	}
}
