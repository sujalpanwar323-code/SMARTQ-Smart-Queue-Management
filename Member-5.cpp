// SMARTQ - assigned code extracted from the original project src/main.cpp
// This is a contribution file for integration, not a standalone executable.

static bool inputEnded = false;
static string input(const string &prompt)
{
    cout << prompt;
    string s;
    if (!getline(cin, s))
        inputEnded = true;
    return s;
}
static int choice(const string &p, int lo, int hi)
{
    for (;;)
    {
        if (inputEnded)
            return hi;
        string s = input(p);
        if (inputEnded)
            return hi;
        try
        {
            size_t n;
            int v = stoi(s, &n);
            if (n == s.size() && v >= lo && v <= hi)
                return v;
        }
        catch (...)
        {
        }
        cout << "Enter a number from " << lo << " to " << hi << ".\n";
    }
}
int main()
{
    QueueManager q;
    cout << "========================================\n SMARTQ - SMART QUEUE OPTIMIZATION SYSTEM\n Don't wait in line. Let the line come to you.\n========================================\n";
    bool running = true;
    while (running)
    {
        cout << "\n1. Student Portal\n2. Staff Portal\n3. Admin Portal\n4. Exit\n";
        int m = choice("Choice: ", 1, 4);
        if (m == 1)
        {
            bool back = false;
            while (!back)
            {
                cout << "\nStudent: 1 Register  2 Join queue  3 Token details/ETA  4 Cancel  5 History  6 Back\n";
                int c = choice("Choice: ", 1, 6);
                if (c == 1)
                {
                    string name = input("Name: ");
                    string contact = input("Contact: ");
                    string id = q.addStudent(name, contact);
                    cout << "Registered. Student ID: " << id << '\n';
                }
                else if (c == 2)
                {
                    string sid = input("Student ID: ");
                    if (!q.hasStudent(sid))
                    {
                        cout << "Unknown student ID. Register first.\n";
                        continue;
                    }
                    q.serviceMenu();
                    int si = choice("Service: ", 1, 4);
                    string s = q.getServices()[si - 1];
                    string detail;
                    if (s == "Xerox")
                    {
                        cout << "1. 10 pages  2. 20 pages  3. 50 pages\n";
                        int d = choice("Request: ", 1, 3);
                        detail = to_string(d == 1 ? 10 : d == 2 ? 20
                                                                : 50) +
                                 " pages";
                    }
                    else if (s == "Library")
                    {
                        cout << "1. Book issue  2. Book return\n";
                        detail = choice("Request: ", 1, 2) == 1 ? "Book issue" : "Book return";
                    }
                    else if (s == "Accounts")
                    {
                        cout << "1. Fee query  2. Payment query\n";
                        detail = choice("Request: ", 1, 2) == 1 ? "Fee query" : "Payment query";
                    }
                    else
                    {
                        cout << "1. Document verification  2. Exam query\n";
                        detail = choice("Request: ", 1, 2) == 1 ? "Document verification" : "Exam query";
                    }
                    string urgent = input("Urgent request? (y/N): ");
                    string token = q.submit(sid, s, detail, !urgent.empty() && (urgent[0] == 'y' || urgent[0] == 'Y'));
                    if (token.empty())
                        cout << "Request could not be created.\n";
                    else
                        cout << "Request accepted. Token: " << token << "\n";
                }
                else if (c == 3)
                {
                    auto *r = q.findToken(input("Token: "));
                    if (!r)
                        cout << "Token not found.\n";
                    else
                    {
                        q.showRequest(*r);
                        cout << "Estimated wait: " << q.eta(*r) << " min | service duration: " << r->duration << " min\n";
                    }
                }
                else if (c == 4)
                {
                    if (q.cancel(input("Token to cancel: ")))
                        cout << "Request cancelled.\n";
                    else
                        cout << "Cannot cancel: token unknown or request is no longer waiting.\n";
                }
                else if (c == 5)
                    q.studentHistory(input("Student ID: "));
                else
                    back = true;
            }
        }
        else if (m == 2)
        {
            string cid = input("Assigned counter ID: ");
            bool back = false;
            while (!back)
            {
                cout << "\nStaff: 1 Assigned requests  2 Call next  3 Start serving  4 Complete  5 No-show  6 Workload/status  7 Pause counter  8 Back\n";
                int c = choice("Choice: ", 1, 8);
                if (c == 1)
                    q.showAssigned(cid);
                else if (c == 2)
                {
                    string t;
                    if (q.callNext(cid, t))
                        cout << "Now calling " << t << ".\n";
                    else
                        cout << "No eligible request or counter inactive.\n";
                }
                else if (c == 3)
                {
                    if (q.start(input("Called token: ")))
                        cout << "Serving started.\n";
                    else
                        cout << "Invalid transition: token must be CALLED.\n";
                }
                else if (c == 4)
                {
                    if (q.finish(input("Serving token: ")))
                        cout << "Request completed.\n";
                    else
                        cout << "Invalid transition: token must be SERVING.\n";
                }
                else if (c == 5)
                {
                    if (q.finish(input("Called token: "), true))
                        cout << "Marked NO_SHOW.\n";
                    else
                        cout << "Invalid transition: token must be CALLED.\n";
                }
                else if (c == 6)
                    q.showCounters();
                else if (c == 7)
                {
                    if (q.activateCounter(cid, false))
                        cout << "Counter paused. Pending requests redistributed where possible.\n";
                    else
                        cout << "Counter not found.\n";
                }
                else
                    back = true;
            }
        }
        else if (m == 3)
        {
            bool back = false;
            while (!back)
            {
                cout << "\nAdmin: 1 All requests  2 Counters  3 Students  4 Statistics  5 Add counter  6 Activate/deactivate  7 Back\n";
                int c = choice("Choice: ", 1, 7);
                if (c == 1)
                    q.allRequests();
                else if (c == 2)
                    q.showCounters();
                else if (c == 3)
                    q.listStudents();
                else if (c == 4)
                    q.analytics();
                else if (c == 5)
                {
                    q.serviceMenu();
                    int s = choice("Service: ", 1, 4);
                    q.addCounter(input("New counter ID: "), q.getServices()[s - 1]);
                    cout << "Counter added.\n";
                }
                else if (c == 6)
                {
                    string id = input("Counter ID: ");
                    string a = input("Activate? (y/N): ");
                    cout << (q.activateCounter(id, !a.empty() && (a[0] == 'y' || a[0] == 'Y')) ? "Counter status updated.\n" : "Counter not found.\n");
                }
                else
                    back = true;
            }
        }
        else
            running = false;
    }
    cout << "Goodbye. SMARTQ data saved.\n";
    return 0;
}
