#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[100];
    char authors[200];   // Supports multiple authors
    char edition[30];
    int isIssued; // 0 = Available, 1 = Issued
};

void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();
void removeBook();   // NEW FUNCTION

int main() {
    int choice;

    while (1) {
        printf("\n====== Library Management System ======\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Remove Book\n"); // NEW MENU OPTION
        printf("7. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // remove leftover newline

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: removeBook(); break; // NEW CASE
            case 7: exit(0);
            default: printf("Invalid option! Try again.\n");
        }
    }
    return 0;
}

// ------------------ ADD BOOK ------------------
void addBook() {
    struct Book b;
    FILE *fp = fopen("library.dat", "ab");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar();

    printf("Enter Book Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0;

    printf("Enter Author(s) (comma-separated): ");
    fgets(b.authors, sizeof(b.authors), stdin);
    b.authors[strcspn(b.authors, "\n")] = 0;

    printf("Enter Book Edition: ");
    fgets(b.edition, sizeof(b.edition), stdin);
    b.edition[strcspn(b.edition, "\n")] = 0;

    b.isIssued = 0;

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book added successfully!\n");
}

// ------------------ DISPLAY BOOKS ------------------
void displayBooks() {
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");

    if (fp == NULL) {
        printf("\nNo books found. The library is empty.\n");
        return;
    }

    printf("\n--- Books in Library ---\n");

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        printf("ID: %d\nTitle: %s\nAuthors: %s\nEdition: %s\nStatus: %s\n\n",
               b.id, b.title, b.authors, b.edition,
               b.isIssued ? "Issued" : "Available");
    }

    fclose(fp);
}

// ------------------ SEARCH BOOK BY ID ------------------
void searchBook() {
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");

    if (fp == NULL) {
        printf("Library is empty. No books to search.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            printf("\n--- Book Found ---\n");
            printf("ID: %d\nTitle: %s\nAuthors: %s\nEdition: %s\nStatus: %s\n",
                b.id, b.title, b.authors, b.edition,
                b.isIssued ? "Issued" : "Available");
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) printf("No book found with this ID.\n");
}

// ------------------ ISSUE BOOK ------------------
void issueBook() {
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");

    if (fp == NULL) {
        printf("Library is empty. No books to issue.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    int id, found = 0;

    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            found = 1;
            if (!b.isIssued) {
                b.isIssued = 1;
                printf("Book issued successfully!\n");
            } else {
                printf("Book is already issued!\n");
            }
        }
        fwrite(&b, sizeof(b), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (!found) printf("Book not found!\n");
}

// ------------------ RETURN BOOK ------------------
void returnBook() {
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");

    if (fp == NULL) {
        printf("Library is empty. No books to return.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    int id, found = 0;

    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            found = 1;
            if (b.isIssued) {
                b.isIssued = 0;
                printf("Book returned successfully!\n");
            } else {
                printf("Book is not issued!\n");
            }
        }
        fwrite(&b, sizeof(b), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (!found) printf("Book not found!\n");
}

// ------------------ REMOVE BOOK (NEW) ------------------
void removeBook() {
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");

    if (fp == NULL) {
        printf("Library is empty. Nothing to remove.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    int id, found = 0;

    printf("Enter Book ID to remove: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            found = 1;
            printf("Book '%s' removed successfully.\n", b.title);
            continue;   // skip writing this book to new file
        }
        fwrite(&b, sizeof(b), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (!found) printf("Book not found.\n");
}

