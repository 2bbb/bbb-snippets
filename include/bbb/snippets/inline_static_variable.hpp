//
//  bbb/snippets/inline_static_variable.hpp
//
//  Created by 2bit on 2025/02/14.
//

#pragma once

#ifndef bbb_snippets_inline_static_variable_hpp
#define bbb_snippets_inline_static_variable_hpp

namespace bbb {
	template <typename type, typename tag = void>
	struct inline_static_variable {
		static type &get() {
			static type _;
			return _;
		}
	};

	struct dummy_ref {
		template <typename type>
		operator type &() {
			return inline_static_variable<type>::get();
		}
	};
}; // namespace bbb

#endif // bbb_snippets_inline_static_variable_hpp
