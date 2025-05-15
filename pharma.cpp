/*
* Description: implementing a pharmaceutical warehouse inventory management
			system. This system will be able to handle a csv database,
			purchase orders with receipt generation, and general database
			functionalities. It will also have functions to load receipt
			counters and database file names from a configuration file
			which will be saved when the program quits.
* Input: SQL database, csv file, char values for selection
* Output: correctly formatted csv file and an assortment of selection cases
		  to output choices thru a inventory management system.
*/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

enum ItemCategory {
	ANTIBIOTIC,
	PAINKILLER,
	ANTIBACTERIAL,
	ANTIHISTAMINE,
	SUPPLEMENT,
	ANTACID,
	UNCATEGORIZED
};


// Note that default values have already been assigned
struct pharmaItem {
	int id = -1;
	string name = "";
	int price = -1;
	int quantity = -1;
	ItemCategory category = ItemCategory::UNCATEGORIZED;
};

const int DATABASE_SIZE = 100;              ///< Maximum database size.
const string DATABASE_FILE_KEY = "db";      ///< Key for database file in config.
const string RECEIPT_COUNT_KEY = "receipt"; ///< Key for receipt count in config.

bool loadConfiguration(string filename, string &dbFilename, int &receiptCount);

pharmaItem parseItem(string csvRow);

ItemCategory stringToCategory(string categoryString);

string categoryToString(ItemCategory categoryEnum);

int findItem(pharmaItem database[], int id);

void showItems(pharmaItem items[]);

bool updateStock(pharmaItem database[], int id, int qtyAdd);

void showReceipt(pharmaItem database[], int itemID[], int itemQty[], int &receiptNumber);

bool processPurchaseOrder(pharmaItem database[], string orderFilename, int &receiptNumber);

void showCategory(pharmaItem database[], ItemCategory category);

void saveDatabase(pharmaItem database[], string databaseFilename, string configFilename,
                 int receiptCount);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Usage: pharma <configuration file>\n";
		return EXIT_SUCCESS;
	}
	string databaseFilename = "";	///< Get this value from the configuration file.
	int receiptCounter;				///< Get this value from the configuration file.
    string configFilename = argv[1];


   
	if (!loadConfiguration(configFilename, databaseFilename, receiptCounter)) {
		cout << "Error loading configuration from file " << argv[1] << ".\n";
		return EXIT_FAILURE;
	}
    

   ifstream databaseFile(databaseFilename);
	if (!databaseFile) {
		cout << "Error loading database from file " << databaseFilename << "\n";
		return EXIT_FAILURE;
	}

    pharmaItem database[DATABASE_SIZE];
	string itemRow;
	getline(databaseFile, itemRow);  // Discard header.
    
    
	for (int currentEntry = 0; getline(databaseFile, itemRow); ++currentEntry) {
		database[currentEntry] = parseItem(itemRow);  
	}
    
	databaseFile.close();

    char userInput;
	do {
		cout << "\n**********WAREHOUSE MANAGER**********\n";
		cout << " s) Show Inventory\n"
		     << " u) Update Stock\n"
		     << " p) Process Purchase Order\n"
		     << " c) Show Category\n"
			 << " w) Write Changes to File\n"
		     << " q) Quit\n"
		     << "\n  >> ";
		cin >> userInput;
		userInput = tolower(userInput); // In case caps-lock is active.
		switch (userInput) {
		case 's':
			showItems(database);
			break;
		case 'u': {
           
			int id, qty;
			cout << "ID and Quantity to Add (separated by space): ";
			cin >> id >> qty;
			if (updateStock(database, id, qty)) {
				cout << "Stock updated successfully!\n";
			} else {
				cout << "Item not found!\n";
			}
            
			break;
		}
		case 'p': {
            
			string orderFile;
			cout << "Order file name: ";
			cin >> orderFile;
			processPurchaseOrder(database, orderFile, receiptCounter);
			break;
            
		}
		case 'c': {
            
			string categoryInput;
			cout << "Category to display: ";
			cin >> categoryInput;
			showCategory(database, stringToCategory(categoryInput)); 	
            
			break;
		}
		case 'w':
            
			saveDatabase(database, databaseFilename, configFilename, receiptCounter);
			cout << "Database Saved!\n";
            
			break;
		case 'q': {
            
			char save;
			cout << "Save Database? (y/n): ";
			cin >> save;
			save = tolower(save);
			if (save == 'y') {
				saveDatabase(database, databaseFilename, argv[1], receiptCounter);
				cout << "Database Saved!\n";
			} else {
				cout << "Quit without saving.\n";
			}
            
			break;
		}
		default:
			cin.clear();
			cin.ignore(100, '\n');
		}
	} while (userInput != 'q');
}
bool loadConfiguration(string filename, string &dbFilename, int &receiptCount) {
    /*
        Open the configuration file as an input file stream. If the file could not
        be opened, then return false. Items in the config file are stored as key,value
        pairs. The string before the equals sign is the key, and the string after the
        equals sign is its corresponding value. This file is guaranteed to contain
        only 2 entries, but the order is not guaranteed to be the same. Use the getline
        function to split the key value pairs. The key string for the database file is 
        given in the DATABASE_FILE_KEY constant and the key string for the receipt count
        is given in the RECEIPT_COUNT_KEY constant.

        Store the value of DATABASE_FILE_KEY key to the dbFilename parameter, and the
        value of the RECEIPT_COUNT_KEY key to the receiptCount parameter. If both keys
        and their corresponding values were found, return true. Otherwise, return false.     
    */
   // declare configuration file
   ifstream configurationFile(filename);
   /* Open the configuration file as an input file stream. If the file could not
	  be opened, then return false.*/
	if (!configurationFile.is_open()){
		return false;
	}
	// declare key and key value strings
	string key, keyValue;
	// flag values
	bool foundDbfile = false;
	bool foundReceipt = false;
	// getline function to split the key value pairs
	while (getline(configurationFile, key, '=')) {
			getline(configurationFile, keyValue);
			// Store the value of DATABASE_FILE_KEY key to the dbFilename
			if (key == DATABASE_FILE_KEY) {
				dbFilename = keyValue;
				// flag true
				foundDbfile = true;
			} else if (key == RECEIPT_COUNT_KEY) {
				// store RECEIPT_COUNT_KEY key to the receiptCount parameter
				receiptCount = stoi(keyValue);
				//flag true
				foundReceipt = true;

			}
	}
	// close filestream
	configurationFile.close();
	// return value
	return foundDbfile && foundReceipt;
}

