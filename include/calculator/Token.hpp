#pragma once

#include <string>
#include <utility>

enum class TokenType {
	UNKNOWN = -1,
	OPERATOR = 0,
	NUMBER = 1,
	LEFT_PARENTHESIS = 2,
	RIGHT_PARENTHESIS = 3
};

struct Token final {
	Token(const std::string& tokenStr = "", TokenType type = TokenType::UNKNOWN)
	: m_token{tokenStr}, m_type{type} { }

	Token(char c, TokenType type)
	: Token(std::string(1, c), type) { }

	Token(std::pair<std::string, TokenType> token)
	: Token(token.first, token.second) { }

	Token(std::pair<char, TokenType> token)
		: Token(token.first, token.second) { }

	auto getType() const {
		return m_type;
	}

	auto getToken() const {
		return m_token;
	}
private:
	std::string m_token{};
	TokenType m_type{TokenType::UNKNOWN};
};
