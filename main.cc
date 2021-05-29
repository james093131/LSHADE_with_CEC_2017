#include "normal.h"

int main(int argc,const char *argv[])
{
    int run = atoi(argv[1]);
    int DIM = atoi(argv[2]);
    int Function_Number = atoi(argv[3]);
    int MAX_NFE = atoi(argv[4]);


    
    if( argc > 1 )
    {
        LSHADE lshade;
        lshade.Run( run, DIM,Function_Number,MAX_NFE);
    }
      
    else 
    {
        LSHADE lshade;
        lshade.Run(1,10,1,100000);

    }
   
    return 0;
}