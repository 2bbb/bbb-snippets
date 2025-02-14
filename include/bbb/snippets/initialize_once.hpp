//
//  bbb/snippets/initialize_once.hpp
//
//  Created by 2bit on 2025/02/14.
//

#ifndef initialize_once_h
#define initialize_once_h

#include <type_traits>

namespace bbb {
    namespace detail {
        template <typename ...>
        using void_t = void;
        
        template <typename, typename = void>
        struct has_static_void_initialize
        : std::false_type {};
        
        template <typename T>
        struct has_static_void_initialize<T, void_t<decltype(T::initialize)>>
        : std::true_type {};
    };
    
    template <typename object_t>
    struct initialize_once {
        initialize_once() {
            static std::once_flag once;
            std::call_once(once, [] {
                call_initialize<object_t>();
            });
        }
        
    private:
        template <typename obj_t>
        static auto call_initialize()
            -> typename std::enable_if<
                detail::has_static_void_initialize<obj_t>::value
            >::type
        {
            object_t::initialize();
        }
        
        template <typename obj_t>
        static auto call_initialize()
            -> typename std::enable_if<
                !detail::has_static_void_initialize<obj_t>::value
            >::type
        {
            std::cerr << "'static void initialize()' is not defined on " << typeid(obj_t).name() << std::endl;
        }
    };
};

#endif /* initialize_once_h */
