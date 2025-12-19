#include <iostream>
using namespace std;

// Define the list_element class (node of the linked list)
class list_element {
public:
    int d; // Data stored in the node
    list_element* next; // Pointer to the next element

    // Constructor for list_element
    list_element(int val, list_element* n = nullptr) : d(val), next(n) {}
};

// Define the list class
class list {
public:
    list() : head(0), cursor(0) {} // Constructor initializes head and cursor to null

    // Deep copy constructor
    list(const list& lst) {
        // Check if the source list is empty
        if (lst.head == 0) {
            head = 0;   // Set head to null
            cursor = 0; // Set cursor to null
        }
        else {
            // Set up the new list by copying elements from the source list
            cursor = lst.head; // Start at the head of the source list

            // Create a new node for the head of the copied list
            list_element* h = new list_element(cursor->d);
            list_element* previous; // Pointer to keep track of the previous node
            head = h; // Set the head of the new list
            previous = h; // Update the previous pointer

            // Traverse the source list and copy its elements
            for (cursor = lst.head->next; cursor != 0; cursor = cursor->next) {
                h = new list_element(cursor->d); // Create a new node for the current element
                previous->next = h; // Link the previous node to the current node
                previous = h; // Move the previous pointer forward
            }
            previous->next = 0; // Set the next pointer of the last node to null
            cursor = head; // Set the cursor back to the head of the copied list
        }
    }

    // Destructor to free dynamically allocated memory
    ~list() {
        for (cursor = head; cursor != 0;) {
            cursor = head->next; // Move cursor to the next element
            delete head;         // Delete the current element
            head = cursor;       // Update the head to the next element
        }
    }

    void prepend(int n); // Insert value n at the front
    int get_element() { return cursor->d; } // Get the data of the current element
    void advance() { cursor = cursor->next; } // Move the cursor to the next element
    void print(); // Print all elements in the list
    void reset_cursor() { cursor = head; } // Reset cursor to the head of the list

private:
    list_element* head; // Pointer to the first element
    list_element* cursor; // Pointer for traversal
};

// Implementation of prepend method
void list::prepend(int n) {
    if (head == 0) // Case: Empty list
        cursor = head = new list_element(n, head);
    else // Case: Non-empty list
        head = new list_element(n, head);
}

// Implementation of print method
void list::print() {
    list_element* h = head;
    while (h != 0) { // Traverse and print all elements
        cout << h->d << ", ";
        h = h->next;
    }
    cout << "###" << endl; // End of the list marker
}

// Main function to test the list class
int main() {
    list myList;

    // Test prepend method
    myList.prepend(10);
    myList.prepend(20);
    myList.prepend(30);

    // Print the list after prepending elements
    cout << "List after prepending elements: ";
    myList.print();

    // Reset cursor to head before traversing
    myList.reset_cursor();

    // Test get_element and advance methods
    cout << "First element of the list: " << myList.get_element() << endl;

    list a, b;

    // Testing list a
    a.prepend(9);
    a.prepend(8);
    cout << "list a " << endl;
    a.print();

    // Testing list b
    for (int i = 0; i < 40; ++i) {
        b.prepend(i * i);
    }
    cout << "list b " << endl;
    b.print();

    // Test deep copy constructor
    list c = a; // Copy list a into list c
    cout << "list c (copy of list a): " << endl;
    c.print();

    return 0;
}