pharmaItem parseItem(string csvRow) {
	pharmaItem newItem; // This will have default values.
    /*
        Write code to parse ONE row of the database and convert it to a corresponding
        pharmaItem object (newItem). Return this object. You won't need a loop here since we
        are only processing one row.
    */
   	// input string stream from the given csvRow string.
	istringstream csvString(csvRow);
	// declare string values
	string id_num, name, price, quantity, category;
	// getline function to parse this row using ,(comma) as the delimiter
	getline(csvString, id_num, ',');
	getline(csvString, name, ',');
	getline(csvString, price, ',');
	getline(csvString, quantity, ',');
	getline(csvString, category, ',');

	// Store each value that’s read into the corresponding newItem property
	newItem.id = stoi(id_num);
	newItem.name = name;
	newItem.price = stoi(price);
	newItem.quantity = stoi(quantity);
	newItem.category = stringToCategory(category);

	// return value
	return newItem;
}

ItemCategory stringToCategory(string categoryString) {
	if (categoryString == "Antibiotic") return ItemCategory::ANTIBIOTIC;
	if (categoryString == "Painkiller") return ItemCategory::PAINKILLER;
	if (categoryString == "Antibacterial") return ItemCategory::ANTIBACTERIAL;
	if (categoryString == "Antihistamine") return ItemCategory::ANTIHISTAMINE;
	if (categoryString == "Antacid") return ItemCategory::ANTACID;
	if (categoryString == "Supplement") return ItemCategory::SUPPLEMENT;
	return ItemCategory::UNCATEGORIZED;
}

string categoryToString(ItemCategory categoryEnum) {
	switch (categoryEnum) {
	case ANTIBIOTIC: return "Antibiotic";
	case PAINKILLER: return "Painkiller";
	case ANTIBACTERIAL: return "Antibacterial";
	case ANTIHISTAMINE: return "Antihistamine";
	case ANTACID: return "Antacid";
	case SUPPLEMENT: return "Supplement";
	case UNCATEGORIZED: return "None";
	default: return "";
	}
}

int findItem(pharmaItem database[], int id) {
	int index = -1;
	/*
        Find the index number of the database item which has the given
        id. Return this index. If the item wasn't found in the database,
        then return -1. You can take advantage of the fact that the default
		id is -1, meaning you can break out of the loop when any id in the
		database is detected as -1.
    */
   // loop until DATABASE_SIZE is met
   	for (int i = 0; i < DATABASE_SIZE; ++i) {
		// if no more entries are found
		if (database[i].id == index) {
			break;	// break if index is met
		}
		// find database id
		if (database[i].id == id) {
			return i;	// return value
		}

	}
	return index;
}

