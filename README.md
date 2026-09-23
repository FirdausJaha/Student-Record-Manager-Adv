# Student Record Manager — Advanced Edition

## Live demo

[Open Student Record Manager — Adv](https://firdausjaha.github.io/Student-Record-Manager-Adv/)

## Overview

A C++-centered Student Record Manager delivered through a responsive browser interface. C++ remains the heart of the application. Emscripten compiles the C++ program to WebAssembly, JavaScript acts as a thin browser bridge, and localStorage preserves the serialized C++ data between visits.

## Final student data model

Each student contains:

- Alphanumeric roll number
- Student name
- Age
- Gender: Male or Female
- 10-digit mobile number
- Branch
- Current semester: 1st to 8th
- Previous-semester SGPAs only
- Automatically calculated CGPA

There is **no marks, percentage, overall-score or pass/fail field** in this edition.

## 10-point GPA rule

SGPA and CGPA use the **0.00–10.00 scale only**.

If the current semester is `N`, the form requests SGPAs only for semesters `1` through `N-1`.

Examples:

- 1st semester → no previous SGPA → CGPA `N/A`
- 2nd semester → SGPA 1
- 3rd semester → SGPA 1 and 2
- 8th semester → SGPA 1 through 7

CGPA is calculated in C++:

`CGPA = (SGPA1 + SGPA2 + ... + SGPA(N-1)) / (N-1)`

The result is **truncated**, not conventionally rounded, to two decimal places.

Example:

`(8.05 + 7.48) / 2 = 7.765 → 7.76`

## No fixed student-count limit

The project does **not** use a hard-coded `MAX_STUDENTS = 100` limit.

C++ uses dynamically growing raw arrays. The initial capacity is small and the arrays automatically grow when more students are added. This keeps the data structures simple enough to explain in a C++ lab while removing an artificial application-level student-count cap.

The practical limit is the browser's available memory and localStorage capacity; no software-level limit of 100 students is imposed.

## Main features

- Dashboard with total students, average/highest/lowest CGPA
- Gender, branch and semester distributions
- Top performers
- Recent students
- Search by roll number, name, mobile number or branch
- Branch, gender and semester filters
- Combined filtering
- Sorting by name, roll number and CGPA
- Add, view, update and delete students
- Dynamic SGPA fields based on current semester
- Automatic C++ CGPA calculation
- CGPA distribution
- Branch, gender and semester analysis
- CGPA ranking
- Clear all students
- 30 original demonstration students on first launch
- localStorage persistence
- Restore saved data instead of overwriting it with demo data

## C++ concepts demonstrated

- Raw dynamic arrays
- Two-dimensional SGPA array
- Dynamic memory allocation and reallocation
- Strings
- Functions
- Loops and conditions
- Linear search
- Filtering
- Sorting
- Array shifting during deletion
- Validation
- Aggregation and analysis
- Emscripten bindings

## Architecture

```text
HTML/CSS
   ↓
JavaScript browser bridge
   ↓
C++ WebAssembly
   ↓
Student dynamic arrays + SGPA/CGPA logic
```

JavaScript does not calculate CGPA. It only collects form values, calls C++ functions, renders results, and persists serialized records in localStorage.

## Deployment

GitHub Actions compiles `src/student_manager.cpp` using Emscripten 4.0.10 and produces `wasm/student_manager.js`.

Enable GitHub Pages using:

**Repository → Settings → Pages → Source → GitHub Actions**

## Project structure

```text
Student-Record-Manager-Adv/
├── .github/
│   └── workflows/
│       └── deploy.yml
├── src/
│   └── student_manager.cpp
├── wasm/
│   └── README.txt
├── index.html
├── style.css
├── README.md
├── PROJECT_DOCUMENTATION.md
├── SAMPLE_OUTPUT.md
└── .gitignore
```
