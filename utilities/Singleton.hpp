#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

template <typename T>
class Singleton
{
protected:
    // Constructeur/destructeur
    Singleton() {}
    ~Singleton() {}

public:
    static T *instance()
    {
        if (_singleton==0)
        {
            _singleton = new T;
        }

        return (static_cast<T*> (_singleton));
    }

    static void kill()
    {
        if (_singleton!=0)
        {
            delete _singleton;
            _singleton = 0;
        }
    }

private:
    static T *_singleton;
};

template <typename T> T *Singleton<T>::_singleton = 0;

#endif // __SINGLETON_HPP__