/*
 * aux.h
 *
 *  Created on: 16 May 2018
 *      Author: bayerc
 */

#include <cmath>
#include <array>
#include <numeric>
#include <algorithm>

typedef std::array<int, 4> MultiIndex;

// integer logarithm for base 2
inline int ilog2(int n) {
	int ret = 0;
	if (n > 0) {
		while (n >>= 1)
			++ret;
	} else
		ret = -1;
	return ret;
}

// double logarithm for base 2
inline double log2(double x) {
	return log(x) / log(2);
}

// overload cout for vector
template<typename T>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
	if (!v.empty()) {
		out << '(';
		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b)";
	}
	return out;
}

// overload cout for MultiIndedx
inline std::ostream& operator<<(std::ostream& out, MultiIndex& v) {
	if (!v.empty()) {
		out << '(';
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>(out, ", "));
		out << "\b\b)";
	}
	return out;
}

// sum two vectors
template<typename T>
inline std::vector<T> sumVector(const std::vector<T>& a, const std::vector<T>& b) {
	std::vector<T> vec(a.size());
	for (size_t i = 0; i < a.size(); ++i)
		vec[i] = a[i] + b[i];
	return vec;
}

// Given a vector of Vectors (assumed to be samples of the Vector), compute the mean Vector
template<typename T>
inline std::vector<T> mean(const std::vector<std::vector<T>>& X) {
	std::vector<T> res(X[0].size(), 0);
	size_t M = X.size();
	res = std::accumulate(X.begin(), X.end(), res,
			[](const Vector& x, const Vector& y) -> Vector {return sumVector(x, y);});
	std::transform(res.begin(), res.end(), res.begin(),
			[M](double x) -> double {return x/M;});
	return res;
}

// Given two vectors of Vectors X and Y, compute the sample moment E[X Y]
// a matrix.
template<typename T>
inline std::vector<std::vector<T>> covariance(
		const std::vector<std::vector<T>>& X,
		const std::vector<std::vector<T>>& Y) {
	size_t M = X.size();
	size_t N = X[0].size();
	std::vector<std::vector<T>> cov(N, std::vector<T>(N, 0));
	for (int m = 0; m < M; ++m) {
		for (int n1 = 0; n1 < N; ++n1) {
			for (int n2 = 0; n2 < N; ++n2)
				cov[n1][n2] += X[m][n1] * Y[m][n2];
		}
	}
	for (size_t i = 0; i < N; ++i) {
		for (auto& x : cov[i])
			x /= M;
	}
	return cov;
}
