#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <emscripten/bind.h>

using namespace std;

const int SEMESTER_COUNT = 8;

string* rollNo = nullptr;
string* studentName = nullptr;
int* age = nullptr;
string* gender = nullptr;
string* mobileNumber = nullptr;
string* branch = nullptr;
int* currentSemester = nullptr;
float (*sgpa)[SEMESTER_COUNT] = nullptr;
float* cgpa = nullptr;

int studentCount = 0;
int capacity = 0;

const string BRANCHES[] = {
    "CSE", "ECE", "EEE", "IT", "MECH",
    "CSE (AI & ML)", "CSE (DS)", "Civil"
};

void ensureCapacity(int required) {
    if (required <= capacity)
        return;

    int newCapacity = (capacity == 0) ? 32 : capacity * 2;

    while (newCapacity < required)
        newCapacity *= 2;

    string* newRollNo = new string[newCapacity];
    string* newStudentName = new string[newCapacity];
    int* newAge = new int[newCapacity];
    string* newGender = new string[newCapacity];
    string* newMobileNumber = new string[newCapacity];
    string* newBranch = new string[newCapacity];
    int* newCurrentSemester = new int[newCapacity];
    float (*newSgpa)[SEMESTER_COUNT] =
        new float[newCapacity][SEMESTER_COUNT];
    float* newCgpa = new float[newCapacity];

    for (int i = 0; i < studentCount; ++i) {
        newRollNo[i] = rollNo[i];
        newStudentName[i] = studentName[i];
        newAge[i] = age[i];
        newGender[i] = gender[i];
        newMobileNumber[i] = mobileNumber[i];
        newBranch[i] = branch[i];
        newCurrentSemester[i] = currentSemester[i];
        newCgpa[i] = cgpa[i];

        for (int j = 0; j < SEMESTER_COUNT; ++j)
            newSgpa[i][j] = sgpa[i][j];
    }

    delete[] rollNo;
    delete[] studentName;
    delete[] age;
    delete[] gender;
    delete[] mobileNumber;
    delete[] branch;
    delete[] currentSemester;
    delete[] sgpa;
    delete[] cgpa;

    rollNo = newRollNo;
    studentName = newStudentName;
    age = newAge;
    gender = newGender;
    mobileNumber = newMobileNumber;
    branch = newBranch;
    currentSemester = newCurrentSemester;
    sgpa = newSgpa;
    cgpa = newCgpa;

    capacity = newCapacity;
}

void initializeStorage() {
    if (capacity == 0)
        ensureCapacity(32);
}

string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\n\r");

    if (start == string::npos)
        return "";

    size_t end = text.find_last_not_of(" \t\n\r");

    return text.substr(start, end - start + 1);
}

string lowerCase(string text) {
    transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        }
    );

    return text;
}

bool validRoll(const string& roll) {
    if (roll.empty() || roll.size() > 24)
        return false;

    for (unsigned char c : roll) {
        if (!isalnum(c))
            return false;
    }

    return true;
}

bool validName(const string& name) {
    if (name.empty() || name.size() > 80)
        return false;

    for (unsigned char c : name) {
        if (!(isalpha(c) ||
              isspace(c) ||
              c == '.' ||
              c == '-' ||
              c == '\'')) {
            return false;
        }
    }

    return true;
}

bool validGender(const string& value) {
    return value == "Male" || value == "Female";
}

bool validBranch(const string& value) {
    for (const string& b : BRANCHES) {
        if (value == b)
            return true;
    }

    return false;
}

bool validMobile(const string& mobile) {
    if (mobile.size() != 10)
        return false;

    for (unsigned char c : mobile) {
        if (!isdigit(c))
            return false;
    }

    return true;
}

bool validSemester(int semester) {
    return semester >= 1 && semester <= 8;
}

bool validSGPA(float value) {
    return value >= 0.0f && value <= 10.0f;
}

int findByRoll(const string& roll) {
    for (int i = 0; i < studentCount; ++i) {
        if (rollNo[i] == roll)
            return i;
    }

    return -1;
}

double truncateTwoDecimals(double value) {
    return floor(value * 100.0 + 1e-9) / 100.0;
}

float calculateCGPA(int semester, const float values[]) {
    if (semester <= 1)
        return -1.0f;

    double total = 0.0;
    int count = semester - 1;

    for (int i = 0; i < count; ++i)
        total += values[i];

    double average = total / count;

    double truncated = truncateTwoDecimals(average);

    return static_cast<float>(truncated);
}

