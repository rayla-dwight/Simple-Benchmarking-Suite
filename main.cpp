/***********************************************************
* Name: Rayla Dwight
* Assignment: COSC-2436 Challenge 1
* Purpose: This is an application which tests and benchmarks three
* different concepts in computer science: Merge Sort vs. Heap Sort,
* Linear Search performance on arrays vs. linked lists, and Matrix
* Multiplication in different loop orders. This application demonstrates
* deep understanding of all course materials to this point as well as
* algorithm analysis and benchmarking theory and practices.
* This single main.cpp file is the fully executable program, no other
* files are required to run the application.
***********************************************************/

/*********
Includes
*********/
#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <cstring>
#include <string>
#include <cmath>

/*************************************************
Pull in the specific standard library items used
Prevents having to prefix every time they are used
*************************************************/
using std::chrono::high_resolution_clock;

using std::cout;
using std::endl;
using std::fixed;
using std::left;
using std::mt19937;
using std::random_device;
using std::setprecision;
using std::setw;
using std::strcmp;
using std::string;
using std::strncpy;
using std::fabs;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

/*************************
Driver Function: main()
*************************/
int main() {
    /* *****************************************************************************
    * This function is the application driver. It rigorously and dynamically
    * tests (benchmarks) the performance of three different concepts in computer
    * science: Merge vs. Heap Sorting, Linear Search performance on arrays vs.
    * linked lists, and Matrix Multiplication performance in different loop orders
    *
    * @param na : na
    * @return (int) : application exit code
    * @exception na : na
    * @note This function uses a number of helper lambdas to comply with the requirement
    * for a single main function in a main.cpp file. It also uses returns in
    * some lambdas to preserve clarity and avoid unnecessary nesting where appropriate,
    * in accordance with the single return per function (or lambda) rule.
    * *****************************************************************************/

    // =============================
    // Shared Constants
    // =============================
    constexpr int DEFAULT_TRIALS = 10;  // reused across all benchmarks
    constexpr int NAME_LEN = 16;        // fixed-length char arrays (sorting + search)
    constexpr double EPSILON = 1e-6;    // floating-point comparison (matrix validation)

    // =============================
    // Local structs
    // =============================
    struct Record
    {
        int id;
        char name[NAME_LEN];
    };

    struct Node
    {
        Record data;
        Node* next;
    };

    // =============================
    // Utility lambdas
    // =============================
    mt19937 rng(random_device{}()); // random number generator

    // Computes elapsed time in milliseconds between two timestamps
    auto getElapsedMs = [](const high_resolution_clock::time_point& start,
                           const high_resolution_clock::time_point& end) -> double
    {
        return std::chrono::duration<double, std::milli>(end - start).count();
    };

    // Fills integer array with random values in given range
    auto fillRandomIntArray = [&rng](int* arr, const int size, const int minValue, const int maxValue) -> void
    {
        uniform_int_distribution<int> dist(minValue, maxValue);

        for (int i = 0; i < size; i++)
        {
            arr[i] = dist(rng);
        }
    };

    // Generates random fixed-length alphabetic string
    auto makeRandomName = [&rng]() -> string
    {
        constexpr int alphabetSize = 52;
        string name(NAME_LEN - 1, ' ');

        uniform_int_distribution<int> charDist(0, alphabetSize - 1);

        for (int i = 0; i < NAME_LEN - 1; i++)
        {
            constexpr char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

            name[i] = alphabet[charDist(rng)];
        }

        return name;
    };

    // Copies string into fixed-length char array safely
    auto copyNameToCharArray = [](char destination[NAME_LEN], const string& source) -> void
    {
        strncpy(destination, source.c_str(), NAME_LEN - 1);
        destination[NAME_LEN - 1] = '\0';
    };

    // =============================
    // Printing lambdas
    // =============================
    cout << fixed << setprecision(3);   // clean formatting for benchmark times

    // Prints formatted section header for benchmark output
    auto printSectionHeader = [](const string& title) -> void
    {
        cout << "\n============================================================\n";
        cout << title << endl;
        cout << "============================================================\n";
    };

    // Prints formatted subsection header
    auto printSubHeader = [](const string& label) -> void
    {
        cout << "\n" << label << endl;
        cout << "------------------------------------------------------------\n";
    };

    // Prints labeled double value with units
    auto printLabelValueDouble = [](const string& label, const double value, const string& units) -> void
    {
        cout << left << setw(32) << label
             << value << ' ' << units << endl;
    };

    // Prints labeled integer value
    auto printLabelValueInt = [](const string& label, const int value) -> void
    {
        cout << left << setw(32) << label
             << value << endl;
    };

    // Prints pass/fail status label
    auto printPassFail = [](const string& label, const bool passed) -> void
    {
        cout << left << setw(32) << label
             << (passed ? "PASS" : "FAIL") << endl;
    };


    /********************************************************************
    Sorting benchmark lambdas
    ********************************************************************/
    // =============================
    // Sorting helper lambdas
    // =============================

    // Copies integer array contents from source to destination
    auto copyIntArray = [](int* destination, const int* source, const int size) -> void
    {
        for (int i = 0; i < size; i++)
        {
            destination[i] = source[i];
        }
    };

    // Copies fixed-length char matrix safely
    auto copyCharMatrix = [](char (*destination)[NAME_LEN], const char (*source)[NAME_LEN], const int size) -> void
    {
        for (int i = 0; i < size; i++)
        {
            strncpy(destination[i], source[i], NAME_LEN - 1);
            destination[i][NAME_LEN - 1] = '\0';
        }
    };

    // Checks if integer array is sorted in nondecreasing order
    auto isIntArraySorted = [](const int* arr, const int size) -> bool
    {
        bool sorted = true;

        for (int i = 1; i < size && sorted; i++)
        {
            if (arr[i - 1] > arr[i])
            {
                sorted = false;
            }
        }

        return sorted;
    };

    // Checks if char matrix is sorted lexicographically
    auto isCharMatrixSorted = [](const char (*arr)[NAME_LEN], const int size) -> bool
    {
        bool sorted = true;

        for (int i = 1; i < size && sorted; i++)
        {
            if (strcmp(arr[i - 1], arr[i]) > 0)
            {
                sorted = false;
            }
        }

        return sorted;
    };

    // Fills char matrix with random fixed-length strings
    auto fillRandomCharMatrix = [&makeRandomName, &copyNameToCharArray](char (*arr)[NAME_LEN], const int size) -> void
    {
        for (int i = 0; i < size; i++)
        {
            string randomName = makeRandomName();
            copyNameToCharArray(arr[i], randomName);
        }
    };

    // =============================
    // Integer merge sort lambdas
    // =============================

    // Merges two sorted subarrays into a single sorted range
    auto mergeIntArrays = [](int* arr, const int left, const int mid, const int right) -> void
    {
        const int leftSize = mid - left + 1;
        const int rightSize = right - mid;

        int* leftTemp = new int[leftSize];
        int* rightTemp = new int[rightSize];

        for (int i = 0; i < leftSize; i++)
        {
            leftTemp[i] = arr[left + i];
        }

        for (int i = 0; i < rightSize; i++)
        {
            rightTemp[i] = arr[mid + 1 + i];
        }

        int i = 0;
        int j = 0;
        int k = left;

        while (i < leftSize && j < rightSize)
        {
            if (leftTemp[i] <= rightTemp[j])
            {
                arr[k] = leftTemp[i];
                i++;
            }
            else
            {
                arr[k] = rightTemp[j];
                j++;
            }

            k++;
        }

        while (i < leftSize)
        {
            arr[k] = leftTemp[i];
            i++;
            k++;
        }

        while (j < rightSize)
        {
            arr[k] = rightTemp[j];
            j++;
            k++;
        }

        delete[] leftTemp;
        delete[] rightTemp;
    };

    // Recursively sorts integer array using merge sort
    auto mergeSortInts = [&](int* arr, const int left, const int right) -> void
    {
        // self passing recursive helper
        auto mergeSortRecursive = [&](auto&& self, int* workingArray, const int low, const int high) -> void
        {
            if (low < high)
            {
                const int mid = low + (high - low) / 2;

                self(self, workingArray, low, mid);
                self(self, workingArray, mid + 1, high);
                mergeIntArrays(workingArray, low, mid, high);
            }
        };

        mergeSortRecursive(mergeSortRecursive, arr, left, right);
    };

    // =============================
    // Integer heap sort lambdas
    // =============================

    // Heapify: restores max-heap property for subtree
    auto heapifyInts = [&](auto&& self, int* arr, const int size, const int rootIndex) -> void
    {
        int largest = rootIndex;
        const int leftChild = 2 * rootIndex + 1;
        const int rightChild = 2 * rootIndex + 2;

        if (leftChild < size && arr[leftChild] > arr[largest])
        {
            largest = leftChild;
        }

        if (rightChild < size && arr[rightChild] > arr[largest])
        {
            largest = rightChild;
        }

        if (largest != rootIndex)
        {
            const int temp = arr[rootIndex];
            arr[rootIndex] = arr[largest];
            arr[largest] = temp;

            self(self, arr, size, largest);
        }
    };

    // Integer heap sort: builds heap and extracts max elements in-place
    auto heapSortInts = [&](int* arr, const int size) -> void
    {
        for (int i = (size / 2) - 1; i >= 0; i--)
        {
            heapifyInts(heapifyInts, arr, size, i);
        }

        for (int i = size - 1; i > 0; i--)
        {
            const int temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;

            heapifyInts(heapifyInts, arr, i, 0);
        }
    };

    // =============================
    // Integer sort benchmark runner
    // =============================

    // Benchmarks merge sort and heap sort on identical integer datasets
    auto benchmarkIntegerSorting = [&]() -> void
    {
        constexpr int sortSizes[] = {10000, 25000, 50000, 100000};
        constexpr int trialCount = DEFAULT_TRIALS;

        for (int sizeIndex = 0; sizeIndex < std::size(sortSizes); sizeIndex++) {
            const int sortSize = sortSizes[sizeIndex];

            double totalMergeTime = 0.0;
            double totalHeapTime = 0.0;
            bool allMergeSorted = true;
            bool allHeapSorted = true;
            bool allOutputsMatch = true;

            int* originalData = new int[sortSize];
            int* mergeData = new int[sortSize];
            int* heapData = new int[sortSize];

            // Checks whether two integer arrays match element-for-element
            auto intArraysMatch = [](const int* left, const int* right, const int size) -> bool
            {
                bool match = true;

                for (int i = 0; i < size && match; i++)
                {
                    if (left[i] != right[i])
                    {
                        match = false;
                    }
                }

                return match;
            };

            printSectionHeader("BENCHMARK 1A: INTEGER SORTING");
            printLabelValueInt("Array size:", sortSize);
            printLabelValueInt("Trial count:", trialCount);

            for (int trial = 0; trial < trialCount; trial++)
            {
                constexpr int maxValue = 1000000;
                constexpr int minValue = 1;
                fillRandomIntArray(originalData, sortSize, minValue, maxValue);

                copyIntArray(mergeData, originalData, sortSize);
                copyIntArray(heapData, originalData, sortSize);

                high_resolution_clock::time_point startTime = high_resolution_clock::now();
                mergeSortInts(mergeData, 0, sortSize - 1);
                high_resolution_clock::time_point endTime = high_resolution_clock::now();
                totalMergeTime += getElapsedMs(startTime, endTime);

                startTime = high_resolution_clock::now();
                heapSortInts(heapData, sortSize);
                endTime = high_resolution_clock::now();
                totalHeapTime += getElapsedMs(startTime, endTime);

                if (!isIntArraySorted(mergeData, sortSize))
                {
                    allMergeSorted = false;
                }

                if (!isIntArraySorted(heapData, sortSize))
                {
                    allHeapSorted = false;
                }

                if (!intArraysMatch(mergeData, heapData, sortSize))
                {
                    allOutputsMatch = false;
                }
            }
            printSubHeader("Results");
            printLabelValueDouble("Merge sort average:", totalMergeTime / trialCount, "ms");
            printLabelValueDouble("Heap sort average:", totalHeapTime / trialCount, "ms");
            printPassFail("Merge sort validation:", allMergeSorted);
            printPassFail("Heap sort validation:", allHeapSorted);
            printPassFail("Sorted outputs match:", allOutputsMatch);

            delete[] originalData;
            delete[] mergeData;
            delete[] heapData;
        }
    };

    // =============================
    // Char-array merge sort lambdas
    // =============================

    // Merges two sorted char subarrays into a single sorted range
    auto mergeCharArrays = [](char (*arr)[NAME_LEN], const int left, const int mid, const int right) -> void
    {
        const int leftSize = mid - left + 1;
        const int rightSize = right - mid;

        char (*leftTemp)[NAME_LEN] = new char[leftSize][NAME_LEN];
        char (*rightTemp)[NAME_LEN] = new char[rightSize][NAME_LEN];

        for (int i = 0; i < leftSize; i++)
        {
            strncpy(leftTemp[i], arr[left + i], NAME_LEN - 1);
            leftTemp[i][NAME_LEN - 1] = '\0';
        }

        for (int i = 0; i < rightSize; i++)
        {
            strncpy(rightTemp[i], arr[mid + 1 + i], NAME_LEN - 1);
            rightTemp[i][NAME_LEN - 1] = '\0';
        }

        int i = 0;
        int j = 0;
        int k = left;

        while (i < leftSize && j < rightSize)
        {
            if (strcmp(leftTemp[i], rightTemp[j]) <= 0)
            {
                strncpy(arr[k], leftTemp[i], NAME_LEN - 1);
                arr[k][NAME_LEN - 1] = '\0';
                i++;
            }
            else
            {
                strncpy(arr[k], rightTemp[j], NAME_LEN - 1);
                arr[k][NAME_LEN - 1] = '\0';
                j++;
            }

            k++;
        }

        while (i < leftSize)
        {
            strncpy(arr[k], leftTemp[i], NAME_LEN - 1);
            arr[k][NAME_LEN - 1] = '\0';
            i++;
            k++;
        }

        while (j < rightSize)
        {
            strncpy(arr[k], rightTemp[j], NAME_LEN - 1);
            arr[k][NAME_LEN - 1] = '\0';
            j++;
            k++;
        }

        delete[] leftTemp;
        delete[] rightTemp;
    };

    // Recursively sorts char matrix lexicographically using merge sort
    auto mergeSortChars = [&](char (*arr)[NAME_LEN], const int left, const int right) -> void
    {
        auto mergeSortRecursive = [&](auto&& self, char (*workingArray)[NAME_LEN], const int low, const int high) -> void
        {
            if (low < high)
            {
                const int mid = low + (high - low) / 2;

                self(self, workingArray, low, mid);
                self(self, workingArray, mid + 1, high);
                mergeCharArrays(workingArray, low, mid, high);
            }
        };

        mergeSortRecursive(mergeSortRecursive, arr, left, right);
    };

    // =============================
    // Char-array heap sort lambdas
    // =============================

    // Heapify: restores max-heap property for char array subtree
    auto heapifyChars = [&](auto&& self, char (*arr)[NAME_LEN], const int size, const int rootIndex) -> void
    {
        int largest = rootIndex;
        const int leftChild = 2 * rootIndex + 1;
        const int rightChild = 2 * rootIndex + 2;

        if (leftChild < size && strcmp(arr[leftChild], arr[largest]) > 0)
        {
            largest = leftChild;
        }

        if (rightChild < size && strcmp(arr[rightChild], arr[largest]) > 0)
        {
            largest = rightChild;
        }

        if (largest != rootIndex)
        {
            char temp[NAME_LEN];

            strncpy(temp, arr[rootIndex], NAME_LEN - 1);
            temp[NAME_LEN - 1] = '\0';

            strncpy(arr[rootIndex], arr[largest], NAME_LEN - 1);
            arr[rootIndex][NAME_LEN - 1] = '\0';

            strncpy(arr[largest], temp, NAME_LEN - 1);
            arr[largest][NAME_LEN - 1] = '\0';

            self(self, arr, size, largest);
        }
    };

    // Heap sort: builds heap and extracts max char elements in-place
    auto heapSortChars = [&](char (*arr)[NAME_LEN], const int size) -> void
    {
        for (int i = (size / 2) - 1; i >= 0; i--)
        {
            heapifyChars(heapifyChars, arr, size, i);
        }

        for (int i = size - 1; i > 0; i--)
        {
            char temp[NAME_LEN];

            strncpy(temp, arr[0], NAME_LEN - 1);
            temp[NAME_LEN - 1] = '\0';

            strncpy(arr[0], arr[i], NAME_LEN - 1);
            arr[0][NAME_LEN - 1] = '\0';

            strncpy(arr[i], temp, NAME_LEN - 1);
            arr[i][NAME_LEN - 1] = '\0';

            heapifyChars(heapifyChars, arr, i, 0);
        }
    };

    // =============================
    // Char-array sort benchmark runner
    // =============================

    // Benchmarks merge sort and heap sort on identical char datasets
    auto benchmarkCharSorting = [&]() -> void
    {
        constexpr int sortSizes[] = {10000, 25000, 50000, 100000};
        constexpr int trialCount = DEFAULT_TRIALS;

        for (int sizeIndex = 0; sizeIndex < std::size(sortSizes); sizeIndex++)
        {
            const int sortSize = sortSizes[sizeIndex];

            double totalMergeTime = 0.0;
            double totalHeapTime = 0.0;
            bool allMergeSorted = true;
            bool allHeapSorted = true;
            bool allOutputsMatch = true;

            char (*originalData)[NAME_LEN] = new char[sortSize][NAME_LEN];
            char (*mergeData)[NAME_LEN] = new char[sortSize][NAME_LEN];
            char (*heapData)[NAME_LEN] = new char[sortSize][NAME_LEN];

            // Checks whether two char matrices match element-for-element
            auto charMatricesMatch = [](const char (*left)[NAME_LEN], const char (*right)[NAME_LEN],
                                        const int size) -> bool {
                bool match = true;

                for (int i = 0; i < size && match; i++) {
                    if (strcmp(left[i], right[i]) != 0) {
                        match = false;
                    }
                }

                return match;
            };

            printSectionHeader("BENCHMARK 1B: CHAR ARRAY SORTING");
            printLabelValueInt("Array size:", sortSize);
            printLabelValueInt("Trial count:", trialCount);

            for (int trial = 0; trial < trialCount; trial++) {
                fillRandomCharMatrix(originalData, sortSize);

                copyCharMatrix(mergeData, originalData, sortSize);
                copyCharMatrix(heapData, originalData, sortSize);

                high_resolution_clock::time_point startTime = high_resolution_clock::now();
                mergeSortChars(mergeData, 0, sortSize - 1);
                high_resolution_clock::time_point endTime = high_resolution_clock::now();
                totalMergeTime += getElapsedMs(startTime, endTime);

                startTime = high_resolution_clock::now();
                heapSortChars(heapData, sortSize);
                endTime = high_resolution_clock::now();
                totalHeapTime += getElapsedMs(startTime, endTime);

                if (!isCharMatrixSorted(mergeData, sortSize)) {
                    allMergeSorted = false;
                }

                if (!isCharMatrixSorted(heapData, sortSize)) {
                    allHeapSorted = false;
                }

                if (!charMatricesMatch(mergeData, heapData, sortSize)) {
                    allOutputsMatch = false;
                }
            }

            printSubHeader("Results");
            printLabelValueDouble("Merge sort average:", totalMergeTime / trialCount, "ms");
            printLabelValueDouble("Heap sort average:", totalHeapTime / trialCount, "ms");
            printPassFail("Merge sort validation:", allMergeSorted);
            printPassFail("Heap sort validation:", allHeapSorted);
            printPassFail("Sorted outputs match:", allOutputsMatch);

            delete[] originalData;
            delete[] mergeData;
            delete[] heapData;
        }
    };

    /**********************************************************
    Linear search benchmark helpers
    **********************************************************/
    // =============================
    // ADT helper lambdas
    // =============================

    // Fills record array with unique ids and random names
    auto fillRandomRecordArray = [&makeRandomName, &copyNameToCharArray](Record* arr, const int size) -> void
    {
        for (int i = 0; i < size; i++)
        {
            arr[i].id = i + 1;
            string randomName = makeRandomName();
            copyNameToCharArray(arr[i].name, randomName);
        }
    };

    // Builds linked list by copying records from array
    auto buildLinkedListFromArray = [](const Record* arr, const int size) -> Node*
    {
        Node* head = nullptr;
        Node* tail = nullptr;
        Node* newNode = nullptr;

        for (int i = 0; i < size; i++)
        {
            newNode = new Node;
            newNode->data = arr[i];
            newNode->next = nullptr;

            if (head == nullptr)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
        }

        return head;
    };

    // Deletes all nodes in linked list
    auto deleteLinkedList = [](Node*& head) -> void
    {
        Node* current = head;
        Node* nextNode = nullptr;

        while (current != nullptr)
        {
            nextNode = current->next;
            delete current;
            current = nextNode;
        }

        head = nullptr;
    };

    // =============================
    // Linear search helper lambdas
    // =============================

    // Performs linear search on record array by id
    auto arrayLinearSearch = [](const Record* arr, const int size, const int targetId) -> int
    {
        int foundIndex = -1;

        for (int i = 0; i < size && foundIndex == -1; i++)
        {
            if (arr[i].id == targetId)
            {
                foundIndex = i;
            }
        }

        return foundIndex;
    };

    // Performs linear search on linked list by id
    auto linkedListLinearSearch = [](Node* head, const int targetId) -> Node*
    {
        Node* foundNode = nullptr;
        Node* current = head;

        while (current != nullptr && foundNode == nullptr)
        {
            if (current->data.id == targetId)
            {
                foundNode = current;
            }
            else
            {
                current = current->next;
            }
        }

        return foundNode;
    };

    // Checks whether array and linked list searches found equivalent records
    auto searchResultsMatch = [](const Record* arr, const int index, const Node* node) -> bool
    {
        bool match = false;

        if (index == -1 && node == nullptr)
        {
            match = true;
        }
        else if (index != -1 && node != nullptr)
        {
            if (arr[index].id == node->data.id && strcmp(arr[index].name, node->data.name) == 0)
            {
                match = true;
            }
        }

        return match;
    };

    // =============================
    // Search benchmark runner
    // =============================

    // Benchmarks linear search on identical array and linked list datasets
    auto benchmarkSearchStructures = [&]() -> void
    {
        constexpr int dataSizes[] = {10000, 25000, 50000, 100000};
        constexpr int trialCount = DEFAULT_TRIALS;

        for (int sizeIndex = 0; sizeIndex < std::size(dataSizes); sizeIndex++){
            constexpr int searchesPerTrial = 1000;
            const int dataSize = dataSizes[sizeIndex];

            double totalArrayTime = 0.0;
            double totalListTime = 0.0;
            bool allSearchesMatched = true;

            Record* recordArray = new Record[dataSize];
            Node *head = nullptr;

            printSectionHeader("BENCHMARK 2: ARRAY VS LINKED LIST SEARCH");
            printLabelValueInt("Data size:", dataSize);
            printLabelValueInt("Trial count:", trialCount);
            printLabelValueInt("Searches per trial:", searchesPerTrial);

            for (int trial = 0; trial < trialCount; trial++) {
                int arrayResultSink = 0;
                int listResultSink = 0;

                fillRandomRecordArray(recordArray, dataSize);

                if (head != nullptr) {
                    deleteLinkedList(head);
                }

                head = buildLinkedListFromArray(recordArray, dataSize);

                // Generates alternating found and not-found search targets
                auto getSearchTargetId = [&](const int searchIndex) -> int {
                    int targetId = 0;

                    if (searchIndex % 2 == 0) {
                        targetId = recordArray[(searchIndex * 37) % dataSize].id;
                    } else {
                        targetId = -(searchIndex + 1);
                    }

                    return targetId;
                };

                high_resolution_clock::time_point startTime = high_resolution_clock::now();

                for (int search = 0; search < searchesPerTrial; search++)
                {
                    const int targetId = getSearchTargetId(search);
                    const int foundIndex = arrayLinearSearch(recordArray, dataSize, targetId);

                    if (foundIndex != -1) {
                        arrayResultSink += recordArray[foundIndex].id;
                    } else {
                        arrayResultSink += -1;
                    }
                }

                high_resolution_clock::time_point endTime = high_resolution_clock::now();
                totalArrayTime += getElapsedMs(startTime, endTime);

                startTime = high_resolution_clock::now();

                for (int search = 0; search < searchesPerTrial; search++) {
                    const int targetId = getSearchTargetId(search);
                    Node *foundNode = linkedListLinearSearch(head, targetId);

                    if (foundNode != nullptr) {
                        listResultSink += foundNode->data.id;
                    } else {
                        listResultSink += -1;
                    }
                }

                endTime = high_resolution_clock::now();
                totalListTime += getElapsedMs(startTime, endTime);

                for (int search = 0; search < searchesPerTrial && allSearchesMatched; search++) {
                    const int targetId = getSearchTargetId(search);
                    const int arrayIndex = arrayLinearSearch(recordArray, dataSize, targetId);
                    Node *foundNode = linkedListLinearSearch(head, targetId);

                    if (!searchResultsMatch(recordArray, arrayIndex, foundNode)) {
                        allSearchesMatched = false;
                    }
                }

                if (arrayResultSink == -999999999 && listResultSink == -999999999) {
                    cout << "";
                }
            }

            printSubHeader("Results");
            printLabelValueDouble("Array search average:", totalArrayTime / trialCount, "ms");
            printLabelValueDouble("Linked list average:", totalListTime / trialCount, "ms");
            printPassFail("Search results match:", allSearchesMatched);

            deleteLinkedList(head);
            delete[] recordArray;
        }
    };

    /**********************************************************
    Matrix Multiplication Benchmark Helpers
    **********************************************************/
    // =============================
    // Shared matrix helper lambdas
    // =============================

    // Fills matrix with random double values in given range
    auto fillRandomMatrix = [&rng](double* matrix, const int size, const double minValue, const double maxValue) -> void
    {
        uniform_real_distribution<double> dist(minValue, maxValue);

        for (int i = 0; i < size * size; i++)
        {
            matrix[i] = dist(rng);
        }
    };

    // Sets all matrix elements to zero
    auto zeroMatrix = [](double* matrix, const int size) -> void
    {
        for (int i = 0; i < size * size; i++)
        {
            matrix[i] = 0.0;
        }
    };

    // Checks whether two matrices match within floating-point tolerance
    auto matricesMatch = [](const double* left, const double* right, const int size) -> bool
    {
        bool match = true;

        for (int i = 0; i < size * size && match; i++)
        {
            if (fabs(left[i] - right[i]) > EPSILON)
            {
                match = false;
            }
        }

        return match;
    };

    // =============================
    // Matrix multiplication lambdas
    // =============================

    // Multiplies matrices using i-j-k loop ordering
    auto multiplyMatrixIJK = [](const double* a, const double* b, double* c, const int size) -> void
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                for (int k = 0; k < size; k++)
                {
                    c[i * size + j] += a[i * size + k] * b[k * size + j];
                }
            }
        }
    };

    // Multiplies matrices using i-k-j loop ordering
    auto multiplyMatrixIKJ = [](const double* a, const double* b, double* c, const int size) -> void
    {
        for (int i = 0; i < size; i++)
        {
            for (int k = 0; k < size; k++)
            {
                const double aValue = a[i * size + k];

                for (int j = 0; j < size; j++)
                {
                    c[i * size + j] += aValue * b[k * size + j];
                }
            }
        }
    };

    // =============================
    // Matrix benchmark runner
    // =============================

    // Benchmarks matrix multiplication using different loop orderings
    auto benchmarkMatrixMultiplication = [&]() -> void
    {
        constexpr int matrixSizes[] = {64, 128, 256, 512};
        constexpr int trialCount = DEFAULT_TRIALS;

        for (int sizeIndex = 0; sizeIndex < std::size(matrixSizes); sizeIndex++)
            {
            const int matrixSize = matrixSizes[sizeIndex];

            double totalIJKTime = 0.0;
            double totalIKJTime = 0.0;
            bool allResultsMatch = true;

            double* matrixA = new double[matrixSize * matrixSize];
            double *matrixB = new double[matrixSize * matrixSize];
            double *resultIJK = new double[matrixSize * matrixSize];
            double *resultIKJ = new double[matrixSize * matrixSize];

            printSectionHeader("BENCHMARK 3: MATRIX MULTIPLICATION LOOP ORDERING");
            printLabelValueInt("Matrix size:", matrixSize);
            printLabelValueInt("Trial count:", trialCount);

            for (int trial = 0; trial < trialCount; trial++) {
                constexpr double maxValue = 10.0;
                constexpr double minValue = 0.0;
                double ijkSink = 0.0;
                double ikjSink = 0.0;

                fillRandomMatrix(matrixA, matrixSize, minValue, maxValue);
                fillRandomMatrix(matrixB, matrixSize, minValue, maxValue);

                zeroMatrix(resultIJK, matrixSize);
                zeroMatrix(resultIKJ, matrixSize);

                high_resolution_clock::time_point startTime = high_resolution_clock::now();
                multiplyMatrixIJK(matrixA, matrixB, resultIJK, matrixSize);
                high_resolution_clock::time_point endTime = high_resolution_clock::now();
                totalIJKTime += getElapsedMs(startTime, endTime);

                startTime = high_resolution_clock::now();
                multiplyMatrixIKJ(matrixA, matrixB, resultIKJ, matrixSize);
                endTime = high_resolution_clock::now();
                totalIKJTime += getElapsedMs(startTime, endTime);

                if (!matricesMatch(resultIJK, resultIKJ, matrixSize)) {
                    allResultsMatch = false;
                }

                ijkSink += resultIJK[0] + resultIJK[(matrixSize * matrixSize) - 1];
                ikjSink += resultIKJ[0] + resultIKJ[(matrixSize * matrixSize) - 1];

                if (ijkSink == -999999999.0 && ikjSink == -999999999.0) {
                    cout << "";
                }
            }

            printSubHeader("Results");
            printLabelValueDouble("IJK average:", totalIJKTime / trialCount, "ms");
            printLabelValueDouble("IKJ average:", totalIKJTime / trialCount, "ms");
            printPassFail("Matrix results match:", allResultsMatch);

            delete[] matrixA;
            delete[] matrixB;
            delete[] resultIJK;
            delete[] resultIKJ;
        }
    };

    /******************************************************
    Benchmark execution
    ******************************************************/

    benchmarkIntegerSorting();
    benchmarkCharSorting();
    benchmarkSearchStructures();
    benchmarkMatrixMultiplication();

    return 0;
}