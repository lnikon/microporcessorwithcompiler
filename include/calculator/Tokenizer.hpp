#pragma once

#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

#include "Token.hpp"

struct Tokenizer final {
	Tokenizer(std::string expression);

	auto getTokens() const {
		return m_tokens;
	}

	bool isStringNumber(const std::string& str);

	std::string::iterator searchNumber(std::string::iterator begin, std::string::iterator end);
private:
	std::vector<Token> m_tokens{};
};


