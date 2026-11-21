// SMARTQ - assigned code extracted from the original project src/main.cpp
// This is a contribution file for integration, not a standalone executable.

class LinkedQueue
{
    struct Node
    {
        string id;
        unique_ptr<Node> next;
        explicit Node(string x) : id(move(x)) {}
    };
    unique_ptr<Node> front;
    Node *rear = nullptr;
    size_t count = 0;

public:
    bool empty() const { return !front; }
    size_t size() const { return count; }
    void enqueue(const string &id)
    {
        auto n = make_unique<Node>(id);
        Node *p = n.get();
        if (rear)
            rear->next = move(n);
        else
            front = move(n);
        rear = p;
        ++count;
    }
    string dequeue()
    {
        if (!front)
            return {};
        string x = front->id;
        front = move(front->next);
        --count;
        if (!front)
            rear = nullptr;
        return x;
    }
    string peek() const { return front ? front->id : ""; }
    bool remove(const string &id)
    {
        Node *prev = nullptr;
        Node *cur = front.get();
        while (cur && cur->id != id)
        {
            prev = cur;
            cur = cur->next.get();
        }
        if (!cur)
            return false;
        if (prev)
        {
            auto doomed = move(prev->next);
            prev->next = move(doomed->next);
            if (rear == doomed.get())
                rear = prev;
        }
        else
        {
            front = move(front->next);
            if (!front)
                rear = nullptr;
        }
        --count;
        return true;
    }
    vector<string> values() const
    {
        vector<string> v;
        for (Node *p = front.get(); p; p = p->next.get())
            v.push_back(p->id);
        return v;
    }
};
