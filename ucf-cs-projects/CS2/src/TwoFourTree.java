
public class TwoFourTree {
    private class TwoFourTreeItem {
        int values = 1;
        int value1 = 0;
        int value2 = 0;
        int value3 = 0;
        boolean isLeaf = true;
        TwoFourTreeItem parent = null;
        TwoFourTreeItem leftChild = null;
        TwoFourTreeItem rightChild = null;
        TwoFourTreeItem centerChild = null;
        TwoFourTreeItem centerLeftChild = null;
        TwoFourTreeItem centerRightChild = null;

        public boolean isTwoNode() {
            return values == 1;
        }

        public boolean isThreeNode() {
            return values == 2;
        }

        public boolean isFourNode() {
            return values == 3;
        }

        public boolean isRoot() {
            return parent == null;
        }

        public TwoFourTreeItem(int value1) {
            this.value1 = value1;
            this.values = 1;
            this.isLeaf = true;
        }

        @SuppressWarnings("unused")
		public TwoFourTreeItem(int value1, int value2) {
            this.value1 = Math.min(value1, value2);
            this.value2 = Math.max(value1, value2);
            this.values = 2;
            this.isLeaf = true;
        }

        @SuppressWarnings("unused")
		public TwoFourTreeItem(int value1, int value2, int value3) {
            int min = Math.min(Math.min(value1, value2), value3);
            int max = Math.max(Math.max(value1, value2), value3);
            this.value1 = min;
            this.value3 = max;
            this.value2 = value1 + value2 + value3 - min - max;
            this.values = 3;
            this.isLeaf = true;
        }
        
        
     // Insert a value into the node
        public void insertValue(int value) {
            if (isTwoNode()) {
                if (value < value1) {
                    value2 = value1;
                    value1 = value;
                } else {
                    value2 = value;
                }
                values++;
            } else if (isThreeNode()) {
                if (value < value1) {
                    value3 = value2;
                    value2 = value1;
                    value1 = value;
                } else if (value < value2) {
                    value3 = value2;
                    value2 = value;
                } else {
                    value3 = value;
                }
                values++;
            }
        }

        private void printIndents(int indent) {
            for(int i = 0; i < indent; i++) {
                System.out.print("  ");
            }
        }

        public void printInOrder(int indent) {
            if(!isLeaf) {
                leftChild.printInOrder(indent + 1);
            }
            printIndents(indent);
            System.out.println(value1);
            if(isThreeNode()) {
                if(!isLeaf) {
                    centerChild.printInOrder(indent + 1);
                }
                printIndents(indent);
                System.out.println(value2);
            } else if(isFourNode()) {
                if(!isLeaf) {
                    centerLeftChild.printInOrder(indent + 1);
                }
                printIndents(indent);
                System.out.println(value2);
                if(!isLeaf) {
                    centerRightChild.printInOrder(indent + 1);
                }
                printIndents(indent);
                System.out.println(value3);
            }
            if(!isLeaf) {
                rightChild.printInOrder(indent + 1);
            }
        }


    }
    private TwoFourTreeItem root = null;
    private int size = 0;

    public TwoFourTree() {
    }


    public boolean addValue(int value) {
        System.out.println("Adding value: " + value);

        if (root == null) {
            System.out.println("Creating new root with value: " + value);
            root = new TwoFourTreeItem(value);
            size++;
            return true;
        }

        TwoFourTreeItem current = root;

        while (true) {
        	if (current == null) {
        		System.out.println ("Current node is null");
        		return false;
        	}
        	
        	System.out.println("Current node values: " + current.value1 + (current.isTwoNode() ? "" : ", " + current.value2) + (current.isThreeNode() ? ", " + current.value3 : ""));
            if (current.isFourNode()) {
                System.out.println("Splitting four-node");
                

                // Determine the correct child to continue insertion
                if (current == root) {
                    root = split(current); // Update root if necessary
                    current = root;
                } else {
                	TwoFourTreeItem parent = current.parent;
                	TwoFourTreeItem split = split(current);
                	if (current == parent.leftChild) {
                		parent.leftChild = split;
                	} else if ( current == parent.centerChild) {
                		parent.centerChild = split;
                	} else {
                		parent.rightChild = split;
                	}
                	split.parent = parent;
                	parent.insertValue(split.value1);
                	current = findChild(parent, value);
            }
                continue;
            }

            if (current.isLeaf) {
                System.out.println("Inserting value into leaf node");
                insertIntoLeaf(current, value);
                size++;
                return true;
            }

            System.out.println("Finding its way to child node");
            current = findChild(current, value);
            System.out.println("Current node after finding child: " + (current == null ? "null" : "not null"));
        }
    }

    
    public boolean hasValue(int value) {
        // If the node is found, return true; otherwise, false
        return findNode(value) != null;
    }
    
