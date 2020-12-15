#include "thread.h"
#include <vector>

using namespace std;

Thread::Thread(QObject* parent, bool isStopped) : QThread(parent), Stop(isStopped)
{
}

void Thread::run()
{
    while(true)
    {
        if(this->Stop) break;

        vector<int> numbers;
        for(int i = 0; i < 9; i++)
        {
            if(this->Stop) break;

            int number = rand() % 10 + i * 10;
            if(number < minimalFrame)
            {
                number = minimalFrame;
            }
            numbers.push_back(number);
        }
        for(int i = 0; i < numbers.size(); i++)
        {
            if(this->Stop) break;

            emit valueChanged(numbers[i]);
            this->sleep(1.5);
        }
    }
}
