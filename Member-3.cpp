// SMARTQ - assigned code extracted from the original project src/main.cpp
// This is a contribution file for integration, not a standalone executable.

class CounterHeap
{
    struct Entry
    {
        string id;
        int workload;
    };
    vector<Entry> h;
    unordered_map<string, size_t> pos;
    bool lessAt(size_t a, size_t b) const { return h[a].workload == h[b].workload ? h[a].id < h[b].id : h[a].workload < h[b].workload; }
    void swapAt(size_t a, size_t b)
    {
        swap(h[a], h[b]);
        pos[h[a].id] = a;
        pos[h[b].id] = b;
    }
    void up(size_t i)
    {
        while (i)
        {
            size_t p = (i - 1) / 2;
            if (!lessAt(i, p))
                break;
            swapAt(i, p);
            i = p;
        }
    }
    void down(size_t i)
    {
        for (;;)
        {
            size_t l = 2 * i + 1, r = l + 1, m = i;
            if (l < h.size() && lessAt(l, m))
                m = l;
            if (r < h.size() && lessAt(r, m))
                m = r;
            if (m == i)
                break;
            swapAt(i, m);
            i = m;
        }
    }

public:
    void set(const string &id, int w)
    {
        auto it = pos.find(id);
        if (it == pos.end())
        {
            pos[id] = h.size();
            h.push_back({id, w});
            up(h.size() - 1);
        }
        else
        {
            size_t i = it->second;
            int old = h[i].workload;
            h[i].workload = w;
            if (w < old)
                up(i);
            else
                down(i);
        }
    }
    void erase(const string &id)
    {
        auto it = pos.find(id);
        if (it == pos.end())
            return;
        size_t i = it->second, last = h.size() - 1;
        pos.erase(it);
        if (i != last)
        {
            h[i] = move(h[last]);
            pos[h[i].id] = i;
        }
        h.pop_back();
        if (i < h.size())
        {
            up(i);
            down(pos[h[i].id]);
        }
    }
    vector<pair<string, int>> entries() const
    {
        vector<pair<string, int>> v;
        for (auto &e : h)
            v.push_back({e.id, e.workload});
        return v;
    }
};
struct Counter
{
    string id, service;
    bool active = true;
    int workload = 0;
    string serving;
    LinkedQueue assigned;
};
