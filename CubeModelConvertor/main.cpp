#include <iostream>

#include "CM_Model.h"
#include <time.h>
#include <filesystem>
using namespace std;


int main(int argc, char *argv[])
{
    if (argc >1)
    {
        cout<<"Please Wait..."<<endl;
        auto old = clock();
        auto theModel = new tzw::CM_Model(argv[1]);
    	float size = 1.0;
    	std::string targetName;
        if (argc > 3)
        {
        	size = atof(argv[3]);
        }
        if (argc > 2)
        {
        	targetName = argv[2];
        }else
        {
	        targetName = "out";
        }
    	theModel->DumpTo(theModel->getFileNameWithOutExtension(targetName) + ".tzw", size);
        cout<<"Done! Time: "<<clock() - old <<" ms" <<endl;
    }else
    {
        cout<<"tzw's model convertor."<<endl;
        cout<<"usage: convertor yourmodel.xx out.tzw"<<endl;;
    }
    return 0;
}
