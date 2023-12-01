# Accounting-based Approach to Managing Personal Budgets #

Inspired by the matching principle in financial accounting, this budget application
aims to match all expenses to corresponding utility. Period expenses are amortized/
depreciated over the lifetime of the expense. The goal is to create a more accurate
representation of personal expenses over time. 

# ⛏️ In Progress ⛏️ #

<img width="1403" alt="image" src="https://github.com/m1chael-lu/advanced-budget/assets/96500006/4f9949fb-1b62-4a54-a8c2-1bb273aeb415">

## Current Features: ##
- C++ Boost-based HTTP backend server
    - Asynchronous Thread-based implementation
    - TCP / IPv4
    - Support for query and body parameters
    - MySQL integration
    - Registering a user
    - Adding an expense
    - Aggregating expenses
    - Delete expenses
 - React
    - React client with utility-based chart view with (recharts)
    - Modal to add new expenses
    - Table view of expenses with filtering / sorting


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
8. Navigate to `client/application` and run `npm install --force`
9. In `/build`, run `./MyServer` to start the server
10. Use `mysql.server start` to start the local mysql server
11. Navigate to `client/application` and run `npm start` to start the React client


## Contributors: ##
- Michael Lu
- Namita Sajai
