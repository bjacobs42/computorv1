#pragma once

#include "token.hpp"
#include <string>
#include <vector>

class parser
{
public:
	parser(const std::string& input);
	~parser();

private:
	std::vector<token> _tokens;
	int _pos;

	token _peek(void) const;
	token _consume(void);
};
