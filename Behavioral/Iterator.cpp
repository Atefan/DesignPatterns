#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class IIterator
{
public:
    virtual ~IIterator() = default;
    virtual bool hasLeft() const = 0;
    virtual int  getNext() = 0;
};

class ICollection
{
public:
    virtual ~ICollection() = default;
    virtual std::unique_ptr<IIterator> getIterator() const = 0;
    virtual int getSize() const = 0;
    virtual int at(int index) const = 0;
};


class SequentialIterator : public IIterator
{
    const ICollection& col;
    int index = 0;
public:
    explicit SequentialIterator(const ICollection& c) : col(c) {}
    bool hasLeft() const override { return index < col.getSize(); }
    int  getNext() override { return col.at(index++); }
};

class PrimeNumberIterator : public IIterator
{
    const ICollection& col;
    int currentIndex = 0;
    int nextPrimeIndex = -1;

    static bool isPrime(int n)
    {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; ++i)
            if (n % i == 0) return false;
        return true;
    }

    void findNext()
    {
        nextPrimeIndex = -1;
        while (currentIndex < col.getSize())
        {
            int i = currentIndex++;
            if (isPrime(col.at(i)))
            {
                nextPrimeIndex = i;
                return;
            }
        }
    }

public:
    explicit PrimeNumberIterator(const ICollection& c) : col(c) { findNext(); }
    bool hasLeft() const override { return nextPrimeIndex != -1; }
    int getNext() override
    {
        int value = col.at(nextPrimeIndex);
        findNext();
        return value;
    }
};

class VectorCollection : public ICollection
{
    std::vector<int> col;
public:
    void add(int v) { col.push_back(v); }
    int getSize() const override { return static_cast<int>(col.size()); }
    int at(int index) const override { return col.at(index); }
    std::unique_ptr<IIterator> getIterator() const override
    {
        return std::make_unique<SequentialIterator>(*this);
    }
};


int main()
{
    VectorCollection numbers;
    for (int i = 1; i <= 15; ++i) numbers.add(i);

    auto all = numbers.getIterator();
    while (all->hasLeft()) std::cout << all->getNext() << " ";
    std::cout << "\n";

    PrimeNumberIterator primes(numbers);
    while (primes.hasLeft()) std::cout << primes.getNext() << " ";
    std::cout << "\n";

    // two independent traversals at the same time
    PrimeNumberIterator p1(numbers), p2(numbers);
    std::cout << p1.getNext() << " " << p1.getNext() << " " << p2.getNext() << "\n";
}