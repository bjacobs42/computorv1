#include "parser.hpp"
#include "utils.hpp"

parser::parser(const std::string& input)
	: _tokens(lexer(input))
	, _pos(0)
{
}
parser::~parser() { }

void parser::parse(void)
{
}

token parser::_peek(void) const
{
	return (_tokens[_pos]);
}

token parser::_consume(void)
{
	token tok = _tokens[_pos];
	++_pos;
	return (tok);
}
