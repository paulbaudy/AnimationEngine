#pragma once

#include <memory>

namespace AN
{
	template<typename T>
	using TUniquePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	[[nodiscard]] constexpr TUniquePtr<T> MakeUnique(Args&& ... InArgs)
	{
		return std::make_unique<T>(std::forward<Args>(InArgs)...);
	}
}