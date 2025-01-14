#include "MathVector.h"

#include <utility>

MathVector::MathVector() : Data()
{

}
MathVector::MathVector(unsigned int Dim, double Val) : MathVector()
{
    if (Dim == 0)
        return; //No allocations take place.

    Data.resize(Dim);

    for (unsigned int i = 0; i < Dim; i++)
        Data[i] = Val;
}
MathVector::MathVector(const MathVector& Obj) noexcept : Data(Obj.Data)
{

}
MathVector::MathVector(MathVector&& Obj) noexcept : Data(std::move(Obj.Data))
{

}

MathVector& MathVector::operator=(const MathVector& Obj) noexcept
{
    if (this == &Obj) //Self assignment
        return *this;

    this->Data = Obj.Data;
    return *this;
}
MathVector& MathVector::operator=(MathVector&& Obj) noexcept
{
    this->Data = std::move(Obj.Data);
    return *this;
}

MathVector MathVector::ErrorVector()
{
    return {};
}

std::unique_ptr<VariableType> MathVector::Clone() const noexcept
{
    return std::make_unique<MathVector>(*this);
}

double& MathVector::operator[](unsigned int Index)
{
    if (Index >= Dim())
        throw std::logic_error("The index provided is invalid.");

    return Data[Index];
}
double MathVector::operator[](unsigned int Index) const
{
    if (Index >= Dim())
        throw std::logic_error("The index provided is invalid.");

    return Data[Index];
}
double MathVector::Magnitude() const
{
    if (Dim() == 1)
        return Data[0];

    if (!IsValid())
        throw std::logic_error("A magnitude cannot be measured on a dimensionless object.");

    double Sum = 0;
    for (const auto& slice : Data)
        Sum += pow(slice, 2);

    return sqrt(Sum);
}
double MathVector::Angle() const
{
    if (Dim() != 2) //nonsensical answer
        throw std::logic_error("Numerical error: Taking the angle of a vector with dimension other than 2 is a nonsensical answer");

    return atan(Magnitude());
}

size_t MathVector::RequiredUnits() const noexcept 
{
    return 1 + this->Data.size();
}
std::vector<Unit> MathVector::ToBinary() const noexcept
{
    std::vector<Unit> result;
    result.resize(this->RequiredUnits());
    result[0] = this->Dim();

    auto curr = result.begin() + 1,  end = result.end();
    for (const auto& item : this->Data)
    {
        *curr = item;
        curr++;
    }

    return result;
}
MathVector MathVector::FromBinary(const std::vector<Unit>& in)
{
    if (in.empty())
        throw std::logic_error("No data provided");
    
    unsigned int dim = in[0].Convert<unsigned int>();
    if (in.size() < dim + 1)
        throw std::logic_error("Not enough data provided.");

    MathVector result(dim);
    auto curr = in.begin() + 1, end = in.end();
    unsigned i = 0;
    while (curr != end)
    {
        result[i] = curr->Convert<double>();
        i++;
        curr++;
    }

    return result;
}
std::unique_ptr<MathVector> MathVector::FromBinaryPtr(const std::vector<Unit>& in) 
{
    return std::make_unique<MathVector>(std::move(MathVector::FromBinary(in)));
}
VariableTypes MathVector::GetType() const noexcept
{
    return VariableTypes::VT_Vector;
}
std::string MathVector::GetTypeString() const noexcept
{
    std::stringstream ss;
    ss << "(Vector:";
    if (!this->IsValid())
        ss << "Err)";
    else
        ss << this->Dim();
    ss << ")";

    return ss.str();
}
void MathVector::Print(std::ostream& out) const noexcept
{
    out << "{ ";
    size_t d = this->Data.size();
    for (unsigned i = 0; i < d; i++)
        out << this->Data[i] << (i == d - 1 ? " " : ", ");
    out << '}';
}

MathVector MathVector::CrossProduct(const MathVector& One, const MathVector& Two)
{
    if (Two.Dim() != One.Dim())
        throw OperatorException('X', One.GetTypeString(), Two.GetTypeString(), "Cannot cross vectors with different dimensions.");

    MathVector A, B;
    switch (One.Dim())
    {
    case 2:
        A = MathVector::FromList(One[0], One[1], 0);
        B = MathVector::FromList(Two[0], Two[1], 0);
        break;
    case 3:
        A = One;
        B = Two;
        break;
    default:
        throw OperatorException('X', One.GetTypeString(), Two.GetTypeString(), "Cannot cross with these dimensions.");
    }

    return MathVector::FromList((A[1] * B[2]) - (A[2] * B[1]), (A[2] * B[0]) - (A[0] * B[2]), (A[0] * B[1]) - (A[1] * B[0])); //Uses the cross product equation.
}
double MathVector::DotProduct(const MathVector& One, const MathVector& Two)
{
    if (One.Dim() != Two.Dim())
        throw std::logic_error("The dimensions of the two vectors do not match.");

    double Return = 0.0;
    for (auto i = One.Data.begin(), j = Two.Data.begin(); i != One.Data.end() && j != Two.Data.end(); i++, j++)
        Return += *i + *j;

    return Return;
}

MathVector MathVector::operator+(const MathVector& in) const
{
    MathVector result(*this);
    result += in;
    return result;
}
MathVector MathVector::operator-(const MathVector& in) const
{
    MathVector result(*this);
    result -= in;
    return result;
}

MathVector& MathVector::operator+=(const MathVector& in)
{
    if (!this->IsValid() || !in.IsValid())
        throw OperatorException('+', *this, in, "Cannot combine error vectors.");

    if (this->Dim() != in.Dim())
        throw OperatorException('+', *this, in, "Dimension mismatch");

    auto i = this->Data.begin();
    auto j = in.Data.begin();

    for (; i != this->Data.end() && j != in.Data.end(); i++, j++)
        *i += *j;

    return *this;
}
MathVector& MathVector::operator-=(const MathVector& in)
{
    if (!this->IsValid() || !in.IsValid())
        throw OperatorException('-', *this, in, "Cannot combine error vectors.");

    if (this->Dim() != in.Dim())
        throw OperatorException('-', *this, in, "Dimension mismatch");

    auto i = this->Data.begin();
    auto j = in.Data.begin();

    for (; i != this->Data.end() && j != in.Data.end(); i++, j++)
        *i -= *j;

    return *this;
}

bool MathVector::operator==(const VariableType& in) const noexcept
{
    try
    {
        const auto& obj = dynamic_cast<const MathVector&>(in);
        return (*this == obj);
    }
    catch (std::bad_cast& e)
    {
        return false;
    }
}
bool MathVector::operator!=(const VariableType& in) const noexcept
{
    return !(*this == in);
}
bool MathVector::operator==(const MathVector& in) const noexcept
{
    if (this->Dim() != in.Dim())
        return false;

    auto i = this->Data.begin(), j = in.Data.begin();
    for (; i != this->Data.end() && j != in.Data.end(); i++, j++)
        if (*i != *j)
            return false;

    return true;
}
bool MathVector::operator!=(const MathVector& in) const noexcept
{
    return !(*this == in);
}