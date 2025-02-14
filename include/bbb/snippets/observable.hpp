//
//  bbb/snippets/observable.hpp
//
//  Created by 2bit on 2025/02/14.
//

#pragma once

#ifndef bbb_snippets_observable_hpp
#define bbb_snippets_observable_hpp

#include <functional>
#include <type_traits>

namespace bbb {
    template <typename value_type>
    struct observable {
        using assign_notification_t = std::function<void(value_type)>;
        using change_notification_t = std::function<bool(value_type, value_type)>;
        using void_change_notification_t = std::function<void(value_type, value_type)>;

        observable() = default;
        observable(const observable &) = default;
        observable(observable &&) = default;
        
        observable(value_type v,
                   assign_notification_t assign,
                   change_notification_t change)
        : v{v}
        , assign{assign}
        , change{change}
        {};
        observable(value_type v, assign_notification_t assign)
        : v{v}
        , assign{assign}
        {};
        observable(assign_notification_t assign)
        : assign{assign}
        {};
        observable(value_type v, change_notification_t change)
        : v{v}
        , change{change}
        {};
        observable(change_notification_t change)
        : change{change}
        {};
        observable(value_type v)
        : v{v}
        {};
        
        observable &operator=(const observable &) = default;
        observable &operator=(observable &&) = default;
        observable &operator=(value_type new_value) {
            if(change) {
                if(v != new_value) {
                    if(change(v, new_value)) {
                        v = new_value;
                    }
                }
            } else {
                v = new_value;
            }
            if(assign) assign(v);
            return *this;
        };
        
        const std::function<void(value_type)> notify{[this](value_type v) { *this = v; }};
        
        operator value_type() const { return v; };
        operator value_type &() { return v; };
        value_type operator()() const { return v; };
        value_type &operator()() { return v; };
        
        observable &on_assign(assign_notification_t assign)
        { this->assign = assign; return *this; }
        
        template <typename change_notification_callback_t>
        auto on_change(change_notification_callback_t change)
            -> typename std::enable_if<
                std::is_same<
                    decltype(std::declval<change_notification_callback_t>()(value_type{}, value_type{})),
                    bool
                >::value,
                observable &
            >::type
        { this->change = change; return *this; }
        
        template <typename change_notification_callback_t>
        auto on_change(change_notification_callback_t change)
            -> typename std::enable_if<
                std::is_same<
                    decltype(std::declval<change_notification_callback_t>()(value_type{}, value_type{})),
                    void
                >::value,
                observable &
            >::type
        {
            return on_change([change](value_type from, value_type to) -> bool {
                change(from, to);
                return true;
            });
        }

    protected:
        assign_notification_t assign{};
        change_notification_t change{};
        value_type v{};
    };
}; // namespace bbb

#endif // bbb_snippets_observable_hpp
