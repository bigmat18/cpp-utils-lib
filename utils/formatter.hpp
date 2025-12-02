#pragma once

#include <tuple>
#include <type_traits>
#include <ranges>
#include <concepts>
#include <format>

template <typename T>
concept Formattable = requires(T v, std::format_context& ctx) {
    std::formatter<std::remove_cvref_t<T>>{}.format(v, ctx);
};

template <typename... Ts>
concept AllFormattable = (Formattable<Ts> && ...);

template <typename T>
constexpr std::string_view type_name() {
    #if defined (__clang__) || defined (__GNUC__)
        std::string_view raw = __PRETTY_FUNCTION__;
        auto start = raw.find('=') + 2;
        auto end = raw.find(';', start);
        return raw.substr(start, end - start);
    #elif defined(_MSC_VER)
        std::string_view p = __FUNCSIG__;

        auto prefix = std::string_view{"type_name_impl<"};
        auto start = p.find(prefix);
        if (start == std::string_view::npos) {
            return {};
        }
        start += prefix.size();
        auto end = p.find('>', start);
        return p.substr(start, end - start);
    #else 
        return ""    
    #endif
}


template <typename T1, typename T2> 
struct std::formatter<std::pair<T1, T2>> : std::formatter<std::string> {
    auto format(const std::pair<T1, T2> p, auto& ctx) const {
        auto s = std::format("({}, {})", p.first, p.second);
        return std::formatter<std::string>::format(s, ctx);
    }
};


template <typename ...T>
struct std::formatter<std::tuple<T...>> : std::formatter<std::string> {
    auto format(const std::tuple<T...> t, auto& ctx) const {
        std::string s = "(";

        std::apply([&](const auto&... args) {
            std::size_t i = 0;
            ((s += std::format("{}: {}{}", type_name<decltype(args)>(), args, (++i < sizeof...(args)) ? ", " : "")), ...);
        }, t);

        s += ")";
        return std::formatter<std::string>::format(s, ctx);
    }
};

template <std::ranges::range R> requires (
    !std::convertible_to<R, std::string_view> &&
    !std::convertible_to<R, std::string>
)
struct std::formatter<R> : std::formatter<std::string> {
    auto format(const R& range, auto& ctx) const {
        std::string s = "[";

        std::size_t i = 0;
        for (auto&& elem : range) {
            s += std::format("{}", elem);
            if (++i < std::ranges::size(range))
                s += ", ";
        }
        
        s += "]";
        return std::formatter<std::string>::format(s, ctx);
    }
};