    public boolean deleteValue(int value) {
        // Find the node containing the value
        TwoFourTreeItem node = findNode(value);
        if (node == null) {
            return false; // If the node isn't found, return false
        }

        // If the node is not a leaf, replace the value with its predecessor
        if (!node.isLeaf) {
            int predecessorValue = 0;
			try {
				predecessorValue = getPredecessor(value);
			} catch (Exception e) {
                // TODO Auto-generated catch block

			}  // Get predecessor value
            deleteValue(predecessorValue);  // Recursively delete the predecessor
            replaceValue(node, value, predecessorValue);  // Replace the deleted value with predecessor
        } else {
            // If the node is a leaf, remove the value directly from the leaf node
            removeFromLeaf(node, value);
        }

        size--; // Decrement the size of the tree
        return true;
    }
    

    private TwoFourTreeItem split(TwoFourTreeItem node) {
    	
    	System.out.println("Splitting node with values: " + node.value1 + ", " + node.value2 + ", " + node.value3);
    	
        TwoFourTreeItem left = new TwoFourTreeItem(node.value1);
        TwoFourTreeItem right = new TwoFourTreeItem(node.value3);
        TwoFourTreeItem newParent = new TwoFourTreeItem(node.value2);

        newParent.leftChild = left;
        newParent.rightChild = right;
        left.parent = newParent;
        right.parent = newParent;

        if (!node.isLeaf) {
            left.leftChild = node.leftChild;
            left.rightChild = node.centerChild;
            right.leftChild = node.centerRightChild;
            right.rightChild = node.rightChild;

            left.isLeaf = false;
            right.isLeaf = false;

            if (left.leftChild != null) left.leftChild.parent = left;
            if (left.rightChild != null) left.rightChild.parent = left;
            if (right.leftChild != null) right.leftChild.parent = right;
            if (right.rightChild != null) right.rightChild.parent = right;
        }

        if (node.parent == null) {
            root = newParent;
            root.isLeaf = false;
        } else {
            insertIntoParent(node.parent, newParent);
        }
        return newParent;
    }
    
    
    
    
    private void insertIntoParent(TwoFourTreeItem parent, TwoFourTreeItem newChild) {
        int valueToInsert = newChild.value1; // Middle value from split node
        TwoFourTreeItem leftChild = newChild.leftChild;
        TwoFourTreeItem rightChild = newChild.rightChild;

        // Case when parent is a 2-node
        if (parent.isTwoNode()) {
            if (valueToInsert < parent.value1) {
                parent.value2 = parent.value1;
                parent.value1 = valueToInsert;
                parent.centerChild = parent.leftChild;
                parent.leftChild = leftChild;
                parent.rightChild = rightChild;
            } else {
                parent.value2 = valueToInsert;
                parent.centerChild = leftChild;
                parent.rightChild = rightChild;
            }
            parent.values++;
        } else if (parent.isThreeNode()) {
            // Case when parent is a 3-node
            if (valueToInsert < parent.value1) {
                parent.value3 = parent.value2;
                parent.value2 = parent.value1;
                parent.value1 = valueToInsert;
                parent.centerRightChild = parent.centerLeftChild;
                parent.centerLeftChild = parent.leftChild;
                parent.leftChild = leftChild;
                parent.rightChild = rightChild;
            } else if (valueToInsert < parent.value2) {
                parent.value3 = parent.value2;
                parent.value2 = valueToInsert;
                parent.centerRightChild = parent.centerLeftChild;
                parent.centerLeftChild = leftChild;
                parent.rightChild = rightChild;
            } else {
                parent.value3 = valueToInsert;
                parent.centerRightChild = leftChild;
                parent.rightChild = rightChild;
            }
            parent.values++;
        } else {
            // Case when parent is also a 4-node
            split(parent);
            insertIntoParent(parent.parent, newChild);
        }

        // Assign the new child's children to their respective parents
        if (leftChild != null) leftChild.parent = parent;
        if (rightChild != null) rightChild.parent = parent;
    }
    
    
    @SuppressWarnings("unused")
	private boolean insert(TwoFourTreeItem node, int value) {
        // If the node is a 4-node, split it
        if (node.isFourNode()) {
            split(node);
            // Adjust the node to be the appropriate child after splitting
            node = findChild(node.parent, value);
        }

        // If the node is a leaf, insert the value into the leaf node
        if (node.isLeaf) {
            insertIntoLeaf(node, value);
            size++;
            return true;
        } else {
            // Find the appropriate child node for the next iteration
            TwoFourTreeItem child = findChild(node, value);
            return insert(child, value);
        }
    }
    
    
    private void insertIntoLeaf (TwoFourTreeItem leaf, int value) {
    	if (leaf.isTwoNode()) {
    		if (value < leaf.value1) {
    			leaf.value2 = leaf.value1;
    			leaf.value1 =value;
    		} else {
    			leaf.value2 = value;
    		}
    		leaf.values ++;
    	} else if (leaf.isThreeNode()) {
    		if (value < leaf.value1) {
    			leaf.value3 = leaf.value2;
    			leaf.value2 = leaf.value1;
    			leaf.value1 =value;  		
    	} else {
    		leaf.value3 = value;
    	}
    		leaf.values ++;
    }
    	// No need to handle 4-node here since they should be split on descent
    }
    
