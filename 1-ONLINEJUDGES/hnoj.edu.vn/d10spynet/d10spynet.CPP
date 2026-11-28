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
//
// int Randint(int minium, int maxium){
//     return uniform_int_distribution<int>(minium, maxium)(rng);
// }
//
// ll Randll(ll minium, ll maxium){
//     return uniform_int_distribution<ll>(minium, maxium)(rng);
// }
//
// int Randvectorint(const vi& a){
//     return a[Randint(0, sz(a) - 1)];
// }
//
// ll Randvectorll(const vll& a){
//     return a[Randint(0, sz(a) - 1)];
// }

void Fast()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}

void File(const string &filename = "")
{
    if (fopen((filename + ".INP").c_str(), "r"))
    {
        freopen((filename + ".INP").c_str(), "r", stdin);
        freopen((filename + ".OUT").c_str(), "w", stdout);
    }
}
int n, m, s;

void Read()
{
    cin >> n >> m >> s;
    if (s == 1)
    {
        cout << n;
        exit(0);
    }
    else if (s == 2)
    {
        cout << m;
        exit(0);
    }
    else if (s >= 5)
    {
        cout << 0;
        exit(0);
    }
}

namespace sequal3
{
    const int MAXN = (int)1e5 + 7;
    static int deg[MAXN], mark[MAXN];
    vector<vi> adj;
    vector<pii> edges;

    bool Issmaller(int u, int v)
    {
        if (deg[u] != deg[v])
            return deg[u] < deg[v];
        return u < v;
    }

    void Read()
    {
        adj.resize(n + 1);
        edges.resize(m);
        REP(i, m)
        {
            cin >> edges[i].fi >> edges[i].se;
            deg[edges[i].fi]++;
            deg[edges[i].se]++;
        }
        REP(i, m)
        {
            int u = edges[i].fi, v = edges[i].se;
            if (Issmaller(u, v))
                adj[u].pb(v);
            else
                adj[v].pb(u);
        }
    }

    void Solve()
    {
        ll ans = 0;
        FOR(a, 1, n)
        {
            REP(i, sz(adj[a]))
            {
                int v = adj[a][i];
                mark[v] = a;
            }
            REP(i, sz(adj[a]))
            {
                int b = adj[a][i];
                REP(j, sz(adj[b]))
                {
                    int c = adj[b][j];
                    if (mark[c] == a)
                        ans++;
                }
            }
        }
        cout << ans;
    }

    void Init()
    {
        Read();
        Solve();
    }
}

namespace sequal4
{
    const int MAXN = (int)1e5 + 7;
    static int deg[MAXN], marka[MAXN], markb[MAXN];
    vector<vi> adj;
    vector<pii> edges;

    bool Issmaller(int u, int v)
    {
        if (deg[u] != deg[v])
            return deg[u] < deg[v];
        return u < v;
    }

    void Read()
    {
        adj.resize(n + 1);
        edges.resize(m);
        REP(i, m)
        {
            cin >> edges[i].fi >> edges[i].se;
            deg[edges[i].fi]++;
            deg[edges[i].se]++;
        }
        REP(i, m)
        {
            int u = edges[i].fi, v = edges[i].se;
            if (Issmaller(u, v))
                adj[u].pb(v);
            else
                adj[v].pb(u);
        }
    }

    void Solve()
    {
        ll ans = 0;
        int timer = 0;
        vi common;
        FOR(a, 1, n)
        {
            REP(i, sz(adj[a]))
            {
                int v = adj[a][i];
                marka[v] = a;
            }
            REP(i, sz(adj[a]))
            {
                int b = adj[a][i];
                timer++;
                common.clear();
                REP(j, sz(adj[b]))
                {
                    int w = adj[b][j];
                    if (marka[w] == a)
                    {
                        markb[w] = timer;
                        common.pb(w);
                    }
                }
                REP(j, sz(common))
                {
                    int c = common[j];
                    REP(k, sz(adj[c]))
                    {
                        int d = adj[c][k];
                        if (markb[d] == timer)
                            ans++;
                    }
                }
            }
        }
        cout << ans;
    }

    void Init()
    {
        Read();
        Solve();
    }
}

void Solve()
{
    if (s == 3)
        sequal3::Init();
    else
        sequal4::Init();
}

signed main()
{
    Fast();
    File("d10spynet");
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