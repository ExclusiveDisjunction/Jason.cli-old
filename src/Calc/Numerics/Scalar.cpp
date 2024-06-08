//
// Created by exdisj on 5/26/24.
//

#include "Scalar.h"

#include <iomanip>

[[nodiscard]] VariableTypes Scalar::GetType() const noexcept
{
    return VariableTypes::VT_Scalar;
}
void Scalar::Sterilize(std::ostream& out) const noexcept
{
    out << "SCA " << this->GetScaType() << ' ' << this->operator double();
}
Scalar* Scalar::FromSterilize(const std::string& in)
{
    try
    {
        std::stringstream ss(in);
        return FromSterilize(ss);
    }
    catch (std::logic_error& e)
    {
        throw e;
    }
}
Scalar* Scalar::FromSterilize(std::istream& in)
{
    std::string header, type;
    auto pos = in.tellg();
    in >> header >> type;
    if (header != "SCA")
        throw std::logic_error("Cannot construct a scalar from this object.");

    in.seekg(pos);
    if (type == "R") //RealNum
        return new RealNumber(in);
    else if (type == "Z") //Integer
        return new Integer(in);
    else if (type == "Q") //Fraction
        return new Fraction(in);
    else
        throw std::logic_error("The scalar's type was not recognized.");
}
[[nodiscard]] std::string Scalar::GetTypeString() const noexcept
{
    return "(Scalar:" + this->GetScaType() + ")";
}

RealNumber Scalar::operator+(double in) const noexcept
{
    RealNumber result(this->operator double());
    result += in;
    return result;
}
RealNumber Scalar::operator-(double in) const noexcept
{
    RealNumber result(this->operator double());
    result -= in;
    return result;
}
RealNumber Scalar::operator*(double in) const noexcept
{
    RealNumber result(this->operator double());
    result *= in;
    return result;
}
RealNumber Scalar::operator/(double in) const noexcept
{
    RealNumber result(this->operator double());
    result /= in;
    return result;
}
[[nodiscard]] RealNumber Scalar::Pow(double in) const noexcept
{
    return RealNumber(pow(this->operator double(), in));
}

bool Scalar::operator==(const VariableType& obj) const noexcept
{
    try
    {
        return dynamic_cast<const Scalar&>(obj).operator double() == this->operator double();
    }
    catch (std::bad_cast& e)
    {
        return false;
    }
}
bool Scalar::operator!=(const VariableType& obj) const noexcept
{
    return !(*this == obj);
}

std::ostream& Scalar::operator<<(std::ostream& out) const noexcept
{
    out << std::scientific << this->operator double();
    return out;
}