bool validateStudent(
    const string& roll,
    const string& name,
    int studentAge,
    const string& studentGender,
    const string& studentBranch,
    const string& mobile,
    int semester,
    const float values[]
) {
    if (!validRoll(roll))
        return false;

    if (!validName(name))
        return false;

    if (studentAge < 1 || studentAge > 100)
        return false;

    if (!validGender(studentGender))
        return false;

    if (!validBranch(studentBranch))
        return false;

    if (!validMobile(mobile))
        return false;

    if (!validSemester(semester))
        return false;

    for (int i = 0; i < semester - 1; ++i) {
        if (!validSGPA(values[i]))
            return false;
    }

    return true;
}

void setSGPAValues(int index, const float values[]) {
    for (int i = 0; i < SEMESTER_COUNT; ++i)
        sgpa[index][i] = -1.0f;

    for (int i = 0; i < currentSemester[index] - 1; ++i)
        sgpa[index][i] = values[i];
}

bool addStudentInternal(
    const string& roll,
    const string& name,
    int studentAge,
    const string& studentGender,
    const string& studentBranch,
    const string& mobile,
    int semester,
    float s1,
    float s2,
    float s3,
    float s4,
    float s5,
    float s6,
    float s7
) {
    float values[7] = {
        s1, s2, s3, s4, s5, s6, s7
    };

    if (!validateStudent(
        roll,
        name,
        studentAge,
        studentGender,
        studentBranch,
        mobile,
        semester,
        values
    )) {
        return false;
    }

    if (findByRoll(roll) != -1)
        return false;

    ensureCapacity(studentCount + 1);

    int index = studentCount;

    rollNo[index] = trim(roll);
    studentName[index] = trim(name);
    age[index] = studentAge;
    gender[index] = studentGender;
    mobileNumber[index] = mobile;
    branch[index] = studentBranch;
    currentSemester[index] = semester;

    setSGPAValues(index, values);

    cgpa[index] = calculateCGPA(semester, values);

    studentCount++;

    return true;
}

bool addStudent(
    const string& roll,
    const string& name,
    int studentAge,
    const string& studentGender,
    const string& studentBranch,
    const string& mobile,
    int semester,
    float s1,
    float s2,
    float s3,
    float s4,
    float s5,
    float s6,
    float s7
) {
    initializeStorage();

    return addStudentInternal(
        roll,
        name,
        studentAge,
        studentGender,
        studentBranch,
        mobile,
        semester,
        s1,
        s2,
        s3,
        s4,
        s5,
        s6,
        s7
    );
}

bool updateStudent(
    int index,
    const string& roll,
    const string& name,
    int studentAge,
    const string& studentGender,
    const string& studentBranch,
    const string& mobile,
    int semester,
    float s1,
    float s2,
    float s3,
    float s4,
    float s5,
    float s6,
    float s7
) {
    if (index < 0 || index >= studentCount)
        return false;

    float values[7] = {
        s1, s2, s3, s4, s5, s6, s7
    };

    if (!validateStudent(
        roll,
        name,
        studentAge,
        studentGender,
        studentBranch,
        mobile,
        semester,
        values
    )) {
        return false;
    }

    int existing = findByRoll(roll);

    if (existing != -1 && existing != index)
        return false;

    rollNo[index] = trim(roll);
    studentName[index] = trim(name);
    age[index] = studentAge;
    gender[index] = studentGender;
    mobileNumber[index] = mobile;
    branch[index] = studentBranch;
    currentSemester[index] = semester;

    setSGPAValues(index, values);

    cgpa[index] = calculateCGPA(semester, values);

    return true;
}

bool deleteStudent(int index) {
    if (index < 0 || index >= studentCount)
        return false;

    for (int i = index; i < studentCount - 1; ++i) {
        rollNo[i] = rollNo[i + 1];
        studentName[i] = studentName[i + 1];
        age[i] = age[i + 1];
        gender[i] = gender[i + 1];
        mobileNumber[i] = mobileNumber[i + 1];
        branch[i] = branch[i + 1];
        currentSemester[i] = currentSemester[i + 1];
        cgpa[i] = cgpa[i + 1];

        for (int j = 0; j < SEMESTER_COUNT; ++j)
            sgpa[i][j] = sgpa[i + 1][j];
    }

    studentCount--;

    return true;
}

void clearAll() {
    studentCount = 0;
}

string formatCGPA(float value) {
    if (value < 0.0f)
        return "N/A";

    ostringstream out;

    out << fixed << setprecision(2) << value;

    return out.str();
}

