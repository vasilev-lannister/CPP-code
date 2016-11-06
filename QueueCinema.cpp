#include <iostream>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <sstream>
using namespace std;

class WaitingQueue
{
private:
    int ticksPerPerson;
    int idleTime;
    queue<string> waiting;
    int remainingTicks;
    bool busy;

public:
    WaitingQueue(int);
    void push(string);
    bool tick(string&);
    int getNumWaiting() const;
    int getIdleTime() const;

};

WaitingQueue::WaitingQueue(int newTicks): ticksPerPerson(newTicks), idleTime(0), remainingTicks(0), busy(false) {}

int WaitingQueue::getIdleTime() const
{
    return idleTime;
}

int WaitingQueue::getNumWaiting() const
{
    return waiting.size();
}

void WaitingQueue::push(string name)
{
    waiting.push(name);
}

bool WaitingQueue::tick(string& poppedClient)
{
    if(!busy)
    {
        if(waiting.empty())
        {
            idleTime++;
            return false;
        }
        else
        {
            busy = true;
            remainingTicks= ticksPerPerson;
            cout<<"Start processing "<<waiting.front()<<endl;
            return false;
        }
    }
    else
    {
        remainingTicks--;
        if(remainingTicks == 0)
        {
            poppedClient = waiting.front();
            waiting.pop();
            busy=false;
            return true;
        }
    }


}


int main()
{
    int numClients = 0;
    WaitingQueue tickets(30);
    WaitingQueue popcorn(90);
    WaitingQueue glasses(15);
    srand(time(NULL));
    bool hasMoved;
    string client;

    for(int t=0; t< 30*60; t++)
    {
        if(t %120 == 0)
        {
            int n = rand()%11;
            for(int i=0;i<n;i++)
            {
                string clientName;
                stringstream result;
                result<<"Client"<<numClients;
                clientName=result.str();
                tickets.push(clientName);
                numClients++;
            }
        }
        hasMoved = tickets.tick(client);
        if(hasMoved)
        {
            cout<<client<<" left ticket queue"<<endl;
            if(rand()%10 < 7) // 0 do 9
            {
                cout<<client<<" entered popcorn queue"<<endl;
                popcorn.push(client);
            }
            else
            {
                cout<<client<<" entered glasses queue"<<endl;
                glasses.push(client);
            }
        }
        hasMoved = popcorn.tick(client);
        if(hasMoved)
        {
            cout<<client<<" left popcorn queue and entered glasses queue"<<endl;
            glasses.push(client);
        }
        hasMoved = glasses.tick(client);
        if(hasMoved)
        {
             cout<<client<<" left glasses queue"<<endl;
        }
    }

    cout<<"Total waiting people:"<<(tickets.getNumWaiting() + popcorn.getNumWaiting() + glasses.getNumWaiting())<<endl;
    cout<<"Tickets queue was empty for "<<tickets.getIdleTime()<<" seconds"<<endl;
    cout<<"Popcorn queue was empty for "<<popcorn.getIdleTime()<<" seconds"<<endl;
    cout<<"Glasses queue was empty for "<<glasses.getIdleTime()<<" seconds"<<endl;



    return 0;
}
