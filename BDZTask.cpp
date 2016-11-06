#include <iostream>
#include <string.h>
using namespace std;

class StudentTicket
{
private:
    char* startDest;
    char* endDest;
public:
     StudentTicket(char* s, char* e)
    {
        startDest= new char[strlen(s)+1];
        endDest = new char[strlen(e)+1];
        strcpy(startDest,s);
        strcpy(endDest,e);
    }
    double price(double p)
    {
        return p/2.0;
    }
    char* getStartDest() const
    {
        return startDest;
    }
    char* getEndDest() const
    {
        return endDest;
    }
};

class GroupTicket
{
private:
    char* startDest;
    char* endDest;
    int count;
public:

    GroupTicket(char* s, char* e, int c)
    {
        startDest= new char[strlen(s)+1];
        endDest = new char[strlen(e)+1];
        strcpy(startDest,s);
        strcpy(endDest,e);
        count =c;
    }

    double price(double p)
    {
        return count*p*3/10.0;
    }
    int getCount() const
    {
        return count;
    }
    char* getStartDest() const
    {
        return startDest;
    }
    char* getEndDest() const
    {
        return endDest;
    }

};

class NormalTicket
{
private:
    char* startDest;
    char* endDest;
public:
    NormalTicket(char* s, char* e)
    {
        startDest= new char[strlen(s)+1];
        endDest = new char[strlen(e)+1];
        strcpy(startDest,s);
        strcpy(endDest,e);
    }
    double price(double p)
    {
        return p;
    }
    char* getStartDest() const
    {
        return startDest;
    }
    char* getEndDest() const
    {
        return endDest;
    }
};

class Train
{
private:
    double price;
    bool freeSeats[150];
    double total;
    char* start;
    char* end;
public:

    Train(double p, char* s, char* e)
    {
        price =p;
        start = new char[strlen(s)+1];
        strcpy(start,s);
        end = new char [strlen(e)+1];
        strcpy(end,e);

        for(int i=0;i<150;i++)
        freeSeats[i]=true;

        total=0;
    }

    void init(const Train& other)
    {
        price=other.price;
        total=other.total;
        for(int i=0;i<150;i++)
            freeSeats[i]=other.freeSeats[i];

        start = new char[strlen(other.start)+1];
        strcpy(start,other.start);
        end = new char[strlen(other.end)+1];
        strcpy(end,other.end);
    }

    void destroy()
    {
        delete start;
        delete end;
    }

    Train(const Train& other)
    {
        init(other);
    }

    Train& operator=(const Train& other)
    {
        if(this!= &other)
        {
            destroy();
            init(other);
        }
        return *this;
    }

    ~Train()
    {
        destroy();
    }

    void SellTicket(StudentTicket st)
    {
        if(!strcmp(start,st.getStartDest()) && !strcmp(end,st.getEndDest()))
        {
        for(int i=0;i<150;i++)
            if(freeSeats[i])
        {
            freeSeats[i]=false;
            break;
            total += st.price(price);
        }
        }
    }

    void SellTicket(GroupTicket gt)
    {
        if(!strcmp(start,gt.getStartDest()) && !strcmp(end,gt.getEndDest()))
        {
        int available=0;
        for(int i=0;i<150;i++)
        {
            if(freeSeats[i])
            available++;
            if(available==gt.getCount() && gt.getCount() >=4)
            {
                freeSeats[i]=false;
                break;
                total += gt.price(price);
            }
        }
        }
    }

    void SellTicket(NormalTicket nt)
    {
         if(!strcmp(start,nt.getStartDest()) && !strcmp(end,nt.getEndDest()))
        {
        for(int i=0;i<150;i++)
            if(freeSeats[i])
        {
            freeSeats[i]=false;
            break;
            total += nt.price(price);
        }
        }
    }

    double getTotal() const
    {
        return total;
    }
};


int main()
{
    char s[6];
    strcpy(s,"start");
    char e[4];
    strcpy(e,"end");
    Train t(2,s,e);
    cout<<t.getTotal()<<endl;

    NormalTicket nt(s,e);
    GroupTicket gt(s,e,5);
    StudentTicket st(s,e);
    t.SellTicket(nt);
    cout<<t.getTotal()<<endl;
    t.SellTicket(gt);
    cout<<t.getTotal()<<endl;
    t.SellTicket(st);
    cout<<t.getTotal();

    return 0;
}
