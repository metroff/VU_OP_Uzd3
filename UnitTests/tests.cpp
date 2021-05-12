#include <iostream>
#include <gtest/gtest.h>
#include "../Vector.hpp"
#include <string>
#include <algorithm>
#include <numeric>

TEST(MemberFunctions, Constructor){
    Vector<std::string> words1 {"the", "frogurt", "is", "also", "cursed"};

    EXPECT_EQ(words1.size(), 5);
    EXPECT_EQ(words1[0], "the");
    EXPECT_EQ(words1[1], "frogurt");
    EXPECT_EQ(words1[2], "is");
    EXPECT_EQ(words1[3], "also");
    EXPECT_EQ(words1[4], "cursed");
 
    Vector<std::string> words2(words1.begin(), words1.end());

    EXPECT_EQ(words2.size(), 5);
    EXPECT_EQ(words2[0], "the");
    EXPECT_EQ(words2[1], "frogurt");
    EXPECT_EQ(words2[2], "is");
    EXPECT_EQ(words2[3], "also");
    EXPECT_EQ(words2[4], "cursed");
 
    Vector<std::string> words3(words1);

    EXPECT_EQ(words3.size(), 5);
    EXPECT_EQ(words3[0], "the");
    EXPECT_EQ(words3[1], "frogurt");
    EXPECT_EQ(words3[2], "is");
    EXPECT_EQ(words3[3], "also");
    EXPECT_EQ(words3[4], "cursed");
 
    Vector<std::string> words4(5, "Mo");

    EXPECT_EQ(words4.size(), 5);
    EXPECT_EQ(words4[0], "Mo");
    EXPECT_EQ(words4[1], "Mo");
    EXPECT_EQ(words4[2], "Mo");
    EXPECT_EQ(words4[3], "Mo");
    EXPECT_EQ(words4[4], "Mo");
}

TEST(MemberFunctions, Destructor){
    Vector<std::string> words1 {"the", "frogurt", "is", "also", "cursed"};

    EXPECT_EQ(words1.size(), 5);
    words1.~Vector();
    EXPECT_EQ(words1.size(), 0);
}

TEST(MemberFunctions, AssignmentOperator){
    Vector<int> nums1 {3, 1, 4, 6, 5, 9};
    Vector<int> nums2;
    Vector<int> nums3;

    EXPECT_EQ(nums1.size(), 6);
    EXPECT_EQ(nums2.size(), 0);
    EXPECT_EQ(nums3.size(), 0);
 
    nums2 = nums1;

    EXPECT_EQ(nums1.size(), 6);
    EXPECT_EQ(nums2.size(), 6);
    EXPECT_EQ(nums3.size(), 0);

    nums3 = std::move(nums1);
 
    EXPECT_EQ(nums1.size(), 0);
    EXPECT_EQ(nums2.size(), 6);
    EXPECT_EQ(nums3.size(), 6);
 
    EXPECT_EQ(nums3[0], 3);
    EXPECT_EQ(nums3[1], 1);
    EXPECT_EQ(nums3[2], 4);
    EXPECT_EQ(nums3[3], 6);
    EXPECT_EQ(nums3[4], 5);
    EXPECT_EQ(nums3[5], 9);

    nums3 = {1, 2, 3};

    EXPECT_EQ(nums3[0], 1);
    EXPECT_EQ(nums3[1], 2);
    EXPECT_EQ(nums3[2], 3);
}

TEST(MemberFunctions, Assign){
    Vector<char> characters;
 
    auto print_vector = [&](){
        for (char c : characters)
            std::cout << c << ' ';
        std::cout << '\n';  
    };
 
    characters.assign(5, 'a');
    
    EXPECT_EQ(characters[0], 'a');
    EXPECT_EQ(characters[1], 'a');
    EXPECT_EQ(characters[2], 'a');
    EXPECT_EQ(characters[3], 'a');
    EXPECT_EQ(characters[4], 'a');
 
    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    
    EXPECT_EQ(characters[0], 'b');
    EXPECT_EQ(characters[1], 'b');
    EXPECT_EQ(characters[2], 'b');
    EXPECT_EQ(characters[3], 'b');
    EXPECT_EQ(characters[4], 'b');
    EXPECT_EQ(characters[5], 'b');
 
    characters.assign({'C', '+', '+', '1', '1'});
    
    EXPECT_EQ(characters[0], 'C');
    EXPECT_EQ(characters[1], '+');
    EXPECT_EQ(characters[2], '+');
    EXPECT_EQ(characters[3], '1');
    EXPECT_EQ(characters[4], '1');
}

