#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DISHES 400
#define FILENAME "restaurant.txt"

struct Dish {
    int sno;
    char name[50];
    float price;
    char category[30];
};

struct Dish dishes[MAX_DISHES];
int dishCount = 0;

void displayMenu();
void addDish();
void editDish();
void deleteDish();
void searchDish();
void displayAllDishes();
void calculateBill();
void saveToFile();
void loadFromFile();
void loadDefaultDishes();
int findDishBySno(int sno);
void clearInputBuffer();
void displayCategories();
int main() {
    int choice;
    
    printf("=== MENU OF RESTAURANT ===\n");
    
    // First load default dishes
    loadDefaultDishes();
    printf("300 Indian dishes loaded successfully!\n");
    
    // Then try to load from file
    loadFromFile();
    
    while(1) {
        displayMenu();
        printf("Enter your choice (1-8): ");
        
        if(scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: addDish(); break;
            case 2: editDish(); break;
            case 3: deleteDish(); break;
            case 4: searchDish(); break;
            case 5: displayAllDishes(); break;
            case 6: calculateBill(); break;
            case 7: 
                saveToFile();
                printf("Data saved successfully!\n");
                break;
            case 8: 
                saveToFile();
                printf("Thank-you for visiting DINE OF INDIA!\nVisit again !\n");
                exit(0);
            default: 
                printf("Invalid choice! Please enter 1-8.\n");
        }
        
        printf("\nPress Enter to continue...");
        clearInputBuffer();
    }
    
    return 0;
}


void displayMenu() {
    printf("\n========================================\n");
    printf("          DINE OF INDIA\n");
    printf("========================================\n");
    printf("1. Add Dish\n");
    printf("2. Edit Dish\n");
    printf("3. Delete Dish\n");
    printf("4. Search Dish\n");
    printf("5. Display All Dishes\n");
    printf("6. Calculate Bill\n");
    printf("7. Save Data\n");
    printf("8. Exit\n");
    printf("========================================\n");
}

void displayCategories() {
    printf("\nAvailable Categories:\n");
    printf("1. North Indian\n");
    printf("2. South Indian\n");
    printf("3. West Indian\n");
    printf("4. East Indian\n");
    printf("5. Street Food\n");
    printf("6. Desserts & Beverages\n");
}

// FIXED ADD FUNCTION
void addDish() {
    if(dishCount >= MAX_DISHES) {
        printf("Menu is full! Cannot add more dishes.\n");
        return;
    }
    
    printf("\n--- Add New Dish ---\n");
    
    // Auto-generate S.NO
    int newSno = dishCount + 1;
    dishes[dishCount].sno = newSno;
    printf("Auto-assigned S.NO: %d\n", newSno);
    
    printf("Enter Dish Name: ");
    fgets(dishes[dishCount].name, 50, stdin);
    dishes[dishCount].name[strcspn(dishes[dishCount].name, "\n")] = 0;
    
    printf("Enter Price: Rs. ");
    scanf("%f", &dishes[dishCount].price);
    clearInputBuffer();
    
    // Show categories and let user choose
    displayCategories();
    printf("Select Category (1-6): ");
    int categoryChoice;
    scanf("%d", &categoryChoice);
    clearInputBuffer();
    
    // Assign category based on choice
    switch(categoryChoice) {
        case 1: strcpy(dishes[dishCount].category, "North Indian"); break;
        case 2: strcpy(dishes[dishCount].category, "South Indian"); break;
        case 3: strcpy(dishes[dishCount].category, "West Indian"); break;
        case 4: strcpy(dishes[dishCount].category, "East Indian"); break;
        case 5: strcpy(dishes[dishCount].category, "Street Food"); break;
        case 6: strcpy(dishes[dishCount].category, "Desserts & Beverages"); break;
        default: 
            printf("Invalid choice! Setting to General category.\n");
            strcpy(dishes[dishCount].category, "General");
    }
    
    dishCount++;
    printf("\nDish added successfully!\n");
    printf("S.NO: %d, Name: %s, Price: Rs. %.2f, Category: %s\n", 
           newSno, dishes[dishCount-1].name, dishes[dishCount-1].price, dishes[dishCount-1].category);
    
    saveToFile();
}

