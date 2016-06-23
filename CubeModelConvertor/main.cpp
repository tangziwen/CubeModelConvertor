#include <iostream>

#include "CM_Model.h"
#include <time.h>
using namespace std;


int main(int argc, char *argv[])
{
    if (argc >1)
    {
        cout<<"Please Wait..."<<endl;
        auto old = clock();
        auto theModel = new tzw::CM_Model(argv[1]);
        if (argc > 2)
        {
            theModel->DumpTo(argv[2]);
        }
        else
        {
            theModel->DumpTo("./out.tzw");
        }
        cout<<"Done! Time: "<<clock() - old <<" ms" <<endl;
    }else
    {
        cout<<"tzw's model convertor."<<endl;
        cout<<"usage: convertor yourmodel.xx out.tzw"<<endl;;
    }
    return 0;
}