TEST(ElementAccess , At){
    Vector<int> data = { 1, 2, 4, 5, 5, 6 };
 
    data.at(1) = 88;

    EXPECT_EQ(data.at(2), 4);
    EXPECT_EQ(data.size(), 6);
 
    int err = 0;
    try {
        // Set element 6
        data.at(6) = 666;
    } catch (std::out_of_range const& exc) {
        err = 1;
    }

    EXPECT_EQ(err, 1);
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 88);
    EXPECT_EQ(data[2], 4);
    EXPECT_EQ(data[3], 5);
    EXPECT_EQ(data[4], 5);
    EXPECT_EQ(data[5], 6);
}

TEST(ElementAccess, AccessOperator){
    Vector<int> numbers {2, 4, 6, 8};

    EXPECT_EQ(numbers[1], 4);
 
    numbers[0] = 5;
 
    EXPECT_EQ(numbers[0], 5);
    EXPECT_EQ(numbers[1], 4);
    EXPECT_EQ(numbers[2], 6);
    EXPECT_EQ(numbers[3], 8);
}

TEST(ElementAccess, Front){
    Vector<char> letters {'o', 'm', 'g', 'w', 't', 'f'};
 
    EXPECT_EQ(letters.front(), 'o');
}

TEST(ElementAccess, Back){
    Vector<char> letters {'o', 'm', 'g', 'w', 't', 'f'};
 
    EXPECT_EQ(letters.back(), 'f');
}

TEST(ElementAccess, Data){
    Vector<int> container { 1, 2, 3, 4 };

    EXPECT_EQ(container.data()[0], 1);
    EXPECT_EQ(container.data()[1], 2);
    EXPECT_EQ(container.data()[2], 3);
    EXPECT_EQ(container.data()[3], 4);
}

TEST(Iterators, Begin){
    Vector<int> nums {1, 2, 4, 8, 16};
    Vector<std::string> fruits {"orange", "apple", "raspberry"};
    Vector<char> empty;

    EXPECT_EQ(std::accumulate(nums.begin(), nums.end(), 0), 31);

    EXPECT_EQ(*fruits.begin(), "orange");

    EXPECT_EQ(empty.begin(), empty.end());
}

TEST(Iterators, End){
    Vector<int> nums {1, 2, 4, 8, 16};
    Vector<std::string> fruits {"orange", "apple", "raspberry"};
    Vector<char> empty;

    EXPECT_EQ(std::accumulate(nums.begin(), nums.end(), 0), 31);

    EXPECT_EQ(*fruits.begin(), "orange");

    EXPECT_EQ(empty.begin(), empty.end());
}

TEST(Iterators, RBegin){
    Vector<int> nums {1, 2, 4, 8, 16};
    Vector<std::string> fruits {"orange", "apple", "raspberry"};
    Vector<char> empty;

    EXPECT_EQ(std::accumulate(nums.rbegin(), nums.rend(), 0), 31);

    EXPECT_EQ(*fruits.rbegin(), "raspberry");

    EXPECT_EQ(empty.rbegin(), empty.rend());
}

TEST(Iterators, REnd){
    Vector<int> nums {1, 2, 4, 8, 16};
    Vector<std::string> fruits {"orange", "apple", "raspberry"};
    Vector<char> empty;

    EXPECT_EQ(std::accumulate(nums.rbegin(), nums.rend(), 0), 31);

    EXPECT_EQ(*fruits.rbegin(), "raspberry");

    EXPECT_EQ(empty.rbegin(), empty.rend());
}

TEST(Capacity, Empty){
    Vector<int> numbers;

    EXPECT_EQ(numbers.empty(), true);
 
    numbers.push_back(42);

    EXPECT_EQ(numbers.empty(), false);
}

TEST(Capacity, Size){
    Vector<int> nums {1, 3, 5, 7};

    EXPECT_EQ(nums.size(), 4);
}

TEST(Capacity, Reserve){
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);

    for (int i = 1; i < 10; ++i)
        v.push_back(i);

    EXPECT_EQ(v.size(), 9);
    EXPECT_EQ(v.capacity(), 16);

    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 16);

    v.resize(20);
    EXPECT_EQ(v.size(), 20);
    EXPECT_EQ(v.capacity(), 32);

    v.resize(1);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), 32);
}

TEST(Capacity, Capacity){
    int sz = 200;
    Vector<int> v1;
 
    EXPECT_EQ(v1.capacity(), 0);
 
    for (int n = 0; n < sz; ++n) {
        v1.push_back(n);
    }
 
    EXPECT_EQ(v1.size(), 200);
    EXPECT_EQ(v1.capacity(), 256);
}