string recordToString(int index) {
    ostringstream out;

    out << rollNo[index] << "~"
        << studentName[index] << "~"
        << age[index] << "~"
        << gender[index] << "~"
        << branch[index] << "~"
        << mobileNumber[index] << "~"
        << currentSemester[index] << "~";

    for (int i = 0; i < 7; ++i) {
        if (sgpa[index][i] < 0.0f)
            out << "-";
        else
            out << fixed << setprecision(2) << sgpa[index][i];

        out << "~";
    }

    out << formatCGPA(cgpa[index]);

    return out.str();
}

string getRecords() {
    ostringstream out;

    for (int i = 0; i < studentCount; ++i) {
        if (i > 0)
            out << "|";

        out << recordToString(i);
    }

    return out.str();
}

string searchStudents(const string& query) {
    string q = lowerCase(trim(query));

    ostringstream out;

    bool first = true;

    for (int i = 0; i < studentCount; ++i) {
        string combined =
            lowerCase(rollNo[i] + " " +
                      studentName[i] + " " +
                      mobileNumber[i] + " " +
                      branch[i]);

        if (q.empty() || combined.find(q) != string::npos) {
            if (!first)
                out << "|";

            out << recordToString(i);

            first = false;
        }
    }

    return out.str();
}

string filterRecords(
    const string& filterBranch,
    const string& filterGender,
    int filterSemester
) {
    ostringstream out;

    bool first = true;

    for (int i = 0; i < studentCount; ++i) {
        bool matchesBranch =
            filterBranch.empty() ||
            filterBranch == "All" ||
            branch[i] == filterBranch;

        bool matchesGender =
            filterGender.empty() ||
            filterGender == "All" ||
            gender[i] == filterGender;

        bool matchesSemester =
            filterSemester <= 0 ||
            currentSemester[i] == filterSemester;

        if (matchesBranch &&
            matchesGender &&
            matchesSemester) {

            if (!first)
                out << "|";

            out << recordToString(i);

            first = false;
        }
    }

    return out.str();
}

void swapStudents(int a, int b) {
    swap(rollNo[a], rollNo[b]);
    swap(studentName[a], studentName[b]);
    swap(age[a], age[b]);
    swap(gender[a], gender[b]);
    swap(mobileNumber[a], mobileNumber[b]);
    swap(branch[a], branch[b]);
    swap(currentSemester[a], currentSemester[b]);
    swap(cgpa[a], cgpa[b]);

    for (int i = 0; i < SEMESTER_COUNT; ++i)
        swap(sgpa[a][i], sgpa[b][i]);
}

string sortByCGPA(bool descending) {
    for (int i = 0; i < studentCount - 1; ++i) {
        for (int j = 0; j < studentCount - i - 1; ++j) {
            bool shouldSwap;

            if (descending)
                shouldSwap = cgpa[j] < cgpa[j + 1];
            else
                shouldSwap = cgpa[j] > cgpa[j + 1];

            if (shouldSwap)
                swapStudents(j, j + 1);
        }
    }

    return getRecords();
}

string sortByName() {
    for (int i = 0; i < studentCount - 1; ++i) {
        for (int j = 0; j < studentCount - i - 1; ++j) {
            if (lowerCase(studentName[j]) >
                lowerCase(studentName[j + 1])) {

                swapStudents(j, j + 1);
            }
        }
    }

    return getRecords();
}

string sortByRoll() {
    for (int i = 0; i < studentCount - 1; ++i) {
        for (int j = 0; j < studentCount - i - 1; ++j) {
            if (lowerCase(rollNo[j]) >
                lowerCase(rollNo[j + 1])) {

                swapStudents(j, j + 1);
            }
        }
    }

    return getRecords();
}

double averageCGPA() {
    if (studentCount == 0)
        return -1.0;

    double total = 0.0;
    int count = 0;

    for (int i = 0; i < studentCount; ++i) {
        if (cgpa[i] >= 0.0f) {
            total += cgpa[i];
            count++;
        }
    }

    if (count == 0)
        return -1.0;

    return truncateTwoDecimals(total / count);
}

int highestCGPAIndex() {
    int index = -1;

    for (int i = 0; i < studentCount; ++i) {
        if (cgpa[i] < 0.0f)
            continue;

        if (index == -1 || cgpa[i] > cgpa[index])
            index = i;
    }

    return index;
}

