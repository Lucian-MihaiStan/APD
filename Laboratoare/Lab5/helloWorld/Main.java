public class Main {
    public static void main(String[] args) {
        int N = 4;
        HelloThread[] threads = new HelloThread[N];

        for (int i = 0; i < N; ++i) {
            threads[i] = new HelloThread(i);
            threads[i].start();
        }

        for (HelloThread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