    private TwoFourTreeItem findChild (TwoFourTreeItem node, int value) {
    	if (node == null) return null; // Null check
    	if (value < node.value1) {
    		return node.leftChild;
    	} else if (node.isTwoNode() || (node.isThreeNode() && value < node.value2)) {
    		return node.centerChild;
    	} else if (node.isThreeNode() || (node.isFourNode() && value < node.value3)) {
    		return node.centerRightChild;
    	} else {
    		return node.rightChild;
    	}
    }
    

    private void removeFromLeaf(TwoFourTreeItem node, int value) {
        if (node == null) {
            System.out.println("Error: Attempt to remove from a null node.");
            return;
        }

        // Handle underflow if the node is a 2-node before removal
        if (node.isTwoNode()) {
            handleUnderflow(node);
        }

        // Remove the value from the node (must have at least 2 values now)
        if (node.value1 == value) {
            node.value1 = node.value2;  // Shift values left
            node.value2 = node.value3;
        } else if (node.value2 == value) {
            node.value2 = node.value3;  // Remove the second value
        } else if (node.value3 == value) {
            // If it's the third value, just clear it
        } else {
            System.out.println("Error: Value " + value + " not found in leaf node");
            return;
        }

        node.value3 = 0;  // Clear any leftover value in the third slot
        node.values--;  // Decrement the number of values in the node

        // Check if there's an underflow after removal
        if (node.values > 0 || node.isRoot()) {
            return; // If node still has values or is the root, we're done
        }

        // Handle underflow if the node has no values left
        handleUnderflow(node);
    }

    private void handleUnderflow(TwoFourTreeItem node) {
        // If the node is the root and it's a 2-node, we might need to shrink the tree
        if (node.isRoot()) {
            if (node.isTwoNode() && !node.isLeaf) {
                root = node.leftChild;  // Set the left child as the new root
                root.parent = null;
            }
            return;
        }

        // Retrieve the parent and siblings of the node
        TwoFourTreeItem parent = node.parent;
        TwoFourTreeItem leftSibling = getLeftSibling(parent, node);
        TwoFourTreeItem rightSibling = getRightSibling(parent, node);

        // Try borrowing from the left sibling if possible
        if (leftSibling != null && !leftSibling.isTwoNode()) {
            borrowFromLeft(node, leftSibling, parent);
        } else if (rightSibling != null && !rightSibling.isTwoNode()) {
            // Try borrowing from the right sibling if possible
            borrowFromRight(node, rightSibling, parent);
        } else {
            // If both siblings are 2-nodes, merge the nodes
            if (leftSibling != null) {
                mergeNodes(leftSibling, node, parent);
            } else {
                mergeNodes(node, rightSibling, parent);
            }
        }
    }

