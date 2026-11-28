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

int N, M;
vector<string> partymap;

void Read()
{
    cin >> N >> M;
    partymap.resize(N);
    REP(i, N)
    cin >> partymap[i];
}

int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};

void Solve()
{
    REP(i, N)
    {
        REP(j, M)
        {
            if (partymap[i][j] == 'B')
            {
                REP(k, 4)
                {
                    int di = i + dx[k], dj = j + dy[k];
                    if (di >= 0 && di < N && dj >= 0 && dj < M)
                    {
                        if (partymap[di][dj] == 'G')
                        {
                            cout << "No" << el;
                            return;
                        }
                        if (partymap[di][dj] == '.')
                        {
                            partymap[di][dj] = '#';
                        }
                    }
                }
            }
        }
    }
    if (partymap[N - 1][M - 1] == 'B')
    {
        cout << "No" << el;
        return;
    }
    bool canexit = true;
    REP(i, N)
    {
        REP(j, M)
        {
            if (partymap[i][j] == 'G')
            {
                queue<pii> q;
                vector<vb> visited(N, vb(M, false));
                q.push({i, j});
                visited[i][j] = true;
                while (!q.empty())
                {
                    int u = q.front().fi, v = q.front().se;
                    q.pop();
                    REP(k, 4)
                    {
                        int du = u + dx[k], dv = v + dy[k];
                        if (du >= 0 && du < N && dv >= 0 && dv < M)
                        {

                            if (!visited[du][dv])
                                if (partymap[du][dv] != '#' && partymap[du][dv] != 'B')
                                {
                                    q.push({du, dv});
                                    visited[du][dv] = true;
                                }
                        }
                    }
                }
                if (!visited[N - 1][M - 1])
                    canexit = false;
                if (!canexit)
                {
                    cout << "No" << el;
                    return;
                }
            }
        }
    }
    cout << "Yes" << el;
}

signed main()
{
    InitIO();
    FileIO("d11exit");
    int T;
    cin >> T;
    while (T--)
    {
        N = M = 0;
        partymap.clear();
        Read();
        Solve();
    }
    return 0;
}

/*
    ---------------------------------------------------
               Code By github/toiyeuyuriiiii
                   I Love You My Girl <3
    ---------------------------------------------------
*/