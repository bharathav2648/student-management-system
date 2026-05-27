#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cctype>

// Helper function to convert strings to lowercase for case-insensitive search
std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

// --- Book Class ---
class Book {
public:
    std::string book_id;
    std::string title;
    std::string author;
    bool is_issued;

    Book(std::string id, std::string t, std::string a) 
        : book_id(id), title(t), author(a), is_issued(false) {}

    void display() const {
        std::string status = is_issued ? "Borrowed" : "Available";
        std::cout << "[" << book_id << "] '" << title << "' by " 
                  << author << " - " << status << "\n";
    }
};

// --- Member Class ---
class Member {
public:
    std::string member_id;
    std::string name;
    // Using weak pointers or shared pointers. Shared makes sure the book 
    // remains valid while borrowed, even if removed from the main library.
    std::vector<std::shared_ptr<Book>> borrowed_books;

    Member(std::string id, std::string n) : member_id(id), name(n) {}

    void display() const {
        std::cout << "Member: " << name << " (ID: " << member_id 
                  << ") | Books Borrowed: " << borrowed_books.size() << "\n";
    }
};

// --- Library Class ---
class Library {
private:
    std::unordered_map<std::string, std::shared_ptr<Book>> books;
    std::unordered_map<std::string, std::shared_ptr<Member>> members;

public:
    // Core Features
    void addBook(const std::string& book_id, const std::string& title, const std::string& author) {
        if (books.find(book_id) != books.end()) {
            std::cout << "Error: Book with ID " << book_id << " already exists.\n";
            return;
        }
        books[book_id] = std::make_shared<Book>(book_id, title, author);
        std::cout << "Added: " << title << "\n";
    }

    void addMember(const std::string& member_id, const std::string& name) {
        if (members.find(member_id) != members.end()) {
            std::cout << "Error: Member with ID " << member_id << " already exists.\n";
            return;
        }
        members[member_id] = std::make_shared<Member>(member_id, name);
        std::cout << "Added Member: " << name << "\n";
    }

    void issueBook(const std::string& book_id, const std::string& member_id) {
        auto book_it = books.find(book_id);
        auto member_it = members.find(member_id);

        if (book_it == books.end()) {
            std::cout << "Issue Failed: Book not found.\n";
            return;
        }
        if (member_it == members.end()) {
            std::cout << "Issue Failed: Member not found.\n";
            return;
        }

        std::shared_ptr<Book> book = book_it->second;
        std::shared_ptr<Member> member = member_it->second;

        if (book->is_issued) {
            std::cout << "Issue Failed: '" << book->title << "' is already borrowed by someone else.\n";
            return;
        }

        // Process the issue
        book->is_issued = true;
        member->borrowed_books.push_back(book);
        std::cout << "Success: '" << book->title << "' has been issued to " << member->name << ".\n";
    }

    void returnBook(const std::string& book_id, const std::string& member_id) {
        auto book_it = books.find(book_id);
        auto member_it = members.find(member_id);

        if (book_it == books.end() || member_it == members.end()) {
            std::cout << "Return Failed: Invalid Book ID or Member ID.\n";
            return;
        }

        std::shared_ptr<Book> book = book_it->second;
        std::shared_ptr<Member> member = member_it->second;

        // Find the book in the member's borrowed list
        auto it = std::find(member->borrowed_books.begin(), member->borrowed_books.end(), book);
        if (it == member->borrowed_books.end()) {
            std::cout << "Return Failed: " << member->name << " does not have '" << book->title << "' borrowed.\n";
            return;
        }

        // Process the return
        book->is_issued = false;
        member->borrowed_books.erase(it);
        std::cout << "Success: '" << book->title << "' has been returned by " << member->name << ".\n";
    }

    void search(const std::string& query) {
        std::cout << "\n--- Search Results for '" << query << "' ---\n";
        std::string lowerQuery = toLowerCase(query);
        bool found = false;

        for (const auto& pair : books) {
            std::shared_ptr<Book> book = pair.second;
            std::string lowerTitle = toLowerCase(book->title);
            std::string lowerAuthor = toLowerCase(book->author);

            // Check if query is a substring of title or author
            if (lowerTitle.find(lowerQuery) != std::string::npos || 
                lowerAuthor.find(lowerQuery) != std::string::npos) {
                book->display();
                found = true;
            }
        }

        if (!found) {
            std::cout << "No books found matching your query.\n";
        }
        std::cout << "-----------------------------------\n";
    }
};

// ==========================================
// Example Usage & Testing the Application
// ==========================================
int main() {
    // 1. Initialize the system
    Library myLibrary;

    // 2. Add Books
    std::cout << "--- Adding Books ---\n";
    myLibrary.addBook("B01", "The Great Gatsby", "F. Scott Fitzgerald");
    myLibrary.addBook("B02", "1984", "George Orwell");
    myLibrary.addBook("B03", "Animal Farm", "George Orwell");

    // 3. Add Members
    std::cout << "\n--- Adding Members ---\n";
    myLibrary.addMember("M01", "Alice Smith");
    myLibrary.addMember("M02", "Bob Jones");

    // 4. Search Functionality (By Author)
    myLibrary.search("George Orwell");

    // 5. Issue Books
    std::cout << "\n--- Issuing Books ---\n";
    myLibrary.issueBook("B02", "M01");  // Alice borrows 1984
    myLibrary.issueBook("B02", "M02");  // Bob tries to borrow 1984 (Should fail)

    // 6. Search Functionality (By Title to see updated status)
    myLibrary.search("1984");

    // 7. Return Books
    std::cout << "\n--- Returning Books ---\n";
    myLibrary.returnBook("B02", "M01"); // Alice returns 1984
    myLibrary.issueBook("B02", "M02");  // Bob tries to borrow 1984 again (Should succeed)

    return 0;
}