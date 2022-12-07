#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct employee {
    std::string first_name = "John";
    std::string last_name = "Doe";
    std::string pay_date = "xxx";
    int payment = 0;
};

bool DateValidation(const std::string& date){
    int day, month;
    day = std::stoi(date.substr(0,2));
    month = std::stoi(date.substr(3,2));
    if (month < 1 || day < 1 || month > 12 || (month == 2 && day > 29) || day > 31)
    {
        std::cout << "Invalid date!" << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

void EnterRecord(employee& input)
{
    //create sstream to receive user input, which will be parsed later
    std::string buffer;
    std::stringstream bufferStream(buffer);

    while (true){
        std::cout << "Enter data (name, surname, date and cash amount)" << std::endl;

        std::getline(std::cin, buffer);     //get data
        bufferStream << buffer;                    //enter data in sstream

        bufferStream >> input.first_name >> input.last_name >> input.pay_date >> input.payment; //parse data
        bufferStream.clear();      //flush data before next iteration

        if (input.last_name.empty() || input.pay_date.empty() || input.payment <= 0 || !DateValidation(input.pay_date))
        {
            std::cout << "Invalid input!" << std::endl;
        } else {
            break;
        }
    }

    return;
}

void SaveEmployee(employee& input, std::string& path)
{
    std::ofstream file (path, std::ios::app);
    if (file.is_open())
    {
        int len = input.first_name.length();         //get size of first_name string
        std::cout << len << std::endl;
        file.write((char*) &len, sizeof(len));        //write size of uncoming first_name string
        file.write(input.first_name.c_str(), len);    //write first_name string, cast to c-style string

        len = input.last_name.length();                     //repeat for last_name and date and re-use variable
        std::cout << len << std::endl;
        file.write((char*) &len, sizeof(len));
        file.write(input.last_name.c_str(), len);

        len = sizeof(input.pay_date.c_str());
        std::cout << len << std::endl;
        file.write((char*) &len, sizeof(len));
        file.write(input.pay_date.c_str(), len);

        file.write((char*) &input.payment, sizeof(input.payment));

        file.close();
    }
    else
    {
        std::cerr << "Failed to open file to write!" << std::endl;
    }

    return;
}

void ListRecords(std::string& path)
{
    std::vector<employee> ledger;
    employee temp;
    int len = 0;
    std::ifstream file(path, std::ios::binary);

    while(!file.eof())
    {
        file.read((char*)&len, sizeof(len));
        temp.first_name.resize(len);
        file.read((char*)temp.first_name.c_str(), len);

        file.read((char*)&len, sizeof(len));
        temp.last_name.resize(len);
        file.read((char*)temp.last_name.c_str(),len);

        file.read((char*)&len, sizeof(len));
        temp.pay_date.resize(len);
        file.read((char*)temp.pay_date.c_str(),len);

        file.read((char*)&temp.payment, sizeof(temp.payment));

        ledger.push_back(temp);

        file.peek();        //try reading to set .eof flag, if all contents of the file were already read
    }

    file.close();

    for (int i = 0; i < ledger.size(); i++)
    {
        std::cout << ledger[i].first_name << " "
                    << ledger[i].last_name << " "
                    << ledger[i].pay_date << " "
                    << ledger[i].payment << std::endl;
    }

    return;
}

int main() {

    std::string file_path = "data.bin";

    std::string command;

    while (command != "exit")
    {
        std::cout << "Please enter command (list, add, exit):" << std::endl;
        std::cin >> command;

        if (command == "exit")
        {
            std::cout << "Bye!" << std::endl;
        }
        else if (command == "add")
        {
            std::cin.ignore();
            employee input;
            EnterRecord(input);
            SaveEmployee(input, file_path);
            std::cout << "Record added to the file!" << std::endl;
        }
        else if (command == "list")
        {
            std::cout << "Following records are stored in the file:" << std::endl;
            ListRecords( file_path);
        }
        else
        {
            std::cout << "Unknown command!" << std::endl;
        }
    }

    return 0;
}
