#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

struct Sem {
    int v;
    std::mutex m;
    std::condition_variable cv;
    explicit Sem(int x) : v(x) {}
};

void down(Sem* s) {
    std::unique_lock<std::mutex> lk(s->m);
    s->cv.wait(lk, [&] { return s->v > 0; });
    --s->v;
}
void up(Sem* s) {
    std::lock_guard<std::mutex> lk(s->m);
    ++s->v;
    s->cv.notify_one();
}
  


Sem mutex(1); int nextTurn = 0;
char msg[3] = { 'A','B','C' };

void test(int i)
{
    if (nextTurn == i)
    {
        std::cout << msg[i] << "\n";
        nextTurn = (i + 1) % 3;
    }
}

void processA()
{
    while (1) {
        if(nextTurn == 0){
        down(&mutex);
        test(0);
        up(&mutex);
        }
    }

}
void processB()
{
    while (1) {
        down(&mutex);
        test(1);
        up(&mutex);
    }

}
void processC()
{
    while (1) {
        down(&mutex);
        test(2);
        up(&mutex);
    }

}
int main() {
    std::thread a(processA), b(processB), c(processC);
    a.join(); b.join();  c.join();
}
