//
//  bbb/snippets/automaton.hpp
//
//  Created by 2bit on 2025/02/14.
//

#ifndef bbb_snippets_automaton_hpp
#define bbb_snippets_automaton_hpp

#include <map>

namespace bbb {
    template <typename value_type>
    struct automaton {
        automaton() = default;
        automaton(const automaton &) = default;
        automaton(automaton &&) = default;
        automaton(value_type value)
        : value(value)
        {};

        automaton &operator=(const automaton &) = default;
        automaton &operator=(automaton &&) = default;
        automaton &operator=(value_type new_value) {
            update(new_value);
            return *this;
        }

        void transtion(value_type from, value_type to) {
            transitions.emplace(from, to);
        }
        
        void reset(value_type initial_value) {
            value = initial_value;
        }
        
        std::pair<bool, value_type> update(value_type new_value) {
            if(transitionable(new_value)) {
                value = new_value;
                return { true, value };
            }
            return { false, value };
        }
        
        bool transitionable(value_type from, value_type to) const {
            auto range = transitions.equal_range(from);
            for(auto it = range.first; it != range.second; ++it) {
                if(it->second == to) {
                    return true;
                }
            }
            return false;
        }

        bool transitionable(value_type new_value) const {
            auto range = transitions.equal_range(value);
            for(auto it = range.first; it != range.second; ++it) {
                if(it->second == new_value) {
                    return true;
                }
            }
            return false;
        }
        
        operator value_type() const
        { return value; }
        
    private:
        std::multimap<value_type, value_type> transitions;
        value_type value;
    };
}

#endif /* bbb_snippets_automaton_hpp */
