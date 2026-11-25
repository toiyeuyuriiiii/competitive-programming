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

struct Point{
    ll x, y;
};

Point p[3];

void Read()
{
    REP(i, 3) cin >> p[i].x >> p[i].y;    
}

__int128 Dist(const Point& a, const Point& b){
    __int128 dx = a.x - b.x;
    __int128 dy = a.y - b.y;
    return dx * dx + dy * dy;
}

void Solve()
{
    __int128 d01 = Dist(p[0], p[1]);
    __int128 d02 = Dist(p[0], p[2]);
    __int128 d12 = Dist(p[1], p[2]);
    if (d01 >= d12 && d01 >= d02) cout << p[0].x << ' ' << p[0].y << ' ' << p[1].x << ' ' << p[1].y;
    else if (d12 >= d01 && d12 >= d02) cout << p[1].x << ' ' << p[1].y << ' ' << p[2].x << ' ' << p[2].y;
    else cout << p[0].x << ' ' << p[0].y << ' ' << p[2].x << ' ' << p[2].y;
}

signed main()
{
    InitIO();
    FileIO("1");
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