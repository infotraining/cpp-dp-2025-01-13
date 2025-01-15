#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

struct StatResult
{
    std::string description;
    double value;

    StatResult(const std::string& desc, double val)
        : description(desc)
        , value(val)
    {
    }
};

using Data = std::vector<double>;
using Results = std::vector<StatResult>;

namespace Canonical
{
    class Statistics
    {
    public:
        virtual ~Statistics() = default;
        virtual void calculate(const Data &data, Results &results) const = 0;
    };

    class StatisticsGroup : public Statistics
    {
        std::vector<std::shared_ptr<Statistics>> stats_;
    public:
        void calculate(const Data &data, Results &results) const override
        {
            for(const auto& stat : stats_)
                stat->calculate(data, results);
        }
    };
}


using Statistics = std::function<void(Data const& data_, Results& results_)>;

// Composite Pattern applied to gather Statistics as single object
class StatisticsGroup
{
    std::vector<Statistics> stats_;
public:
    StatisticsGroup() = default;

    void add(Statistics stat)
    {
        stats_.push_back(stat);
    }

    void operator()(Data const& data_, Results& results_) const
    {
        for(auto&& stat : stats_)
        {
            stat(data_, results_);
        }
    }
};

class AnalyzerStrategies
{
public:
    static void Avg(Data const& data_, Results& results_)
    {
        double sum = std::accumulate(data_.begin(), data_.end(), 0.0);
        double avg = sum / data_.size();

        StatResult result("Avg", avg);
        results_.push_back(result);
    }

    static void Min(Data const& data_, Results& results_)
    {
        double min = *(std::min_element(data_.begin(), data_.end()));

        results_.push_back(StatResult("Min", min));
    }

    static void Max(Data const& data_, Results& results_)
    {
        double max = *(std::max_element(data_.begin(), data_.end()));

        results_.push_back(StatResult("Max", max));
    }

    static void Sum(Data const& data_, Results& results_)
    {
        double sum = std::accumulate(data_.begin(), data_.end(), 0.0);

        results_.push_back(StatResult("Sum", sum));
    }
};

class Avg
{
public:
    void operator()(Data const& data_, Results& results_) const
    {
        double sum = std::accumulate(data_.begin(), data_.end(), 0.0);
        double avg = sum / data_.size();

        StatResult result("Avg", avg);
        results_.push_back(result);
    }
};

class MinMax
{
public:
    void operator()(Data const& data_, Results& results_) const
    {
        double min = *(std::min_element(data_.begin(), data_.end()));
        double max = *(std::max_element(data_.begin(), data_.end()));

        results_.push_back(StatResult("Min", min));
        results_.push_back(StatResult("Max", max));
    }
};

class Sum
{
public:
    void operator()(Data const& data_, Results& results_) const
    {
        double sum = std::accumulate(data_.begin(), data_.end(), 0.0);

        results_.push_back(StatResult("Sum", sum));
    }
};

class DataAnalyzer
{
    Statistics strategy_;
    Data data_;
    Results results_;

public:
    DataAnalyzer(Statistics strategy)
        : strategy_{strategy}
    {
    }

    void load_data(const std::string& file_name)
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

    void set_strategy(Statistics strategy)
    {
        strategy_ = strategy;
    }

    void calculate()
    {
        strategy_(data_, results_);
    }

    const Results& results() const
    {
        return results_;
    }
};

void show_results(const Results& results)
{
    for (const auto& rslt : results)
        std::cout << rslt.description << " = " << rslt.value << std::endl;
}

const auto avg = &AnalyzerStrategies::Avg;

const auto min_max = [] { 
    StatisticsGroup min_max;
    min_max.add(&AnalyzerStrategies::Min);
    min_max.add(&AnalyzerStrategies::Max);
    return min_max;
}();

const auto sum = Sum{};

int main()
{
    StatisticsGroup std_stats;
    std_stats.add(avg);
    std_stats.add(min_max);
    std_stats.add(sum);

    DataAnalyzer da(std_stats);
    da.load_data("stats_data.dat");
    da.calculate();

    show_results(da.results());

    std::cout << "\n\n";

    da.load_data("new_stats_data.dat");
    da.calculate();

    show_results(da.results());
}
