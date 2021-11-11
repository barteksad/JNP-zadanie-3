#include <fuzzy.h>

#include <algorithm>
#include <iostream>
#include <cmath>


namespace {

    constexpr real_t maxOf3(real_t a, real_t b, real_t c) {
		return std::max({a,b,c});
	}

	constexpr real_t MedianOf3(real_t a, real_t b, real_t c) {
		if (a >= b >= c)
			return b;
		else if (b >= a >= c)
			return a;
		return c;
	}

	constexpr real_t minOf3(real_t a, real_t b, real_t c) {
		return std::min({a,b,c});
	}
} // namespace 

// TriFuzzyNum : 

constexpr TriFuzzyNum::TriFuzzyNum(real_t a, real_t b, real_t c)
		: l(minOf3(a, b, c)), m(MedianOf3(a, b, c)), u(maxOf3(a, b, c)) {};

constexpr real_t TriFuzzyNum::lower_value() const {
    return l;
}

constexpr real_t TriFuzzyNum::modal_value() const {
    return m;
}

constexpr real_t TriFuzzyNum::upper_value() const {
    return u;
}


TriFuzzyNum& TriFuzzyNum::operator+=(const TriFuzzyNum& other) {
    l += other.l;
    m += other.m;
    u += other.u;

    return *this;
}

TriFuzzyNum& TriFuzzyNum::operator-=(const TriFuzzyNum& other) {
    l -= other.u;
    m -= other.m;
    u -= other.l;

    return *this;
}

TriFuzzyNum& TriFuzzyNum::operator*=(const TriFuzzyNum& other) {
    l *= other.l;
    m *= other.m;
    u *= other.u;

    return *this;
}

TriFuzzyNum operator+(TriFuzzyNum lhs, const TriFuzzyNum& rhs) {
    lhs += rhs;

    return lhs;
}

TriFuzzyNum operator-(TriFuzzyNum lhs, const TriFuzzyNum& rhs) {
    lhs -= rhs;

    return lhs;
}

TriFuzzyNum operator*(TriFuzzyNum lhs, const TriFuzzyNum& rhs) {
    lhs *= rhs;

    return lhs;
}

fuzzy_rank TriFuzzyNum::computeRank() const {
     	real_t z = (u - l) + sqrt(1 + (u - m) * (u - m)) + sqrt(1 + (m - l) * (m - l));
		real_t x = ((u - l) * m + sqrt(1 + (u - m) * (u - m)) * l + sqrt(1 + (m - l) * (m - l)) * u) / z;
     	real_t y = (u - l) / z;

		return std::make_tuple(x - y / 2, 1 - y, m);
}

std::strong_ordering TriFuzzyNum::operator<=>(const TriFuzzyNum& other) const {
    fuzzy_rank this_rank = computeRank();
    fuzzy_rank other_rank = other.computeRank();

    if (this_rank < other_rank)
        return std::strong_ordering::less;
    else if (this_rank > other_rank)
        return std::strong_ordering::greater;
    else
        return std::strong_ordering::equal;
}

// TriFuzzyNumSet : 

TriFuzzyNumSet::TriFuzzyNumSet(std::initializer_list<TriFuzzyNum> init_list)
    : num_set(init_list) {};


TriFuzzyNumSet& TriFuzzyNumSet::operator=(const TriFuzzyNumSet& other) {
    num_set = other.num_set;

    return *this;
}

TriFuzzyNumSet& TriFuzzyNumSet::operator=(TriFuzzyNumSet&& other) {
    num_set = move(other.num_set);

    return *this;
}

void TriFuzzyNumSet::remove(const TriFuzzyNum& num) {
    num_set.erase(num);
}

TriFuzzyNum TriFuzzyNumSet::arithmetic_mean() {
    if (num_set.size() == 0)
        throw std::length_error("TriFuzzyNumSet::arithmetic_mean - the set is empty.");

    real_t m_acc = 0;
    real_t l_acc = 0;
    real_t u_acc = 0;

    for(auto i = num_set.begin(); i != num_set.end(); i++) {
        m_acc += (*i).lower_value();
        l_acc += (*i).modal_value();
        u_acc += (*i).upper_value();
    }

    m_acc /= num_set.size();
    l_acc /= num_set.size();
    u_acc /= num_set.size();

    TriFuzzyNum result = TriFuzzyNum(m_acc, l_acc, u_acc);
}