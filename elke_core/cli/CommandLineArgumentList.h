#ifndef COMMAND_LINE_ARGUMENT_LIST_H
#define COMMAND_LINE_ARGUMENT_LIST_H

#include <vector>
#include <string>

namespace elk
{

struct CommandLineArgument;

/**Small utility object for handling command line arguments.*/
class CommandLineArgumentList
{
public:
  CommandLineArgumentList() = default;
  void add(const CommandLineArgument& arg);
  bool has(const std::string& name) const;
  bool hasShortName(const std::string& short_name) const;
  const CommandLineArgument& getCLAbyName(const std::string& name) const;
  CommandLineArgument& getCLAbyName(const std::string& name);
  const CommandLineArgument&
  getCLAbyShortName(const std::string& short_name) const;

  const std::vector<CommandLineArgument>& getCLAlist() const;

private:
  std::vector<CommandLineArgument> m_list;
};

} // namespace elk

#endif // COMMAND_LINE_ARGUMENT_LIST_H
