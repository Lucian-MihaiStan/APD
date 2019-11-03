package synchronizedList;

import java.io.FileReader;
import java.io.File;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Writer extends Thread {
    private final int id;
    private Semaphore sem;
    private List<Integer> buffer;

    Writer(int id, Semaphore sem, List<Integer> buffer) {
        this. id    = id;
        this.sem    = sem;
        this.buffer = buffer;
    }

    @Override
    public void run() {
        File inputFile = new File("elemente" + id + ".txt");

        try {
            FileReader fileReader = new FileReader(inputFile);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String number;

            while ((number = bufferedReader.readLine()) != null) {
                    buffer.add(Integer.valueOf(number));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        sem.release();
    }
}
