/**
 * @author cristian.chilipirea
 *
 */
public class Main {
    public static void main(String[] args) {
        if (args.length < 1) {
        	System.out.println("Not enough arguments.");
        	System.exit(1);
		}
        int numThreads = Integer.valueOf(args[0]);
    	int N = 100000013;
        int[] v = new int[N];
        MultVector[] threads = new MultVector[numThreads];

        for (int i = 0; i < N; ++i) {
            v[i] = i;
        }

        for (int i = 0; i < threads.length; ++i) {
        	threads[i] = new MultVector(v, i, N, numThreads);
        	threads[i].start();
		}

        for (MultVector thread : threads) {
			try {
				thread.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

        for (int i = 0; i < N; ++i) {
            if(v[i] != i * 2) {
                System.out.println("Wrong answer");
                System.exit(1);
            }
        }
        System.out.println("Correct");
    }

}
