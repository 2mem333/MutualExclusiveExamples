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


Sem empty(1), full(0), mutexSem(1);

std::queue<std::string> buffer;
std::string notdefteri;

void producer() {
    for (int i = 0; i < 10; ++i) {
        down(&empty);
        down(&mutexSem);
        std::cout << "Producer: urettim.\n";
        up(&mutexSem);
        up(&full);
    }
}

void consumer() {
    for (int i = 0; i < 10; ++i) {
        down(&full);
        down(&mutexSem);
        std::cout << "Consumer: tukettim.\n";
        up(&mutexSem);
        up(&empty);
    }
}

int main() {
    std::thread p(producer), c(consumer);
    p.join(); c.join();
}
