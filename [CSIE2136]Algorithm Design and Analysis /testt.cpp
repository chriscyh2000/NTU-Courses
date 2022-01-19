#include <iostream>
#include <vector>

int64_t RiskValue(const std::vector<int64_t> &b) {
    int64_t value = 0;
    for (size_t i = 0; i < b.size(); ++i) value += b[i];
    for (size_t i = 1; i < b.size(); ++i) value -= std::abs(b[i] - b[i - 1]);
    for (size_t i = 2; i < b.size(); ++i)
        value += b[i - 1] * ((b[i - 2] & b[i - 1]) ^ (b[i - 1] | b[i]) ^ (b[i - 2] + b[i]));
    return value;
}

int main() {
    // std::cout << "[Sample 1] RiskValue: "
    //           << RiskValue({5, 1, 2}) + RiskValue({3, 4}) << std::endl;

    // std::cout << "[Sample 2] RiskValue: "
    //           << RiskValue({5, 1}) + RiskValue({2, 3}) + RiskValue({4}) << std::endl;

    // std::cout << "[Sample 3] RiskValue: "
    //           << RiskValue({1}) + RiskValue({2}) + RiskValue({3}) + RiskValue({4}) + RiskValue({5})
    //           << std::endl;
    std::cout << "RiskValue({5, 1}) + RiskValue({2, 3}): " << RiskValue({5, 1}) + RiskValue({2, 3})<< '\n';
    std::cout << "RiskValue({5, 1, 2}) + RiskValue({3}): " << RiskValue({5, 1, 2}) + RiskValue({3})<< '\n';
    std::cout << "RiskValue({5}) + RiskValue({1, 2, 3}): " << RiskValue({5, 1}) + RiskValue({2, 3})<< '\n';
    std::cout << "RiskValue({5, 1, 2, 3}): " << RiskValue({5, 1, 2, 3})<< '\n';
    return 0;
}