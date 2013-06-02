Simple ORM for Qt

**Features**

1. Adding, removing, selection, editing objects;
2. Conditions;
3. Search options;
4. Aggregate functions;
5. Relations one-to-one and one-to-many;
6. Solution to N + 1 queries problem;
7. Tested on MySQL, SQLite, PostgreSQL;
8. Generate SQL schema from source code by ormtool;
9. Simple and comfortable using;
10. Doxygen-based documentation (English)
11. Unit tests.

**How to build documentation:**

1. Install doxygen
2. Clone repository
3. Run next commands

```
cd path-to-repository/docs
doxygen Doxyfile
```
**How to add library to the project**

1. Compile library
2. Add next string to your .pro file:

```
LIBS += -Lpath-to-compiled-library -lORM
```
