#include <iostream>
#include <list>
#include <queue>
#include <thread>
#include <chrono>
#include <functional>
#include <mutex>
#include <string>
#include <condition_variable>
#include <future>
#include <vector>

void merge(std::vector<int>& input, int left, int mid, int right)
{
    int size_left = mid - left + 1;
    int size_right = right - mid;

    std::vector<int> tmp_left(size_left);
    std::vector<int> tmp_right(size_right);

    for (int i = 0; i < size_left; ++i)
    {
        tmp_left[i] = input[left + i];
    }

    for (int i = 0; i < size_right; ++i)
    {
        tmp_right[i] = input[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while ((i < size_left) && (j < size_right))
    {
        if (tmp_left[i] <= tmp_right[j])
        {
            input[k] = tmp_left[i];
            i++;
        }
        else
        {
            input[k] = tmp_right[j];
            j++;
        }
        k++;
    }

    while (i < size_left)
    {
        input[k] = tmp_left[i];
        i++;
        k++;
    }

    while (j < size_right)
    {
        input[k] = tmp_right[j];
        j++;
        k++;
    }
}

void MergeSort(std::vector<int>& input, int left, int right)
{
    if (left >= right) return;

    int mid = (left + right - 1) / 2;

    MergeSort(input, left, mid);

    MergeSort(input, mid + 1, right);

    merge(input, left, mid, right);
}


void MergeSortMultithread(std::vector<int>& input, int left, int right, bool multithread_enable = true)
{
    if (left >= right) return;

    int mid = (left + right - 1) / 2;

    int size = right - left;

    if (size < 10000)
    {
        MergeSortMultithread(input, left, mid, false);

        MergeSortMultithread(input, mid + 1, right, false);

    }
    else
    {
        auto f = std::async(std::launch::async, MergeSortMultithread, std::reference_wrapper<std::vector<int> >(input), left, mid, multithread_enable);


        MergeSortMultithread(input, mid + 1, right, multithread_enable);

    }
    merge(input, left, mid, right);
}


void printv(const std::vector<int>& v)
{
    for (size_t i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
}

void copyv(std::vector<int>& v1, std::vector<int>& v2)
{
    for (size_t i = 0; i < v1.size(); ++i)
    {
        v2[i] = v1[i];
    }
}

void checkv(const std::vector<int>& v)
{
    for (size_t i = 0; i < v.size() - 1; ++i)
    {
        if (v[i] > v[i + 1])
        {
            std::cout << "BAD\n";
            return;
        }
    }
    std::cout << "FINE\n";
}

int main()
{
    srand(423423);
    std::vector<int> test(200000);

    for (size_t i = 0; i < test.size(); ++i)
    {
        test[i] = rand() % 10000;
    }

    std::vector<int>test2(200000);
    copyv(test, test2);

    std::cout << "SINGLE THREAD:\n";
    auto start1 = std::chrono::high_resolution_clock::now();
    MergeSort(test2, 0, test2.size() - 1);
    auto stop1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time1 = stop1 - start1;
    std::cout << "Elapsed time = " << elapsed_time1.count() << " sec\n";
    std::cout << "\n\n\n";

    std::cout << "MULTIPLE THREAD:\n";
    auto start2 = std::chrono::high_resolution_clock::now();
    MergeSortMultithread(test, 0, test.size() - 1);
    auto stop2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time2 = stop2 - start2;
    std::cout << "Elapsed time = " << elapsed_time2.count() << " sec\n";
    std::cout << "\n\n\n";
    checkv(test);
    checkv(test2);

    return 0;
}