#pragma once

#include <stdexcept>
#include <string>
class ParserError : public std::runtime_error
{
public:
  ParserError(const std::string &msg, const std::string &input, int pos)
      : std::runtime_error(_build_message(msg, input, pos))
  {
  }

private:
  std::string
  _build_message(const std::string &msg, const std::string &input, int pos)
  {
    std::string error_msg = "Parser: " + msg + '\n';
    std::string location(std::max(pos - 1, 0), ' ');

    location += "v\n" + input;
    return (error_msg + location);
  }
};
