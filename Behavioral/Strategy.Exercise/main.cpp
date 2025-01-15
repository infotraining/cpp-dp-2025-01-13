#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>

struct StatResult
{
    std::string description;
    double value;

    StatResult(const std::string &desc, double val)
        : description(desc), value(val)
    {
    }
};

using Data = std::vector<double>;
using Results = std::vector<StatResult>;

class StatisticsStrategy
{
public:
    virtual ~StatisticsStrategy() = default;
    virtual void calculate(const Data &data, Results &results) const = 0;
};

class AverageStrategy : public StatisticsStrategy
{
public:
    void calculate(const Data &data, Results &results) const override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double avg = sum / data.size();
        results.push_back(StatResult("Avg", avg));
    }
};

class MinMaxStrategy : public StatisticsStrategy
{
public:
    void calculate(const Data &data, Results &results) const override
    {
        double min = *(std::min_element(data.begin(), data.end()));
        double max = *(std::max_element(data.begin(), data.end()));
        results.push_back(StatResult("Min", min));
        results.push_back(StatResult("Max", max));
    }
};

class SumStrategy : public StatisticsStrategy
{
public:
    void calculate(const Data &data, Results &results) const override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        results.push_back(StatResult("Sum", sum));
    }
};

class DataAnalyzer
{
    std::shared_ptr<StatisticsStrategy> statistics_;

    Data data_;
    Results results_;

public:
    DataAnalyzer(std::shared_ptr<StatisticsStrategy> stat_type_)
        : statistics_{stat_type_}
    {
    }

    void load_data(const std::string &file_name)
    {
        data_.clear();
        results_.clear();

        std::ifstream fin(file_name.c_str());
        if (!fin)
            throw std::runtime_error("File not opened");

        double d;
        while (fin >> d)
        {
            data_.push_back(d);
        }

        std::cout << "File " << file_name << " has been loaded...\n";
    }

    void set_statistics(std::shared_ptr<StatisticsStrategy> stat_type)
    {
        statistics_ = stat_type;
    }

    void set_strategy(std::shared_ptr<StatisticsStrategy> strategy)
    {
        statistics_ = std::move(strategy);
    }

    void calculate()
    {
        if (statistics_)
        {
            statistics_->calculate(data_, results_);
        }
        else
        {
            throw std::runtime_error("Strategy not set");
        }
    }

    const Results &results() const
    {
        return results_;
    }
};

void show_results(const Results &results)
{
    for (const auto &rslt : results)
        std::cout << rslt.description << " = " << rslt.value << std::endl;
}

const auto avg = std::make_shared<AverageStrategy>();
const auto min_max = std::make_shared<MinMaxStrategy>();
const auto sum = std::make_shared<SumStrategy>();

int main()
{
    DataAnalyzer da{avg};
    std::cout << "Strategy: Average\n";
    da.load_data("stats_data.dat");

    da.calculate();

    da.set_statistics(min_max);
    da.calculate();

    da.set_statistics(sum);
    da.calculate();

    show_results(da.results());

    std::cout << "\n\n";

    da.load_data("new_stats_data.dat");
    da.calculate();

    show_results(da.results());
}
