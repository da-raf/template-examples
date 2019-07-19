#include <iostream>

// machinery
namespace operator_check {
    struct No {};
    template <typename T, typename Arg> No operator+ (const T&, const Arg&);

    template <typename T, typename Arg = T>
    struct PlusExists
    {
        enum { value = !std::is_same<decltype(*(T*)(0) + *(Arg*)(0)), No>::value };
    };
}

template <bool has_plus>
struct sum_selector {
    template <typename T>
    static T sum_impl(const T& r, const T& l);
};

template <bool has_plus>
template <typename T>
T sum_selector<has_plus>::sum_impl(const T& r, const T& l) {
    std::cout << "selecting dummy implementation" << std::endl;
    return {};
}

template <>
template <typename T>
T sum_selector<true>::sum_impl(const T& r, const T& l) {
    std::cout << "selecting native implementation" << std::endl;
    return r + l;
}


// example classes
class WithPlus {
public:
    WithPlus operator+ (const WithPlus& other) const {
        std::cout << "now adding \"WithPlus\"-objects" << std::endl;
        return {};
    }
};

class WithoutPlus {};

template <typename T>
class Adder {
public:
    T sum(const T& x1, const T& x2);
};


template <typename T>
T Adder<T>::sum(const T& x1, const T& x2) {
    return sum_selector<operator_check::PlusExists<T>::value>::sum_impl(x1, x2);
}

int main(int argc, char** argv) {
    Adder<WithPlus> adder;
    Adder<WithoutPlus> adder_wo;
    WithPlus res_with = adder.sum(WithPlus(), WithPlus());
    WithoutPlus res_wo = adder_wo.sum(WithoutPlus(), WithoutPlus());

    Adder<int> adder_i;
    std::cout << adder_i.sum(4,5) << std::endl;
}

