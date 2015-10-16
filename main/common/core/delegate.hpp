/*
 * delegate.hpp
 * This file is part of Litepix
 *
 * Copyright (C) 2015 - Florian Rommel
 *
 * Litepix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Litepix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Litepix. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * object oriented method pointer
 */

#ifndef _CORE__DELEGATE_HPP_
#define _CORE__DELEGATE_HPP_


/**
 * Create a new Delegate from method.
 * @param   signature   the methods signature: ReturnType(ParameterType)
 * @param   obj_ptr     a pointer to an instance of the class containing the
 *                      method
 * @param   method      the method's name
 * @return  a delegate object encapsulatng the method and the object pointer.
 */
#define DELEGATE_SIG(signature, obj_ptr, method)\
    Delegate<signature>\
    ::bind<_internal::rm_ptr<decltype(obj_ptr)>::type,\
    &_internal::rm_ptr<decltype(obj_ptr)>::type::method>(obj_ptr)

/**
 * Create a new Delegate from method.
 * For overloaded methods use #DELEGATE_SIG which removes ambiguity
 * by accepting the method's signature as a parameter.
 * @param   obj_ptr     a pointer to an instance of the class containing the
 *                      method
 * @param   method      the method's name
 * @return  a delegate object encapsulatng the method and the object pointer.
 */
#define DELEGATE(obj_ptr, method)\
    DELEGATE_SIG(decltype(_internal::delegate_type_helper(\
        &_internal::rm_ptr<decltype(obj_ptr)>::type::method))::signature,\
    obj_ptr, method)

/**
 * Create a new Delegate from static method or function.
 * @param   signature   the methods signature: ReturnType(ParameterType)
 * @param   function    the function's name
 * @return  a delegate object encapsulatng the function pointer.
 */
#define DELEGATE_STATIC_SIG(signature, function)\
    Delegate<signature>::bind_static<function>()

/**
 * Create a new Delegate from static method or function.
 * For overloaded functions use #DELEGATE_STATIC_SIG which removes ambiguity
 * by accepting the function's signature as a parameter.
 * @param   method      the function's name
 * @return  a delegate object encapsulatng the function pointer.
 */
#define DELEGATE_STATIC(function)\
    DELEGATE_STATIC_SIG(decltype(_internal::delegate_static_type_helper(\
        function))::signature, function)


/**
 * A object oriented method pointer that holds not only the method pointer
 * itself but also a pointer to an instance. It can be used to implement
 * observer behaviour.
 */
template<typename Signature>
class Delegate;


/**
 * Delegate template specialization with one argument.
 */
template<typename RetT, typename ArgT>
class Delegate<RetT(ArgT)> {
  public:
    
    /**
     * Initialize an empty delegate.
     */
    constexpr Delegate() : obj_ptr(nullptr), handler(nullptr) {}
    
    /**
     * Create delegate from a method and an object pointer.
     * Use #DELEGATE or #DELEGATE_SIG macro instead.
     * @param   T         object type
     * @param   handler   method pointer
     * @param   obj_ptr   an instance of class T
     * @return  an initialized delegate object.
     */
    template<typename T, RetT (T::*handler)(ArgT)>
    static Delegate<RetT(ArgT)> bind(T* obj_ptr) {
        return Delegate<RetT(ArgT)>(obj_ptr,
            reinterpret_cast<RetT (*)(void*, ArgT)>
            (&(Delegate<RetT(ArgT)>::handler_wrapper<T, handler>)));
    }
    
    /**
     * Create delegate from a static method or function.
     * Use #DELEGATE_STATIC or #DELEGATE_STATIC_SIG macro instead.
     * @param   T         object type
     * @param   handler   method pointer
     * @return  an initialized delegate object.
     */
    template<RetT (*handler)(ArgT)>
    static Delegate<RetT(ArgT)> bind_static() {
        return Delegate<RetT(ArgT)>(nullptr,
            reinterpret_cast<RetT (*)(void*, ArgT)>
            (&(Delegate<RetT(ArgT)>::static_handler_wrapper<handler>)));
    }
    
    /**
     * Execute bound method.
     * @param   arg   method argument
     * @return  method return value
     */
    RetT operator()(ArgT arg) const {
        return handler(obj_ptr, arg);
    }
    
