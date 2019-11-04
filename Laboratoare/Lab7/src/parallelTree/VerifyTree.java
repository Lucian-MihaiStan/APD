package parallelTree;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

/**
 * @author cristian.chilipirea
 *
 */
public class VerifyTree implements Runnable {
	private TreeNode tree;
	private CyclicBarrier barrier;

	VerifyTree(TreeNode tree, CyclicBarrier barrier) {
		this.barrier 	= barrier;
		this.tree 		= tree;
	}

	private boolean isCorrect(TreeNode tree) {
		if (tree == null || tree.name >= 64) {
			return true;
		} else if (tree.left == null || tree.right == null) {
			return false;
		}

		final boolean aux = tree.left.name + tree.right.name == tree.name * 4 + 1;

		return aux && isCorrect(tree.left) && isCorrect(tree.right);
	}

	@Override
	public void run() {
		try {
			barrier.await();
		} catch (InterruptedException | BrokenBarrierException e) {
			e.printStackTrace();
		}

		if (isCorrect(tree)) {
			System.out.println("Correct");
		}
		else {
			System.out.println("Wrong");
		}
	}
}
