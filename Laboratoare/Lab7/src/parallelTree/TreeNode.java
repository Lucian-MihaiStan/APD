package parallelTree;

/**
 * @author cristian.chilipirea
 *
 */
class TreeNode {
	int name;
	TreeNode left;
	TreeNode right;
	private static Object[] locks;
	private static final int NUM_PARENTS = 64;

	TreeNode(int name) {
		this.name 	= name;
		left 		= null;
		right		= null;

		if (name == 1) {
			locks = new Object[NUM_PARENTS];

			for (int i = 1; i < NUM_PARENTS; ++i) {
				locks[i] = new Object();
			}
		}
	}

	void addChild(TreeNode child) {
		synchronized (locks[name]) {
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
