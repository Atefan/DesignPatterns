#include <iostream>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <memory>

// Determinant calculator
// There are different ways to calculate a matrix,
// Depending on the size

using Matrix = std::vector<std::vector<double>>;

class IStrategy
{
public:
    virtual double calculate(const Matrix& matrix) = 0;
    virtual ~IStrategy() = default;
};

void requireSize(const Matrix& m, std::size_t n)
{
    if (m.size() != n)
        throw std::invalid_argument("matrix must have " + std::to_string(n) + " rows");
    for (const auto& row : m)
        if (row.size() != n)
            throw std::invalid_argument("matrix must have " + std::to_string(n) + " columns in every row");
}

class StrategyMatrix2X2 : public IStrategy
{  
public:
    double calculate(const Matrix& matrix) override
    {   
        requireSize(matrix, 2);
        return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
    }
};


class StrategyMatrix3X3 : public IStrategy
{
public:
    
    double calculate(const Matrix& matrix) override
    {
        requireSize(matrix, 3);
        return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
             - matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
             + matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    }
};

class StrategyMatrix3X3Sarrus : public IStrategy
{
public:
    double calculate(const Matrix& matrix) override
    {
        requireSize(matrix, 3);
        double plus = matrix[0][0] * matrix[1][1] * matrix[2][2]
                    + matrix[0][1] * matrix[1][2] * matrix[2][0]
                    + matrix[0][2] * matrix[1][0] * matrix[2][1];

        double minus = matrix[0][2] * matrix[1][1] * matrix[2][0]
                     + matrix[0][1] * matrix[1][0] * matrix[2][2]
                     + matrix[0][0] * matrix[1][2] * matrix[2][1];

        return plus - minus;
    }
};

class Calculator
{
    std::unique_ptr<IStrategy> strat;
public:
    Calculator() {}
    void setStrategy(std::unique_ptr<IStrategy> s) {strat = std::move(s);}
    double calculate(const Matrix& m)
    {
        if(strat == nullptr)
            throw std::logic_error("no strategy set");
        return strat->calculate(m);
    }
};


int main()
{
    Calculator c;

    Matrix m2 = {{1, 2}, {7, 2}};
    Matrix m3 = {{2, 1, 3}, {0, 4, 1}, {5, 2, 0}};

    try
    {
        c.setStrategy(std::make_unique<StrategyMatrix2X2>());
        std::cout << c.calculate(m2) << "\n";

        c.setStrategy(std::make_unique<StrategyMatrix3X3>());
        std::cout << c.calculate(m3) << "\n";

        c.setStrategy(std::make_unique<StrategyMatrix3X3Sarrus>());
        std::cout << c.calculate(m3) << "\n";

        c.calculate(m2);
    }
    catch (const std::exception& e)
    {
        std::cout << "error: " << e.what() << "\n";
    }

    return 0;
}
