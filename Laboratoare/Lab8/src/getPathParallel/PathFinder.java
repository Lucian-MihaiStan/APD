package getPathParallel;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;

public class PathFinder extends Thread {
    private ExecutorService tpe;
    private ArrayList<Integer> crtPath;
    private final int target;

    PathFinder(ExecutorService tpe, ArrayList<Integer> crtPath, int target) {
        this.tpe        = tpe;
        this.crtPath    = crtPath;
        this.target     = target;
    }

    @Override
    public void run() {
        if (crtPath.get(crtPath.size() - 1) == target) {
            System.out.println(crtPath);
            tpe.shutdown();
        }

        // Try to add all possible next nodes that do not create a cycle.
        int lastNode = crtPath.get(crtPath.size() - 1);

        for (int[] edge : Main.graph) {
            if (edge[0] == lastNode && !crtPath.contains(edge[1])) {
                ArrayList<Integer> newPath = (ArrayList<Integer>)crtPath.clone();
                newPath.add(edge[1]);
                tpe.submit(new PathFinder(tpe, newPath, target));
            }
        }
    }
}