TEST(Capacity, ShrinkToFit){
    Vector<int> v;
    EXPECT_EQ(v.capacity(), 0);
    v.resize(100);
    EXPECT_EQ(v.capacity(), 100);
    v.resize(50);
    EXPECT_EQ(v.capacity(), 100);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 50);
    v.clear();
    EXPECT_EQ(v.capacity(), 0);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 0);
    for (int i = 1000; i < 1300; ++i)
        v.push_back(i);
    EXPECT_EQ(v.capacity(), 512);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 300);
}

TEST(Modifiers, Clear){
    Vector<int> container{1, 2, 3};

    EXPECT_EQ(container.size(), 3);
    EXPECT_EQ(container.capacity(), 3);
 
    container.clear();
 
    EXPECT_EQ(container.size(), 0);
    EXPECT_EQ(container.capacity(), 0);
}

TEST(Modifiers, Insert){
    Vector<int> vec(3,100);
    EXPECT_EQ(vec[0], 100);
    EXPECT_EQ(vec[1], 100);
    EXPECT_EQ(vec[2], 100);
 
    auto it = vec.begin();
    it = vec.insert(it, 200);
    EXPECT_EQ(vec[0], 200);
    EXPECT_EQ(vec[1], 100);
    EXPECT_EQ(vec[2], 100);
    EXPECT_EQ(vec[3], 100);
 
    vec.insert(it,2,300);
    EXPECT_EQ(vec[0], 300);
    EXPECT_EQ(vec[1], 300);
    EXPECT_EQ(vec[2], 200);
    EXPECT_EQ(vec[3], 100);
    EXPECT_EQ(vec[4], 100);
    EXPECT_EQ(vec[5], 100);
 
    it = vec.begin();
 
    Vector<int> vec2(2,400);
    vec.insert(it+2, vec2.begin(), vec2.end());
    EXPECT_EQ(vec[0], 300);
    EXPECT_EQ(vec[1], 300);
    EXPECT_EQ(vec[2], 400);
    EXPECT_EQ(vec[3], 400);
    EXPECT_EQ(vec[4], 200);
    EXPECT_EQ(vec[5], 100);
    EXPECT_EQ(vec[6], 100);
    EXPECT_EQ(vec[7], 100);
 
    int arr[] = { 501,502,503 };
    vec.insert(vec.begin(), arr, arr+3);
    EXPECT_EQ(vec[0], 501);
    EXPECT_EQ(vec[1], 502);
    EXPECT_EQ(vec[2], 503);
    EXPECT_EQ(vec[3], 300);
    EXPECT_EQ(vec[4], 300);
    EXPECT_EQ(vec[5], 400);
    EXPECT_EQ(vec[6], 400);
    EXPECT_EQ(vec[7], 200);
    EXPECT_EQ(vec[8], 100);
    EXPECT_EQ(vec[9], 100);
    EXPECT_EQ(vec[10], 100);
}

TEST(Modifiers, Emplace){
    Vector<std::string> container;
    // reserve enough place so vector does not have to resize
    container.reserve(10);
 
    container.emplace(container.end(), "one");
 
    container.emplace(container.end(), "two");
 
    container.emplace(container.end(), std::move("three"));

    EXPECT_EQ(container[0], "one");
    EXPECT_EQ(container[1], "two");
    EXPECT_EQ(container[2], "three");
}

TEST(Modifiers, Erase){
    Vector<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 
    c.erase(c.begin());
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);
    EXPECT_EQ(c[3], 4);
    EXPECT_EQ(c[4], 5);
    EXPECT_EQ(c[5], 6);
    EXPECT_EQ(c[6], 7);
    EXPECT_EQ(c[7], 8);
    EXPECT_EQ(c[8], 9);
 
    c.erase(c.begin()+2, c.begin()+5);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 6);
    EXPECT_EQ(c[3], 7);
    EXPECT_EQ(c[4], 8);
    EXPECT_EQ(c[5], 9);
 
    // Erase all even numbers (C++11 and later)
    for (auto it = c.begin(); it != c.end(); ) {
        if (*it % 2 == 0) {
            it = c.erase(it);
        } else {
            ++it;
        }
    }
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 7);
    EXPECT_EQ(c[2], 9);
}

TEST(Modifiers, PushBack){
    Vector<std::string> letters;
 
    letters.push_back("abc");
    std::string s = "def";
    letters.push_back(std::move(s));

    EXPECT_EQ(letters[0], "abc");
    EXPECT_EQ(letters[1], "def");
}

