// ProjectTwo.cpp
// ABCU Advising Assistance Program (CS 300 Project Two)
// Single-file C++ console application using a Binary Search Tree (BST)
//
// Menu:
// 1) Load Data Structure
// 2) Print Course List (alphanumeric by course number)
// 3) Print Course Information (title + prerequisites w/ titles if available)
// 9) Exit
//
// Notes:
// - Input file format: CSV per line => CourseNumber,CourseTitle[,Prereq1,Prereq2,...]
// - Example: "CS200,Data Structures,CS100,MATH101"
// - No external headers; standard library only.
// - Robust input handling and inline comments included.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// ----------------------------- Utilities -----------------------------

static inline string ltrim(string s) {
    size_t i = 0;
    while (i < s.size() && isspace(static_cast<unsigned char>(s[i]))) ++i;
    return s.substr(i);
}
static inline string rtrim(string s) {
    if (s.empty()) return s;
    size_t i = s.size() - 1;
    while (i != string::npos && isspace(static_cast<unsigned char>(s[i]))) {
        if (i == 0) { return ""; }
        --i;
    }
    return s.substr(0, i + 1);
}
static inline string trim(string s) { return rtrim(ltrim(std::move(s))); }

static inline void to_upper_inplace(string& s) {
    for (char& c : s) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
}

// ----------------------------- Data Model -----------------------------

struct Course {
    string courseNumber;             // key (e.g., "CS200")
    string courseTitle;              // e.g., "Data Structures"
    vector<string> prerequisites;    // e.g., {"CS100", "MATH101"}
};

// ----------------------------- BST (by courseNumber) -----------------------------

struct Node {
    Course data;
    Node* left;
    Node* right;
    explicit Node(const Course& c) : data(c), left(nullptr), right(nullptr) {}
};

class CourseBST {
public:
    CourseBST() : root(nullptr), count_(0) {}
    ~CourseBST() { destroy(root); }

    // Insert or update course (by courseNumber)
    void Insert(const Course& c) {
        bool inserted = false;
        root = insertRec(root, c, inserted);
        if (inserted) ++count_;
    }

