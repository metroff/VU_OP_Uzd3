# DIY Vektorius
- [Vector::push_back](#vectorpush_back)
- [Vector::assign](#vectorassign)
- [Vector::at](#vectorat)
- [Vector::shrink_to_fit](#vectorshrink_to_fit)
- [Vector::clear](#vectorclear)
- [Palyginimo operatoriai](#palyginimo-operatoriai)

# Vector::push_back
## Spartos su `std::vector` palyginimas
| Tipas \ Elementų kiekis | 10000    | 100000  | 1000000 | 10000000| 100000000 |
| :---:                   | :---:    | :---:   | :---:   | :---:   | :---:     |
| std::vector             | 0.00005  | 0.00043 | 0.00425 | 0.04858 | 0.39814   |
| custom vector           | 0.00003  | 0.00027 | 0.00291 | 0.02849 | 0.25466   |

### Atminties perskirstymas (100000000 elementų)
| std::vector | custom vector | 
| :---:       |     :---:     |
| 27          |      27       |

Savadarbis vektorius `Vector` veikia žymiai greičiau nei `std::vector`, o atminties perskirstymo kiekis yra vienodas.
### Savadarbio `Vector` naudojimas praeitame darbe
| 100000        | Nuskaitymas | Rūšiavimas | Skirstymas | Išvedimas | Iš viso |
| :---:         | :---:       | :---:      | :---:      | :---:     | :---:   |
| std::vector   | 0.35237     | 0.04005    | 0.00794    | 0.16850   | 0.56888 |
| custom vector | 0.35277     | 0.04045    | 0.01170    | 0.16149   | 0.56642 |

Šis `Vector` nenusileidžia `std::vector` pagal greitumą.

---
## Realizacija

```c++
void push_back(const value_type& val);
void push_back(const value_type&& val);
```
Prideda nurodytą reikšmę (`val`) prie `Vector` konteinerio galo.

## Naudojimo pavyzdys
```c++
#include "Vector.hpp"
#include <iostream>
 
int main()
{
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
```
## Rezultatas
```
Vektoriuje esantys elementai:
Vektoriuje esantys elementai: abc
Vektoriuje esantys elementai: abc def
```

# Vector::assign
## Realizacija
```c++
void assign(size_type count, const T& value);
void assign(InputIt first, InputIt last);
void assign(std::initializer_list<T> init);
```
Ši funkcija pakeičia `Vector` konteineryje laikomą informaciją. Realizuota (overload):
1. Konteinerio informacija pakeičiama nurodyta reikšme (`value`) pakartojant `count` kartų.
2. Konteinerio informacija pakeičiama pateikiant pirmą ir paskutinę kito konteinerio rodyklę (iterator).
3. Konteinerio informacija pakeičiama pateikiant inicalizacijos sąrašą.

## Naudojimo pavyzdys
```c++
#include "Vector.hpp"
#include <iostream>
 
int main()
{
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
```
## Rezultatas
```
1 1 1 1 1
2 2 2 2 2 2
1 2 3 2 1
```

# Vector::at
## Realizacija
```c++
reference at(size_type pos);
```
Ši funkcija grąžina rodykle į elementą, kuris yra indekso (`pos`) vietoje. Taip pat patikrina ar indeksas (`pos`) yra konteinerio ribose.

## Naudojimo pavyzdys
```c++
#include "Vector.hpp"
#include <iostream>
 
int main()
{
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
```
## Rezultatas
```
Antras elementas vektoriuje: 12
[Vector::at] Index out of range!
```

# Vector::shrink_to_fit
## Realizacija
```c++
void shrink_to_fit()
```
Ši funkcija panaikina visą atmintyje išskirtą, bet nepanaudotą vietą (talpa sumažinama iki elementų skaičiaus).

## Naudojimo pavyzdys
```c++
#include "Vector.hpp"
#include <iostream>
 
int main()
{
    Vector<int> numbers;
    std::cout << "Naujai sukurto vektoriaus talpa: " << numbers.capacity() << '\n';
    // Pridedama 300 naujų elementų
    for (int i = 0; i < 300; ++i)
        numbers.push_back(i);
    std::cout << "Talpa po 300 elementu pridejimo: " << numbers.capacity() << '\n';
    numbers.shrink_to_fit();
    std::cout << "Talpa po shrink_to_fit(): " << numbers.capacity() << '\n';
}
```
## Rezultatas
```
Naujai sukurto vektoriaus talpa: 0
Talpa po 300 elementu pridejimo: 512
Talpa po shrink_to_fit(): 300
```

# Vector::clear
## Realizacija
```c++
void clear()
```
Ši funkcija ištrina visus elementus esančius `Vector` konteineryje ir atlaisvina atmintį.

## Naudojimo pavyzdys
```c++
#include "Vector.hpp"
#include <iostream>
 
int main()
{
    std::cout << "Vektoriaus talpa: " << numbers.capacity() << ", elementu kiekis: " << numbers.size() << '\n';
    // Pridedama 300 naujų elementų
    numbers.clear();
    // Po išvalymo
    std::cout << "Vektoriaus talpa: " << numbers.capacity() << ", elementu kiekis: " << numbers.size() << '\n';
}
```
## Rezultatas
```
Vektoriaus talpa: 3, elementu kiekis: 3
Vektoriaus talpa: 0, elementu kiekis: 0
```

# Palyginimo operatoriai
## Realizacija
```c++
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs);
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs);
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs);
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs);
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs);
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs);
```
Palygina dviejų `Vector` konteinerių elementus.

## Naudojimo pavyzdys
```c++
#include "Vector.hpp"
#include <iostream>
 
int main()
{
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
```
## Rezultatas
```
pirmas == antras grazina false
pirmas != antras grazina true
pirmas <  antras grazina true
pirmas <= antras grazina true
pirmas >  antras grazina false
pirmas >= antras grazina false
```