void editDish() {
    int sno, index;
    
    printf("\n--- Edit Dish ---\n");
    printf("Enter Dish S.NO to edit: ");
    
    if(scanf("%d", &sno) != 1) {
        printf("Invalid S.NO!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    index = findDishBySno(sno);
    if(index == -1) {
        printf("Error: Dish S.NO %d not found!\n", sno);
        return;
    }
    
    struct Dish *d = &dishes[index];
    
    printf("\nCurrent Details:\n");
    printf("S.NO: %d\n", d->sno);
    printf("Name: %s\n", d->name);
    printf("Price: Rs. %.2f\n", d->price);
    printf("Category: %s\n", d->category);
    
    printf("\nEnter New Details:\n");
    
    // Get new name
    printf("Enter New Name: ");
    char newName[50];
    fgets(newName, 50, stdin);
    newName[strcspn(newName, "\n")] = 0;
    if(strlen(newName) > 0) {
        strcpy(d->name, newName);
    }
    
    // Get new price
    printf("Enter New Price: Rs. ");
    float newPrice;
    if(scanf("%f", &newPrice) == 1) {
        d->price = newPrice;
    }
    clearInputBuffer();
    
    // Get new category
    displayCategories();
    printf("Select New Category (1-6): ");
    int categoryChoice;
    scanf("%d", &categoryChoice);
    clearInputBuffer();
    
    // Assign category based on choice
    switch(categoryChoice) {
        case 1: strcpy(d->category, "North Indian"); break;
        case 2: strcpy(d->category, "South Indian"); break;
        case 3: strcpy(d->category, "West Indian"); break;
        case 4: strcpy(d->category, "East Indian"); break;
        case 5: strcpy(d->category, "Street Food"); break;
        case 6: strcpy(d->category, "Desserts & Beverages"); break;
        default: 
            printf("Invalid choice! Keeping current category.\n");
    }
    
    printf("\nDish updated successfully!\n");
    printf("Updated Details:\n");
    printf("S.NO: %d\n", d->sno);
    printf("Name: %s\n", d->name);
    printf("Price: Rs. %.2f\n", d->price);
    printf("Category: %s\n", d->category);
    
    saveToFile();
}

void deleteDish() {
    int sno, index;
    
    printf("\n--- Delete Dish ---\n");
    printf("Enter Dish S.NO to delete: ");
    scanf("%d", &sno);
    clearInputBuffer();
    
    index = findDishBySno(sno);
    if(index == -1) {
        printf("Error: Dish not found!\n");
        return;
    }
    
    printf("Deleting: %s (S.NO: %d)\n", dishes[index].name, dishes[index].sno);
    
    for(int i = index; i < dishCount - 1; i++) {
        dishes[i] = dishes[i + 1];
    }
    dishCount--;
    
    printf("Dish deleted successfully!\n");
    saveToFile();
}

void searchDish() {
    int choice, found = 0;
    
    printf("\n--- Search Dish ---\n");
    printf("1. Search by S.NO\n");
    printf("2. Search by Name\n");
    printf("3. Search by Category\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    clearInputBuffer();
    
    if(choice == 1) {
        int sno;
        printf("Enter S.NO: ");
        scanf("%d", &sno);
        clearInputBuffer();
        
        int index = findDishBySno(sno);
        if(index != -1) {
            printf("\nDish Found:\n");
            printf("S.NO: %d\n", dishes[index].sno);
            printf("Name: %s\n", dishes[index].name);
            printf("Price: Rs. %.2f\n", dishes[index].price);
            printf("Category: %s\n", dishes[index].category);
        } else {
            printf("Dish not found!\n");
        }
    }
    else if(choice == 2) {
        char name[50];
        printf("Enter Dish Name: ");
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0;
        
        printf("\nSearch Results:\n");
        for(int i = 0; i < dishCount; i++) {
            if(strstr(dishes[i].name, name) != NULL) {
                printf("S.NO: %d\n Name: %s\n Price: Rs. %.2f\n Category: %s\n", 
                       dishes[i].sno, dishes[i].name, dishes[i].price, dishes[i].category);
                found = 1;
            }
        }
        if(!found) printf("No dishes found!\n");
    }
    else if(choice == 3) {
        char category[30];
        printf("Enter Category: ");
        fgets(category, 30, stdin);
        category[strcspn(category, "\n")] = 0;
        
        printf("\nSearch Results:\n");
        for(int i = 0; i < dishCount; i++) {
            if(strstr(dishes[i].category, category) != NULL) {
                printf("S.NO: %d, Name: %s, Price: Rs. %.2f, Category: %s\n", 
                       dishes[i].sno, dishes[i].name, dishes[i].price, dishes[i].category);
                found = 1;
            }
        }
        if(!found) printf("No dishes found!\n");
    }
    else {
        printf("Invalid choice!\n");
    }
}

void displayAllDishes() {
    if(dishCount == 0) {
        printf("No dishes in menu!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("          DINE OF INDIA - FULL MENU\n");
    printf("========================================\n");
    printf("%-6s %-35s %-10s %s\n", "S.NO", "Dish Name", "Price", "Category");
    printf("------ ----------------------------------- ---------- --------------------\n");
    
    // Display ALL dishes without pagination
    for(int i = 0; i < dishCount; i++) {
        printf("%-6d %-35s Rs.%-8.2f %s\n", 
               dishes[i].sno, dishes[i].name, dishes[i].price, dishes[i].category);
    }
    
    printf("========================================\n");
    printf("Total Dishes: %d\n", dishCount);
}

void calculateBill() {
    int numItems, sno, quantity, index;
    float total = 0;
    
    printf("\n--- Calculate Bill ---\n");
    printf("How many items? ");
    scanf("%d", &numItems);
    clearInputBuffer();
    
    printf("\n%-6s %-25s %-8s %-8s %s\n", "S.NO", "Dish", "Price", "Qty", "Amount");
    printf("------------------------------------------------------------\n");
    
    for(int i = 0; i < numItems; i++) {
        printf("\nItem %d:\n", i+1);
        printf("Enter S.NO: ");
        scanf("%d", &sno);
        
        index = findDishBySno(sno);
        if(index == -1) {
            printf("Dish not found! Skipping...\n");
            clearInputBuffer();
            continue;
        }
        
        printf("Enter Quantity: ");
        scanf("%d", &quantity);
        clearInputBuffer();
        
        float amount = dishes[index].price * quantity;
        total += amount;
        
        printf("%-6d %-25s %-8.2f %-8d %.2f\n", 
               dishes[index].sno, dishes[index].name, dishes[index].price, quantity, amount);
    }
    
    printf("------------------------------------------------------------\n");
    printf("TOTAL BILL: Rs. %.2f\n", total);
}

void saveToFile() {
    FILE *fp = fopen(FILENAME, "w");
    if(fp == NULL) {
        printf("Error: Cannot create file '%s'!\n", FILENAME);
        return;
    }
    
    fprintf(fp, "========================================\n");
    fprintf(fp, "          DINE OF INDIA MENU\n");
    fprintf(fp, "========================================\n");
    fprintf(fp, "%-6s %-35s %-12s %s\n", "S.NO", "Dish Name", "Price", "Category");
    fprintf(fp, "------ ----------------------------------- ------------ --------------------\n");
    
    for(int i = 0; i < dishCount; i++) {
        fprintf(fp, "%-6d %-35s Rs.%-8.2f %s\n", 
                dishes[i].sno, dishes[i].name, dishes[i].price, dishes[i].category);
    }
    
    fprintf(fp, "========================================\n");
    fprintf(fp, "Total Dishes: %d\n", dishCount);
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL) {
        return;
    }
    
    char line[256];
    dishCount = 0;
    
    // Skip header lines
    while(fgets(line, sizeof(line), fp)) {
        if(strstr(line, "S.NO") && strstr(line, "Dish Name")) {
            break;
        }
    }
    
    // Read separator line
    fgets(line, sizeof(line), fp);
    
    // Read dish data
    while(fgets(line, sizeof(line), fp) && dishCount < MAX_DISHES) {
        if(strstr(line, "====")) break;
        
        int sno;
        float price;
        char name[50] = "", category[30] = "";
        
        // Simple parsing
        if(sscanf(line, "%d %49[^0-9] Rs.%f %29[^\n]", &sno, name, &price, category) >= 3) {
            dishes[dishCount].sno = sno;
            
            // Remove trailing spaces from name
            int len = strlen(name);
            while(len > 0 && name[len-1] == ' ') {
                name[--len] = '\0';
            }
            
            strcpy(dishes[dishCount].name, name);
            dishes[dishCount].price = price;
            
            if(strlen(category) > 0) {
                strcpy(dishes[dishCount].category, category);
            } else {
                strcpy(dishes[dishCount].category, "General");
            }
            
            dishCount++;
        }
    }
    fclose(fp);
}
void loadDefaultDishes() {
    // 300 UNIQUE Indian dishes
    char *allDishes[] = {
        // North Indian (60 dishes)
        "Butter Chicken", "Paneer Butter Masala", "Dal Makhani", "Chole Bhature", "Rajma Chawal",
        "Aloo Paratha", "Palak Paneer", "Matar Paneer", "Kadai Paneer", "Malai Kofta",
        "Navratan Korma", "Shahi Paneer", "Baingan Bharta", "Aloo Gobi", "Mix Vegetable",
        "Tandoori Chicken", "Chicken Tikka", "Seekh Kebab", "Reshmi Kebab", "Afghani Chicken",
        "Chicken Curry", "Mutton Curry", "Fish Amritsari", "Egg Curry", "Kadai Chicken",
        "Butter Naan", "Garlic Naan", "Tandoori Roti", "Lachha Paratha", "Missi Roti",
        "Onion Kulcha", "Amritsari Kulcha", "Pudina Paratha", "Cheese Naan", "Peshawari Naan",
        "Chicken Biryani", "Mutton Biryani", "Veg Biryani", "Egg Biryani", "Hyderabadi Biryani",
        "Lucknowi Biryani", "Sindhi Biryani", "Kashmiri Pulao", "Peas Pulao", "Jeera Rice",
        "Matar Pulao", "Vegetable Pulao", "Tomato Rice", "Lemon Rice", "Curd Rice",
        "Chicken Korma", "Mutton Rogan Josh", "Chicken Do Pyaza", "Mutton Korma", "Chicken Handi",
        "Paneer Tikka", "Hariyali Chicken", "Tandoori Pomfret", "Chicken Reshmi", "Mutton Seekh",

        // South Indian (60 dishes)
        "Masala Dosa", "Plain Dosa", "Rava Dosa", "Mysore Masala Dosa", "Set Dosa",
        "Idli Sambar", "Medu Vada", "Uttapam", "Pongal", "Bisi Bele Bath",
        "Puliyogare", "Curd Rice", "Lemon Rice", "Coconut Rice", "Tomato Rice",
        "Appam", "Puttu", "Upma", "Rava Idli", "Mango Rice",
        "Sambar Vada", "Rasam Vada", "Masala Vada", "Dal Vada", "Onion Uttapam",
        "Tomato Uttapam", "Mixed Uttapam", "Rava Uttapam", "Ven Pongal", "Khara Pongal",
        "Sakkarai Pongal", "Milagu Pongal", "Tamarind Rice", "Mango Rice", "Ghee Rice",
        "Palappam", "Vellayappam", "Kallappam", "Neyappam", "Kadala Curry",
        "Kerala Parotta", "Malabar Parotta", "Egg Parotta", "Chicken Stew", "Mutton Stew",
        "Vegetable Stew", "Kerala Fish Curry", "Meen Molee", "Prawn Curry", "Crab Masala",
        "Avial", "Olan", "Thoran", "Erisherry", "Pachadi",
        "Kootu", "Poricha Kuzhambu", "Vatha Kuzhambu", "More Kuzhambu", "Puli Kuzhambu",

        // West Indian (60 dishes)
        "Pav Bhaji", "Vada Pav", "Misal Pav", "Dabeli", "Kanda Poha",
        "Sabudana Khichdi", "Thalipeeth", "Puran Poli", "Modak", "Shrikhand",
        "Dhokla", "Khandvi", "Patra", "Thepla", "Handvo",
        "Undhiyu", "Fafda", "Jalebi", "Gathiya", "Methi Thepla",
        "Kachori", "Samosa", "Aloo Tikki", "Pani Puri", "Sev Puri",
        "Bhel Puri", "Ragda Pattice", "Dahi Puri", "Papdi Chaat", "Bhalla Papdi",
        "Mirchi Bajji", "Bread Pakora", "Corn Chaat", "Pav Vada", "Kachori Sambar",
        "Bombay Sandwich", "Grilled Sandwich", "Masala Toast", "Cheese Toast", "Pav Samosa",
        "Gujarati Kadhi", "Rasawala Kaju", "Surati Undhiyu", "Khaman Dhokla", "Rava Dhokla",
        "Methi Dhokla", "Toor Dal Dhokla", "Khatta Dhokla", "Nylon Khaman", "Muthiya",
        "Khandvi Roll", "Patra Roll", "Dal Dhokli", "Gujarati Dal", "Kathiawadi Dal",
        "Bajra Rotla", "Jowar Roti", "Makki Ki Roti", "Thepla", "Bhakri",

        // East Indian (60 dishes)
        "Machher Jhol", "Chingri Malai Curry", "Kosha Mangsho", "Shorshe Ilish", "Chitol Muitha",
        "Luchi Alur Dom", "Rasgulla", "Sandesh", "Mishti Doi", "Cham Cham",
        "Ledikeni", "Mihidana", "Sitabhog", "Langcha", "Pantua",
        "Dalma", "Pakhala", "Chhena Poda", "Rasabali", "Chhena Gaja",
        "Macher Chop", "Mochar Chop", "Aloor Chop", "Beguni", "Phoolkopir Singara",
        "Mughlai Paratha", "Kathi Roll", "Egg Roll", "Chicken Roll", "Mutton Roll",
        "Kochuri", "Radhaballavi", "Mangsher Singara", "Chingri Macher Cutlet", "Bhetki Paturi",
        "Doi Maach", "Tel Koi", "Pabda Jhal", "Rohu Kalia", "Katla Kalia",
        "Chingri Bhape", "Crab Kalia", "Prawn Malai Curry", "Fish Orly", "Baked Bhetki",
        "Mutton Rezala", "Chicken Dak Bungalow", "Kashmiri Chicken", "Mutton Chaap", "Chicken Chaap",
        "Kosha Mangsho", "Mutton Biryani", "Chicken Biryani", "Tehari", "Kachchi Biryani",
        "Aloo Posto", "Shukto", "Chorchori", "Dhokar Dalna", "Potol Dolma",

        // Street Food (30 dishes)
        "Chicken 65", "Chicken Lollipop", "Chicken Manchurian", "Gobi Manchurian", "Paneer Manchurian",
        "Chilli Chicken", "Dragon Chicken", "Szechuan Chicken", "Hakka Noodles", "Fried Rice",
        "Schezwan Fried Rice", "Vegetable Fried Rice", "Egg Fried Rice", "Chicken Fried Rice", "Prawn Fried Rice",
        "Mushroom Fried Rice", "Paneer Fried Rice", "Garlic Fried Rice", "Spring Roll", "Chicken Spring Roll",
        "Vegetable Spring Roll", "Paneer Spring Roll", "American Chop Suey", "Chinese Bhel", "Soups",
        "Hot & Sour Soup", "Sweet Corn Soup", "Manchow Soup", "Tomato Soup", "Clear Soup",

        // Desserts & Beverages (30 dishes)
        "Gulab Jamun", "Rasgulla", "Ras Malai", "Kheer", "Gajar Halwa",
        "Moong Dal Halwa", "Badam Halwa", "Suji Halwa", "Besan Ladoo", "Coconut Ladoo",
        "Jalebi", "Rabri", "Phirni", "Shahi Tukda", "Double Ka Meetha",
        "Masala Chai", "Ginger Tea", "Cardamom Tea", "Filter Coffee", "Cold Coffee",
        "Mango Lassi", "Sweet Lassi", "Salted Lassi", "Rose Lassi", "Banana Lassi",
        "Buttermilk", "Jaljeera", "Aam Panna", "Kokum Sherbet", "Bael Sherbet"
    };
    
    char *categories[] = {
        "North Indian", "South Indian", "West Indian", "East Indian", "Street Food", "Desserts & Beverages"
    };
    
    dishCount = 0;
    
    // Generate 300 UNIQUE dishes (60 from first 4 categories, 30 from last 2)
    int dishesPerCategory[] = {60, 60, 60, 60, 30, 30};
    
    int currentIndex = 0;
    for(int cat = 0; cat < 6 && dishCount < 300; cat++) {
        for(int i = 0; i < dishesPerCategory[cat] && dishCount < 300; i++) {
            dishes[dishCount].sno = dishCount + 1;
            strcpy(dishes[dishCount].name, allDishes[currentIndex]);
            
            // Set prices based on category
            if(cat == 5) { // Desserts & Beverages - cheaper
                dishes[dishCount].price = 50 + (i % 8) * 10;
            } else if(cat == 4) { // Street Food - medium price
                dishes[dishCount].price = 80 + (i % 10) * 15;
            } else { // Main courses - standard price
                dishes[dishCount].price = 120 + (i % 12) * 18;
            }
            
            strcpy(dishes[dishCount].category, categories[cat]);
            dishCount++;
            currentIndex++;
        }
    }
}

int findDishBySno(int sno) {
    for(int i = 0; i < dishCount; i++) {
        if(dishes[i].sno == sno) {
            return i;
        }
    }
    return -1;
}

void clearInputBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}