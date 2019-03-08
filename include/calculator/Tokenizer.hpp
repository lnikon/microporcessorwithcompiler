#pragma once

#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

#include "Token.hpp"

struct Tokenizer final {
	Tokenizer(std::string expression) {
		if(expression.empty()) {
			throw "empty expression given to tokenizer\n";
		}

		// Remove all whitespaces from string
		expression.erase(
				std::remove(
					std::begin(expression),
					std::end(expression),
					char(' ')),
				std::end(expression));

		auto token = std::string{};
		auto type = TokenType{TokenType::UNKNOWN};

		std::string::size_type prevPos = 0;
		std::string::size_type pos = 0;

		auto isTokenValidNumber = bool{false};
		const auto delimeters = std::string{"+-*/()"};

		while((pos = expression.find_first_of(delimeters, pos)) != std::string::npos) {
			token = expression.substr(prevPos, pos - prevPos);

			isTokenValidNumber = isStringNumber(token);
			if(isTokenValidNumber) {
				type = TokenType::NUMBER;
				m_tokens.emplace_back(Token{token, type});
			}

			char tokenChar = expression[pos];
			switch(tokenChar) {
			case '+':
			case '-':
			case '*':
			case '/':
				type = TokenType::OPERATOR;
				break;
			case '(':
				type = TokenType::LEFT_PARENTHESIS;
				break;
			case ')':
				type = TokenType::RIGHT_PARENTHESIS;
				break;
			default:
				type = TokenType::UNKNOWN;
				break;
			}

			m_tokens.emplace_back(Token{tokenChar, type});

			prevPos = ++pos;
		}

		token = expression.substr(prevPos, pos - prevPos);
		isTokenValidNumber = isStringNumber(token);
		if(isTokenValidNumber) {
			type = TokenType::NUMBER;
			m_tokens.emplace_back(Token{token, type});
		}
	}

	auto getTokens() const {
		return m_tokens;
	}

	auto isStringNumber(const std::string& str) {
		return !str.empty() &&
				std::find_if(
				std::begin(str),
				std::end(str),
				[](char c) { return !std::isdigit(c); }) == std::end(str);
	}

	auto searchNumber(std::string::iterator begin, std::string::iterator end)
	{
		return std::find_if_not(begin, end,
				[](char c) { return std::isdigit(c) || c == '.'; });
	}
private:
	std::vector<Token> m_tokens{};
};
