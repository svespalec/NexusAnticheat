#pragma once

#include <type_traits>
#include <functional>
#include <unordered_map>
#include <memory>
#include "../utils/hash.hpp"

namespace nexus
{
class feature_base
{
public:
	virtual ~feature_base()                   = default;
	virtual bool     initialize()             = 0;
	virtual void     shutdown()               = 0;
	virtual bool     run_check()              = 0;
	virtual uint64_t get_feature_hash() const = 0;

	bool is_enabled() const
	{
		return enabled_;
	}

	void set_enabled( bool state )
	{
		enabled_ = state;
	}

private:
	bool enabled_ = false;
};

class feature_registry
{
public:
	using feature_ptr     = std::unique_ptr<feature_base>;
	using feature_creator = std::function<feature_ptr()>;

	template <typename T>
	static void register_feature()
	{
		static_assert( std::is_base_of_v<feature_base, T>, "T must derive from feature_base" );

		get_creators()[T::feature_hash] = []() -> feature_ptr {
			return std::make_unique<T>();
		};
	}

	static feature_ptr create_feature( uint64_t hash )
	{
		auto& creators = get_creators();
		auto  it       = creators.find( hash );

		return ( it != creators.end() ) ? it->second() : nullptr;
	}

private:
	static std::unordered_map<uint64_t, feature_creator>& get_creators()
	{
		static std::unordered_map<uint64_t, feature_creator> creators;
		return creators;
	}
};

} // namespace nexus
