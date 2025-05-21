/*
Destiny Ellenwood
UCFID: 5276398
Major Project: Skip List

*/
package skiplist;

import java.util.*;

//This class implements a skip list as a SortedSet, ensuring elements are stored in sorted order.
//The constructor allows initialization with any collection, adding all elements while maintaining sorted order.
public class SkipListSet<T extends Comparable<T>> implements SortedSet<T> {
	
	// Constructor that accepts any Collection of elements type T 
	public SkipListSet(Collection<? extends T> collection) {
	    this(); // Calls the default constructor. Initializes skip list.
	    addAll(collection); // Add all elements from the collection into the skip list.
	}

	
	// Represents a single node in the skip list, holding a value and pointers to the next nodes at each level.
	// The height determines how many levels this node participates in, with pointers initialized to null.
    private class SkipListSetItem {
        T value;
        List<SkipListSetItem> next; // Pointers for each level

        public SkipListSetItem(T value, int height) {
            this.value = value; // Assigns the given value to the node's value field.
            this.next = new ArrayList<>(Collections.nCopies(height, null)); // Creates a list of height elements all initialized to null.
        }
    }

    // Internal iterator class for traversing the skip list
    private class SkipListSetIterator implements Iterator<T> {
    	// Pointer to the current node being visited by the iterator
        private SkipListSetItem current;

        // Constructor for the iterator, initializing it to start at the given node
        public SkipListSetIterator(SkipListSetItem start) {
            this.current = start; // Set the initial position of the iterator
        }

        // Check if there is another node to visit by verifying if `current` is not null
        @Override
        public boolean hasNext() {
            return current != null;
        }

        @Override
        public T next() {
        	// If there is no next node, throw an exception to indicate the end of the traversal
            if (!hasNext()) throw new NoSuchElementException();
            T value = current.value; // Store the value of the current node to return it
            current = current.next.get(0); // Move the iterator to the next node on the bottom-most level of the skip list
            return value;  // Return the value of the node that was just visited
        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException("Remove is not supported by this iterator.");
        }
    }

    // Fields for the SkipListSet
    private SkipListSetItem head; // Sentinel (dummy) node at the start of the skip list
    private int maxLevel; // Maximum levels in the skip list
    private int size; // Number of elements in the skip list
    private Random random; // Random generator for node height

    // Constructor for the SkipListSet
    public SkipListSet() {
        this.head = new SkipListSetItem(null, 1); // Initialize the head node of the skip list as a sentinel (dummy) node with no value (null)
        // and a height of 1. This node serves as the starting point for the skip list at all levels.
        
        // Initialize the maximum level of the skip list to 1.
        // At the start, the skip list has only one level (the bottom most level).
        this.maxLevel = 1; 
        this.size = 0; // Initially empty
        this.random = new Random(); // Create a new random number generator to be used for determining the heights of new nodes.
    }

