#include <iostream>
#include <fstream>
#include <deque>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "thread.h"

using std::cout;
using std::deque;
using std::endl;
using std::ifstream;
using std::map;
using std::string;
using std::vector;

int MAX_DISK_QUEUE = 0;
int num_threads = 0;
int cur_num_track = 0;

// mutexes
mutex qmutex; // queue lock
mutex dmutex; // deque lock
mutex mmutex; // map lock
mutex pmutex; // print lock

// conditional variables
cv qnf; // queue not full
cv qf;  // queue full

struct Request
{
    int tag;
    int num_track;
};

deque<Request> requests;
vector<deque<int>> tracks;
map<int, int> num_requests;

void starter(void *a);
void servicer(void *a);
void requester(void *t);
bool isServiced(int tag);
bool shortest_seek_time(Request &start, Request &end);

int main()
{
    // command line arguments
    MAX_DISK_QUEUE = 3;
    num_threads = 5;

    // starts the thread library and creates the initial thread
    cpu::boot(1, (thread_startfunc_t)starter, (void *)0, false, false, 0);
    return 0;
}

void starter(void *a)
{
    deque<int> disk0;
    disk0.push_back(785);
    disk0.push_back(53);
    tracks.push_back(disk0);
    uintptr_t num = 0;
    thread tr0((thread_startfunc_t)requester, (void *)num);

    deque<int> disk1;
    disk1.push_back(350);
    disk1.push_back(914);
    tracks.push_back(disk1);
    num = 1;
    thread tr1((thread_startfunc_t)requester, (void *)num);

    deque<int> disk2;
    disk2.push_back(827);
    disk2.push_back(567);
    tracks.push_back(disk2);
    num = 2;
    thread tr2((thread_startfunc_t)requester, (void *)num);

    deque<int> disk3;
    disk3.push_back(302);
    disk3.push_back(230);
    tracks.push_back(disk3);
    num = 3;
    thread tr3((thread_startfunc_t)requester, (void *)num);

    deque<int> disk4;
    disk4.push_back(631);
    disk4.push_back(11);
    tracks.push_back(disk4);
    num = 4;
    thread tr4((thread_startfunc_t)requester, (void *)num);

    thread ts((thread_startfunc_t)servicer, (void *)0);
}

void servicer(void *a)
{
    while (num_threads > 0 || (requests.size() > 0 && MAX_DISK_QUEUE >= 0))
    {
        qmutex.lock();
        assert_interrupts_enabled();
        if (MAX_DISK_QUEUE > num_threads)
            MAX_DISK_QUEUE = num_threads;
        while (requests.size() < MAX_DISK_QUEUE)
        {
            qf.wait(qmutex);
        }
        sort(requests.begin(), requests.end(), shortest_seek_time);
        Request r = requests.front();
        requests.pop_front();
        cur_num_track = r.num_track;
        mmutex.lock();
        assert_interrupts_enabled();
        num_requests[r.tag]--;
        mmutex.unlock();
        assert_interrupts_enabled();
        if (num_requests[r.tag] == 0)
        {
            num_threads--;
        }
        pmutex.lock();
        assert_interrupts_enabled();
        cout << "service requester " << r.tag << " track " << r.num_track << endl;
        pmutex.unlock();
        assert_interrupts_enabled();
        qnf.broadcast();
        assert_interrupts_enabled();
        qmutex.unlock();
        assert_interrupts_enabled();
    }
}

void requester(void *t)
{
    dmutex.lock();
    assert_interrupts_enabled();
    uintptr_t tag = (uintptr_t)t;
    deque<int> track = tracks[tag];
    mmutex.lock();
    assert_interrupts_enabled();
    num_requests[tag] = track.size();
    mmutex.unlock();
    assert_interrupts_enabled();
    dmutex.unlock();
    assert_interrupts_enabled();
    while (num_requests[tag] > 0 && MAX_DISK_QUEUE > 0)
    {
        qmutex.lock();
        assert_interrupts_enabled();
        while (requests.size() == MAX_DISK_QUEUE || (track.empty() && num_requests[tag] >= 0) || isServiced(tag))
        {
            qnf.wait(qmutex);
            assert_interrupts_enabled();
        }
        if (!track.empty())
        {
            Request r;
            r.num_track = track.front();
            track.pop_front();
            r.tag = tag;
            requests.push_back(r);
            pmutex.lock();
            assert_interrupts_enabled();
            cout << "requester " << r.tag << " track " << r.num_track << endl;
            pmutex.unlock();
            assert_interrupts_enabled();
            qf.signal();
            assert_interrupts_enabled();
            qmutex.unlock();
            assert_interrupts_enabled();
        }
    }
}

bool isServiced(int tag)
{
    for (int i = 0; i < requests.size(); i++)
    {
        if (requests[i].tag == tag)
        {
            return true;
        }
    }
    return false;
}

bool shortest_seek_time(Request &start, Request &end)
{
    return abs(start.num_track - cur_num_track) < abs(end.num_track - cur_num_track);
}