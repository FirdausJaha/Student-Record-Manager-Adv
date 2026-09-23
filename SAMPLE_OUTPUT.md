# Sample Output — Advanced Edition

## First launch

The application opens with 30 original demonstration students when no saved browser data exists.

The Dashboard shows:

- Total Students
- Average CGPA
- Highest CGPA
- Lowest CGPA
- Gender distribution
- Branch distribution
- Semester distribution
- Top performers
- Recent students

There is no `Records 0 / 100` capacity card.

## Add Student — Example

```text
Roll Number: 25B11CS259
Student Name: Aarav Reddy
Age: 20
Gender: Male
Mobile: 9876543210
Branch: CSE
Current Semester: 3rd

1st Semester SGPA: 8.05
2nd Semester SGPA: 7.48

Calculated CGPA: 7.76
```

The CGPA calculation is performed by C++:

```text
(8.05 + 7.48) / 2 = 7.765
Truncated to two decimals = 7.76
```

## 1st Semester

```text
Current Semester: 1st
Previous SGPA fields: None
CGPA: N/A
```

## 8th Semester

```text
Current Semester: 8th
SGPA fields: 1st, 2nd, 3rd, 4th, 5th, 6th, 7th
CGPA: Average of SGPA 1 through SGPA 7
```

## Students

The Students page supports:

- Search
- Branch filter
- Gender filter
- Semester filter
- Combined filters
- Sorting
- View details
- Edit
- Delete

The interface does not impose a 100-student limit.

## Analysis

The Analysis page contains:

- Highest CGPA
- Lowest CGPA
- Average CGPA
- CGPA distribution
- Performance summary
- Branch performance
- Gender analysis
- Semester analysis
- CGPA ranking

## Academic scale

Only the **10-point GPA scale** is used:

```text
SGPA: 0.00 – 10.00
CGPA: 0.00 – 10.00
```

No marks, percentage or overall-score field is part of this edition.