    private void borrowFromLeft(TwoFourTreeItem node, TwoFourTreeItem leftSibling, TwoFourTreeItem parent) {
        // Shift the parent's value to the node
        if (parent.rightChild == node) {
            node.value1 = parent.value2;
            parent.value2 = leftSibling.values == 3 ? leftSibling.value3 : leftSibling.value2;
        } else {
            node.value1 = parent.value1;
            parent.value1 = leftSibling.values == 3 ? leftSibling.value3 : leftSibling.value2;
        }

        // Adjust the left sibling's values after borrowing
        if (leftSibling.values == 3) {
            leftSibling.value3 = 0;
        } else {
            leftSibling.value2 = 0;
        }
        leftSibling.values--;

        // If the node isn't a leaf, move the sibling's child to this node
        if (!node.isLeaf) {
            TwoFourTreeItem childToMove = (leftSibling.values == 2) ? leftSibling.rightChild : leftSibling.centerRightChild;
            node.rightChild = node.centerChild;
            node.centerChild = node.leftChild;
            node.leftChild = childToMove;
            childToMove.parent = node;

            // Update the left sibling's children
            if (leftSibling.values == 2) {
                leftSibling.rightChild = null;
            } else {
                leftSibling.centerRightChild = null;
            }
        }

        node.values++;  // Increase the values count in the node
    }

    private void borrowFromRight(TwoFourTreeItem node, TwoFourTreeItem rightSibling, TwoFourTreeItem parent) {
        // Shift the parent's value to the node
        if (parent.leftChild == node) {
            node.value2 = parent.value1;
            parent.value1 = rightSibling.value1;
        } else {
            node.value2 = parent.value2;
            parent.value2 = rightSibling.value1;
        }

        // Shift the right sibling's values after borrowing
        rightSibling.value1 = rightSibling.value2;
        rightSibling.value2 = rightSibling.value3;
        rightSibling.value3 = 0;
        rightSibling.values--;

        // If the node isn't a leaf, move the sibling's child to this node
        if (!node.isLeaf) {
            TwoFourTreeItem childToMove = rightSibling.leftChild;
            node.centerChild = childToMove;
            childToMove.parent = node;

            // Update the right sibling's children
            rightSibling.leftChild = rightSibling.centerChild;
            rightSibling.centerChild = rightSibling.rightChild;
            rightSibling.rightChild = rightSibling.centerRightChild;
            rightSibling.centerRightChild = null;
        }

        node.values++;  // Increase the values count in the node
    }
    
    private void mergeNodes(TwoFourTreeItem leftNode, TwoFourTreeItem rightNode, TwoFourTreeItem parent) {
    	
    	if (leftNode == null || rightNode == null || parent == null) {
    	        System.out.println("Error: Null node in mergeNodes");
    	        return;
    	    }
    	
    	int parentIndex = (parent.leftChild == leftNode) ? 0 :
                          (parent.centerChild == leftNode) ? 1 : 2;

        switch (parentIndex) {
            case 0 -> {
                leftNode.value2 = parent.value1;
                parent.value1 = parent.value2;
                parent.value2 = parent.value3;
            }
            case 1 -> {
                leftNode.value2 = parent.value2;
                parent.value2 = parent.value3;
            }
            default -> leftNode.value2 = parent.value3;
        }
        parent.value3 = 0;
        parent.values--;

        leftNode.value3 = rightNode.value1;
        leftNode.values = 3;

        if (!leftNode.isLeaf) {
            leftNode.centerRightChild = rightNode.leftChild;
            leftNode.rightChild = rightNode.centerChild;
            if (leftNode.centerRightChild != null) leftNode.centerRightChild.parent = leftNode;
            if (leftNode.rightChild != null) leftNode.rightChild.parent = leftNode;
        }

        switch (parentIndex) {
            case 0 -> {
                parent.leftChild = leftNode;
                parent.centerChild = parent.rightChild;
                parent.rightChild = parent.centerRightChild;
            }
            case 1 -> {
                parent.centerChild = leftNode;
                parent.rightChild = parent.centerRightChild;
            }
            default -> parent.rightChild = leftNode;
        }
        parent.centerRightChild = null;

        if (parent.values == 0) {
            handleUnderflow(parent);
        }
    }

