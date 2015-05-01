#ifndef SINGLETON_H
#define SINGLETON_H


// usage: MyClass *MyClass = singleton<MyClass>();
template <class Type>
inline Type *singleton()
{
    static Type *x = new Type;
    return x;
}


#endif // SINGLETON_H

