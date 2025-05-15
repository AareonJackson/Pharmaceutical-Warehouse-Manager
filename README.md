Warehouse Manager
A pharmaceutical warehouse inventory management system implemented in C++ that handles CSV-based database operations, purchase order processing, and receipt generation.
Overview
This project implements a comprehensive inventory management system for pharmaceutical products. The system manages inventory data through CSV files and provides essential functionality for warehouse operations including stock updates, purchase order processing, and categorical item viewing.
Features

CSV Database Management: Load and manage pharmaceutical inventory from CSV files with full CRUD operations
Interactive Menu System: User-friendly command-line interface for all operations
Purchase Order Processing: Process orders from text files with automatic stock updates and receipt generation
Receipt Generation: Generate detailed receipts with itemized totals for all transactions
Configuration Management: Automatically save and load receipt counters and database file names from configuration files
Category Filtering: View items by pharmaceutical category (Antibiotic, Painkiller, Antibacterial, etc.)
Stock Updates: Add or subtract inventory quantities with validation
Persistent Settings: Save configuration and database changes on exit

System Architecture
Data Structures

pharmaItem struct: Contains ID, name, price, quantity, and category
ItemCategory enum: Pharmaceutical categories including Antibiotic, Painkiller, Antibacterial, Antihistamine, Supplement, Antacid
Array-based storage: Fixed-size array for in-memory database operations

Key Functions

loadConfiguration(): Load database filename and receipt counter from config file
parseItem(): Parse CSV rows into pharmaItem structures
updateStock(): Modify inventory quantities with validation
processPurchaseOrder(): Handle order files and generate receipts
showCategory(): Display items by category sorted by price
saveDatabase(): Persist changes to CSV and configuration files

File Structure

Configuration File (config.cfg): Stores database filename and receipt counter
Database File (warehousedb.csv): CSV inventory data with headers
Order Files (order1.txt): Purchase orders with item ID and quantity pairs

Usage
Compilation
cppg++ -o pharma pharma.cpp
Execution
bash./pharma config.cfg
Menu Options

s: Show complete inventory
u: Update stock levels for specific items
p: Process purchase orders from file
c: Display items filtered by category
w: Save changes to database file
q: Quit with optional save prompt

Configuration Format
db=warehousedb.csv
receipt=102
Order File Format
1206 50
1217 90
1219 100
Technical Details
The system demonstrates fundamental concepts used in inventory management:

File I/O operations for CSV and configuration management
String parsing and data conversion
Array-based data structures and algorithms
Menu-driven user interfaces
Financial calculations with proper formatting

Educational Purpose
This project showcases essential programming concepts including:

File handling and CSV parsing
Struct and enum usage in C++
Search algorithms (linear search for item lookup)
Sorting algorithms (selection sort for category display)
Input validation and error handling
Configuration file management

Real-World Applications
While this implementation uses CSV files and arrays, production inventory systems typically feature:

SQL database backends for scalability
Multi-user concurrent access
Web-based or GUI interfaces
Barcode/RFID integration
Advanced reporting and analytics
Network-based distributed systems

Error Handling
The system includes robust error handling for:

Missing configuration files
Invalid database files
Non-existent order files
Invalid user input
Out-of-stock scenarios

Future Enhancements
Potential improvements could include:

SQL database integration
Network connectivity for distributed systems
Graphical user interface
Barcode scanning support
Advanced search and filtering
Backup and recovery systems
User authentication and permissions
