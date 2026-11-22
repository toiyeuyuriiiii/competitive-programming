#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using vi = vector<int>;
using vll = vector<long long>;
using vb = vector<bool>;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

#define FOR(a, b, c) for (auto a = (b); a <= (c); a++)
#define FORD(a, b, c) for (auto a = (b); a >= (c); a--)
#define REP(i, n) for (auto i = 0; i < (n); i++)
#define REPD(i, n) for (int i = (int)(n) - 1; i >= 0; --i)
#define fi first
#define se second
#define pb push_back
#define eb emplace_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (int)(x).size()

const char el = '\n';

// mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

// int Randint(int minium, int maxium)
// {
//     return uniform_int_distribution<int>(minium, maxium)(rng);
// }

// ll Randll(ll minium, ll maxium)
// {
//     return uniform_int_distribution<ll>(minium, maxium)(rng);
// }

// int Randvectorint(const vi &a)
// {
//     return a[Randint(0, sz(a) - 1)];
// }

// ll Randvectorll(const vll &a)
// {
//     return a[Randint(0, sz(a) - 1)];
// }

void InitIO()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}

void FileIO(const string &filename = "")
{
    if (fopen((filename + ".INP").c_str(), "r"))
    {
        freopen((filename + ".INP").c_str(), "r", stdin);
        freopen((filename + ".OUT").c_str(), "w", stdout);
    }
}

int a, b, c;

void Read()
{
    cin >> a >> b >> c;
}

ll Mul(ll mula, ll mulb, ll mulmod)
{
    if (mulb == 0)
        return 0;
    if (mulb == 1)
        return mula % mulmod;
    ll half = Mul(mula, mulb / 2, mulmod) % mulmod;
    if (mulb % 2 == 0)
        return (half + half) % mulmod;
    return (half + half + mula) % mulmod;
}

ll Pow(ll powa, ll powb, ll powmod)
{
    if (powb == 0)
        return 1 % powmod;
    if (powb == 1)
        return powa % powmod;
    ll half = Pow(powa, powb / 2, powmod) % powmod;
    ll mul = Mul(half, half, powmod) % powmod;
    if (powb % 2 == 0)
        return mul % powmod;
    return Mul(mul, powa, powmod) % powmod;
}

void Solve()
{
    cout << Pow(a, b, c);
}

signed main()
{
    InitIO();
    FileIO("24");
    Read();
    Solve();
    return 0;
}

/*
    ---------------------------------------------------
               Code By github/toiyeuyuriiiii
                   I Love You My Girl <3
    ---------------------------------------------------
*/