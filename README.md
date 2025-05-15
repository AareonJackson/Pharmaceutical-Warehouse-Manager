# Pharmaceutical Warehouse Manager

A C++ implementation of a pharmaceutical warehouse inventory management system that demonstrates file I/O, data structures, and algorithm implementation concepts.

## Assignment Overview

This project implements a complete inventory management system for pharmaceutical products with the following core capabilities:
- CSV database management with persistent storage
- Purchase order processing with automatic receipt generation
- Configuration file handling for system settings
- Category-based item filtering and display
- Interactive command-line interface

## System Architecture

### Data Structures

**ItemCategory Enum**
```cpp
enum ItemCategory {
    ANTIBIOTIC, PAINKILLER, ANTIBACTERIAL, 
    ANTIHISTAMINE, SUPPLEMENT, ANTACID, 
    UNCATEGORIZED
}
```

**pharmaItem Struct**
```cpp
struct pharmaItem {
    int id = -1;              // Unique identifier
    string name = "";         // Product name
    int price = -1;           // Price in cents
    int quantity = -1;        // Stock quantity
    ItemCategory category = UNCATEGORIZED;
}
```

### Key Constants
- `DATABASE_SIZE = 100`: Maximum inventory capacity
- `DATABASE_FILE_KEY = "db"`: Configuration key for database filename
- `RECEIPT_COUNT_KEY = "receipt"`: Configuration key for receipt counter

## Implementation Details

### Core Functions Implemented

**Configuration Management**
- `loadConfiguration()`: Parses key-value pairs from configuration file
- `saveDatabase()`: Persists inventory and configuration changes

**Data Processing**
- `parseItem()`: Converts CSV rows to pharmaItem objects
- `findItem()`: Linear search algorithm for item lookup
- `updateStock()`: Validates and updates inventory quantities

**Business Logic**
- `processPurchaseOrder()`: Handles order files with stock validation
- `showCategory()`: Filters and sorts items by pharmaceutical category
- `showReceipt()`: Generates formatted transaction receipts

**Utility Functions**
- `stringToCategory()` / `categoryToString()`: Enum conversion utilities
- `showItems()`: Formatted console output for inventory display

## File Formats

### Configuration File (`config.cfg`)
```
db=warehousedb.csv
receipt=102
```

### Database File (`warehousedb.csv`)
```csv
id,item,price,quantity,category
1200,Aspirin,599,45,Painkiller
1201,Amoxicillin,1299,30,Antibiotic
```

### Purchase Order File (`order1.txt`)
```
1206 50
1217 90
1219 100
```

## Algorithm Implementation

### Search Algorithm
- **Linear Search**: Used in `findItem()` for O(n) item lookups
- Optimized with early termination on encountering default values

### Sorting Algorithm
- **Selection Sort**: Implemented in `showCategory()` for price-based sorting
- O(n²) complexity suitable for small datasets

### File Processing
- **CSV Parsing**: Manual delimiter-based parsing using `getline()`
- **Configuration Parsing**: Key-value pair extraction with `=` delimiter

## Usage Instructions

### Compilation
```bash
g++ -o pharma pharma.cpp
```

### Execution
```bash
./pharma config.cfg
```

### Interactive Menu
```
**********WAREHOUSE MANAGER**********
 s) Show Inventory
 u) Update Stock  
 p) Process Purchase Order
 c) Show Category
 w) Write Changes to File
 q) Quit
```

## Error Handling

The system includes comprehensive error handling for:
- **File Operations**: Missing or corrupted configuration/database files
- **Order Processing**: Invalid item IDs and insufficient stock scenarios
- **Input Validation**: Out-of-range quantities and invalid menu selections
- **Data Integrity**: Prevents negative stock levels

## Educational Objectives

This assignment demonstrates mastery of:

### Programming Concepts
- **Struct and Enum Usage**: Complex data type definition and manipulation
- **File I/O Operations**: Reading/writing CSV and configuration files
- **Array Manipulation**: Fixed-size arrays for database simulation
- **String Processing**: Manual parsing without external libraries

### Algorithm Implementation
- **Search Algorithms**: Linear search with optimization
- **Sorting Algorithms**: Selection sort implementation
- **Data Validation**: Input sanitization and boundary checking

### Software Design
- **Modular Programming**: Well-defined function interfaces
- **Configuration Management**: External configuration file handling
- **Error Recovery**: Graceful handling of exceptional conditions

## Real-World Connections

While this implementation uses simplified approaches for educational purposes, it demonstrates concepts used in production systems:

**Current Implementation** → **Industry Standards**
- CSV Files → SQL Databases (PostgreSQL, MySQL)
- Arrays → Dynamic Data Structures (Vectors, Hash Tables)
- Linear Search → Indexed Database Queries
- Console Interface → Web/Desktop GUIs
- Manual Parsing → ORM Libraries

## Performance Characteristics

- **Time Complexity**: O(n) for search operations, O(n²) for sorting
- **Space Complexity**: O(1) additional space for most operations
- **Scalability**: Suitable for datasets up to `DATABASE_SIZE` items

## Testing Scenarios

The system handles various test cases:
1. **Normal Operations**: Standard inventory updates and order processing
2. **Edge Cases**: Orders exceeding stock, non-existent items
3. **File Handling**: Missing files, corrupted data formats
4. **Boundary Conditions**: Maximum database size, zero quantities

## Future Enhancements

Potential improvements for advanced implementations:
- Dynamic memory allocation for unlimited inventory
- SQL database integration with ODBC/ADO connections
- Multi-threading for concurrent order processing
- Network protocols for distributed warehouse management
- Advanced search algorithms (binary search, hash tables)
- GUI framework integration (Qt, GTK)

## Academic Integrity

This implementation serves as a learning tool for understanding:
- Fundamental data structures and algorithms
- File handling in C++
- Business logic implementation
- Software design principles
