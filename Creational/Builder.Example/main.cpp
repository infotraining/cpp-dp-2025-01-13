#include "report_builder.hpp"
#include "data_parser.hpp"
#include <fstream>
#include <iostream>

using namespace std;

constexpr auto file_name = "data_builder.txt";

HtmlDocument build_html_document()
{
    HtmlReportBuilder html_builder;

    DataParser parser(html_builder);
    parser.Parse(file_name);

    return html_builder.get_report();
}

CsvDocument build_csv_document()
{
    CsvReportBuilder csv_builder;

    DataParser parser(csv_builder);
    parser.Parse(file_name);

    return csv_builder.get_report();
}

int main()
{
    HtmlDocument doc_html = build_html_document();

    cout << doc_html << endl;

    ///////////////////////////////////////////////////////////
    cout << "///////////////////////////////////////////////////////////\n";

    CsvDocument csv_doc = build_csv_document();

    for (const auto& line : csv_doc)
        cout << line << endl;

    ///////////////////////////////////////////////////////////
    cout << "///////////////////////////////////////////////////////////\n";

    HtmlReportBuilder html_builder;

    html_builder
        .add_header("TITLE")
        .begin_data()
            .add_row({"1", "2", "3"})
            .add_row({"4", "5", "6"})
        .end_data()
        .add_footer("##################");
    
    std::cout << html_builder.get_report() << std::endl;
}
