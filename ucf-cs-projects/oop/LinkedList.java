/* 

Destiny Ellenwood
COP3330 Assignment 3
UCFID: 5276398

LinkedList:
This class represents a data structure implementation of a singly linked list, where each element points to the next element. 
It includes a private 'head' attribute where it serves as the reference to the first node in the list.
The constructor of the class initializes the 'head' to null. The 'add' method allows the addition of new nodes to the end of the list.
If this list is initially empty, the new node becomes the head. Otherwise, the method traverses the list to find the last node and attaches the new node to it.
The 'delete' method assists with the removal of a specified node. If the node that's to be deleted is the head, the head is updated to the next node. 
If the node is not the head, the method updated the previous node to point to the node after the current one.
The 'getHead' method is a getter that provides access to the head of the list. The 'setHead' method allows for the head to be set to a specified node.
This class does not include methods for altering the content of nodes once they are created. 

*/



package assignment3;

public class LinkedList {
	
	private Node head; // Points to the first node in the list.
	
	public LinkedList() { // Constructor to initialize an empty list with "head" set to null.
		this.head = null;
		
	}
	
	public void add(int item) { // Method that adds a new node to the end of the list. If the list is empty, it sets the new node as the head.
		Node newNode = new Node(item);
		if (head == null) {
			head = newNode;
		} else {
			Node current = head;
			while (current.getNext() != null) {
				current = current.getNext();
			}
			current.setNext(newNode);
		}
	}
	
	public void delete(Node previous, Node current) { // Method that deletes the specified node "current". If "current" is the head, it updates the head to the next node.
		if (previous == null) {
			head = current.getNext();
		} else {
			previous.setNext(current.getNext());
		}
	}
	
	public Node getHead() { // Getter method that returns the head node of the list.
		return head;
	}
	
	public void setHead(Node head) {
		this.head = head;
	}

}
