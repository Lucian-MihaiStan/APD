public class HelloThread extends Thread {
    private int tid;

    HelloThread(int tid) {
        this.tid = tid;
    }

    @Override
    public void run() {
        System.out.println("Hello from thread " + tid);
    }
}
