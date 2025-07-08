# Skip List Set Implementation (Java)

This project implements a **Skip List** in Java as a custom set data structure, offering efficient search, insert, and delete operations with average-case logarithmic time complexity.

##  Features

- Fully functional skip list implementation (`SkipListSet`)
- Generic type support (`<T extends Comparable<T>>`)
- Key operations:
  - `add(T item)`
  - `remove(T item)`
  - `contains(T item)`
  - `clear()`
  - `size()`
- Probabilistic height balancing for fast performance
- Dynamic layer management for scalable storage

##  File Structure

skip-list-set/
└── SkipListSet.java // Core skip list implementation


##  How Skip Lists Work

Skip lists are layered linked lists with express lanes. Each element may appear in multiple levels, allowing fast jumps across the structure for efficient search. Insertion and deletion use randomized height selection to maintain balance.

##  Key Concepts

- **Probabilistic balancing** using coin flips
- **Logarithmic average-case** complexity for operations
- **No rebalancing required** (unlike AVL or Red-Black trees)
- **Space-efficient** compared to other balanced trees

##  Usage Example

```java
SkipListSet<Integer> set = new SkipListSet<>();
set.add(10);
set.add(20);
set.contains(10);  // true
set.remove(10);
set.contains(10);  // false

 Learning Outcome

This project demonstrates:

- Data structure design with generics

- Probabilistic algorithms

- Layered node management

- Custom collection class creation
