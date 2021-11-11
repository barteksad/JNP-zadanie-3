#pragma once

#include <tuple>
#include <initializer_list>
#include <set>

using real_t = double;
using fuzzy_rank = std::tuple<real_t, real_t, real_t>;

class TriFuzzyNum {
private:
    
	real_t l;
	real_t m;
	real_t u;

    fuzzy_rank computeRank() const;

public:
	TriFuzzyNum() = delete;

	constexpr TriFuzzyNum(real_t a, real_t b, real_t c);

	TriFuzzyNum(const TriFuzzyNum& other);

	TriFuzzyNum(TriFuzzyNum&& other) noexcept;

    constexpr real_t lower_value() const;

	constexpr real_t modal_value() const;

	constexpr real_t upper_value() const;

    TriFuzzyNum& operator+=(const TriFuzzyNum& other);

	TriFuzzyNum& operator-=(const TriFuzzyNum& other);

	TriFuzzyNum& operator*=(const TriFuzzyNum& other);

	friend TriFuzzyNum operator+(TriFuzzyNum lhs, const TriFuzzyNum& rhs);

	friend TriFuzzyNum operator-(TriFuzzyNum lhs, const TriFuzzyNum& rhs);

	friend TriFuzzyNum operator*(TriFuzzyNum lhs, const TriFuzzyNum& rhs);

    std::strong_ordering operator<=>(const TriFuzzyNum& other) const;
};


class TriFuzzyNumSet {
private:
	
	std::multiset<TriFuzzyNum> num_set;

public:

	TriFuzzyNumSet();
	TriFuzzyNumSet(std::initializer_list<TriFuzzyNum> init_list);

    TriFuzzyNumSet& operator=(const TriFuzzyNumSet& other);

	TriFuzzyNumSet& operator=(TriFuzzyNumSet&& other);

    void remove(const TriFuzzyNum& num);

    TriFuzzyNum arithmetic_mean();
};
