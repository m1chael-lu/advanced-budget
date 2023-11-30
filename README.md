# Accounting-based Approach to Managing Personal Budgets #

Inspired by the matching principle in financial accounting, this budget application
aims to match all expenses to corresponding utility. Period expenses are amortized/
depreciated over the lifetime of the expense. The goal is to create a more accurate
representation of personal expenses over time. 

# ⛏️ In Progress ⛏️ #

## Current Features: ##
- C++ Boost-based HTTP backend server
    - Asynchronous Thread-based implementation
    - TCP / IPv4
    - Support for query and body parameters
    - MySQL integration
    - Registering a user


## Dependencies: ##
- Boost 
- MySQL Connector/C++
- nlohmann/json
- OpenSSL

## Getting Started: (locally) ##
1. Ensure that all dependencies are installed, and linked correctly in the CMake config
2. If homebrew is used, ensure that library links are correct
3. Download and set up local mysql database with `username=root` and `password=localsqlpass`
4. Start the database with `mysql.server start`
5. Run `cmake ..` in the `/build` directory
6. Run `cmake --build .` in the `/build` directory
7. Navigate to `src/database/` and run `./setup`
8. In `/build`, run `./MyServer` to start the server 

## Current Feature Plans: ##
- Routing Several API's:
    - Add User
    - Authenticate User
    - Add Expense 
    - Modify Expense
    - Delete Expense
    - Aggregate Report 
- React-based Frontend:
    - Allowing user login/registration
    - UI-based expense creationg

## Contributors: ##
- Michael Lu
- Namita Sajai