    // Find course by key; returns nullptr if not found
    const Course* Find(const string& courseNumber) const {
        Node* cur = root;
        string key = courseNumber;
        // ensure key format matches tree ordering
        // (caller generally uppercases first, but do it again defensively)
        string k = key;
        to_upper_inplace(k);
        while (cur) {
            if (k == cur->data.courseNumber) return &cur->data;
            if (k < cur->data.courseNumber) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

    // In-order traversal (ascending alphanumeric by courseNumber)
    template <typename Func>
    void InOrder(Func f) const {
        inOrderRec(root, f);
    }

    // Clear all data
    void Clear() {
        destroy(root);
        root = nullptr;
        count_ = 0;
    }

    // Count nodes
    size_t Size() const { return count_; }

private:
    Node* root;
    size_t count_;

    static void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    static Node* insertRec(Node* n, const Course& c, bool& inserted) {
        if (!n) {
            inserted = true;
            return new Node(c);
        }
        if (c.courseNumber == n->data.courseNumber) {
            // Update existing (title/prereqs) to handle duplicates in file
            n->data.courseTitle = c.courseTitle;
            n->data.prerequisites = c.prerequisites;
            inserted = false;
            return n;
        }
        else if (c.courseNumber < n->data.courseNumber) {
            n->left = insertRec(n->left, c, inserted);
        }
        else {
            n->right = insertRec(n->right, c, inserted);
        }
        return n;
    }

    template <typename Func>
    static void inOrderRec(Node* n, Func f) {
        if (!n) return;
        inOrderRec(n->left, f);
        f(n->data);
        inOrderRec(n->right, f);
        return;
    }
};

// ----------------------------- Parsing -----------------------------

// Parse one CSV line into a Course { number, title, [prereqs...] }.
// Returns true if a valid course (must have number & title).
static bool parseCourseLine(const string& line, Course& out) {
    // Split by commas (simple CSV; titles are assumed not to contain commas in the provided dataset)
    stringstream ss(line);
    string number, title;

    if (!getline(ss, number, ',')) return false;
    if (!getline(ss, title, ',')) return false;

    number = trim(number);
    title = trim(title);
    if (number.empty() || title.empty()) return false;

    // Standardize course number to uppercase to ensure consistent keys
    to_upper_inplace(number);

    vector<string> prereqs;
    string prereq;
    while (getline(ss, prereq, ',')) {
        prereq = trim(prereq);
        if (!prereq.empty()) {
            to_upper_inplace(prereq);
            prereqs.push_back(prereq);
        }
    }

    out.courseNumber = number;
    out.courseTitle = title;
    out.prerequisites = std::move(prereqs);
    return true;
}

// ----------------------------- Program Actions -----------------------------

// Load CSV into BST; returns number of courses loaded; clears existing data first.
static size_t loadDataFileIntoBST(const string& filename, CourseBST& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file \"" << filename << "\".\n";
        return 0;
    }

    bst.Clear();

    string line;
    size_t lineNum = 0;
    size_t loaded = 0;

    while (getline(file, line)) {
        ++lineNum;
        // Skip empty or whitespace-only lines
        string trimmed = trim(line);
        if (trimmed.empty()) continue;

        Course c;
        if (parseCourseLine(trimmed, c)) {
            bst.Insert(c);
            ++loaded;
        }
        else {
            cout << "Warning: Invalid line " << lineNum << " skipped.\n";
        }
    }

    if (loaded == 0) {
        cout << "Warning: No valid courses found in file.\n";
    }
    return loaded;
}

// Prints all courses as "COURSE#: Title" in alphanumeric order by course number
static void printCourseList(const CourseBST& bst) {
    if (bst.Size() == 0) {
        cout << "No data loaded. Please choose option 1 to load a data file first.\n";
        return;
    }
    cout << "\nCourse List (alphanumeric):\n";
    cout << "----------------------------------------\n";
    bst.InOrder([](const Course& c) {
        cout << c.courseNumber << ": " << c.courseTitle << "\n";
        });
    cout << "----------------------------------------\n";
}

// Prints information for a single course, with prerequisites (numbers and titles if available)
static void printCourseInfo(const CourseBST& bst) {
    if (bst.Size() == 0) {
        cout << "No data loaded. Please choose option 1 to load a data file first.\n";
        return;
    }

    cout << "Enter course number: ";
    string key;
    cin >> key;
    key = trim(key);
    to_upper_inplace(key);

    const Course* c = bst.Find(key);
    if (!c) {
        cout << "Course \"" << key << "\" not found.\n";
        return;
    }

    cout << "\n" << c->courseNumber << ", " << c->courseTitle << "\n";

    if (c->prerequisites.empty()) {
        cout << "Prerequisites: None\n";
        return;
    }

    cout << "Prerequisites: ";
    // Print as "CS100 (Intro to ...), MATH101 (College Algebra)" where possible
    for (size_t i = 0; i < c->prerequisites.size(); ++i) {
        const string& pnum = c->prerequisites[i];
        const Course* pc = bst.Find(pnum);
        cout << pnum;
        if (pc) {
            cout << " (" << pc->courseTitle << ")";
        }
        else {
            cout << " (title not found)";
        }
        if (i + 1 < c->prerequisites.size()) cout << ", ";
    }
    cout << "\n";
}

// ----------------------------- Menu Loop -----------------------------

static void printMenu() {
    cout << "\n===== ABCU Advising Assistance =====\n";
    cout << "  1. Load Data Structure\n";
    cout << "  2. Print Course List\n";
    cout << "  3. Print Course Information\n";
    cout << "  9. Exit\n";
    cout << "Select an option: ";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    CourseBST bst;
    bool running = true;
    bool dataLoaded = false;

    while (running) {
        printMenu();

        // Validate numeric input
        int choice = 0;
        if (!(cin >> choice)) {
            // Clear bad input and discard line
            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "Invalid input. Please enter a number from the menu.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            cout << "Please enter the file name: ";
            string filename;
            cin >> filename;
            size_t loaded = loadDataFileIntoBST(filename, bst);
            if (loaded > 0) {
                cout << "Loaded " << loaded << " course(s) from \"" << filename << "\".\n";
                dataLoaded = true;
            }
            else {
                dataLoaded = false;
            }
            break;
        }
        case 2:
            if (!dataLoaded) {
                cout << "No data loaded. Please choose option 1 to load a data file first.\n";
            }
            else {
                printCourseList(bst);
            }
            break;

        case 3:
            if (!dataLoaded) {
                cout << "No data loaded. Please choose option 1 to load a data file first.\n";
            }
            else {
                printCourseInfo(bst);
            }
            break;

        case 9:
            cout << "Goodbye!\n";
            running = false;
            break;

        default:
            cout << "Invalid option. Please choose 1, 2, 3, or 9.\n";
            break;
        }
    }

    return 0;
}
