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

int n, q;
map<ll, ll> cnt;
ll weight = 0;

void Read()
{
    cin >> n >> q;
    FOR(i, 1, n)
    {
        ll x;
        cin >> x;
        cnt[x]++;
        weight += x;
    }
}

void Solve()
{
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            ll x;
            cin >> x;
            cnt[x]++;
            weight += x;
        }
        else if (type == 2)
        {
            ll x;
            cin >> x;
            cnt[x]--;
            weight -= x;
            if (!cnt[x])
                cnt.erase(x);
        }
        else
        {
            ll w;
            cin >> w;
            auto it = cnt.lower_bound(w);
            vector<pair<ll, ll>> update;
            while (it != cnt.end())
            {
                update.push_back(*it);
                it++;
            }
            for (const auto &[f, s] : update)
            {
                cnt.erase(f);
                weight -= f * s;
                cnt[f % w] += s;
                weight += (f % w) * s;
            }
        }
        cout << weight << el;
    }
}

signed main()
{
    InitIO();
    FileIO("edu_stl_fish");
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