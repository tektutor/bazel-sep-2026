#pragma once
// lib/core/result.h — header-only Result<T, E>
#include <string>
#include <variant>
#include <stdexcept>

namespace core {

template <typename T, typename E = std::string>
class Result {
public:
    static Result Ok(T value)  { return Result(std::in_place_index<0>, std::move(value)); }
    static Result Err(E error) { return Result(std::in_place_index<1>, std::move(error)); }

    bool ok()  const { return data_.index() == 0; }
    bool err() const { return data_.index() == 1; }

    const T& value() const {
        if (!ok()) throw std::runtime_error("Result::value() on Err");
        return std::get<0>(data_);
    }
    const E& error() const {
        if (!err()) throw std::runtime_error("Result::error() on Ok");
        return std::get<1>(data_);
    }

private:
    template <std::size_t I, typename V>
    Result(std::in_place_index_t<I> tag, V&& v) : data_(tag, std::forward<V>(v)) {}
    std::variant<T, E> data_;
};

}  // namespace core
