//
// Created by Hollan on 10/4/24.
//

#ifndef JASON_COMMANDPARSER_H
#define JASON_COMMANDPARSER_H

//Represents a collection of commands, arguments, values, and flags.

#include <iostream>
#include <string>
#include <vector>
#include <optional>

class CommandParser;
class CommandSpecifier;
class CommandValue;
class CommandSingleValue;
class CommandMultiValue;


class CommandParser
{
private:
    std::string top_command;
    std::vector<char> flags;
    std::vector<CommandSpecifier> specifiers;
    std::vector<CommandValue*> values;

    using val_iter = std::vector<CommandValue*>::const_iterator;

public:
    CommandParser();
    [[maybe_unused]] CommandParser(std::string name, std::vector<char> flags, std::vector<CommandSpecifier> specifiers, std::vector<CommandValue*> values);
    ~CommandParser();

    [[nodiscard]] const std::string& TopCommand() const noexcept;
    [[nodiscard]] const std::vector<char>& Flags() const noexcept;
    [[nodiscard]] const std::vector<CommandSpecifier>& Specifiers() const noexcept;
    [[nodiscard]] const std::vector<CommandValue*>& Values() const noexcept;

    [[nodiscard]] static CommandParser Parse(std::istream& in);

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const CommandParser& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandParser& obj);

class CommandSpecifier
{
private:
    CommandValue* val;
public:
    CommandSpecifier();
    explicit CommandSpecifier(std::string name);
    CommandSpecifier(std::string name, CommandSingleValue&& obj);
    CommandSpecifier(std::string name, CommandMultiValue&& obj);
    CommandSpecifier(const CommandSpecifier& obj) noexcept;
    CommandSpecifier(CommandSpecifier&& obj) noexcept;

    [[nodiscard]] static CommandSpecifier Parse(std::istream& in);
    [[nodiscard]] static CommandSpecifier Parse(const std::string& in);

    CommandSpecifier& operator=(const CommandSpecifier& obj) noexcept;
    CommandSpecifier& operator=(CommandSpecifier&& obj) noexcept;

    std::string Name;
    [[nodiscard]] const CommandValue& Value() const;
    [[nodiscard]] CommandValue& Value();
    [[nodiscard]] bool HasValue() const noexcept;

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const CommandSpecifier& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandSpecifier& obj);

class CommandValue
{
public:
    CommandValue() = default;
    virtual ~CommandValue() = default;

    [[nodiscard]] virtual bool IsSingleValue() const noexcept = 0;

    virtual void Print(std::ostream& out) const noexcept = 0;
    [[nodiscard]] static CommandValue* Parse(std::istream& in);
};

class CommandSingleValue : public CommandValue
{
public:
    CommandSingleValue() = default;
    explicit CommandSingleValue(std::string val) : Value(std::move(val)) {}
    CommandSingleValue(const CommandSingleValue& obj) noexcept;
    CommandSingleValue(CommandSingleValue&& obj) noexcept;

    [[nodiscard]] static CommandSingleValue Parse(std::istream& in);

    CommandSingleValue& operator=(const CommandSingleValue& obj) noexcept;
    CommandSingleValue& operator=(CommandSingleValue&& obj) noexcept;

    std::string Value;

    [[nodiscard]] bool IsSingleValue() const noexcept override { return true; }
    void Print(std::ostream& out) const noexcept override;
};
class CommandMultiValue : public CommandValue
{
public:
    CommandMultiValue() = default;
    [[maybe_unused]] explicit CommandMultiValue(std::vector<std::string> list) : Value(std::move(list)) {}
    CommandMultiValue(const CommandMultiValue& obj) noexcept;
    CommandMultiValue(CommandMultiValue&& obj) noexcept;

    CommandMultiValue& operator=(const CommandMultiValue& obj) noexcept;
    CommandMultiValue& operator=(CommandMultiValue&& obj) noexcept;

    static const char DenoteStart, DenoteEnd;
    std::vector<std::string> Value;

    [[nodiscard]] static CommandMultiValue Parse(std::istream& in);

    [[nodiscard]] bool IsSingleValue() const noexcept override { return false; }
    void Print(std::ostream& out) const noexcept override;
};

std::ostream& operator<<(std::ostream& out, const CommandValue& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandSingleValue& obj);
std::istream& operator>>(std::istream& in, CommandMultiValue& obj);


#endif //JASON_COMMANDPARSER_H