TEST(Modifiers, EmplaceBack){
    Vector<std::string> person;
    person.emplace_back("Nelson Mandela");
    person.emplace_back("South Africa");
    person.emplace_back("1994");

    EXPECT_EQ(person[0], "Nelson Mandela");
    EXPECT_EQ(person[1], "South Africa");
    EXPECT_EQ(person[2], "1994");
}

TEST(Modifiers, PopBack){
    Vector<int> numbers;

    numbers.push_back(5);
    numbers.push_back(3);
    numbers.push_back(4);

    EXPECT_EQ(numbers[0], 5);
    EXPECT_EQ(numbers[1], 3);
    EXPECT_EQ(numbers[2], 4);
 
    numbers.pop_back();
 
    EXPECT_EQ(numbers[0], 5);
    EXPECT_EQ(numbers[1], 3);
    int err = 0;
    try {
        numbers.at(2);
    } catch (std::out_of_range const& exc) {
        err = 1;
    }
    EXPECT_EQ(err, 1);
}

TEST(Modifiers, Resize){
    Vector<int> c = {1, 2, 3};
    c.resize(5);
    EXPECT_EQ(c.size(), 5);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);
    EXPECT_EQ(c[3], 0);
    EXPECT_EQ(c[4], 0);
    
    c.resize(2);
    EXPECT_EQ(c.size(), 2);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    int err = 0;
    try {
        c.at(2);
    } catch (std::out_of_range const& exc) {
        err = 1;
    }
    EXPECT_EQ(err, 1);
}

TEST(Modifiers, Swap){
    Vector<int> a1{1, 2, 3}, a2{4, 5};
 
    auto it1 = std::next(a1.begin());
    auto it2 = std::next(a2.begin());
 
    int& ref1 = a1.front();
    int& ref2 = a2.front();
 
    EXPECT_EQ(a1[0], 1);
    EXPECT_EQ(a1[1], 2);
    EXPECT_EQ(a1[2], 3);
    EXPECT_EQ(a2[0], 4);
    EXPECT_EQ(a2[1], 5);
    EXPECT_EQ(*it1, 2);
    EXPECT_EQ(*it2, 5);
    EXPECT_EQ(ref1, 1);
    EXPECT_EQ(ref2, 4);
    a1.swap(a2);
    EXPECT_EQ(a2[0], 1);
    EXPECT_EQ(a2[1], 2);
    EXPECT_EQ(a2[2], 3);
    EXPECT_EQ(a1[0], 4);
    EXPECT_EQ(a1[1], 5);
    EXPECT_EQ(*it1, 2);
    EXPECT_EQ(*it2, 5);
    EXPECT_EQ(ref1, 1);
    EXPECT_EQ(ref2, 4);
}

TEST(NonMemberFunctions, OperatorOverload){
    Vector<int> alice{1, 2, 3};
    Vector<int> bob{7, 8, 9, 10};
    Vector<int> eve{1, 2, 3};

    EXPECT_EQ((alice == bob), false);
    EXPECT_EQ((alice != bob), true);
    EXPECT_EQ((alice < bob), true);
    EXPECT_EQ((alice <= bob), true);
    EXPECT_EQ((alice > bob), false);
    EXPECT_EQ((alice >= bob), false);

    EXPECT_EQ((alice == eve), true);
    EXPECT_EQ((alice != eve), false);
    EXPECT_EQ((alice < eve), false);
    EXPECT_EQ((alice <= eve), true);
    EXPECT_EQ((alice > eve), false);
    EXPECT_EQ((alice >= eve), true);
}

TEST(NonMemberFunctions, Swap){
    Vector<int> alice{1, 2, 3};
    Vector<int> bob{7, 8, 9, 10};
 
    EXPECT_EQ(alice[0], 1);
    EXPECT_EQ(alice[1], 2);
    EXPECT_EQ(alice[2], 3);
    EXPECT_EQ(bob[0], 7);
    EXPECT_EQ(bob[1], 8);
    EXPECT_EQ(bob[2], 9);
    EXPECT_EQ(bob[3], 10);

    std::swap(alice, bob);
 
    EXPECT_EQ(bob[0], 1);
    EXPECT_EQ(bob[1], 2);
    EXPECT_EQ(bob[2], 3);
    EXPECT_EQ(alice[0], 7);
    EXPECT_EQ(alice[1], 8);
    EXPECT_EQ(alice[2], 9);
    EXPECT_EQ(alice[3], 10);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}