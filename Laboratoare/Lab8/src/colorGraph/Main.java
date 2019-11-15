package colorGraph;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * @author cristian.chilipirea
 * 
 */
public class Main {
	static final int N 			= 10;
	static final int COLORS 	= 3;
	static final int[][] graph 	= { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 },
			{ 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 }, { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 },
			{ 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 }, { 6, 8 }, { 6, 9 },
			{ 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
			{ 9, 7 } };

	public static void main(String[] args) {
		ExecutorService tpe = Executors.newFixedThreadPool(12); // sarakilor
		int[] colours 		= new int[N];

		tpe.submit(new ColouringFinder(tpe, colours, 0));
	}
}