int lowestCGPAIndex() {
    int index = -1;

    for (int i = 0; i < studentCount; ++i) {
        if (cgpa[i] < 0.0f)
            continue;

        if (index == -1 || cgpa[i] < cgpa[index])
            index = i;
    }

    return index;
}

string getAnalysis() {
    int withoutPreviousSGPA = 0;
    int aboveOrEqual8 = 0;
    int below6 = 0;

    for (int i = 0; i < studentCount; ++i) {
        if (currentSemester[i] == 1) {
            withoutPreviousSGPA++;
            continue;
        }

        if (cgpa[i] >= 8.0f)
            aboveOrEqual8++;

        if (cgpa[i] < 6.0f)
            below6++;
    }

    int highest = highestCGPAIndex();
    int lowest = lowestCGPAIndex();

    ostringstream out;

    out << "{";

    out << "\"total\":" << studentCount << ",";

    out << "\"averageCGPA\":";

    if (averageCGPA() < 0.0)
        out << "null";
    else
        out << fixed << setprecision(2) << averageCGPA();

    out << ",";

    out << "\"highestCGPA\":";

    if (highest == -1)
        out << "null";
    else
        out << fixed << setprecision(2) << cgpa[highest];

    out << ",";

    out << "\"lowestCGPA\":";

    if (lowest == -1)
        out << "null";
    else
        out << fixed << setprecision(2) << cgpa[lowest];

    out << ",";

    out << "\"cgpa8OrAbove\":" << aboveOrEqual8 << ",";
    out << "\"below6\":" << below6 << ",";
    out << "\"withoutPreviousSGPA\":" << withoutPreviousSGPA;

    out << "}";

    return out.str();
}

string getBranchAnalysis() {
    ostringstream out;

    bool first = true;

    for (const string& currentBranch : BRANCHES) {
        int count = 0;
        double total = 0.0;
        int cgpaCount = 0;

        for (int i = 0; i < studentCount; ++i) {
            if (branch[i] == currentBranch) {
                count++;

                if (cgpa[i] >= 0.0f) {
                    total += cgpa[i];
                    cgpaCount++;
                }
            }
        }

        if (!first)
            out << "|";

        out << currentBranch << "~"
            << count << "~";

        if (cgpaCount == 0)
            out << "N/A";
        else
            out << fixed << setprecision(2)
                << truncateTwoDecimals(total / cgpaCount);

        first = false;
    }

    return out.str();
}

string getGenderAnalysis() {
    const string genders[] = {
        "Male",
        "Female"
    };

    ostringstream out;

    for (int g = 0; g < 2; ++g) {
        if (g > 0)
            out << "|";

        int count = 0;
        double total = 0.0;
        int cgpaCount = 0;

        for (int i = 0; i < studentCount; ++i) {
            if (gender[i] == genders[g]) {
                count++;

                if (cgpa[i] >= 0.0f) {
                    total += cgpa[i];
                    cgpaCount++;
                }
            }
        }

        out << genders[g] << "~"
            << count << "~";

        if (cgpaCount == 0)
            out << "N/A";
        else
            out << fixed << setprecision(2)
                << truncateTwoDecimals(total / cgpaCount);
    }

    return out.str();
}

string getSemesterAnalysis() {
    ostringstream out;

    for (int semester = 1; semester <= 8; ++semester) {
        if (semester > 1)
            out << "|";

        int count = 0;
        double total = 0.0;
        int cgpaCount = 0;

        for (int i = 0; i < studentCount; ++i) {
            if (currentSemester[i] == semester) {
                count++;

                if (cgpa[i] >= 0.0f) {
                    total += cgpa[i];
                    cgpaCount++;
                }
            }
        }

        out << semester << "~"
            << count << "~";

        if (cgpaCount == 0)
            out << "N/A";
        else
            out << fixed << setprecision(2)
                << truncateTwoDecimals(total / cgpaCount);
    }

    return out.str();
}

string getCGPADistribution() {
    int ranges[5] = {
        0, 0, 0, 0, 0
    };

    for (int i = 0; i < studentCount; ++i) {
        if (cgpa[i] < 0.0f)
            continue;

        if (cgpa[i] >= 9.0f)
            ranges[0]++;
        else if (cgpa[i] >= 8.0f)
            ranges[1]++;
        else if (cgpa[i] >= 7.0f)
            ranges[2]++;
        else if (cgpa[i] >= 6.0f)
            ranges[3]++;
        else
            ranges[4]++;
    }

    ostringstream out;

    out << "9.00-10.00~" << ranges[0] << "|";
    out << "8.00-8.99~" << ranges[1] << "|";
    out << "7.00-7.99~" << ranges[2] << "|";
    out << "6.00-6.99~" << ranges[3] << "|";
    out << "Below 6.00~" << ranges[4];

    return out.str();
}