/*
    Note that this function prints an item array of type pharmaItem.
    It can be used to print the database, or even a subset of it.
*/
void showItems(pharmaItem items[]) {
	cout << "\n ";
	for (int i = 0; i < 71; i++) cout << "-";
	cout << " \n";
	cout << left;
	cout << "|" << setw(6) << "ID"
	     << "|" << setw(25) << "Name"
	     << "|" << setw(8) << "Price"
	     << "|" << setw(8) << "Qty"
	     << "|" << setw(20) << "Category"
	     << "|\n";
	cout << setfill('=');
	cout << "|" << setw(6) << ""
	     << "|" << setw(25) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(20) << ""
	     << "|\n";
	cout << setfill(' ');
	for (int i = 0; i < DATABASE_SIZE; i++) {
		if (items[i].id == -1) break;
		cout << left;
		cout << "|" << setw(6) << items[i].id
		     << "|" << setw(25) << items[i].name
		     << "|" << fixed << setprecision(2) << setw(1) << "$"
		     << setw(7) << (float)items[i].price / 100
		     << "|" << setw(8) << items[i].quantity
		     << "|" << setw(20)
		     << categoryToString(items[i].category) << "|\n";
	}
	cout << " ";
	for (int i = 0; i < 71; i++) cout << "-";
	cout << "\n";
}

bool updateStock(pharmaItem database[], int id, int qtyAdd) {
    /*
        Use the findItem function you implemented above to get
        the index for the item id that has to be changed. Add the
        qtyAdd value to the quantity of this item. Note that
        qtyAdd can be negative! If the resulting quantity is less
        than 0, then set it to 0. If the item to be updated was
        found and updated, return true. Otherwise, return false.
    */
   // Use findItem to get the index for the item id that has to be changed
   int index = findItem(database, id);
   // if no item is found return false
   if (index == -1) {
	return false;	// return value
   }
   // Add the qtyAdd value to the current quantity of the found item
   database[index].quantity += qtyAdd;
   // if the updated item quantity is negative (less than 0) then set it to 0
   if (database[index].quantity < 0) {
		database[index].quantity = 0;
   }
   // If the item to be updated was found and updated, return true
   if (index > -1) {
	return true;
   } else {	// Otherwise, return false.
	return false;	// return value
   }
}

void showReceipt(pharmaItem database[], int itemID[], int itemQty[], int &receiptNo) {
	int total = 0;
	cout << "\n ";
	for (int i = 0; i < 59; i++) cout << "*";
	cout << " \n|";
	cout << left << setw(59) << "Receipt #" + to_string(receiptNo)
	     << "|\n|";
	for (int i = 0; i < 59; i++) cout << "-";
	cout << "|\n";
	cout << "|" << setw(6) << "ID"
	     << "|" << setw(25) << "Name"
	     << "|" << setw(8) << "Price"
	     << "|" << setw(8) << "Qty"
	     << "|" << setw(8) << "Total"
	     << "|\n";
	cout << setfill('=');
	cout << "|" << setw(6) << ""
	     << "|" << setw(25) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|\n";
	cout << setfill(' ');
	for (int i = 0; i < DATABASE_SIZE; i++) {
		if (itemID[i] == -1) break;
		int index = findItem(database, itemID[i]);
		cout << "|" << setw(6) << itemID[i];
		if (index == -1) {
			cout << "|" << setw(25) << "**NO STOCK**"
			     << "|" << fixed << setprecision(2) << setw(1) << " "
			     << setw(7) << " "
			     << "|" << setw(8) << " "
			     << "|" << setw(1) << " "
			     << setw(7) << " "
				 << "|\n";
		} else {
			total += itemQty[i] * database[index].price;
			cout << "|" << setw(25) << database[index].name
			     << "|" << fixed << setprecision(2) << setw(1) << "$"
			     << setw(7) << (float)database[index].price / 100
			     << "|" << setw(8) << itemQty[i]
			     << "|" << setw(1) << "$"
			     << setw(7) << (float)itemQty[i] * database[index].price / 100
				 << "|\n";
		}
	}
	cout << setfill('-');
	cout << "|" << setw(6) << ""
	     << "|" << setw(25) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|\n";
	cout << setfill(' ');
	cout << "|" << setw(6) << ""
	     << "|" << setw(25) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
		 << "|" << setw(1) << "$"
	     << setw(7) << (float)total/100
	     << "|\n";
	cout << " ";
	for (int i = 0; i < 59; i++) cout << "*";
	cout << " \n";
}

