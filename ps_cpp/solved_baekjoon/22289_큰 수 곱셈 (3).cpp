#include <iostream>
#include <vector>

using std::string;
using std::vector;

typedef long long int integer;

constexpr int mod = 998244353;
constexpr int w = 3;

integer positive_mod(integer a)
{
	return (a % mod + mod) % mod;
}

integer power_mod(integer a, integer n)
{
	integer result = 1;
	while (n)
	{
		if (n & 1)
		{
			result = (result * a) % mod;
		}
		a = (a * a) % mod;
		n >>= 1;
	}
	return result;
}


void ntt(vector<integer>& a, bool inverse = false)
{
	const auto n = static_cast<integer>(a.size());
	int j = 0;
	for (integer i = 1; i < n; ++i)
	{
		int reverse = n / 2;
		while (j >= reverse)
		{
			j -= reverse;
			reverse /= 2;
		}
		j += reverse;
		if (i < j)
		{
			const integer tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
		}
	}
	integer step = 2;
	while (step <= n)
	{
		integer half = step / 2;
		integer u = power_mod(w, mod / step);
		if (inverse)
		{
			u = power_mod(u, mod - 2);
		}
		for (integer i = 0; i < n; i += step)
		{
			integer wi = 1;
			for (integer j = i; j < i + half; ++j)
			{
				integer v = a[j + half] * wi;
				a[j + half] = positive_mod(a[j] - v);
				a[j] = positive_mod(a[j] + v);
				wi = positive_mod(u * wi);
			}
		}
		step *= 2;
	}

	if (inverse)
	{
		integer num = mod - (mod - 1) / n;
		for (integer i = 0; i < n; ++i)
		{
			a[i] = (a[i] * num) % mod;
		}
	}
}

vector<integer>& multiply(vector<integer>& a, vector<integer>& b)
{
	integer n = 1;
	while (n < static_cast<integer>(a.size()))
	{
		n <<= 1;
	}
	while (n < static_cast<integer>(b.size()))
	{
		n <<= 1;
	}
	n <<= 1;
	while (static_cast<integer>(a.size()) < n)
	{
		a.push_back(0);
	}
	while (static_cast<integer>(b.size()) < n)
	{
		b.push_back(0);
	}
	ntt(a);
	ntt(b);
	auto c = new vector<integer>();
	for (integer i = 0; i < n; ++i)
	{
		c->push_back(a[i] * b[i]);
	}
	ntt(*c, true);
	return *c;
}

void print_vector(const vector<integer>& v)
{
	for (auto i = 0; i < v.size(); ++i)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}


int main()
{
	const auto v1 = new vector<integer>();
	const auto v2 = new vector<integer>();
	string a, b;
	std::cin >> a >> b;
	if (a == "0" || b == "0")
	{
		std::cout << "0";
	}
	for (int i = a.size() - 1; i >= 0; --i)
	{
		v1->push_back(a.at(i) - '0');
	}
	for (int i = b.size() - 1; i >= 0; --i)
	{
		v2->push_back(b.at(i) - '0');
	}
	auto v3 = multiply(*v1, *v2);
	for (size_t i = 0; i < v3.size() - 1; ++i)
	{
		v3[i + 1] += v3[i] / 10;
		v3[i] %= 10;
	}
	bool flag = true;
	for (int i = v3.size() - 1; i >= 0; --i)
	{
		if (flag && v3[i] == 0)
		{
			continue;
		}
		if (flag && v3[i] != 0)
		{
			flag = false;
		}
		std::cout << v3[i];
	}

	return 0;
}
