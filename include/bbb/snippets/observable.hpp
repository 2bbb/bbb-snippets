#pragma once

#ifndef BBB_OBSERVABLE_HPP
#define BBB_OBSERVABLE_HPP

#include <functional>

namespace bbb {
    template <typename value_type>
    struct observable {
        using assign_notification_t = std::function<void(value_type)>;
        using change_notification_t = std::function<void(value_type, value_type)>;
        
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
            if(change && v != new_value) change(v, new_value);
            v = new_value;
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
        observable &on_change(change_notification_t change)
        { this->change = change; return *this; } 
        
    protected:
        assign_notification_t assign{};
        change_notification_t change{};
        value_type v{};
    };
}; // namespace bbb

#endif // BBB_OBSERVABLE_HPP
