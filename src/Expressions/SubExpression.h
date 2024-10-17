//
// Created by Hollan on 7/27/24.
//

#ifndef JASON_SUBEXPRESSION_H
#define JASON_SUBEXPRESSION_H

#include <string>
#include <memory>

#include "../Calc/VariableType.h"
#include "../Calc/Numerics/Scalar.h"
#include "../IO/PackageEntryKey.h"
#include "ExpressionElement.h"

class ParsedSubExpression;

enum SubExpressionType
{
    Numeric,
    Variable,
    Declaration
};

class SubExpression : public ExpressionElement
{
public:
    [[nodiscard]] virtual SubExpressionType GetType() const noexcept = 0;
    [[nodiscard]] ExpressionElementT ElementType() const noexcept override
    {
        return ExpressionElementT::SubExpr;
    }

    [[nodiscard]] virtual const VariableType& GetValue() const = 0;

    [[nodiscard]] static ParsedSubExpression Parse(std::istream& in) noexcept;
};

class NumericExpr : public SubExpression
{
private:
    Scalar Value;

public:
    explicit NumericExpr(Scalar Value) : Value(std::move(Value)) { }

    static bool IsNumericalString(std::istream& in) noexcept;

    [[nodiscard]] SubExpressionType GetType() const noexcept override
    {
        return SubExpressionType::Numeric;
    }
    void Print(std::ostream& out) const noexcept override
    {
        out << Value;
    }

    [[nodiscard]] const VariableType& GetValue() const override
    {
        return Value;
    }
};
/// @breif Holds a key to a package entry, but will do nothing after this object goes out of scope.
class VariableExpr : public SubExpression
{
private:
    PackageEntryKey key;

public:
    explicit VariableExpr(PackageEntryKey key);

    [[nodiscard]] SubExpressionType GetType() const noexcept override
    {
        return SubExpressionType::Variable;
    }
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] const VariableType& GetValue() const override;
};
/// @breif Holds a key to a package entry, but will call to delete it after this object goes out of scope.
/// @breif Note that once this object is copied, it will call to copy its object.
class DeclarationExpr : public SubExpression
{
private:
    PackageEntryKey key;

public:
    explicit DeclarationExpr(PackageEntryKey key) : key(key) { }
    DeclarationExpr(const DeclarationExpr& obj);
    DeclarationExpr(DeclarationExpr&& obj) noexcept;
    ~DeclarationExpr() override;

    DeclarationExpr& operator=(const DeclarationExpr& obj);
    DeclarationExpr& operator=(DeclarationExpr&& obj);

    [[nodiscard]] SubExpressionType GetType() const noexcept override
    {
        return SubExpressionType::Declaration;
    }
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] const VariableType& GetValue() const override;
};
/// @breif Returned when the expression to be parsed is invalid.
class InvalidExpr
{
private:
    std::string message;

public:
    InvalidExpr(std::string m) : message(std::move(m)) {}

    [[nodiscard]] const std::string& GetMessage() const noexcept
    {
        return this->message;
    }
    [[nodiscard]] std::string FormatMessage() const noexcept
    {
        std::string Message("Invalid Expression: \" ");
        Message += this->message;
        Message += " \"";

        return Message;
    }
    void Print(std::ostream& out) const noexcept
    {
        out << this->FormatMessage();
    }

};

struct ParsedSubExpression
{
private:
    std::optional<std::unique_ptr<SubExpression*>> parsed;
    std::optional<InvalidExpr> invalid;

    ParsedSubExpression(std::unique_ptr<SubExpression*>&& parsed);
    ParsedSubExpression(InvalidExpr&& invalid);

public:
    /// @breif Returns a ParsedSubExpression containing the parsed expression, but if the pointer is nullptr, it will return an error.
    [[nodiscard]] static ParsedSubExpression FromParsed(std::unique_ptr<SubExpression*> parsed) noexcept;
    /// @breif Returns a ParsedSubExpression with the corresponding invalid expression.
    [[nodiscard]] static ParsedSubExpression FromError(InvalidExpr&& expr) noexcept;
};

#endif //JASON_SUBEXPRESSION_H