bool loadDemoData() {
    clearAll();

    const string demoRoll[] = {
        "25B11CS101","25B11EC102","25B11EE103","25B11IT104",
        "25B11ME105","25B11CS106","25B11DS107","25B11CV108",
        "25B11CS109","25B11EC110","25B11EE111","25B11IT112",
        "25B11ME113","25B11CS114","25B11DS115","25B11CV116",
        "25B11CS117","25B11EC118","25B11EE119","25B11IT120",
        "25B11ME121","25B11CS122","25B11DS123","25B11CV124",
        "25B11CS125","25B11EC126","25B11EE127","25B11IT128",
        "25B11ME129","25B11CS130"
    };

    const string demoNames[] = {
        "Aarav Sharma",
        "Diya Reddy",
        "Rahul Kumar",
        "Ananya Rao",
        "Vikram Singh",
        "Sneha Patel",
        "Arjun Mehta",
        "Ishita Nair",
        "Karan Verma",
        "Priya Shah",
        "Rohan Gupta",
        "Meera Joshi",
        "Aditya Das",
        "Nisha Rani",
        "Varun Iyer",
        "Kavya Menon",
        "Siddharth Jain",
        "Aisha Khan",
        "Harsh Vardhan",
        "Pooja Devi",
        "Manish Yadav",
        "Sana Ali",
        "Akash Rao",
        "Neha Thomas",
        "Zoya Ahmed",
        "Ritesh Kapoor",
        "Tanvi Roy",
        "Mohit Agarwal",
        "Simran Kaur",
        "Dev Malhotra"
    };

    const int demoAge[] = {
        20,19,21,20,22,20,21,19,22,23,
        21,22,20,21,23,20,19,22,23,20,
        21,22,19,20,23,21,20,22,19,23
    };

    const string demoGender[] = {
        "Male","Female","Male","Female","Male",
        "Female","Male","Female","Male","Female",
        "Male","Female","Male","Female","Male",
        "Female","Male","Female","Male","Female",
        "Male","Female","Male","Female","Female",
        "Male","Female","Male","Female","Male"
    };

    const string demoMobile[] = {
        "9000001001","9000001002","9000001003","9000001004",
        "9000001005","9000001006","9000001007","9000001008",
        "9000001009","9000001010","9000001011","9000001012",
        "9000001013","9000001014","9000001015","9000001016",
"9000001017","9000001018","9000001019","9000001020",
"9000001021","9000001022","9000001023","9000001024",
"9000001025","9000001026","9000001027","9000001028",
"9000001029","9000001030"
};

const string demoBranch[] = {
    "CSE","ECE","EEE","IT","MECH","CSE (AI & ML)",
    "CSE (DS)","Civil","CSE","ECE","EEE","IT",
    "MECH","CSE (AI & ML)","CSE (DS)","Civil",
    "CSE","ECE","EEE","IT","MECH","CSE (AI & ML)",
    "CSE (DS)","Civil","CSE","ECE","EEE","IT",
    "MECH","CSE"
};

const int demoSemester[] = {
    3,2,4,5,6,3,4,2,7,8,
    5,6,3,5,8,4,2,6,7,3,
    8,7,5,1,8,4,2,7,3,8
};

const float demoSGPA[30][7] = {
    {8.05f,7.48f,-1,-1,-1,-1,-1},
    {8.21f,-1,-1,-1,-1,-1,-1},
    {7.42f,7.68f,7.55f,-1,-1,-1,-1},
    {8.62f,8.31f,8.74f,8.58f,-1,-1,-1},
    {7.15f,7.42f,7.08f,7.66f,7.54f,-1,-1},
    {8.74f,8.92f,-1,-1,-1,-1,-1},
    {8.15f,8.42f,8.76f,-1,-1,-1,-1},
    {7.88f,-1,-1,-1,-1,-1,-1},
    {8.12f,8.44f,8.21f,8.56f,8.73f,8.65f,-1},
    {8.34f,8.51f,8.08f,8.62f,8.77f,8.69f,8.91f},
    {7.32f,7.64f,7.51f,7.88f,-1,-1,-1},
    {8.72f,8.54f,8.63f,8.81f,8.92f,-1,-1},
    {7.05f,7.31f,-1,-1,-1,-1,-1},
    {8.91f,9.02f,8.74f,9.12f,-1,-1,-1},
    {7.84f,8.12f,7.96f,8.35f,8.44f,8.21f,8.57f},
    {6.88f,7.12f,7.24f,-1,-1,-1,-1},
    {8.76f,-1,-1,-1,-1,-1,-1},
    {7.91f,8.05f,8.22f,7.88f,8.31f,-1,-1},
    {7.21f,7.48f,7.62f,7.55f,7.74f,7.68f,-1},
    {8.41f,8.18f,-1,-1,-1,-1,-1},
    {6.92f,7.18f,7.35f,7.42f,7.21f,7.56f,7.63f},
    {8.86f,9.11f,8.92f,9.03f,8.78f,9.16f,-1},
    {7.72f,7.95f,8.14f,8.02f,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {9.02f,8.91f,9.12f,8.86f,9.05f,9.18f,9.24f},
    {8.04f,8.22f,8.31f,-1,-1,-1,-1},
    {7.28f,-1,-1,-1,-1,-1,-1},
    {8.14f,8.26f,8.39f,8.47f,8.58f,8.61f,-1},
    {6.74f,7.02f,-1,-1,-1,-1,-1},
    {8.63f,8.77f,8.84f,8.92f,9.01f,8.96f,9.08f}
};

for (int i = 0; i < 30; ++i) {
    addStudentInternal(
        demoRoll[i],
        demoNames[i],
        demoAge[i],
        demoGender[i],
        demoBranch[i],
        demoMobile[i],
        demoSemester[i],
        demoSGPA[i][0],
        demoSGPA[i][1],
        demoSGPA[i][2],
        demoSGPA[i][3],
        demoSGPA[i][4],
        demoSGPA[i][5],
        demoSGPA[i][6]
    );
}

return true;
}

bool importRecords(const string& serialized) {
    clearAll();

    if (serialized.empty())
        return true;

    string row;
    stringstream rows(serialized);

    while (getline(rows, row, '|')) {
        string fields[15];
        string field;
        stringstream parts(row);

        int fieldCount = 0;

        while (getline(parts, field, '~') &&
               fieldCount < 15) {
            fields[fieldCount++] = field;
        }

        if (fieldCount < 15)
            continue;

        try {
            float values[7];

            for (int i = 0; i < 7; ++i) {
                values[i] =
                    (fields[7 + i] == "-")
                    ? -1.0f
                    : stof(fields[7 + i]);
            }

            addStudentInternal(
                fields[0],
                fields[1],
                stoi(fields[2]),
                fields[3],
                fields[4],
                fields[5],
                stoi(fields[6]),
                values[0],
                values[1],
                values[2],
                values[3],
                values[4],
                values[5],
                values[6]
            );
        }
        catch (...) {
        }
    }

    return true;
}

int countStudents() {
    return studentCount;
}

EMSCRIPTEN_BINDINGS(student_manager) {

    emscripten::function(
        "addStudent",
        &addStudent
    );

    emscripten::function(
        "updateStudent",
        &updateStudent
    );

    emscripten::function(
        "deleteStudent",
        &deleteStudent
    );

    emscripten::function(
        "getRecords",
        &getRecords
    );

    emscripten::function(
        "searchStudents",
        &searchStudents
    );

    emscripten::function(
        "filterRecords",
        &filterRecords
    );

    emscripten::function(
        "getAnalysis",
        &getAnalysis
    );

    emscripten::function(
        "getBranchAnalysis",
        &getBranchAnalysis
    );

    emscripten::function(
        "getGenderAnalysis",
        &getGenderAnalysis
    );

    emscripten::function(
        "getSemesterAnalysis",
        &getSemesterAnalysis
    );

    emscripten::function(
        "getCGPADistribution",
        &getCGPADistribution
    );

    emscripten::function(
        "sortByCGPA",
        &sortByCGPA
    );

    emscripten::function(
        "sortByName",
        &sortByName
    );

    emscripten::function(
        "sortByRoll",
        &sortByRoll
    );

    emscripten::function(
        "averageCGPA",
        &averageCGPA
    );

    emscripten::function(
        "highestCGPAIndex",
        &highestCGPAIndex
    );

    emscripten::function(
        "lowestCGPAIndex",
        &lowestCGPAIndex
    );

    emscripten::function(
        "countStudents",
        &countStudents
    );

    emscripten::function(
        "clearAll",
        &clearAll
    );

    emscripten::function(
        "loadDemoData",
        &loadDemoData
    );

    emscripten::function(
        "importRecords",
        &importRecords
    );
}
