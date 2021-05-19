#include <iostream>
#include "type_list.h"


template <class TypeList>
void printTypeList() {
    std::cout << typeid(typename TypeList::Head).name() << " ";
    printTypeList<typename TypeList::Tail>();
};

template <>
void printTypeList<NullType>() {
    std::cout << '\n';
}

template <>
void printTypeList<TypeList<>>() {
    std::cout << "EmptyList\n";
};


int main () {
    std::cout << "Initial typelist = {int, char, std::ios_base, char}\n";
    using TL = TypeList<int, char, std::ios_base, char>;

    printTypeList<TL>();


    std::cout << "\nErasing char\n";

    using TL2 = Erase<TL, char>::type;

    printTypeList<TL2>();


    std::cout << "\nErasing another char\n";

    using TL3 = Erase<TL2, char>::type;

    printTypeList<TL3>();


    std::cout << "\nErasing long\n";

    using TL4 = Erase<TL3, long>::type;

    printTypeList<TL4>();


    std::cout << "\nErasing int and then std::ios_base\n";

    using TL5 = Erase<Erase<TL4, int>::type, std::ios_base>::type;

    printTypeList<TL5>();


    std::cout << "\nErasing int from empty\n";

    using TL6 = Erase<TL5, int>::type;

    printTypeList<TL6>();
}
