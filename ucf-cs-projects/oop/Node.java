/* 

Destiny Ellenwood
COP3330 Assignment 3
UCFID: 5276398

Node:
This class represents each individual element within the list. It contains two attributes, an 'item' and 'next'.
This 'item' attribute which is declared as final stores the integer value of the node and cannot be changed once assigned.
The 'next' attribute is a reference to the next node in the list, representing a link between the nodes.
The constructor 'Node' initializes the node with a specified integer value and sets the 'next' reference to null.
The 'getItem' method is a getter that returns the value stored in the node, allowing others to access to the node's data. 
The 'getNext' method is another getter that provides the reference to the next node in the sequence, allowing to traverse through the list.
The 'setNext' method s a setter that allows the 'next' reference to be updated, linking the current node to another node in the list.


*/



package assignment3;

public class Node {
	
	private final int item; // Stores the value of the node.
	private Node next; // Points to the next node in the list.
	
	public Node(int item) { // Constructor to initialize the node with a value and sets "next" to null.
		
		this.item = item;
		this.next = null;
	}

	
	public int getItem() { // Getter method that returns the value of the node.
		
		return item;
	}
	
	public Node getNext() { // Getter method that returns the reference to the next node.
		
		return next;
	}
	
	public void setNext (Node next) { // Setter method that sets the reference to the next node.
		
		this.next = next;
	}
	
	
}
