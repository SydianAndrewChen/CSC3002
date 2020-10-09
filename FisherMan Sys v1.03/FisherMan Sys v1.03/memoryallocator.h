#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H

#include <iostream>

using namespace std;
enum status { FREE, BUSY };

class MemoryAllocator
{
public:
    MemoryAllocator();

    struct freespace
    {
        int num;         //Area id
        int size;        //Area size
        int address;     //Area first address
        status state;    //Area situation£¬FREE or BUSY
    };

    typedef struct node
    {
        freespace data;
        node *head;
        node *next;
    }*Linklist;

    Linklist first,last;

    unsigned int MAXSIZE = 640;   //Defind the total size of the virtual memory , the unit is KB

    void initial(); //initialization, run before other funtions like main funtion
    int bestAlloc(int num,int size);//get the best allocation strategy,
    int recycle(int num);//recycle an area
    int getspace(int num); //get the size of an area


    //Testing Method
    int firstAlloc(int num,int size);  //get the first allocation strategy
    void menu(); //control the menu
    void display();//showing the situation of memory allocation
};

#endif // MEMORYALLOCATOR_H
