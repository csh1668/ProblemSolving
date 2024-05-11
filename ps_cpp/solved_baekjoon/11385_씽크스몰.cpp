#include <iostream>
#include <vector>
using std::vector;

typedef long long int integer;

integer power_mod(integer a, integer n, integer mod)
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

integer positive_mod(integer a, integer mod)
{
	return (a % mod + mod) % mod;
}

template<integer Mod, integer W>
class ntt
{
public:
	void operator()(vector<integer>& a, bool inverse = false) const
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
			const integer half = step / 2;
			integer u = power_mod(W, Mod / step, Mod);
			if (inverse)
			{
				u = power_mod(u, Mod - 2, Mod);
			}
			for (integer i = 0; i < n; i += step)
			{
				integer wi = 1;
				for (integer j = i; j < i + half; ++j)
				{
					const integer v = a[j + half] * wi;
					a[j + half] = positive_mod(a[j] - v, Mod);
					a[j] = positive_mod(a[j] + v, Mod);
					wi = positive_mod(u * wi, Mod);
				}
			}
			step *= 2;
		}

		if (inverse)
		{
			const integer num = Mod - (Mod - 1) / n;
			for (integer i = 0; i < n; ++i)
			{
				a[i] = (a[i] * num) % Mod;
			}
		}
	}
	vector<integer>& multiply(vector<integer>& a, vector<integer>& b) const
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
		operator()(a);
		operator()(b);
		const auto c = new vector<integer>();
		for (integer i = 0; i < n; ++i)
		{
			c->push_back(a[i] * b[i]);
		}
		operator()(*c, true);
		return *c;
	}
};


void print_vector(vector<integer> const& v3)
{
	for (const integer& value : v3)
	{
		std::cout << value << " ";
	}
	std::cout << std::endl;
}

int main()
{
	constexpr integer p1 = 998244353, p2 = 1004535809, w1 = 3, w2 = 62;
	int n, m;
	std::cin >> n >> m;
	const auto v1_1 = new vector<integer>(n + 1);
	const auto v2_1 = new vector<integer>(m + 1);
	constexpr auto ntt1 = ntt<p1, w1>();
	constexpr auto ntt2 = ntt<p2, w2>();
	for (int i = 0; i < n + 1; i++)
	{
		std::cin >> (*v1_1)[i];
	}
	for (int i = 0; i < m + 1; i++)
	{
		std::cin >> (*v2_1)[i];
	}
	const auto v1_2 = new vector<integer>(*v1_1);
	const auto v2_2 = new vector<integer>(*v2_1);
	const auto& v3_1 = ntt1.multiply(*v1_1, *v2_1);
	const auto& v3_2 = ntt2.multiply(*v1_2, *v2_2);

	const auto v3 = new vector<integer>(v3_1.size());
	for (size_t i = 0; i < v3_1.size(); i++)
	{
		integer a = p2 * (v3_1[i] * power_mod(p2, p1 - 2, p1) % p1), 
                b = p1 * (v3_2[i] * power_mod(p1, p2 - 2, p2) % p2), 
                c = (a + b) % (p1 * p2);
		//std::cout << a << " " << b << " " << c << std::endl;
		(*v3)[i] = c;
	}

	integer result = (*v3)[0];
	for (int i = 1; i < n + m + 1; i++)
	{
		result ^= (*v3)[i];
	}
	//print_vector(v3_1);
	//print_vector(v3_2);
	//print_vector(*v3);
	std::cout << result << std::endl;

	return 0;
}
