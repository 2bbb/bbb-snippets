//
//  bbb/snippets/noncopyable.hpp
//
//  Created by 2bit on 2025/02/15.
//

#ifndef bbb_snippets_noncopyable_hpp
#define bbb_snippets_noncopyable_hpp

namespace bbb {
    struct noncopyable {
        noncopyable() {}
    private:
        noncopyable(const noncopyable &) = delete;
        noncopyable& operator=(const noncopyable &) = delete;
    };
};

#endif /* bbb_snippets_noncopyable_hpp */