    // Randomize List Item Heights. Follows that there's a 50% chance of being height 2, 25% chance pf being height 3, etc.
    private int randomLevel() {
        int level = 1; // Start with a default level of 1 for the new node.
        // Continue increasing the level with a 50% probability as long as:
        // 1. The random value is less than 0.5 (indicating success for a coin flip),
        // 2. The current level is less than maxLevel + 1 to prevent exceeding allowed levels.
        while (random.nextDouble() < 0.5 && level < maxLevel + 1) {
            level++; // Increment the level for the new node.
        }
        return level; // Return the determined level for the new node.
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    // Checks if the specified object exists in the skip list
    @Override
    public boolean contains(Object o) {
        if (o == null) return false; // If the input object is null, immediately return false since null values are not allowed in the skip list.
        @SuppressWarnings("unchecked")
        T value = (T) o; // Cast the input object to the generic type T of the skip list.
        
        SkipListSetItem current = head; // Start the traversal at the dummy head node.
        
        for (int i = maxLevel - 1; i >= 0; i--) { // Traverse each level of the skip list starting from the top level.
            while (current.next.get(i) != null && current.next.get(i).value.compareTo(value) < 0) {  // Move forward while the next node at this level exists and its value is less than the target value.
                current = current.next.get(i); // Advance to the next node at the current level.
            }
        }
     // At this point, we are at the position where the value could exist at the bottom level (level 0).
        current = current.next.get(0); // Move to the next node at the lowest level.
        return current != null && current.value.equals(value);  // Check if the current node is not null and if its value equals the target value.
    }

    // Traverse the skip list to find the correct position for the new value.
    // Use a randomized height to determine the levels for the new node.
    // Update pointers to insert the new node into all relevant levels.
    // Maintain the skip list's properties.
    @Override
    @SuppressWarnings("unchecked") // Suppress unchecked cast warning
    public boolean add(T value) {
        if (value == null) throw new NullPointerException("Value cannot be null.");

        SkipListSetItem current = head;
        SkipListSetItem[] update = (SkipListSetItem[]) java.lang.reflect.Array.newInstance(SkipListSetItem.class, maxLevel);

        // Traverse the skip list to find the insertion point
        for (int i = maxLevel - 1; i >= 0; i--) {
            while (current.next.get(i) != null && current.next.get(i).value.compareTo(value) < 0) {
                current = current.next.get(i); // Move forward at level i
            }
            update[i] = current; // Track the node to update
        }

        // Check if the value already exists at the bottom level
        current = current.next.get(0);
        if (current != null && current.value.equals(value)) {
            return false; // Duplicate value; skip insertion
        }

        // Generate the random height for the new node
        int level = randomLevel();
        if (level > maxLevel) {
            // Adjust the update array and head node for the new levels
            SkipListSetItem[] newUpdate = (SkipListSetItem[]) java.lang.reflect.Array.newInstance(SkipListSetItem.class, level);
            System.arraycopy(update, 0, newUpdate, 0, maxLevel); // Copy old update values
            for (int i = maxLevel; i < level; i++) {
                newUpdate[i] = head; // New levels point to the head node
                head.next.add(null); // Extend head's next pointers
            }
            update = newUpdate;
            maxLevel = level; // Update maxLevel
        }

        // Create the new node
        SkipListSetItem newNode = new SkipListSetItem(value, level);

        // Insert the new node and update pointers
        for (int i = 0; i < level; i++) {
            newNode.next.set(i, update[i].next.get(i)); // Link to the next node
            update[i].next.set(i, newNode); // Update the previous node's pointer
        }

        size++; // Increment the size of the skip list
        
     // Adjust maxLevel if needed
        adjustMaxLevel();
        
        return true;
    }



    @Override
    public boolean remove(Object o) {
        if (o == null) return false;  // If the input object is null, return false (cannot remove null values).
        @SuppressWarnings("unchecked")
        T value = (T) o; // Cast the object to the expected type T.

        SkipListSetItem current = head; // Start from the head (sentinel node).
        SkipListSetItem[] update = (SkipListSetItem[]) java.lang.reflect.Array.newInstance(SkipListSetItem.class, maxLevel); // Array to track the nodes at each level for update.

     // Traverse the skip list from the highest level down to the lowest level to find the node to remove.
        for (int i = maxLevel - 1; i >= 0; i--) {
        	// While the current node has a next node at this level and the next node's value is smaller than the value to remove, move forward.
            while (current.next.get(i) != null && current.next.get(i).value.compareTo(value) < 0) {
                current = current.next.get(i); // Move forward at level i
            }
            update[i] = current; // Track the node to update
        }

     // After reaching the bottom level, check if the next node is the one to be removed.
        current = current.next.get(0); // Move to the node at the lowest level
        if (current == null || !current.value.equals(value)) {
            return false; // If the node was not found, return false (no removal).
        }

        // Update pointers to remove the node
        for (int i = 0; i < maxLevel; i++) {
            if (update[i].next.get(i) != current) break; // If there's no need to update at this level, break out of the loop.
            update[i].next.set(i, current.next.get(i)); // Bypass the current node at each level.
        
        }

     // Reduce the height of the skip list if any levels become empty.
        while (maxLevel > 1 && head.next.get(maxLevel - 1) == null) {
            maxLevel--; // Decrease maxLevel if the topmost level has no nodes.
        }

        size--; // Decrease the size of the skip list since one node was removed.
        
        // Adjust maxLevel if needed
        adjustMaxLevel();
        
        return true; // Node successfully removed
    }
    
    // Should re-reandomize the height of all existing nodes in the skip list
    public void reBalance() {
        List<T> elements = new ArrayList<>();
        for (T value : this) {
            elements.add(value); // Collect all elements
        }
        clear(); // Clear the skip list
        for (T value : elements) {
            add(value); // Re-add all elements (randomizing their heights)
        }
    }

    // Calculate the new max level based on the size of the skip list.
    private void adjustMaxLevel() {
        int newMaxLevel = Math.max(4, (int) (Math.log(size) / Math.log(2)) + 1);  // The new max level is at least 4 or the logarithm base 2 of the size (plus one), whichever is larger.
        
        // Check if the new max level is greater than the current max level.
        if (newMaxLevel > maxLevel) {
        	// If so, extend the head node's 'next' list to include additional levels.
            while (head.next.size() < newMaxLevel) {
                head.next.add(null); // Add null pointers for the new levels.
            }
        } else if (newMaxLevel < maxLevel) {
        	 // If the new max level is smaller than the current max level, shrink the head's 'next' list.
            while (head.next.size() > newMaxLevel) {
                head.next.remove(head.next.size() - 1); // Remove pointers for the unnecessary levels.
            }
        }
        maxLevel = newMaxLevel;  // Update the maxLevel field to reflect the new max level.
    }

    // Creates and returns an iterator starting at the first element of the skip list.
    @Override
    public Iterator<T> iterator() {
        return new SkipListSetIterator(head.next.get(0)); 
    }

    @Override
    public Comparator<? super T> comparator() {
        return null; // Use natural ordering
    }

    @Override
    public SortedSet<T> subSet(T fromElement, T toElement) {
        throw new UnsupportedOperationException();
    }

    @Override
    public SortedSet<T> headSet(T toElement) {
        throw new UnsupportedOperationException();
    }

    @Override
    public SortedSet<T> tailSet(T fromElement) {
        throw new UnsupportedOperationException();
    }

    // Returns the first (smallest) element in the skip list.
    @Override
    public T first() {
        if (isEmpty()) throw new NoSuchElementException(); // Throw an exception if the skip list is empty.
        return head.next.get(0).value; // Return the value of the first node at the lowest level.
    }

    // Returns the last (largest) element in the skip list.
    @Override
    public T last() {
        if (isEmpty()) throw new NoSuchElementException(); // Throw an exception if the skip list is empty.
        SkipListSetItem current = head; // Start at the dummy node.
        for (int i = maxLevel - 1; i >= 0; i--) { // Traverse down the highest levels of the skip list.
            while (current.next.get(i) != null) {
                current = current.next.get(i);
            }
        }
        return current.value; // Return the value of the last node.
    }

    // Converts the elements of the skip list to an array.
    @Override
    public Object[] toArray() {
        Object[] result = new Object[size]; // Creates an array to hold the elements.
        int index = 0; // Initialize an index for inserting elements.
        for (T value : this) { // Iterate through the skip list.
            result[index++] = value; //  Add each element to the array.
        }
        return result; // Return the new array.
    }

    // Converts the elements of the skip list to an array of type T1.
    @SuppressWarnings("unchecked")
	@Override
    public <T1> T1[] toArray(T1[] a) {
    	// If the given array is too small, it creates a new array.
        if (a.length < size) {
            a = (T1[]) java.lang.reflect.Array.newInstance(a.getClass().getComponentType(), size);
        }

        int index = 0; // Initialize index for inserting elements.
        for (T value : this) { // Iterate through the skip list.
            a[index++] = (T1) value;  //  Add each element to the array.
        }

        if (a.length > size) {
            a[size] = null; // Null-terminate if the input array is larger than the skip list
        }
        return a; // Return the new array.
    }

    // Checks if the skip list contains all elements in the given collection c.
    @Override
    public boolean containsAll(Collection<?> c) {
        for (Object o : c) { // Iterate through each element in the collection.
            if (!contains(o)) { // If all elements are not found in the skip list,
                return false; // Return false.
            }
        }
        return true; // If elements are found return true.
    }

    // Adds all elements in the given collection c to the skip list.
    @Override
    public boolean addAll(Collection<? extends T> c) {
        boolean modified = false; // Hasn't been modified yet.
        for (T value : c) { // Iterate through each element in the collection.
            if (add(value)) { // Add element to the skip list
                modified = true; // Changes were made to the list.
            }
        }
        return modified;
    }

    // Retains only the elements in the skip list that are also in the given collection c.
    @Override
    public boolean retainAll(Collection<?> c) {
        boolean modified = false; // Hasn't been modified yet.
        Iterator<T> iterator = iterator(); // Create an iterator for the skip list.
        while (iterator.hasNext()) { // Iterate through each element in the skip list.
            T value = iterator.next(); // Get the next element in the skip list.
            if (!c.contains(value)) { // If the element is not in the given collection,
                remove(value); // Remove it from the skip list.
                modified = true; // Changes were made to the list.
            }
        }
        return modified;
    }

 // Removes all elements in the given collection c from the skip list
    @Override
    public boolean removeAll(Collection<?> c) {
        boolean modified = false; // Hasn't been modified yet.
        for (Object o : c) { // Iterate through each element in the collection
            if (remove(o)) { // Attempt to remove the element from the skip list.
                modified = true; // Changes were made to list.
            }
        }
        return modified;
    }


    @Override
    public void clear() {
    	// Resets the skip list to its initial empty state
        head = new SkipListSetItem(null, 1); // Create a new dummy node with height of 1.
        maxLevel = 1; // Reset the maximum level to 1.
        size = 0; // Set the size of the skip list to 0.
    }

}
