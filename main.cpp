#include <chrono>
#include <functional>
#include <print>
#include <string>
#include <utility>

class ExecutionTimer {
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start = Clock::now();

public:
    ~ExecutionTimer(){
        const auto end = Clock::now();
        const auto elapsed = std::chrono::duration<double, std::micro>(end - start);
        std::println("Execution time: {:.3f} us", elapsed.count());
    }
};

template <typename Callable>
class Timed{
    Callable m_callable;
    std::string m_name;

public:
    Timed(Callable callable, std::string name)
    : m_callable(std::move(callable))
    , m_name(std::move(name)) {}

    template <typename... Args>
    decltype(auto) operator()(Args... args){
        ExecutionTimer timer;
        return std::invoke(m_callable, std::forward<Args>(args)...);
    }
};

int multiply(int a, int b)
{
    return a * b;
}

class Mlass{
    std::string txt;
public:
    Mlass(std::string c): txt(std::move(c)) {}
    char& operator[](std::size_t idx) { return txt[idx]; }
    const std::string& get() const { return txt; }
};

int main() {
    Timed t1(multiply, "multiply");
    int result = t1(10, 20);
    std::println("Result of multiply: {}", result);


    Timed t2([](int a, int b) { return a * b; }, "multiply_lambda");
    int result2 = t2(12, 13);
    std::println("Result of multiply_lambda: {}", result2);

    Mlass obj("Hello");
    Timed t3(&Mlass::operator[], "Mlass::operator[]");
    t3(&obj, 1) = 'a';
    std::println("Result of Mlass::operator[]: {}", obj.get());

}
