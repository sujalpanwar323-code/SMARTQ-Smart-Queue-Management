// SMARTQ - assigned code extracted from the original project src/main.cpp
// This is a contribution file for integration, not a standalone executable.

struct Request
{
    string id, token, studentId, service, description, counterId, created;
    int duration = 0;
    bool urgent = false;
    Status status = Status::WAITING;
    long long createdEpoch = 0, startedEpoch = 0, completedEpoch = 0;
};

class QueueManager
{
    unordered_map<string, Student> students;
    unordered_map<string, Request> requests;
    unordered_map<string, Counter> counters;
    unordered_map<string, int> durations;
    unordered_map<string, int> tokenSeq;
    unordered_map<string, LinkedQueue> normal;
    vector<string> history;
    CounterHeap heap;
    int requestSeq = 1, studentSeq = 1;
    vector<string> services = {"Xerox", "Library", "Accounts", "Examination"};
    static string prefix(const string &s)
    {
        if (s == "Xerox")
            return "X";
        if (s == "Library")
            return "L";
        if (s == "Accounts")
            return "A";
        return "E";
    }
    void updateHeap(Counter &c)
    {
        if (c.active)
            heap.set(c.id, c.workload);
        else
            heap.erase(c.id);
    }
    int workloadOf(const Counter &c) const { return c.workload; }
    Counter *choose(const string &s)
    {
        Counter *best = nullptr;
        for (auto &kv : counters)
        {
            auto &c = kv.second;
            if (c.active && c.service == s && (!best || c.workload < best->workload || (c.workload == best->workload && c.id < best->id)))
                best = &c;
        }
        return best;
    }
    void assign(Request &r, Counter &c)
    {
        r.counterId = c.id;
        c.assigned.enqueue(r.id);
        c.workload += r.duration;
        updateHeap(c);
    }
    void saveAll()
    {
        ofstream f("data/students.txt");
        for (auto &x : students)
            f << x.second.getId() << '|' << clean(x.second.getName()) << '|' << clean(x.second.getContact()) << '\n';
        ofstream cf("data/counters.txt");
        for (auto &x : counters)
            cf << x.second.id << '|' << x.second.service << '|' << x.second.active << '\n';
        ofstream rf("data/requests.txt");
        for (auto &x : requests)
        {
            auto &r = x.second;
            rf << r.id << '|' << r.token << '|' << r.studentId << '|' << r.service << '|' << clean(r.description) << '|' << r.duration << '|' << r.urgent << '|' << r.counterId << '|' << statusName(r.status) << '|' << r.created << '|' << r.createdEpoch << '|' << r.startedEpoch << '|' << r.completedEpoch << '\n';
        }
        ofstream sf("data/services.txt");
        for (auto &x : durations)
            sf << x.first << '|' << x.second << '\n';
        ofstream hf("data/history.txt");
        for (auto &id : history)
            hf << id << '\n';
    }
    void load()
    {
        auto split = [](const string &line)
        {vector<string>v;stringstream ss(line);string t;while(getline(ss,t,'|'))v.push_back(t);return v; };
        ifstream sf("data/services.txt");
        string line;
        while (getline(sf, line))
        {
            auto v = split(line);
            if (v.size() == 2)
                try
                {
                    durations[v[0]] = stoi(v[1]);
                }
                catch (...)
                {
                }
        }
        ifstream st("data/students.txt");
        while (getline(st, line))
        {
            auto v = split(line);
            if (v.size() == 3)
            {
                students.emplace(v[0], Student(v[0], v[1], v[2]));
                if (v[0].size() > 1)
                    try
                    {
                        studentSeq = max(studentSeq, stoi(v[0].substr(1)) + 1);
                    }
                    catch (...)
                    {
                    }
            }
        }
        ifstream ct("data/counters.txt");
        while (getline(ct, line))
        {
            auto v = split(line);
            if (v.size() == 3)
            {
                Counter c;
                c.id = v[0];
                c.service = v[1];
                c.active = v[2] == "1";
                counters.emplace(c.id, move(c));
            }
        }
        ifstream rf("data/requests.txt");
        while (getline(rf, line))
        {
            auto v = split(line);
            if (v.size() < 13)
                continue;
            try
            {
                Request r;
                r.id = v[0];
                r.token = v[1];
                r.studentId = v[2];
                r.service = v[3];
                r.description = v[4];
                r.duration = stoi(v[5]);
                r.urgent = v[6] == "1";
                r.counterId = v[7];
                r.status = v[8] == "WAITING" ? Status::WAITING : v[8] == "CALLED"  ? Status::CALLED
                                                             : v[8] == "SERVING"   ? Status::SERVING
                                                             : v[8] == "COMPLETED" ? Status::COMPLETED
                                                             : v[8] == "CANCELLED" ? Status::CANCELLED
                                                                                   : Status::NO_SHOW;
                r.created = v[9];
                r.createdEpoch = stoll(v[10]);
                r.startedEpoch = stoll(v[11]);
                r.completedEpoch = stoll(v[12]);
                requests.emplace(r.id, r);
                if (r.id.size() > 1)
                    requestSeq = max(requestSeq, stoi(r.id.substr(1)) + 1);
                string p = prefix(r.service);
                if (r.token.size() > 1)
                    tokenSeq[p] = max(tokenSeq[p], stoi(r.token.substr(1)) + 1);
                if (r.status == Status::WAITING || r.status == Status::CALLED || r.status == Status::SERVING)
                {
                    auto ci = counters.find(r.counterId);
                    if (ci != counters.end())
                    {
                        ci->second.assigned.enqueue(r.id);
                        ci->second.workload += r.duration;
                        if (r.status == Status::SERVING)
                            ci->second.serving = r.id;
                    }
                    else if (r.status == Status::WAITING)
                        normal[r.service].enqueue(r.id);
                }
            }
            catch (...)
            {
            }
        }
        ifstream hf("data/history.txt");
        while (getline(hf, line))
            if (requests.count(line))
                history.push_back(line);
        for (auto &x : counters)
            updateHeap(x.second);
    }

public:
    QueueManager()
    {
        system("if not exist data mkdir data >nul 2>nul");
        durations = {{"Xerox:10 pages", 3}, {"Xerox:20 pages", 5}, {"Xerox:50 pages", 10}, {"Library:Book issue", 4}, {"Library:Book return", 2}, {"Accounts:Fee query", 7}, {"Accounts:Payment query", 5}, {"Examination:Document verification", 8}, {"Examination:Exam query", 5}};
        load();
        if (counters.empty())
        {
            addCounter("C1", "Xerox");
            addCounter("C2", "Library");
            addCounter("C3", "Accounts");
            addCounter("C4", "Examination");
        }
        saveAll();
    }
    ~QueueManager() { saveAll(); }
    const vector<string> &getServices() const { return services; }
    string addStudent(string name, string contact)
    {
        string id = "S" + to_string(studentSeq++);
        students.emplace(id, Student(id, clean(name), clean(contact)));
        saveAll();
        return id;
    }
    void listStudents() const
    {
        for (auto &x : students)
            cout << x.first << " | " << x.second.getName() << " | " << x.second.getContact() << '\n';
        if (students.empty())
            cout << "No students registered.\n";
    }
    bool hasStudent(const string &id) const { return students.count(id); }
    void addCounter(const string &id, const string &s)
    {
        if (counters.count(id))
            return;
        Counter c;
        c.id = id;
        c.service = s;
        counters.emplace(id, move(c));
        updateHeap(counters.at(id));
        saveAll();
    }
    void showCounters() const
    {
        for (auto &x : counters)
            cout << x.first << " | " << x.second.service << " | " << (x.second.active ? "ACTIVE" : "INACTIVE") << " | workload " << x.second.workload << " min\n";
    }
    bool activateCounter(const string &id, bool active)
    {
        auto it = counters.find(id);
        if (it == counters.end())
            return false;
        Counter &c = it->second;
        if (c.active == active)
            return true;
        c.active = active;
        if (!active)
        {
            auto ids = c.assigned.values();
            for (auto &rid : ids)
            {
                auto ri = requests.find(rid);
                if (ri == requests.end())
                    continue;
                Request &r = ri->second;
                if (r.status == Status::SERVING)
                    continue;
                if (r.status == Status::WAITING || r.status == Status::CALLED)
                {
                    c.assigned.remove(rid);
                    c.workload = max(0, c.workload - r.duration);
                    r.status = Status::WAITING;
                    r.counterId.clear();
                    Counter *dest = choose(r.service);
                    if (dest)
                    {
                        assign(r, *dest);
                        r.status = Status::WAITING;
                        cout << r.token << " reassigned to " << dest->id << ".\n";
                    }
                    else
                        normal[r.service].enqueue(r.id);
                }
            }
        }
        updateHeap(c);
        saveAll();
        return true;
    }
    string submit(const string &sid, const string &s, const string &detail, bool urgent)
    {
        if (!students.count(sid) || find(services.begin(), services.end(), s) == services.end())
            return {};
        string key = s + ":" + detail;
        if (!durations.count(key))
            return {};
        Request r;
        r.id = "R" + to_string(requestSeq++);
        string p = prefix(s);
        int seq = tokenSeq[p];
        if (seq < 100)
            seq = 100;
        r.token = p + to_string(seq);
        tokenSeq[p] = seq + 1;
        r.studentId = sid;
        r.service = s;
        r.description = detail;
        r.duration = durations[key];
        r.urgent = urgent;
        r.created = nowText();
        r.createdEpoch = time(nullptr);
        Counter *c = choose(s);
        if (c)
            assign(r, *c);
        else
            normal[s].enqueue(r.id);
        string id = r.id;
        requests.emplace(id, move(r));
        saveAll();
        return requests.at(id).token;
    }
    Request *findToken(const string &t)
    {
        for (auto &x : requests)
            if (x.second.token == t)
                return &x.second;
        return nullptr;
    }
    void showRequest(const Request &r) const { cout << r.token << " | " << r.id << " | student " << r.studentId << " | " << r.service << " / " << r.description << " | " << r.duration << " min | " << (r.urgent ? "URGENT" : "NORMAL") << " | counter " << (r.counterId.empty() ? "unassigned" : r.counterId) << " | " << statusName(r.status) << "\n"; }
    int eta(const Request &r) const
    {
        if (r.status != Status::WAITING)
            return 0;
        int wait = 0;
        auto ci = counters.find(r.counterId);
        if (ci != counters.end())
        {
            for (auto &id : ci->second.assigned.values())
            {
                if (id == r.id)
                    break;
                auto q = requests.find(id);
                if (q != requests.end() && (q->second.status == Status::WAITING || q->second.status == Status::CALLED || q->second.status == Status::SERVING))
                    wait += q->second.duration;
            }
        }
        else
        {
            for (auto &id : normal.at(r.service).values())
            {
                if (id == r.id)
                    break;
                auto q = requests.find(id);
                if (q != requests.end())
                    wait += q->second.duration;
            }
        }
        long long age = (time(nullptr) - r.createdEpoch) / 120;
        if (age > 0)
            wait = max(0, wait - (int)min<long long>(age, wait));
        return wait;
    }
    bool cancel(const string &t)
    {
        Request *r = findToken(t);
        if (!r || r->status != Status::WAITING)
            return false;
        if (!r->counterId.empty())
        {
            auto &c = counters.at(r->counterId);
            c.assigned.remove(r->id);
            c.workload = max(0, c.workload - r->duration);
            updateHeap(c);
        }
        else
            normal[r->service].remove(r->id);
        r->status = Status::CANCELLED;
        history.push_back(r->id);
        saveAll();
        return true;
    }
    vector<string> assigned(const string &cid) const
    {
        auto it = counters.find(cid);
        return it == counters.end() ? vector<string>{} : it->second.assigned.values();
    }
    void showAssigned(const string &cid) const
    {
        auto it = counters.find(cid);
        if (it == counters.end())
        {
            cout << "Counter not found.\n";
            return;
        }
        for (const auto &id : it->second.assigned.values())
        {
            auto r = requests.find(id);
            if (r != requests.end())
                showRequest(r->second);
        }
        if (it->second.assigned.empty())
            cout << "No assigned requests.\n";
    }
    bool callNext(const string &cid, string &token)
    {
        auto ci = counters.find(cid);
        if (ci == counters.end() || !ci->second.active)
            return false;
        Counter &c = ci->second;
        string best;
        int bestScore = -1;
        for (auto &id : c.assigned.values())
        {
            auto &r = requests.at(id);
            if (r.status != Status::WAITING)
                continue;
            int score = (r.urgent ? 1000 : 0) + (int)((time(nullptr) - r.createdEpoch) / 120);
            if (score > bestScore)
            {
                bestScore = score;
                best = id;
            }
        }
        if (best.empty())
            return false;
        Request &r = requests.at(best);
        r.status = Status::CALLED;
        token = r.token;
        saveAll();
        return true;
    }
    bool start(const string &t)
    {
        Request *r = findToken(t);
        if (!r || r->status != Status::CALLED)
            return false;
        r->status = Status::SERVING;
        r->startedEpoch = time(nullptr);
        if (counters.count(r->counterId))
            counters.at(r->counterId).serving = r->id;
        saveAll();
        return true;
    }
    bool finish(const string &t, bool noshow = false)
    {
        Request *r = findToken(t);
        if (!r || ((noshow && r->status != Status::CALLED) || (!noshow && r->status != Status::SERVING)))
            return false;
        auto ci = counters.find(r->counterId);
        if (ci != counters.end())
        {
            ci->second.assigned.remove(r->id);
            ci->second.workload = max(0, ci->second.workload - r->duration);
            if (ci->second.serving == r->id)
                ci->second.serving.clear();
            updateHeap(ci->second);
        }
        r->status = noshow ? Status::NO_SHOW : Status::COMPLETED;
        r->completedEpoch = time(nullptr);
        history.push_back(r->id);
        if (ci != counters.end())
            for (auto &id : normal[r->service].values())
            {
                if (!ci->second.active)
                    break;
                normal[r->service].remove(id);
                auto &q = requests.at(id);
                assign(q, ci->second);
                break;
            }
        saveAll();
        return true;
    }
    void allRequests() const
    {
        for (auto &x : requests)
            showRequest(x.second);
        if (requests.empty())
            cout << "No requests.\n";
    }
    void studentHistory(const string &sid) const
    {
        bool found = false;
        for (auto &x : requests)
            if (x.second.studentId == sid)
            {
                showRequest(x.second);
                found = true;
            }
        if (!found)
            cout << "No request history.\n";
    }
    void analytics() const
    {
        int total = 0, done = 0, waiting = 0, cancelled = 0, noShow = 0, waitSum = 0, serviceSum = 0, serviceN = 0;
        unordered_map<string, int> byService;
        for (auto &x : requests)
        {
            auto &r = x.second;
            ++total;
            ++byService[r.service];
            if (r.status == Status::COMPLETED)
            {
                ++done;
                serviceSum += r.duration;
                ++serviceN;
                waitSum += max(0, (int)(r.startedEpoch - r.createdEpoch) / 60);
            }
            if (r.status == Status::WAITING)
                ++waiting;
            if (r.status == Status::CANCELLED)
                ++cancelled;
            if (r.status == Status::NO_SHOW)
                ++noShow;
        }
        string busy = "n/a";
        int mx = 0;
        for (auto &x : byService)
            if (x.second > mx)
                mx = x.second, busy = x.first;
        cout << "Requests " << total << " | completed " << done << " | waiting " << waiting << " | cancelled " << cancelled << " | no-show " << noShow << "\nAverage wait " << (done ? waitSum / done : 0) << " min | average service " << (serviceN ? serviceSum / serviceN : 0) << " min | busiest service " << busy << "\nCounter utilization (queued workload minutes):\n";
        showCounters();
    }
    void serviceMenu() const
    {
        int i = 1;
        for (auto &s : services)
        {
            cout << i++ << ". " << s << "\n";
            for (auto &d : durations)
                if (d.first.rfind(s + ":", 0) == 0)
                    cout << "   " << d.first.substr(s.size() + 1) << " - " << d.second << " min\n";
        }
    }
};
