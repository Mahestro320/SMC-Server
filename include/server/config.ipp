#pragma once

#include "util/resolver/evs_str.hpp"

template<typename T> T Config::getValueFromTree(const std::string& key) {
    if constexpr (std::is_same_v<T, std::string>) {
        const std::string raw_val{property_tree.get<std::string>(key)};
        EVSStrResolver<> resolver{};
        resolver.setInput(raw_val);
        if (!resolver.resolve()) {
            throw std::runtime_error{"error while resolving key " + key};
        }
        return resolver.getOutput();
    } else {
        return property_tree.get<T>(key);
    }
}
