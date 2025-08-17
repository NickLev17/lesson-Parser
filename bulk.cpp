#include <iostream>
#include <vector>
#include "logger.h"
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

class Parser
{
    vector<string> vectime;
    vector<string> data;

public:
    int size_pack{};
    explicit Parser(int size) : size_pack(size)
    {
        data.clear();
    }

    void showData()
    {
        if (!data.empty())
        {
            cout << "bulk: ";
            for (const auto &d : data)
                cout << d << " ";
            cout << "\n";
        }

        // if (!vectime.empty())
        // {
        //     for (const auto &t : vectime)
        //         cout << t << " ";
        //     cout << "\n";
        // }
    }

    void createLogFile()
    {

        if (!data.empty())
        {

            string filename = "bulk" + vectime.front() + ".log";

            Logger loger(filename);

            for (const auto &d : data)
                loger.log(d);
        }
        else
        {
            cout << "Data is empty !\n";
        }
    }

    void extractPackage()
    {
        data.pop_back(); // удаляем закрывающую скобку
        vector<string> temp;

        for (auto iter = data.rbegin(); iter != data.rend(); ++iter)
        {
            if (*iter == "{")
            {
                data.erase(iter.base() - 1); // так как реверсивный итератор erase  не принимает, его нужно привести к норм форме
                                             //  через base()-1
                break;
            }
            temp.push_back(*iter);
        }
    }

    void findPackage()
    {
        string str;
        std::getline(std::cin, str);
        vectime.push_back(getCurrentTime());

        if (str.empty())
        {
            vectime.pop_back();
            data.clear();
            return;
        }
        if (str == "{")
        {
            vectime.pop_back();
            data.push_back(str);
            findPackage();
        }
        else if (str == "}")
        {
            vectime.pop_back();
            data.push_back(str);

            ///  cout << "BLOCK ";
            extractPackage(); // можно так сделать, извлекли данные и записали в команду. потом эту команду в другую команду итд

            return;
        }
        else
        {
            data.push_back(str);
        }
        findPackage();
    }

    void start()
    {

        for (int i{}; i < size_pack; i++)
        {
            string str;

            std::getline(cin, str);
            vectime.push_back(getCurrentTime());
            if (str.empty())
            {
                vectime.pop_back();
                showData();
                createLogFile();
                return;
            }
            else if (str == "{")
            {
                vectime.pop_back();
                showData();
                data.clear();
                findPackage();
                break;
            }
            else
            {
                data.push_back(str);
                str.clear();
            }
        }

        showData();
        createLogFile();
        data.clear();
        vectime.clear();
        start();
    }

    string getCurrentTime()
    {

        time_t now = time(0);

        tm *timeinfo = localtime(&now);
        char timestamp[20];

        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

        return string(timestamp);
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error , not correct parametr\n";
        return 1;
    }
    int N = std::atoi(argv[1]);
    Parser pars(N);
    pars.start();

    return 0;
}