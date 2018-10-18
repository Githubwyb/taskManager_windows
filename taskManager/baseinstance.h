//
//  BaseInstance.hpp
//  CoreBusiness
//
//  Created by LS on 28/06/2018.
//  Copyright © 2018 LS. All rights reserved.
//
/**
 *  继承该类的对象为单例
 */
#ifndef BaseInstance_hpp
#define BaseInstance_hpp

class BaseNoCopy {
public:
    BaseNoCopy(){ }
private:
    BaseNoCopy(const BaseNoCopy &obj);
    BaseNoCopy(BaseNoCopy *pObj);
    BaseNoCopy(const BaseNoCopy *pObj);
    BaseNoCopy& operator = (const BaseNoCopy &obj);
    BaseNoCopy& operator = (const BaseNoCopy *pObj);
};


template <class T> class BaseInstance: public BaseNoCopy {
public:
    static T &getInstance() {
        static T s_instance;
        CreateObject.do_nothing();

        return s_instance;
    }
protected:
    BaseInstance(){ }
private:
    BaseInstance(const BaseInstance &obj);
    BaseInstance operator = (const BaseInstance &obj);

    struct ObjectCreator {
        ObjectCreator() {
            BaseInstance<T>::getInstance();
        }

        inline void do_nothing()const{ }
    };
    static ObjectCreator CreateObject;
};

template <typename T>
typename BaseInstance<T>::ObjectCreator BaseInstance<T>::CreateObject;

#if 0

class G: public cb::BaseInstance<G> {
public:
    void test() {
        std::cout << this << std::endl;
    }
};

void main() {
    G& g = G::getInstance();

    g.test();
}

#endif


#endif /* BaseInstance_hpp */
