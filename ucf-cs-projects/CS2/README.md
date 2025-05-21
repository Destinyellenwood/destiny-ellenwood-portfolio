# 2-4 Tree Implementation and Performance Comparison

This project implements a **2-4 Tree (also known as a 2-3-4 tree)** from scratch in Java, with support for insertion, deletion, search, and in-order traversal. It also includes benchmarking to compare the custom 2-4 tree’s performance against Java’s built-in `TreeSet`.

## 🔍 Features

- Full implementation of a self-balancing 2-4 tree structure
- Node splitting, merging, and rebalancing logic
- Insertion, search (`hasValue`), and deletion operations
- Tree validation and height calculation
- Visual in-order printing with indentation
- Performance testing against `TreeSet` on:
  - Add operations
  - Find operations
  - Delete operations

## 📂 Project Structure

├── App.java // Driver class that benchmarks the 2-4 Tree vs TreeSet
├── TwoFourTree.java // Full 2-4 Tree implementation


## 📈 Benchmarking

The `App.java` class runs timed experiments for adding, finding, and deleting values from:
- A custom `TwoFourTree`
- Java's `TreeSet<Integer>`

It executes tests with varying dataset sizes:
- 100
- 1,000
- 10,000
- 100,000
- 1,000,000
- 10,000,000

Each test:
1. Generates a list of random integers
2. Inserts them into both structures
3. Performs search and deletion operations
4. Compares timing results

## 🛠 Key Concepts

- **2-4 Tree Nodes**: Each node may contain 1–3 values and 2–4 children.
- **Balancing**: Maintains tree balance via node splitting and merging.
- **Efficiency**: Provides log(n) complexity for insert, search, and delete.
- **Benchmarking**: Shows real-world performance vs. built-in Java collections.

## 🚀 How to Run

Compile and run using a Java IDE or the terminal:

```bash
javac App.java TwoFourTree.java
java App

🧠 Learning Outcome
This project demonstrates:

Understanding and implementation of balanced multi-way search trees

Object-oriented design using nested classes

Recursion, pointer logic, and tree traversal

Time benchmarking and performance analysis