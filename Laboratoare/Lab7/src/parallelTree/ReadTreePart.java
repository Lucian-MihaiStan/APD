package parallelTree;

/**
 * @author cristian.chilipirea
 *
 */
public class ReadTreePart implements Runnable {
	TreeNode tree;
	String fileName;

	public ReadTreePart(TreeNode tree, String fileName) {
		this.tree = tree;
		this.fileName = fileName;
	}

	@Override
	public void run() {

	}
}
