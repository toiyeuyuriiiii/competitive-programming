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

const int MAXN = 1007;
int N, M; 
vector<bitset<MAXN>> adj;

void Read()
{
    cin >> N >> M;
    adj.resize(MAXN + 1);
    REP(i, M)
    {
        int u, v;
        cin >> u >> v;
        adj[u].set(v);
        adj[v].set(u);
    }
}

void Solve()
{
    vector<bitset<MAXN>> validadj;
    FOR(i, 1, N) if (adj[i].count() >= 3) validadj.pb(adj[i]);
    int k = sz(validadj);
    REP(i, k){
        FOR(j, i + 1, k - 1){
            bitset<MAXN> ab = validadj[i] & validadj[j];
            if (ab.count() >= 3){
                FOR(l, j + 1, k - 1){
                    if ((ab & validadj[l]).count() >= 3){
                        cout << "YES";
                        return;
                    }
                }
            }
        }
    }
    cout << "NO";
}

signed main()
{
    InitIO();
    FileIO("d10perfect");
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