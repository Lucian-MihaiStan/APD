package queensProblem;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    static final int N = 10;

    public static void main(String[] args) {
        ExecutorService tpe     = Executors.newFixedThreadPool(12);  // sarakilor
        int[] queenPositions    = new int[N];

        tpe.submit(new QueensPlacer(tpe, queenPositions, 0));
    }
}
