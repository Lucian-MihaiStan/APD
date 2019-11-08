package queensProblem;

import java.util.concurrent.ExecutorService;

class QueensPlacer extends Thread {
    private ExecutorService tpe;
    private final int[] positions;
    private final int crtStep;

    QueensPlacer(ExecutorService tpe, int[] positions, int crtStep) {
        this.tpe        = tpe;
        this.positions  = positions;
        this.crtStep    = crtStep;
    }

    @Override
    public void run() {
        if (crtStep == Main.N) {
            printQueens();
            tpe.shutdownNow();
        }

        for (int i = 0; i < Main.N; ++i) {
            int[] newPositions = positions.clone();
            newPositions[crtStep] = i;

            if (areQueensValid(newPositions)) {
                tpe.submit(new QueensPlacer(tpe, newPositions, crtStep + 1));
            }
        }
    }

    private void printQueens() {
        StringBuilder buffer = new StringBuilder();

        for (int i = 0; i < Main.N; ++i) {
            for (int j = 0; j < Main.N; ++j) {
                if (j == positions[i]) {
                    buffer.append("Q");
                } else {
                    buffer.append("-");
                }
            }

            buffer.append("\n");
        }

        System.out.println(buffer);
    }

    private boolean areQueensValid(int[] pos) {
        for (int i = 0; i < crtStep; ++i) {
            for (int j = i + 1; j <= crtStep; ++j) {
                if (pos[i] == pos[j] || i - pos[i] == j - pos[j] || i + pos[i] == j + pos[j]) {
                    return false;
                }
            }
        }

        return true;
    }
}