bool processPurchaseOrder(pharmaItem database[], string orderFilename, int &receiptNumber) {
    int itemID[DATABASE_SIZE], itemQty[DATABASE_SIZE];
    /*
        This function processes a purchase order text file and generates a receipt for it.
        Two parallel arrays have been given to you: itemID, and itemQty. Set all values in
        these arrays to -1 (use a for loop). Open the orderFilename as an input file stream.
        If it couldn't be opened, return false. If it could be opened, then process each
        order line. For each line, add the order ID to the itemID array; check if the order
        quantity is greater than the quantity in stock. If the quantity in stock is less than
        the order, then add the entire inventory stock to the itemQty array in the corresponding
        ID position. Make sure to call the updateStock function you have implemented above to
        update the database inventory. If no stock of a product exists, then simply put a 0 in the
        corresponding itemQty index.

        Finally, call the showReceipt function while passing in the database, itemID array, itemQty
        array, and the receiptNumber. Increment the receiptNumber after the showReceipt function!
        Return true.
    */
   // use a for loop to set all values in these arrays to -1
   for (int i = 0; i < DATABASE_SIZE; ++i) {
		itemID[i] = -1;
		itemQty[i] = -1;
   }
   // Open the orderFilename
   ifstream orderFile(orderFilename);
   // if file stream couldn't be opened, return false
   if (!orderFile.is_open()) {
		return false;
   }
   // declare variables
   int orderID, orderQty; 
   int index = 0; 	// loop counter variable
   // loop data into variable(s)
   while (orderFile >> orderID >> orderQty) {
		itemID[index] = orderID; 	// add the order ID to the itemID array
		// variable claration to findItem
		int itemIndex = findItem(database, orderID);
		if (itemIndex != -1) {
			// check if the order quantity is greater than the quantity in stock
			if (orderQty > database[itemIndex].quantity) {
				itemQty[index] = database[itemIndex].quantity;
			} else {
				itemQty[index] = orderQty;
			}
			// call updateStock function to update the database inventory
			updateStock(database, orderID, -itemQty[index]);
		} else {
			// set item quantity to 0
			itemQty[index] = 0;
		}
		// update loop control variable
		index++;
   }
   // close file
   orderFile.close();

   // call the showReceipt function
   showReceipt(database, itemID, itemQty, receiptNumber);

   // increment receiptNumber
   receiptNumber++;

   // return value
   return true;
   
}

void showCategory(pharmaItem database[], ItemCategory category) {
    /*
        Display all items in the specified category sorted (selection sort) by price.
        You can create a new array of pharmaItem and insert the items of the specified
        category into it. Sort this array, and pass it to the showItem function.
    */
   // array to store items
   pharmaItem categorySelection[DATABASE_SIZE];
   // counter variable
   int categoryCount = 0;
   // linear search for loop
   for (int i = 0; i < DATABASE_SIZE; ++i) {
		// if -1 is found then break
		if (database[i].id == -1) {
			break;
		}
		// store item found
		if (database[i].category == category) {
			categorySelection[categoryCount++] = database[i];
		}
   }
   // selection sort to sort array price
   for (int i = 0; i < categoryCount - 1; ++i) {
		int minIndex = i;
		for (int j = i + 1; j < categoryCount; ++j) {
			if (categorySelection[j].price < categorySelection[minIndex].price) {
				minIndex = j;
			}
		}
		// swap items
		if (minIndex != i) {
			pharmaItem temp = categorySelection[i];
			categorySelection[i] = categorySelection[minIndex];
			categorySelection[minIndex] = temp;
		}
   }
   // call the showItem function with the sorted array
   showItems(categorySelection);
}

void saveDatabase(pharmaItem database[], string databaseFilename, string configFilename,
                 int receiptCount) {
    /*
        Write the database to the database file. You can use ifstream to get the header row
        from the original file and print it out as is back to the file. Make sure that when you
        write to the database file, values are stored in the correct csv format.

        Also write the configuration file in the same format. Write the updated receiptCounter
        to it as well as the databaseFilename. Note that the config file format is NOT the
        same as a regular csv file.
    */
   // open ifstream file to get header
   ifstream headerFile(databaseFilename);
   // declare variable
   string header;
   // getline to get header
   getline(headerFile, header);
   // close headerFile
   headerFile.close();
   // open database file stream
   ofstream databaseFile(databaseFilename, ofstream::trunc);
   //
   if (!databaseFile.is_open()) {
		return; // return to end program
   }
   // write header to database file
   databaseFile << header << "\n";
   // loop counter variable
   int i = 0;
   // write each database item to the database file
   while ( i < DATABASE_SIZE && database[i].id != -1) {
		// format csv file
		databaseFile << database[i].id << "," << database[i].name << ","
					 << database[i].price << "," << database[i].quantity
					 << "," << categoryToString(database[i].category) << "\n";
					 // increment loop counter variable
					 i++;
   }
   // close file stream 
   databaseFile.close();
	// open the configuration file as an output file stream w/truncate flag
	ofstream outputConfiguration(configFilename, ofstream::trunc);
	outputConfiguration << DATABASE_FILE_KEY << "=" << databaseFilename << "\n";
	outputConfiguration << RECEIPT_COUNT_KEY << "=" << receiptCount << "\n";
	// close file stream
	outputConfiguration.close();

}
