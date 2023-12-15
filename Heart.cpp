#include "Heart.h"

Heart::~Heart()
{
}

int Heart::mySend(vector<char>& data, size_t comm_fd)
{

    HeartCheckRes dat;

    write((int)comm_fd, &dat, dat.head.len);

    cout << ((Head*)data.data())->len<<endl;


    return 0;
}
