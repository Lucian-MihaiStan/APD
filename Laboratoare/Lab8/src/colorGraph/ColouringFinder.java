package colorGraph;

import java.util.concurrent.ExecutorService;

public class ColouringFinder extends Thread {
    private ExecutorService tpe;
    private final int[][] graph;
    private final int[] colours;
    private final int crtStep;

    ColouringFinder(ExecutorService tpe, int[][] graph, int[] colours, int crtStep) {
        this.tpe        = tpe;
        this.graph      = graph;
        this.colours    = colours;
        this.crtStep    = crtStep;
    }

    @Override
    public void run() {
        if (crtStep == Main.N) {
            printColors();
            tpe.shutdownNow();
        }

        // for the node at position step try all possible colors
        for (int i = 0; i < Main.COLORS; i++) {
            int[] newColours    = colours.clone();
            newColours[crtStep] = i;

            if (verifyColors(newColours)) {
                tpe.submit(new ColouringFinder(tpe, graph, newColours, crtStep + 1));
            }
        }
    }

    private boolean verifyColors(int[] newColours) {
        for (int i = 0; i < crtStep; i++) {
            if (newColours[i] == newColours[crtStep] && isEdge(i, crtStep)) {
                return false;
            }
        }
        return true;
    }

    private boolean isEdge(int a, int b) {
        for (int[] edge : graph) {
            if (edge[0] == a && edge[1] == b) {
                return true;
            }
        }

        return false;
    }

    private void printColors() {
        StringBuilder buffer = new StringBuilder();

        for (int colour : colours) {
            buffer.append(colour).append(" ");
        }

        System.out.println(buffer);
    }
}
