#pragma once

#ifndef BBB_INLINE_STATIC_VARIABLE_HPP
#define BBB_INLINE_STATIC_VARIABLE_HPP

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

#endif // BBB_INLINE_STATIC_VARIABLE_HPP