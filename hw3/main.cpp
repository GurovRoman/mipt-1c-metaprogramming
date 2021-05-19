#include <iostream>
#include <map>
#include <memory>
#include <limits>


class LongevityQueue {

    // type erasure
    struct DeleteHelperBase {
        virtual ~DeleteHelperBase() = default;
    };

    template <class T>
    struct DeleteHelper : DeleteHelperBase {
        explicit DeleteHelper(T* ptr) : ptr_(ptr) {};

        ~DeleteHelper() override {
            delete ptr_;
        }
    private:
        T* ptr_;
    };

private:
    LongevityQueue() = default;

public:
    LongevityQueue(const LongevityQueue&) = delete;
    LongevityQueue(LongevityQueue&&) = default;
    LongevityQueue& operator= (const LongevityQueue&) = delete;
    LongevityQueue& operator= (LongevityQueue&&) = default;

    template<typename T>
    static void setLongevity(T* obj, unsigned int longevity) {
        if (!instance_) {
            instance_ = new LongevityQueue;
        }
        instance_->pr_queue_[std::make_pair(longevity, instance_->next_id_--)] = std::make_unique<DeleteHelper<T>>(obj);
        std::atexit(deleteNext);
    }

    static void deleteNext() {
        auto next = instance_->pr_queue_.begin();

        // this calls the destructor of entry
        instance_->pr_queue_.erase(next);

        if (instance_->pr_queue_.empty()) {
            delete instance_;
            instance_ = nullptr;
        }
    }


private:
    std::map<std::pair<unsigned int, size_t>, std::unique_ptr<DeleteHelperBase>> pr_queue_;
    size_t next_id_ {std::numeric_limits<size_t>::max()};

    inline static LongevityQueue* instance_ {nullptr};
};

template<typename T>
void setLongevity(T* obj, unsigned int longevity) {
    LongevityQueue::setLongevity(obj, longevity);
}


struct A {
    explicit A(int n) : value(n) {}
    ~A() {
        std::cout << "Class A with longevity " << value << " got destroyed\n";
    }
private:
    int value;
};

struct B {
    explicit B(int n) : value(n) {}
    ~B() {
        std::cout << "Class B with longevity " << value << " got destroyed\n";
    }
private:
    int value;
};


int main () {
    setLongevity(new A(0), 0);
    setLongevity(new B(1), 1);
    setLongevity(new A(1), 1);
    setLongevity(new B(2), 100);

}
