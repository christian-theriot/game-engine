#ifndef __ENGINE_V2_CORE_RESULT_HPP
#define __ENGINE_V2_CORE_RESULT_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/serialization/serializable.hpp>
#include <string>
#include <variant>
#include <utility>
#include <stdexcept>

namespace Engine::Core
{
    template <typename T>
    class Result : public Serialization::ISerializable
    {
        std::variant<T, std::runtime_error> data;

    public:
        inline Result(T value) : data(std::move(value)) {}
        inline Result(std::runtime_error error) : data(std::move(error)) {}

        inline bool ok() const { return std::holds_alternative<T>(data); }
        explicit operator bool() const { return ok(); }

        inline T &value() { return std::get<T>(data); }
        inline const T &value() const { return std::get<T>(data); }
        inline const std::runtime_error &error() const { return std::get<std::runtime_error>(data); }

        inline T valueOr(T fallback) const
        {
            return ok() ? std::get<T>(data) : std::move(fallback);
        }
    };

    template <typename T>
    inline Result<T> Ok(T value) { return Result<T>(std::move(value)); }

    template <typename T>
    inline Result<T> Fail(const std::string &message) { return Result<T>(std::runtime_error(message)); }
}

#endif