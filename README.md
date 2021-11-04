# Word search solver

A simple and deterministic word search game solver.

## How to build

Execute the following commands to clone the git repository and to generate the program executable :
```
git clone https://github.com/RICCIARDI-Adrien/Word_Search_Solver
cd Word_Search_Solver
make
```

This will create an executable program named `word-search-solver`.

## How to use

Type `./word-search-solver Word_Search_File`.

Word search file format consists of the grid content, followed by the separating character '-', then followed by the list of the words to search (one per line).  
Only ASCII letters (from A to Z), both uppercase of lowercase, are accepted.  
See test files in the `Tests` directory for examples.

Some sample files are provided in the `Tests` directory, they can be automatically ran with the following commands :
```
cd Tests
./Tests.sh
```
