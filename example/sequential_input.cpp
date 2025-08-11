#include <iostream>

int main() {
    const int N = 1000000;
    int *arr = new int[N];
    long long total = 0;
    long long sum = 0;

    for (int i = 0; i < N; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < N; i++) {
        total += arr[i];
        sum+=arr[i+1];
    }

    std::cout << "Sum: " << total << std::endl;
    delete[] arr;
    return 0;
}
