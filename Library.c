#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100
#define MAX_BOOKS 100
#define FILENAME "library.txt"

// Structure for storing book details
typedef struct {
    int book_id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int stock;  // Number of available copies
} Book;

Book books[MAX_BOOKS];
int bookCount = 0;

// Function declarations
void loadBooksFromFile();
void saveBooksToFile();
void addBook();
void editBook();
void deleteBook();
void searchBook();
void displayBooks();
void borrowBook();
void returnBook();
void login();
int isAdmin = 0;  // Role-based access: 1 = Admin, 0 = User

// Main function
int main() {
    int choice;

    // Load existing books from file
    loadBooksFromFile();

    // Login
    login();

    // Menu loop
    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Edit Book\n");
        printf("3. Delete Book\n");
        printf("4. Search Book\n");
        printf("5. Display All Books\n");
        printf("6. Borrow Book\n");
        printf("7. Return Book\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume newline left in the buffer

        switch (choice) {
            case 1:
                if (isAdmin) addBook();
                else printf("Access denied! Only admins can add books.\n");
                break;
            case 2:
                if (isAdmin) editBook();
                else printf("Access denied! Only admins can edit books.\n");
                break;
            case 3:
                if (isAdmin) deleteBook();
                else printf("Access denied! Only admins can delete books.\n");
                break;
            case 4:
                searchBook();
                break;
            case 5:
                displayBooks();
                break;
            case 6:
                borrowBook();
                break;
            case 7:
                returnBook();
                break;
            case 8:
                saveBooksToFile();
                printf("Exiting... Data saved.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// Function to load books from file
void loadBooksFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No previous data found. Starting with an empty library.\n");
        return;
    }

    while (fscanf(file, "%d\n", &books[bookCount].book_id) != EOF) {
        fgets(books[bookCount].title, MAX_TITLE_LENGTH, file);
        books[bookCount].title[strcspn(books[bookCount].title, "\n")] = '\0';
        fgets(books[bookCount].author, MAX_AUTHOR_LENGTH, file);
        books[bookCount].author[strcspn(books[bookCount].author, "\n")] = '\0';
        fscanf(file, "%d\n", &books[bookCount].stock);
        bookCount++;
    }

    fclose(file);
}

// Function to save books to file
void saveBooksToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error saving data.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "%d\n", books[i].book_id);
        fprintf(file, "%s\n", books[i].title);
        fprintf(file, "%s\n", books[i].author);
        fprintf(file, "%d\n", books[i].stock);
    }

    fclose(file);
}

// Function to add a book
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full. Cannot add more books.\n");
        return;
    }

    Book newBook;
    printf("Enter Book ID: ");
    scanf("%d", &newBook.book_id);
    getchar();

    // Check for duplicate ID
    for (int i = 0; i < bookCount; i++) {
        if (books[i].book_id == newBook.book_id) {
            printf("Book ID already exists. Try again.\n");
            return;
        }
    }

    printf("Enter Book Title: ");
    fgets(newBook.title, MAX_TITLE_LENGTH, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0';

    printf("Enter Book Author: ");
    fgets(newBook.author, MAX_AUTHOR_LENGTH, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';

    printf("Enter Stock (Number of Copies): ");
    scanf("%d", &newBook.stock);

    books[bookCount++] = newBook;
    printf("Book added successfully!\n");
}

// Function to edit a book
void editBook() {
    int book_id, found = 0;
    printf("Enter Book ID to edit: ");
    scanf("%d", &book_id);
    getchar();

    for (int i = 0; i < bookCount; i++) {
        if (books[i].book_id == book_id) {
            printf("Editing Book ID %d\n", book_id);
            printf("Enter New Title: ");
            fgets(books[i].title, MAX_TITLE_LENGTH, stdin);
            books[i].title[strcspn(books[i].title, "\n")] = '\0';

            printf("Enter New Author: ");
            fgets(books[i].author, MAX_AUTHOR_LENGTH, stdin);
            books[i].author[strcspn(books[i].author, "\n")] = '\0';

            printf("Enter New Stock: ");
            scanf("%d", &books[i].stock);

            printf("Book updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) printf("Book with ID %d not found.\n", book_id);
}

// Function to delete a book
void deleteBook() {
    int book_id, found = 0;
    printf("Enter Book ID to delete: ");
    scanf("%d", &book_id);

    for (int i = 0; i < bookCount; i++) {
        if (books[i].book_id == book_id) {
            for (int j = i; j < bookCount - 1; j++) {
                books[j] = books[j + 1];
            }
            bookCount--;
            printf("Book with ID %d deleted successfully.\n", book_id);
            found = 1;
            break;
        }
    }

    if (!found) printf("Book with ID %d not found.\n", book_id);
}

// Function to search for a book
void searchBook() {
    char searchTerm[MAX_TITLE_LENGTH];
    int found = 0;

    printf("Enter Book Title or Author to search: ");
    fgets(searchTerm, MAX_TITLE_LENGTH, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';

    printf("\nSearch Results:\n");
    for (int i = 0; i < bookCount; i++) {
        if (strstr(books[i].title, searchTerm) != NULL || strstr(books[i].author, searchTerm) != NULL) {
            printf("Book ID: %d\n", books[i].book_id);
            printf("Title: %s\n", books[i].title);
            printf("Author: %s\n", books[i].author);
            printf("Stock: %d\n\n", books[i].stock);
            found = 1;
        }
    }

    if (!found) printf("No books found matching the search term.\n");
}

// Function to display all books
void displayBooks() {
    if (bookCount == 0) {
        printf("No books available in the library.\n");
        return;
    }

    printf("\nList of Books:\n");
    for (int i = 0; i < bookCount; i++) {
        printf("Book ID: %d\n", books[i].book_id);
        printf("Title: %s\n", books[i].title);
        printf("Author: %s\n", books[i].author);
        printf("Stock: %d\n\n", books[i].stock);
    }
}

// Function to borrow a book
void borrowBook() {
    int book_id, found = 0;
    printf("Enter Book ID to borrow: ");
    scanf("%d", &book_id);

    for (int i = 0; i < bookCount; i++) {
        if (books[i].book_id == book_id) {
            if (books[i].stock > 0) {
                books[i].stock--;
                printf("Book borrowed successfully!\n");
            } else {
                printf("Book is out of stock.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) printf("Book with ID %d not found.\n", book_id);
}

// Function to return a book
void returnBook() {
    int book_id, found = 0;
    printf("Enter Book ID to return: ");
    scanf("%d", &book_id);

    for (int i = 0; i < bookCount; i++) {
        if (books[i].book_id == book_id) {
            books[i].stock++;
            printf("Book returned successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) printf("Book with ID %d not found.\n", book_id);
}

// Function to handle login
void login() {
    char username[20], password[20];

    printf("Login:\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "pass123") == 0) {
        printf("Admin access granted.\n");
        isAdmin = 1;
    } else {
        printf("User access granted. Limited permissions.\n");
        isAdmin = 0;
    }
}
