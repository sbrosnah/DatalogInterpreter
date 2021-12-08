#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include<string>

class Tuple {
private:
    std::vector<std::string> values;
public:
    Tuple(){};
    ~Tuple(){};
    bool operator< (const Tuple &rhs) const {
        return values < rhs.values;
    }
    void SetValues(std::vector<std::string> values);
    void AddValue(std::string value);
    std::string GetValue(int index);
    std::vector<std::string> GetValues();
    void EditValue(int index, std::string value);
    void EraseValue(int index);
};

#endif