#ifndef SINGLETON_H
#define SINGLETON_H
#include <memory>
#include <mutex>
#include <iostream>

template<typename T>
class Singleton{
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag flag;
        std::call_once(flag,[&](){
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    ~Singleton(){
        std::cout << "this is singleton destruct" << std::endl;
    }

    void PrintAdress(){
        std::cout << _instance.get() << std::endl;
    }

protected:
    static std::shared_ptr<T> _instance;
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator= (const Singleton<T>&) = delete;
// private:
//     friend class Singleton<T>;
//     Singleton();
};

template <typename T>

std::shared_ptr<T> Singleton<T>::_instance = nullptr;


#endif // SINGLETON_H