    private TwoFourTreeItem getLeftSibling(TwoFourTreeItem parent, TwoFourTreeItem node) {
        if (parent.leftChild == node) return null;
        if (parent.centerChild == node) return parent.leftChild;
        if (parent.rightChild == node) return parent.centerChild;
        if (parent.centerRightChild == node) return parent.centerLeftChild;
        return null;
    }

    private TwoFourTreeItem getRightSibling(TwoFourTreeItem parent, TwoFourTreeItem node) {
        if (parent.leftChild == node) return parent.centerChild;
        if (parent.centerChild == node) return parent.rightChild;
        if (parent.centerLeftChild == node) return parent.centerRightChild;
        if (parent.centerRightChild == node) return parent.rightChild;
        return null;
    }

    private void replaceValue(TwoFourTreeItem node, int oldValue, int newValue) {
        if (oldValue == node.value1) node.value1 = newValue;
        else if (oldValue == node.value2) node.value2 = newValue;
        else if (oldValue == node.value3) node.value3 = newValue;
    }

    private TwoFourTreeItem findNode(int value) {
        TwoFourTreeItem current = root;
        while (current != null) {
            if (value < current.value1) {
                current = current.leftChild;
            } else if (value == current.value1) {
                return current;
            } else if (current.isTwoNode() || (current.isThreeNode() && value < current.value2)) {
                current = current.centerChild;
            } else if (value == current.value2) {
                return current;
            } else if (current.isThreeNode() || (current.isFourNode() && value < current.value3)) {
                current = current.centerRightChild;
            } else if (value == current.value3) {
                return current;
            } else {
                current = current.rightChild;
            }
        }
        return null;
    }


    private int getPredecessor(int value) throws Exception {
        TwoFourTreeItem node = findNode(value);
        if (node == null) {
            throw new Exception("Value not found in the tree.");
        }
        
        // Go left, and then traverse to the rightmost node
        if (node.leftChild != null) {
            TwoFourTreeItem current = node.leftChild;
            while (current.rightChild != null) {
                current = current.rightChild;
            }
            return current.value1;  // Return the largest value in the left subtree
        }
        
        // Traverse upwards if left child is null
        TwoFourTreeItem current = node;
        TwoFourTreeItem parent = node.parent;
        while (parent != null && current == parent.leftChild) {
            current = parent;
            parent = parent.parent;
        }
        
        return parent != null ? parent.value1 : -1;  // Return the predecessor
    }

    public void printInOrder() {
        if (root != null) {
            root.printInOrder(0);
        }
    }

    public int size() {
        return size;
    }

    public void clear() {
        root = null;
        size = 0;
    }

    public int height() {
        return height(root);
    }

    private int height(TwoFourTreeItem node) {
        if (node == null) {
            return -1;
        }
        if (node.isLeaf) {
            return 0;
        }
        return 1 + height(node.leftChild);
    }

    public boolean validateTree() {
        if (root == null) {
            return true;
        }
        return validateNode(root, null, null) != -1;
    }

    private int validateNode(TwoFourTreeItem node, Integer min, Integer max) {
        if (node == null) {
            return 0;
        }
        
        if (node.values < 1 || node.values > 3) {
            return -1;
        }
        
        if (node.values > 1 && node.value1 >= node.value2) {
            return -1;
        }
        if (node.values > 2 && node.value2 >= node.value3) {
            return -1;
        }
        
        if (min != null && node.value1 <= min) {
            return -1;
        }
        if (max != null && node.value3 >= max) {
            return -1;
        }
        
        if (node.isLeaf) {
            return 1;
        }
        
        int leftHeight = validateNode(node.leftChild, min, node.value1);
        int centerHeight = node.values > 1 ? validateNode(node.centerChild, node.value1, node.value2) : 0;
        int centerLeftHeight = node.values > 2 ? validateNode(node.centerLeftChild, node.value1, node.value2) : 0;
        int centerRightHeight = node.values > 2 ? validateNode(node.centerRightChild, node.value2, node.value3) : 0;
        int rightHeight = validateNode(node.rightChild, node.values > 2 ? node.value3 : node.value2, max);
        
        if (leftHeight != rightHeight || (centerHeight != 0 && centerHeight != leftHeight) ||
            (centerLeftHeight != 0 && centerLeftHeight != leftHeight) ||
            (centerRightHeight != 0 && centerRightHeight != leftHeight)) {
            return -1;
        }
        
        return leftHeight + 1;
    }
}
    		
    		
    		
    		
    		