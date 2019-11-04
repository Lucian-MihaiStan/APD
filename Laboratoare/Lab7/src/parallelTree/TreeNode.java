package parallelTree;

/**
 * @author cristian.chilipirea
 *
 */
class TreeNode {
	int name;
	TreeNode left;
	TreeNode right;
	private final Object lock;

	TreeNode(int name) {
		this.name 	= name;
		left 		= null;
		right		= null;
		lock 		= new Object();
	}

	void addChild(TreeNode child) {
		synchronized (lock) {
			if (left == null) {
				left = child;
			} else {
				right = child;
			}
		}
	}

	TreeNode getNode(int name) {
		TreeNode aux = null;

		if (this.name == name) {
			return this;
		}

		if (left != null) {
			aux = left.getNode(name);
		}
		if (aux != null) {
			return aux;
		}

		if (right != null) {
			aux = right.getNode(name);
		}
		if (aux != null) {
			return aux;
		}

		return null;
	}
}