    /**
     * Determine if the delegate is empty.
     * @return   true when the delegate is empty, otherwise false
     */
    bool empty() const {
        return (handler == nullptr);
    }
    
    /**
     * Clear delegate.
     */
    void clear() {
        handler = nullptr;
        obj_ptr = nullptr;
    }
    
  private:
    void* obj_ptr;
    RetT (*handler)(void* obj_ptr, ArgT arg);
    
    constexpr Delegate(void* obj_ptr, RetT (*handler)(void*, ArgT arg))
        : obj_ptr(obj_ptr), handler(handler) {}
    
    template <typename T, RetT (T::*handler)(ArgT arg)>
    static RetT handler_wrapper(T* obj_ptr, ArgT arg) {
        return (obj_ptr->*handler)(arg);
    }
    
    template <RetT (handler)(ArgT arg)>
    static RetT static_handler_wrapper(void*, ArgT arg) {
        return handler(arg);
    }
};


/**
 * Delegate template specialization with no argument.
 */
template<typename RetT>
class Delegate<RetT()> {
  public:
    
    /**
     * Initialize an empty delegate.
     */
    constexpr Delegate() : obj_ptr(nullptr), handler(nullptr) {}
    
    /**
     * Create delegate from a method and an object pointer.
     * Use #DELEGATE or #DELEGATE_SIG macro instead.
     * @param   T         object type
     * @param   handler   method pointer
     * @param   obj_ptr   an instance of class T
     * @return  an initialized delegate object.
     */
    template<typename T, RetT (T::*handler)()>
    static Delegate<RetT()> bind(T* obj_ptr) {
        return Delegate<RetT()>(obj_ptr, reinterpret_cast<RetT (*)(void*)>
            (&(Delegate::handler_wrapper<T, handler>)));
    }
    
    /**
     * Create delegate from a static method or function.
     * Use #DELEGATE_STATIC or #DELEGATE_STATIC_SIG macro instead.
     * @param   T         object type
     * @param   handler   method pointer
     * @return  an initialized delegate object.
     */
    template<RetT (*handler)()>
    static Delegate<RetT()> bind_static() {
        return Delegate<RetT()>(nullptr, reinterpret_cast<RetT (*)(void*)>
            (&(Delegate<RetT()>::static_handler_wrapper<handler>)));
    }
    
    /**
     * Execute bound method.
     * @return  method return value
     */
    RetT operator()() const {
        return handler(obj_ptr);
    }
    
    /**
     * Determine if the delegate is empty.
     * @return   true when the delegate is empty, otherwise false
     */
    bool empty() const {
        return (handler == nullptr);
    }
    
    /**
     * Clear delegate.
     */
    void clear() {
        handler = nullptr;
        obj_ptr = nullptr;
    }
    
  private:
    void* obj_ptr;
    RetT (*handler)(void* obj_ptr);
    
    constexpr Delegate(void* obj_ptr, RetT (*handler)(void*))
        : obj_ptr(obj_ptr), handler(handler) {}
    
    template <typename T, RetT (T::*handler)()>
    static RetT handler_wrapper(T* obj_ptr) {
        return (obj_ptr->*handler)();
    }
    
    template <RetT (handler)()>
    static RetT static_handler_wrapper(void*) {
        return handler();
    }
};


#ifndef DOCUMENTATION_SKIP
// Internal namespace used by DELEGATE* macros
namespace _internal {
    template<typename T> struct rm_ptr {typedef T type;};
    template<typename T> struct rm_ptr<T*> {typedef T type;};
    template<typename T> struct rm_ptr<T* const> {typedef T type;};
    template<typename T> struct rm_ptr<T* volatile> {typedef T type;};
    template<typename T> struct rm_ptr<T* const volatile> {typedef T type;};
    
    template<typename S> struct DelegateTypeInfo {typedef S signature;};
    template<typename T, typename R, typename P>
    DelegateTypeInfo<R (P)> delegate_type_helper(R (T::*f)(P));
    template<typename T, typename R>
    DelegateTypeInfo<R ()> delegate_type_helper(R (T::*f)());
    template<typename R, typename P>
    DelegateTypeInfo<R (P)> delegate_static_type_helper(R (f)(P));
    template<typename R>
    DelegateTypeInfo<R ()> delegate_static_type_helper(R (f)());
}
#endif


#endif
