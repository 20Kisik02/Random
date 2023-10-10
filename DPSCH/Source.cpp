#include <iostream>
#include <random>
#include <vector>
#include <cstdint>
#include <locale> 

class CustomRandomGenerator {
public:
    CustomRandomGenerator(int64_t seed = 42) : Xn(seed) {
        // ��������� ��������� ������������� ������
        a = 1664525;
        c = 1013904223;
        m = 4294967296;
    }

    int64_t getRandomNumber() {
        Xn = (a * Xn + c) % m;
        return Xn % 70; // ����������� ��������� �� 0 �� 70
    }

private:
    int64_t Xn;
    int64_t a;
    int64_t c;
    int64_t m;
};

int main() {
    setlocale(LC_ALL, "Russian");
    const int num_samples = 100;
    const int num_intervals = 7; // 7 ���������� �� 10 ����� ������

    CustomRandomGenerator custom_generator;

    std::vector<int64_t> custom_random_numbers;
    for (int i = 0; i < num_samples; ++i) {
        int64_t random_number = custom_generator.getRandomNumber();
        custom_random_numbers.push_back(random_number);
    }

    std::vector<int64_t> std_random_numbers;
    std::mt19937_64 std_generator(42);
    for (int i = 0; i < num_samples; ++i) {
        int64_t random_number = std_generator() % 70;
        std_random_numbers.push_back(random_number);
    }

    // ������� ������� ��� �������� ����� � ������ ���������
    std::vector<int> custom_interval_count(num_intervals, 0); // ��� ����� ����������
    std::vector<int> std_interval_count(num_intervals, 0);    // ��� ������������ ����������

    // ������� ����� � ������ ��������� � ���������� ����������
    for (unsigned int number : custom_random_numbers) {
        int interval = number / 10;
        if (interval < num_intervals) {
            custom_interval_count[interval]++;
        }
    }

    for (unsigned int number : std_random_numbers) {
        int interval = number / 10;
        if (interval < num_intervals) {
            std_interval_count[interval]++;
        }
    }

    // ���������� ���������� ����������
    std::cout << "����������� ��� ����� ����������:" << std::endl;
    for (int i = 0; i < num_intervals; ++i) {
        std::cout << "[" << i * 10 << " - " << (i + 1) * 10 - 1 << "]: ";
        for (int j = 0; j < custom_interval_count[i]; ++j) {
            std::cout << "-";
        }
        std::cout << " (�����: " << custom_interval_count[i] << ")" << std::endl;
    }

    std::cout << "����������� ��� ������������ ����������:" << std::endl;
    for (int i = 0; i < num_intervals; ++i) {
        std::cout << "[" << i * 10 << " - " << (i + 1) * 10 - 1 << "]: ";
        for (int j = 0; j < std_interval_count[i]; ++j) {
            std::cout << "-";
        }
        std::cout << " (�����: " << std_interval_count[i] << ")" << std::endl;
    }

    // �������������� �����: ���� ��-�������
    double chi_square_custom = 0.0;
    double chi_square_std = 0.0;

    for (int i = 0; i < num_intervals; ++i) {
        double expected = static_cast<double>(num_samples) / num_intervals; // ����������� ������������� �� ����������
        chi_square_custom += pow(custom_interval_count[i] - expected, 2) / expected;
        chi_square_std += pow(std_interval_count[i] - expected, 2) / expected;
    }

    std::cout << "�������� ��-������� ��� ����� ����������: " << chi_square_custom << std::endl;
    std::cout << "�������� ��-������� ��� ������������ ����������: " << chi_square_std << std::endl;

    return 0;
}
