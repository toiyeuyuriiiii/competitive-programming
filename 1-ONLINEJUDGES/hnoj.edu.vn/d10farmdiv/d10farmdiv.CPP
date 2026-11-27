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

const int MAXN = (int)3e5 + 7;
int N, M;

void Read()
{
    cin >> N >> M;
}

int siz[MAXN], par[MAXN];

int Find(int u){
    return (par[u] == u ? u : par[u] = Find(par[u]));
}

bool Unite(int u, int v){
    u = Find(u);
    v = Find(v);
    if (u == v) return false;
    if (siz[u] < siz[v]) swap(u, v);
    par[v] = u;
    siz[u] += siz[v];
    return true;
}

void Solve()
{
    int ans = 0;
    FOR(i, 1, N)
    {
        siz[i] = 1;
        par[i] = i;
    }
    FOR(i, 1, M){
        int u, v;
        cin >> u >> v;
        if (!Unite(u, v)) ans ++;
        cout << ans << el;
    }

}

signed main()
{
    InitIO();
    FileIO("d10farmdiv");
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
