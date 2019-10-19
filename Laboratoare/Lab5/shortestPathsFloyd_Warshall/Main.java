/**
 * @author cristian.chilipirea
 *
 */
public class Main {
	private static void printMatrix(int[][] mat, int N) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				System.out.print(mat[i][j] + " ");
			}
			System.out.println();
		}
	}

    public static void main(String[] args) {
        int M = 9;
        int numThreads = 2;
        int[][] graph = { { 0, 1, M, M, M },
                          { 1, 0, 1, M, M },
                          { M, 1, 0, 1, 1 },
                          { M, M, 1, 0, M },
                          { M, M, 1, M, 0 } };
		int[][] parGraph = { { 0, 1, M, M, M },
							 { 1, 0, 1, M, M },
							 { M, 1, 0, 1, 1 },
							 { M, M, 1, 0, M },
							 { M, M, 1, M, 0 } };

        // Parallelize me (You might want to keep the original code in order to compare)
        for (int k = 0; k < 5; ++k) {
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
        }

        RoyFloyd[] threads = new RoyFloyd[numThreads];

        for (int i = 0; i < numThreads; ++i) {
        	threads[i] = new RoyFloyd(parGraph, i, 5, numThreads);
        	threads[i].start();
		}

		for (RoyFloyd thread : threads) {
			try {
				thread.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		printMatrix(graph, 5);
		System.out.println();
		printMatrix(parGraph, 5);

    }
}
