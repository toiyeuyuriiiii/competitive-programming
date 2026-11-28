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

const double EPS = 1e-8;

struct Point
{
    double x, y;
    bool operator<(const Point &o) const
    {
        if (abs(x - o.x) > EPS)
            return x < o.x;
        return y < o.y - EPS;
    }
    bool operator==(const Point &o) const
    {
        return abs(x - o.x) <= EPS && abs(y - o.y) <= EPS;
    }
};

struct Circle
{
    double x, y, r;
};

vector<Point> getIntersections(const Circle &c1, const Circle &c2)
{
    double d = hypot(c1.x - c2.x, c1.y - c2.y);
    if (d > c1.r + c2.r + EPS || d < abs(c1.r - c2.r) - EPS || d < EPS)
        return {};
    double a = (c1.r * c1.r - c2.r * c2.r + d * d) / (2 * d);
    double h = sqrt(max(0.0, c1.r * c1.r - a * a));
    double cx = c1.x + a * (c2.x - c1.x) / d;
    double cy = c1.y + a * (c2.y - c1.y) / d;
    Point p1 = {cx + h * (c1.y - c2.y) / d, cy + h * (c2.x - c1.x) / d};
    Point p2 = {cx - h * (c1.y - c2.y) / d, cy - h * (c2.x - c1.x) / d};
    if (abs(h) < EPS)
        return {p1};
    return {p1, p2};
}

const int OFFSET = 2007;
const int MAXN = (int)4e6 + 2015;

int n;
vector<Circle> circles;
vector<Point> nodes;
int siz[MAXN], par[MAXN];

void Read()
{
    cin >> n;
    circles.resize(n);
    REP(i, n)
    {
        cin >> circles[i].x >> circles[i].y >> circles[i].r;
    }
}

int Getid(const Point &p)
{
    return OFFSET + (lower_bound(all(nodes), p)) - nodes.begin();
}

int Find(int u)
{
    return (par[u] == u ? u : par[u] = Find(par[u]));
}

bool Unite(int u, int v)
{
    u = Find(u);
    v = Find(v);
    if (u == v)
        return false;
    if (siz[u] < siz[v])
        swap(u, v);
    par[v] = u;
    siz[u] += siz[v];
    return true;
}

void Solve()
{
    nodes.reserve(n * n);
    vector<pair<int, Point>> redges;
    REP(i, n)
    {
        FOR(j, i + 1, n - 1)
        {
            vector<Point> points = getIntersections(circles[i], circles[j]);
            for (const Point &e : points)
            {
                nodes.pb(e);
                redges.pb({i, e});
                redges.pb({j, e});
            }
        }
    }
    sort(all(nodes));
    nodes.erase(unique(all(nodes)), nodes.end());
    vector<pii> edges;
    edges.reserve(sz(redges));
    for (const auto &e : redges)
    {
        edges.pb({e.fi, Getid(e.se)});
    }
    sort(all(edges));
    edges.erase(unique(all(edges)), edges.end());
    int total = OFFSET + sz(nodes);
    FOR(i, 0, total - 1)
    {
        siz[i] = 1;
        par[i] = i;
    }
    ll ans = n + 1;
    for (const auto &e : edges)
    {
        int u = e.fi, v = e.se;
        if (!Unite(u, v))
            ans++;
    }
    cout << ans;
}

signed main()
{
    InitIO();
    FileIO("d10ring");
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