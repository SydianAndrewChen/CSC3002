#include "memoryallocator.h"

MemoryAllocator::MemoryAllocator()
{

}
void MemoryAllocator::initial () //initializing
{
    first = new node;
    last = new node;
    first->head = NULL;
    first->next = last;
    last->head = first;
    last->next = NULL;
    last->data.address = 0;
    last->data.size = MAXSIZE;
    last->data.num = 0;
    last->data.state = FREE;
}

void MemoryAllocator::display()
{
    cout << "****************main memory allocation****************" << endl;
    node *p = first->next;
    while (p)
    {
        cout << "Node number:";
        if (p->data.num == 0)
        {
            cout << "Leisure node" << "  " << "Start address:" << p->data.address << "	" << "End address:" << p->data.address+p->data.size<<
                "	" << "Node space:" <<
                p->data.size << "KB" << "	" << "State:leisure" << endl;
        }
        else
        {
            cout << p->data.num;
            cout << "	" << "Start address:" << p->data.address << "	" << "End address:" << p->data.address + p->data.size << "	"
                "Node space:" << p->data.size
                << "KB" << "	" << "State:";
            if (p->data.state == FREE)
                cout << "leisure" << endl;
            else if (p->data.state == BUSY)
                cout << "occupied" << endl;
        }
        p = p->next;
    }
    cout << "**********************************************" << endl << endl;
}

int MemoryAllocator::firstAlloc(int num, int size)
{
    Linklist list = new node;
    list->data.num = num;
    list->data.size = size;
    list->data.state = BUSY;
    node *p = first->next;
    while (p)
    {
        if (p->data.state == FREE&&p->data.size == size)// When the sapce are equal
        {
            p->data.state = BUSY;
            p->data.num = num;
            display();
            return 1;
        }
        if (p->data.state == FREE&&p->data.size > size)// When the free space is bigger
        {
            list->head = p->head;
            list->next = p;
            list->data.address = p->data.address;
            p->head->next = list;
            p->head = list;
            p->data.address = list->data.address + list->data.size;
            p->data.size -= size;
            display();
            return 1;
        }
        p = p->next;
    }
    display();
    return 0;
}

int MemoryAllocator::bestAlloc(int num, int size)
{
    int min_space=MAXSIZE;
    Linklist list = new node;
    list->data.num = num;
    list->data.size = size;
    list->data.state = BUSY;
    node *p = first->next;
    node *q = NULL;
    while (p)//find the best node
    {
        if ((p->data.size > size || p->data.size == size)&&p->data.state==FREE)
        {
            if (p->data.size - size < min_space)
            {
                q = p;
                min_space = p->data.size - size;
            }
        }
        p = p->next;
    }

    if (q == NULL)
    {
        return 0;
    }
    else
    {
        if (min_space == 0)
        {
            q->data.num = num;
            q->data.state = BUSY;
            display();
            return 1;
        }
        else
        {
            list->head = q -> head;
            list->next = q;
            list->data.address = q->data.address;
            q->head->next = list;
            q->head = list;
            q->data.address += size;
            q->data.size -= size;
            display();
            return 1;
        }
    }
}

int MemoryAllocator::recycle(int num)
{
    node *p = first;
    while (p)
    {
        if (p->data.num == num)
        {
            p->data.state = FREE;
            p->data.num = 0;
            if (p->head->data.state == FREE)//If adjacent to the previous free area, then merge
            {
                p->head->data.size += p->data.size;
                p->head->next = p->next;
                p->next->head = p->head;
            }
            if (p->next->data.state == FREE)//If adjacent to the following free area,then merge
            {
                p->data.size += p->next->data.size;
                p->next->next->head = p;
                p->next = p->next->next;
            }
            break;
        }
        p = p->next;
    }
    display();
    return 1;

}

int MemoryAllocator::getspace(int num)  //Enter the node number you want to return in parentheses
{
    node *p = first->next;
    while (p)
    {
        if (p->data.num == num)
        {
         return p->data.size;
        }
        p = p->next;
    }
}

void MemoryAllocator::menu()
{
    cout << "********************memory allocation********************" << endl;
    cout << "*              1.use best alloc to distribute          " << endl;
    cout << "*              2.get back current data                  " << endl;
    cout << "*              3.get back memory                          " << endl;
    cout << "*              4.quit                              " << endl;
    cout << "*              5.Individually get back sapce of node 2" <<endl;
    cout << "****************************************************" << endl;
    cout << "PLZ choose:" << endl;

}
