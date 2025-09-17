/* 

Destiny Ellenwood
COP3330 Assignment 3
UCFID: 5276398

LinkedListTester:
This class processes a linked list of integers. It begins by initializing a 'LinkedList' object and a count variable starting at -100. 
The class reads integers from a file 'data.txt', adding only positive integers to the linked list using a BufferedReader. 
After making the list, the processList method traverses the linked list to modify it based on the specific conditions. 
For every pair of adjacent nodes where the second node's value equals twice the first node's value plus 7, 
both nodes are deleted and replaced with a new node containing the current count value, which is then incremented with each insertion. 

*/
package assignment3;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;


public class LinkedListTester {

    public static void main(String[] args) { // Main method that initializes the linked list and count variable.
        LinkedList list = new LinkedList(); // Create a new LinkedList instance
        int count = -100; // Initialize the count variable to -100

        // Pre-condition: 'data.txt' exists and contains integers separated by spaces.
        // Post-condition: Positive integers from the file are added to the linked list.
        try (BufferedReader reader = new BufferedReader(new FileReader("data.txt"))) { // Reads integers from "data.txt" and adds positive integers to the linked list.
            String line; // Variable to hold each line read from the file
            while ((line = reader.readLine()) != null) { // Reach each line until the end of the file
                String[] numbers = line.split(" "); // Split the line into individual numbers
                for (String number : numbers) { // Iterate over each number
                    int num = Integer.parseInt(number); // Parse the number as an integer
                    if (num > 0) { // If the number is positive
                        list.add(num); // Add the number to the linked list
                    }
                }
            }
        } catch (IOException e) { // Handle possible IOExceptions
            e.printStackTrace(); // Print the stack trace for debugging
        }

        processList(list, count); // Calls "processList" method to traverse and modify the list.

        // Write to 'processed.txt'
        try (FileWriter writer = new FileWriter("processed.txt")) { // Create a FileWriter to write to 'processed.txt'
            Node current = list.getHead(); // Start from the head of the linked list
            while (current != null) { // Traverse the linked list
                writer.write(current.getItem()+ " "); // Write the current node's item to the file and add a space if it's not the last item
                current = current.getNext(); // Move to the next node
            }
        } catch (IOException e) { // Handles the case where the file cannot be created 
            e.printStackTrace(); // Print the stack trace for debugging
        }
    }

    // Pre-condition: The linked list is not null and properly initialized.
    // Post-condition: The linked list is modified based on the specified condition, and a final count node is added at the end.
    private static void processList(LinkedList list, int count) {
        Node previous = null; // Initialize the previous node as null
        Node current = list.getHead(); // Start from the head of the linked list

        while (current != null && current.getNext() != null) { // Traverse the linked list
            Node next = current.getNext(); // Get the next node
            if (next.getItem() == current.getItem() * 2 + 7) { // Check the specified condition
                list.delete(previous, current); // Delete the current node
                list.delete(previous, next); // Delete the next node

                Node newNode = new Node(count); // Create a new node with the count value as the item
                if (previous == null) { // If inserting at the head of the list
                    newNode.setNext(list.getHead()); // Set the new node's next to the current head
                    list.setHead(newNode); // Update the head of the list
                } else {
                    newNode.setNext(next.getNext()); // Set the new node's next to the node after next
                    previous.setNext(newNode); // Update the previous node's next to the new node
                }
                count++; // Increases the count for the next node
                current = newNode.getNext(); // Continue processing from the new node after the newly inserted node
            } else {
                previous = current; // Move the previous pointer to the current node
                current = next; // Move the current pointer to the next node
            }
        }

        list.add(count); // Add the final count node at the end of the list
    }
}

