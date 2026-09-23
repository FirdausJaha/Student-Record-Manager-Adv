# Project Documentation — Advanced Edition

## 1. Problem Statement

Managing student information becomes harder as the number of students grows. A useful student manager should provide structured data entry, fast searching, filtering, editing, deletion and academic analysis without introducing a separate database or backend for a second-year C++ lab project.

## 2. Objective

The project provides a browser-based student management system while keeping C++ as the core/root of the application. HTML/CSS provides the interface, JavaScript acts as a thin bridge to WebAssembly, and browser localStorage provides persistence.

## 3. Student Data

Each student contains:

- Roll number — alphanumeric string
- Student name
- Age
- Gender — Male or Female
- Mobile number — 10-digit string
- Branch
- Current semester — 1st to 8th
- Previous-semester SGPAs
- Calculated CGPA

No marks, percentage or overall-score field is used.

## 4. Dynamic Semester Logic

If the current semester is `N`, the application asks only for SGPAs from semesters `1` through `N-1`.

| Current semester | Required SGPA fields | CGPA |
|---|---|---|
| 1st | None | N/A |
| 2nd | 1st | SGPA 1 |
| 3rd | 1st–2nd | Average of 1–2 |
| 4th | 1st–3rd | Average of 1–3 |
| 5th | 1st–4th | Average of 1–4 |
| 6th | 1st–5th | Average of 1–5 |
| 7th | 1st–6th | Average of 1–6 |
| 8th | 1st–7th | Average of 1–7 |

## 5. GPA Scale and CGPA Rule

Only the 10-point GPA scale is accepted.

- SGPA minimum: `0.00`
- SGPA maximum: `10.00`
- CGPA is calculated only from completed previous semesters.
- CGPA is calculated by C++.
- CGPA is truncated to two decimal places rather than rounded.

Example:

`(8.05 + 7.48) / 2 = 7.765`

Displayed result:

`7.76`

## 6. Data Structures and Capacity

The project does not use `std::vector` and does not impose a 100-student array limit.

Instead, C++ uses dynamically allocated raw arrays:

```cpp
string* rollNo;
string* studentName;
int* age;
string* gender;
string* mobileNumber;
string* branch;
int* currentSemester;
float (*sgpa)[8];
float* cgpa;
```

When the current capacity is insufficient, `ensureCapacity()` allocates larger arrays, copies the existing elements and releases the old arrays. The capacity grows automatically, so there is no artificial maximum student count in the application logic.

Actual use is still subject to available browser memory and localStorage quota.

## 7. Main C++ Operations

- Add student
- Update student
- Delete student
- Search students
- Filter by branch, gender and semester
- Sort by name, roll number and CGPA
- Calculate CGPA
- Find highest and lowest CGPA
- Calculate average CGPA
- Analyze branch, gender and semester performance
- Generate CGPA distribution
- Load demonstration data
- Import serialized browser data
- Clear all students

## 8. Dashboard

The Dashboard is the quick overview page. It shows:

- Total students
- Average CGPA
- Highest CGPA
- Lowest CGPA
- Gender distribution
- Branch distribution
- Semester distribution
- Top performers
- Recent students
- Quick actions

Distribution groups can be clicked to open the Students page with matching filters.

## 9. Students Page

The Students page is the main management area. It supports:

- Search by roll number, name, mobile number or branch
- Branch filter
- Gender filter
- Semester filter
- Combined filters
- Sorting
- View details
- Edit
- Delete

There is no `0 / 100` capacity display and no fixed student-count limit.

## 10. Analysis Page

The Analysis page provides deeper academic analysis without marks:

- Highest CGPA
- Lowest CGPA
- Average CGPA
- CGPA distribution
- Students with CGPA ≥ 8.00
- Students below 6.00
- Students without previous SGPA/CGPA
- Branch performance
- Gender analysis
- Semester analysis
- CGPA ranking

## 11. Persistence

On the first visit, if no saved data exists, the application loads 30 original demonstration students.

After data is saved, localStorage stores the serialized C++ records. On later visits, JavaScript sends the saved serialized data to the C++ `importRecords()` function. The C++ program reconstructs the dynamic arrays.

The demo data is therefore not used to overwrite an existing user's data.

## 12. WebAssembly Architecture

```text
User Interface
HTML + CSS
      ↓
JavaScript
thin event/data bridge
      ↓
Emscripten WebAssembly
      ↓
C++ student manager
      ↓
Dynamic arrays + SGPA/CGPA calculations + analysis
```

The browser layer does not independently calculate CGPA. The calculation belongs to C++.

## 13. Viva Explanation

> "The project is a C++ Student Record Manager exposed to a browser through WebAssembly. C++ stores the student information in dynamically growing raw arrays and performs validation, searching, filtering, sorting, SGPA handling, CGPA calculation and analysis. The application uses only the 10-point GPA scale and calculates CGPA from completed previous-semester SGPAs. JavaScript is intentionally kept as a thin bridge between the HTML interface and C++ functions, while localStorage preserves the serialized C++ data between visits. There is no hard-coded 100-student limit; the arrays grow as required until practical browser memory or storage limits are reached."
