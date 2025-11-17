#pragma once
#include <string>
#include <variant>
#include <map>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <type_traits>

namespace nlohmann {

class json {
public:
    using object_t = std::map<std::string, json>;
    using array_t = std::vector<json>;
    using value_t = std::variant<std::nullptr_t, bool, double, std::string, object_t, array_t>;

    json() : value_(nullptr) {}
    json(std::nullptr_t) : value_(nullptr) {}
    json(bool b) : value_(b) {}
    json(double d) : value_(d) {}
    json(int i) : value_(static_cast<double>(i)) {}
    json(const std::string& s) : value_(s) {}
    json(const char* s) : value_(std::string(s)) {}
    json(const object_t& o) : value_(o) {}
    json(const array_t& a) : value_(a) {}

    static json parse(const std::string& text) {
        size_t pos = 0;
        json result = parse_value(text, pos);
        skip_ws(text, pos);
        if (pos != text.size()) {
            throw std::runtime_error("Unexpected trailing characters in json");
        }
        return result;
    }

    bool is_object() const { return std::holds_alternative<object_t>(value_); }
    bool is_array() const { return std::holds_alternative<array_t>(value_); }
    bool is_string() const { return std::holds_alternative<std::string>(value_); }
    bool is_number() const { return std::holds_alternative<double>(value_); }
    bool is_boolean() const { return std::holds_alternative<bool>(value_); }

    object_t& get_object() { return std::get<object_t>(value_); }
    const object_t& get_object() const { return std::get<object_t>(value_); }
    array_t& get_array() { return std::get<array_t>(value_); }
    const array_t& get_array() const { return std::get<array_t>(value_); }

    json& operator[](const std::string& key) {
        if (!is_object()) {
            value_ = object_t{};
        }
        return std::get<object_t>(value_)[key];
    }

    const json& operator[](const std::string& key) const {
        const auto& obj = std::get<object_t>(value_);
        auto it = obj.find(key);
        if (it == obj.end()) throw std::out_of_range("json key not found");
        return it->second;
    }

    json& operator[](size_t idx) {
        return std::get<array_t>(value_).at(idx);
    }

    const json& operator[](size_t idx) const {
        return std::get<array_t>(value_).at(idx);
    }

    bool contains(const std::string& key) const {
        if (!is_object()) return false;
        return get_object().count(key) > 0;
    }

    template<typename T>
    T get() const {
        if constexpr (std::is_same_v<T, std::string>) {
            return std::get<std::string>(value_);
        } else if constexpr (std::is_same_v<T, bool>) {
            return std::get<bool>(value_);
        } else if constexpr (std::is_integral_v<T>) {
            return static_cast<T>(std::get<double>(value_));
        } else if constexpr (std::is_floating_point_v<T>) {
            return static_cast<T>(std::get<double>(value_));
        } else if constexpr (std::is_same_v<T, array_t>) {
            return std::get<array_t>(value_);
        } else if constexpr (std::is_same_v<T, object_t>) {
            return std::get<object_t>(value_);
        } else {
            static_assert(sizeof(T) == 0, "Unsupported type for json::get");
        }
    }

    template<typename T>
    T value(const std::string& key, const T& default_value) const {
        if (!contains(key)) return default_value;
        return (*this)[key].get<T>();
    }

    array_t::const_iterator begin() const {
        return get_array().begin();
    }

    array_t::const_iterator end() const {
        return get_array().end();
    }

private:
    static void skip_ws(const std::string& text, size_t& pos) {
        while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos]))) pos++;
    }

    static json parse_value(const std::string& text, size_t& pos) {
        skip_ws(text, pos);
        if (pos >= text.size()) throw std::runtime_error("Unexpected end of json");
        char c = text[pos];
        if (c == '{') return parse_object(text, pos);
        if (c == '[') return parse_array(text, pos);
        if (c == '"') return parse_string(text, pos);
        if (c == 't' || c == 'f') return parse_bool(text, pos);
        if (c == 'n') return parse_null(text, pos);
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) return parse_number(text, pos);
        throw std::runtime_error("Invalid json value");
    }

    static json parse_object(const std::string& text, size_t& pos) {
        object_t obj;
        pos++; // skip {
        skip_ws(text, pos);
        if (text[pos] == '}') { pos++; return obj; }
        while (pos < text.size()) {
            auto key = parse_string(text, pos).get<std::string>();
            skip_ws(text, pos);
            if (text[pos] != ':') throw std::runtime_error("Expected ':' in object");
            pos++; // skip :
            obj[key] = parse_value(text, pos);
            skip_ws(text, pos);
            if (text[pos] == '}') { pos++; break; }
            if (text[pos] != ',') throw std::runtime_error("Expected ',' in object");
            pos++;
            skip_ws(text, pos);
        }
        return obj;
    }

    static json parse_array(const std::string& text, size_t& pos) {
        array_t arr;
        pos++; // skip [
        skip_ws(text, pos);
        if (text[pos] == ']') { pos++; return arr; }
        while (pos < text.size()) {
            arr.push_back(parse_value(text, pos));
            skip_ws(text, pos);
            if (text[pos] == ']') { pos++; break; }
            if (text[pos] != ',') throw std::runtime_error("Expected ',' in array");
            pos++;
            skip_ws(text, pos);
        }
        return arr;
    }

    static json parse_string(const std::string& text, size_t& pos) {
        if (text[pos] != '"') throw std::runtime_error("Expected string");
        pos++; // skip opening quote
        std::string result;
        while (pos < text.size()) {
            char c = text[pos++];
            if (c == '"') break;
            if (c == '\\') {
                if (pos >= text.size()) throw std::runtime_error("Invalid escape");
                char esc = text[pos++];
                switch (esc) {
                    case '"': result.push_back('"'); break;
                    case '\\': result.push_back('\\'); break;
                    case '/': result.push_back('/'); break;
                    case 'b': result.push_back('\b'); break;
                    case 'f': result.push_back('\f'); break;
                    case 'n': result.push_back('\n'); break;
                    case 'r': result.push_back('\r'); break;
                    case 't': result.push_back('\t'); break;
                    default: throw std::runtime_error("Unsupported escape");
                }
            } else {
                result.push_back(c);
            }
        }
        return result;
    }

    static json parse_bool(const std::string& text, size_t& pos) {
        if (text.compare(pos, 4, "true") == 0) { pos += 4; return json(true); }
        if (text.compare(pos, 5, "false") == 0) { pos += 5; return json(false); }
        throw std::runtime_error("Invalid boolean");
    }

    static json parse_null(const std::string& text, size_t& pos) {
        if (text.compare(pos, 4, "null") != 0) throw std::runtime_error("Invalid null");
        pos += 4;
        return json(nullptr);
    }

    static json parse_number(const std::string& text, size_t& pos) {
        size_t start = pos;
        if (text[pos] == '-') pos++;
        while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) pos++;
        if (pos < text.size() && text[pos] == '.') {
            pos++;
            while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) pos++;
        }
        double val = std::stod(text.substr(start, pos - start));
        return json(val);
    }

    value_t value_;
};

} // namespace nlohmann

