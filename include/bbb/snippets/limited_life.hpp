#pragma once
#ifndef BBB_LIMITED_LIFE_HPP
#define BBB_LIMITED_LIFE_HPP

#include <type_traits>
#include <chrono>
#include <thread>

namespace bbb {
	namespace {
		float timer() {
			using namespace std::chrono;
			static auto start = high_resolution_clock::now();
			return duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000000.0f;
		}
	};
	
	struct limited_life_base {
		limited_life_base(float born, float lifespan)
		: priv{born, lifespan}
		{};
		virtual float time() const = 0;
		
		float born() const noexcept
		{ return priv.born; }
		float lifespan() const noexcept
		{ return priv.lifespan; }
		
		float age() const noexcept { return time() - born(); }
		float progress() const noexcept { return std::min(1.0f, age() / lifespan()); }
		float progress_inv() const noexcept { return 1.0f - progress(); }
		bool alive() const noexcept { return age() <= lifespan(); }
		bool dead() const noexcept { return !alive(); }
		
	private:
		struct {
			float born;
			float lifespan;
		} priv;
	};
	
	template <typename object_t, float time_func() = timer>
	struct limited_life_injector : limited_life_base {
		limited_life_injector() noexcept
		: limited_life_injector{ settings().lifespan }
		{};
		
		limited_life_injector(float lifespan) noexcept
		: limited_life_base{ time(), lifespan }
		{};
		
		virtual float time() const override
		{ return time_func(); };
		
		friend
		auto dispose(std::vector<object_t> &vs) {
			auto it = std::remove_if(vs.begin(), vs.end(), [](const auto &v) {
				return !v.alive();
			});
			vs.erase(it, vs.end());
		}
		friend
		auto sort(std::vector<object_t> &vs) {
			auto it = std::sort(vs.begin(), vs.end(), [](const auto &x, const auto &y) {
				return x.born() < y.born();
			});
		}
		
		struct settings {
			float lifespan{1.0f};
		};
		static settings &settings() {
			static struct settings _;
			return _;
		}
	};
	
	template <typename base_type, float time_func() = timer>
	struct limited_life : limited_life_injector<base_type, time_func> {
		using injector = limited_life_injector<base_type, time_func>;
		limited_life() noexcept
		: injector{}
		, v{}
		{};
		
		template <typename = typename std::enable_if<!std::is_arithmetic<base_type>::value>::type>
		limited_life(float lifespan) noexcept
		: injector{ lifespan }
		, v{}
		{};
		
		limited_life(const base_type &v) noexcept
		: injector{}
		, v{v}
		{};
		
		limited_life(const base_type &v, float lifespan) noexcept
		: injector{ lifespan }
		, v{v}
		{};
		
		limited_life(base_type &&v) noexcept
		: injector{}
		, v{std::move(v)}
		{};
		
		limited_life(base_type &&v, float lifespan) noexcept
		: injector{ lifespan }
		, v{std::move(v)}
		{};
		
		base_type &operator()() noexcept { return v; }
		const base_type &operator()() const noexcept { return v; }
		base_type &operator->() noexcept { return v; }
		const base_type &operator->() const noexcept { return v; }
		
		friend
		auto dispose(std::vector<limited_life> &vs) {
			auto it = std::remove_if(vs.begin(), vs.end(), [](const auto &v) {
				return !v.alive();
			});
			vs.erase(it, vs.end());
		}
		
		friend
		auto copy(const std::vector<base_type> &src, std::vector<limited_life> &vs) {
			std::copy(src.begin(), src.end(), std::back_inserter(vs));
		}
		friend
		auto copy(const std::vector<base_type> &src, std::vector<limited_life> &vs, std::function<float(const base_type &)> &&lifetime) {
			std::transform(src.begin(),
				src.end(),
				std::back_inserter(vs),
				[&lifetime](const base_type &v) { return limited_life_injector{ v, lifetime(v) }; });
		}
		friend
		auto move(std::vector<base_type> &&src, std::vector<limited_life> &vs) {
			std::move(src.begin(), src.end(), std::back_inserter(vs));
			src.clear();
		}
		friend
		auto move(std::vector<base_type> &&src, std::vector<limited_life> &vs, std::function<float(const base_type &)> &&lifetime) {
			std::transform(std::make_move_iterator(src.begin()),
				std::make_move_iterator(src.end()),
				std::back_inserter(vs),
				[&lifetime](base_type &&v) { return limited_life_injector{ std::move(v), lifetime(v) }; });
			src.clear();
		}
		
	protected:
		base_type v;
	};
}; // namespace bbb

#endif // BBB_LIMITED_LIFE_HPP
