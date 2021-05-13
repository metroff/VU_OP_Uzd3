#include "Vector.hpp"
#include <iostream>
#include "timer.hpp"
#include <vector>
 
void testAssign();
void testAt();
void testShrinkToFit();
void testClear();
void testOperators();
void testPushBack();

int main()
{
    int sizes[5] {10000, 100000, 1000000, 10000000, 100000000};
    double partTime;
    Timer timer;
    
    for (int j = 0; j < 5; j++) {
        int sz = sizes[j];
        timer.reset();
        std::vector<int> v1;
        for (int i = 1; i <= sz; ++i)
            v1.push_back(i);
        
        partTime = timer.elapsed();
        std::cout << sz << " std::vector laikas: " << partTime << std::endl;
        timer.reset();

        Vector<int> v2;
        for (int i = 1; i <= sz; ++i)
            v2.push_back(i);
        
        partTime = timer.elapsed();
        std::cout << sz << " Custom vector laikas: " << partTime << std::endl;
    }

    std::vector<int> vecstd;
    Vector<int> vec;
    int vecstdTimes = 0, vecTimes = 0;
    for (int i = 1; i <= 100000000; ++i) {
        vecstd.push_back(i);
        vec.push_back(i);
        if (vecstd.size() == vecstd.capacity())
            vecstdTimes++;
        if (vec.size() == vec.capacity())
            vecTimes++;
    }
    std::cout << "Std::vector atminties perskirstymas ivyko: " << vecstdTimes << " kartus." << std::endl;
    std::cout << "Custom vector atminties perskirstymas ivyko: " << vecTimes << " kartus." << std::endl;

    // testAssign();
    // testAt();
    // testShrinkToFit();
    // testClear();
    // testPushBack();
}

void testAssign() {
    std::cout << "Assign\n";
    Vector<int> numbers;
 
    auto print_vector = [&](){
        for (int number : numbers)
            std::cout << number << ' ';
        std::cout << '\n';  
    };
 
    // 1
    numbers.assign(5, 1);
    print_vector();
    // 2
    const Vector<int> extra(6, 2);
    numbers.assign(extra.begin(), extra.end());
    print_vector();
    // 3
    numbers.assign({1, 2, 3, 2, 1});
    print_vector();
}

void testAt() {
    std::cout << "At\n";
    Vector<int> numbers = {1, 2, 3, 4, 5};
    // Pakeičiamas skatmuo iš 2 į 12
    numbers.at(1) = 12;
    // Spausdianama reikšmė esanti 2 vietoje  
    std::cout << "Antras elementas vektoriuje: " << numbers.at(1) << "\n";
    try {
        // Pakeičiamas 6 skatmuo, kurio nėra konteineryje
        numbers.at(6) = 123;
    } catch (std::out_of_range e) {
        std::cout << e.what() << '\n';
    }
}

void testShrinkToFit() {
    std::cout << "ShrinkToFit\n";
    Vector<int> numbers;
    std::cout << "Naujai sukurto vektoriaus talpa: " << numbers.capacity() << '\n';
    // Pridedama 300 naujų elementų
    for (int i = 0; i < 300; ++i)
        numbers.push_back(i);
    std::cout << "Talpa po 300 elementu pridejimo: " << numbers.capacity() << '\n';
    numbers.shrink_to_fit();
    std::cout << "Talpa po shrink_to_fit(): " << numbers.capacity() << '\n';
}

void testClear() {
    std::cout << "Clear\n";
    Vector<int> numbers = {1, 2, 3};
    std::cout << "Vektoriaus talpa: " << numbers.capacity() << ", elementu kiekis: " << numbers.size() << '\n';
    // Pridedama 300 naujų elementų
    numbers.clear();
    // Po išvalymo
    std::cout << "Vektoriaus talpa: " << numbers.capacity() << ", elementu kiekis: " << numbers.size() << '\n';
}

void testOperators() {
    std::cout << "Operators\n";
    Vector<int> first{1, 2, 3};
    Vector<int> second{7, 8, 9, 10};
 
    std::cout << std::boolalpha;
 
    // Palyginama ar lygūs
    std::cout << "pirmas == antras grazina " << (first == second) << '\n';
    std::cout << "pirmas != antras grazina " << (first != second) << '\n';
    // Palyginama daugiau, mažiau arba lygūs
    std::cout << "pirmas <  antras grazina " << (first < second) << '\n';
    std::cout << "pirmas <= antras grazina " << (first <= second) << '\n';
    std::cout << "pirmas >  antras grazina " << (first > second) << '\n';
    std::cout << "pirmas >= antras grazina " << (first >= second) << '\n';
}

void testPushBack() {
    std::cout << "PushBack\n";
    Vector<std::string> strings;

    std::cout << "Vektoriuje esantys elementai: ";
    for (auto str: strings) std::cout << str << " ";
    std::cout << "\n";
 
    strings.push_back("abc");

    std::cout << "Vektoriuje esantys elementai: ";
    for (auto str: strings) std::cout << str << " ";
    std::cout << "\n";

    std::string s = "def";
    strings.push_back(std::move(s));

    std::cout << "Vektoriuje esantys elementai: ";
    for (auto str: strings) std::cout << str << " ";
    std::cout << "\n";
}