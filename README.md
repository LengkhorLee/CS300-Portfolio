# ABCU Advising Assistance Program

## Project Overview
This project is a C++ console application designed for the Computer Science department at ABCU.  
The program loads course data from a CSV file into a Binary Search Tree (BST), allowing advisors to:
- View an alphanumeric list of all courses in the Computer Science program
- Look up a specific course and see its title and prerequisites
- Exit when finished

The BST was chosen for its efficient search, insert, and in-order traversal capabilities, which makes it a good fit for keeping course data sorted without extra sorting steps.

---

## Runtime and Memory Analysis

### Binary Search Tree (BST)
- **Insert**: Average O(log n), Worst O(n) if unbalanced  
- **Search**: Average O(log n), Worst O(n)  
- **In-order Traversal**: O(n)  
- **Memory**: Each node stores a course object and pointers to left/right children.  
  This makes it more memory-intensive than a simple vector, but the tradeoff is faster search in average cases.

### Why BST Was Chosen
The BST keeps the course list in sorted order automatically, so printing the list is just an in-order traversal.  
Compared to a vector, the BST avoids having to re-sort data after each insertion.  
Compared to a hash table, it can easily return a sorted course list without extra processing.

---

## Reflection

**1. What was the problem you were solving in the projects for this course?**  
I needed to create a program that could store and organize course data so academic advisors could quickly look up and display course information, including prerequisites.  

**2. How did you approach the problem?**  
I first compared multiple data structures and decided on a Binary Search Tree.  
BSTs naturally keep data in sorted order and allow for quick lookups, which matched the program’s needs.  

**3. How did you overcome any roadblocks you encountered while going through the activities or project?**  
The main challenge was handling file input correctly and making sure empty or invalid lines in the CSV didn’t break the program.  
I solved this by trimming whitespace, checking for empty fields, and validating before inserting data into the BST.  

**4. How has your work on this project expanded your approach to designing software and developing programs?**  
It reinforced the importance of picking the right data structure early in the design process.  
Choosing the right structure can make the code simpler, more efficient, and easier to maintain.  

**5. How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?**  
I’ve started breaking code into smaller, well-named functions and adding inline comments so the program is easier to follow.  
I also tried to write the code so changes (like switching to a hash table) wouldn’t require rewriting the entire program.

---

## How to Run the Program
1. Compile `ProjectTwo.cpp` in any C++11+ environment.
2. Place the CSV file in the same folder as the executable.
3. Run the program and follow the menu prompts.

---

## Example Menu Output
===== ABCU Advising Assistance =====

Load Data Structure

Print Course List

Print Course Information

Exit
Select an option:
