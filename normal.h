#include<stdio.h>
#include<fstream>
#include<iostream>
#include<sstream>
#include<stdlib.h>
#include <string.h>
#include<time.h>
#include<vector>
#include <algorithm>
#include<math.h>
#include<float.h>
#include <sys/stat.h>
#include <random>
#include <chrono>
#include "cec17_test_func.cc"

using namespace std;

typedef vector<char> c1d;
typedef vector<c1d> c2d;
typedef vector<c2d> c3d;
typedef vector<c3d> c4d;
typedef vector<int> i1d;
typedef vector<i1d> i2d;
typedef vector<i2d> i3d;
typedef vector<i3d> i4d;
typedef vector<double>d1d;
typedef vector<d1d> d2d;
typedef vector<d2d> d3d;
typedef vector<d3d> d4d;


class LSHADE{
    public :
        d1d Run_Result;
        d1d Run_Evaluation_Result;
        double max;
        double min;
        double optimal;
        double Current_best;
    public : 
        void Run(int run ,int DIM,int Function,int MAX_NFE)
        {
            srand( time(NULL) ); 
            Initial(DIM,Function);

            Evaluation(DIM,Function,Particle,Objective);
            while(NFE < MAX_NFE)
            {
                Transition(DIM,Function);
                Linear_Reduction( DIM, NFE,MAX_NFE);
            }
        }
    


    private :
        d2d H_Table; //record table
        d2d Particle; // population
        d1d Objective; //objective value
        d1d Rank_index; //objective Value ranking sort ascending
        d2d Archieve;
        int Archieve_MAX_SIZE;
        int H_Current;
        int NFE;
    private : 
        void RANDOM_INI(int DIM)
        {

            int SIZE = Particle.size();
            for(int i=0;i<SIZE;i++)
            {
                for(int j=0;j<DIM;j++)
                {
                    double a = ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
                    Particle[i][j] = a;
                }
               
            }

        }
        void Initial(int DIM,int F)
        {
            max = 100.0;
            min = -100.0;
            optimal = 100*F;
            Archieve_MAX_SIZE = 2.6*DIM*18;
            Current_best = DBL_MAX;
            H_Current = 0;
            NFE = 0;
            Particle.clear();
            Particle.swap((Particle));

            Objective.clear();
            Objective.swap(Objective);

            H_Table.clear();
            H_Table.swap(H_Table);

            Rank_index.clear();
            Rank_index.swap(Rank_index);

            Archieve.clear();
            Archieve.swap(Archieve);

            Particle.resize(DIM*18,d1d(DIM,0));

            Objective.resize(DIM*18);

            H_Table.resize(6*DIM,d1d(2,0.5));//0->CR 1->F 

            RANDOM_INI(DIM);
        }
    double Normal_Distribution(double mean)
        {
        if (mean < 0)
            return 0;

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine g1 (seed);            
        std::normal_distribution<double> distribution (mean,0.1);
        while( distribution(g1) <= 0 )
        {
            
                std::default_random_engine g1 (seed);      
                std::normal_distribution<double> distribution (mean,0.1);    
             
        }
        if(distribution(g1)>1)
            return 1;
       
        else
            return distribution(g1);
        }
    double Cauchy_Distribution(double mean)
    {
        std::default_random_engine g2;
        std::cauchy_distribution<double> distribution(mean,0.1);
        if(distribution(g2) > 1)
            return 1;
        else if(distribution(g2) < 0)
            return 0;
        else
            return distribution(g2);
    }
    int FIND_Pbest()
    {
        int P = round( Particle.size()*0.11) ;
        int x = rand() % (P - 0 + 1) + 0;

        return Rank_index[x];
    }
    double CEC_Objective_Check(double OBJ )
    {
        if(OBJ - optimal < 1e-8 )
            OBJ = optimal;

        return OBJ ;
    }
    d1d pairsort(d1d a) 
    { 
        int n =a.size();
        pair<double, double> pairt[n]; 
    
        // Storing the respective array 
        // elements in pairs. 
        for (int i = 0; i < n; i++)  
        { 
            pairt[i].first = a[i]; 
            pairt[i].second = i; 
        } 
       
        // Sort the pair array. 
        sort(pairt, pairt + n); 
       

        d1d b(n,0);
        // Modifying original arrays 
        for (int i = 0; i < n; i++)  
        { 
            b[i] = pairt[i].second; 
        } 
        return b;

    } 
    void Rank()//sort descending
    {
      Rank_index = pairsort(Objective); 
    }
  
    void Evaluation(int DIM,int Function,d2d arr,d1d &OBJ)
    {
        int P = arr.size();
        for(int i=0;i<P;i++)
        {
            // cout<<"B"<<endl;
            // for(int j=0;j<DIM;j++)
            // {
            //     cout<<arr[i][j]<<' ';
            // }
            // cout<<OBJ[i]<<endl;
            cec17_test_func(&arr[i][0],&OBJ[i],DIM,1,Function);
                // cout<<"A"<<endl;


            OBJ[i] = CEC_Objective_Check( OBJ[i] );
            

            if(OBJ[i] < Current_best)
                Current_best = OBJ[i];

            if( NFE % 500 == 0 )
                cout<<NFE<<' '<<Current_best<<endl;
            
            NFE ++;

        }
        Rank();
    }

    void UPDATE_H_Table(d1d CR,d1d F,d1d OBj)
    {
        int SIZE = CR.size();
        double OBj_SUM = 0.0;
        if(SIZE != 0)
        {

            for(int i=0;i<SIZE;i++)
            {
                OBj_SUM += OBj[i];
            }


            double CR_temp1 = 0.0;
            double CR_temp2 = 0.0;
            double F_temp1  = 0.0;
            double F_temp2  = 0.0;
            for(int i=0;i<SIZE;i++)
            {
                CR_temp1 += (OBj[i]/OBj_SUM) * CR[i] * CR[i];
                CR_temp2 += (OBj[i]/OBj_SUM) * CR[i];
                
                F_temp1 += (OBj[i]/OBj_SUM) * F[i] * F[i];
                F_temp2 += (OBj[i]/OBj_SUM) * F[i] ;
            }


            if(H_Table[H_Current][0]  != -1)
            {
                if(CR_temp2 == 0)
                    H_Table[H_Current][0] = -1;
                else
                    H_Table[H_Current][0] = (CR_temp1/CR_temp2);
            }
            else{
                H_Table[H_Current][0]  = -1;
            }

            H_Table[H_Current][1] = (F_temp1/F_temp2);

            H_Current ++;
            if(H_Current == H_Table.size())
                H_Current = 0;
        }
    }

    void Transition(int DIM,int Function)
    {
        int P = Particle.size();
        int H_Size = H_Table.size();    
        d2d Particle_V(P,d1d(DIM,0));
        d1d Objective_V(P);
        d1d Table_CR,Table_F,S_CR,S_F,S_Obj;
        double j_random = rand() % ( (DIM -1) - 0 + 1) + 0;
        for(int i=0;i<P;i++)
        {
            int rH = rand() % ( (H_Size -1) - 0 + 1) + 0;
            double CR = Normal_Distribution(H_Table[rH][0]);
            double F  = Cauchy_Distribution(H_Table[rH][1]);
            for(int j =0;j<DIM;j++)
            {
                double r_cr = (1 - 0) * rand() / (RAND_MAX + 1.0) + 0;

                if( r_cr <= CR || j_random == j )
                {
                    Particle_V[i][j] = Generate_vector(i,j,P,F);
                }
                else{
                    Particle_V[i][j] = Particle[i][j];
                }
            }
            Table_CR.push_back(CR);
            Table_F.push_back(F);
        }
        Evaluation(DIM,Function,Particle_V,Objective_V);
        for(int i=0;i<P;i++) // if better than before Update!
        {
            if (Objective_V[i] < Objective[i] )
            {
                S_CR.push_back(Table_CR[i]);
                S_F.push_back(Table_F[i]);
                S_Obj.push_back(Objective[i] - Objective_V[i]);

                Particle[i].assign(Particle_V[i].begin(), Particle_V[i].end());
                Objective[i] = Objective_V[i];
                
                Objective[i] = CEC_Objective_Check(Objective[i]);

                Archieve.push_back(Particle[i]);
                while(Archieve.size() >  Archieve_MAX_SIZE)
                {
                    int r_DEL = rand() % ( ( Archieve.size() -1 ) - 0 + 1) + 0;
                    Archieve.erase(Archieve.begin()+r_DEL);
                }
               
            }
        }
        UPDATE_H_Table(S_CR,S_F,S_Obj);
        Rank();

    }
    double Generate_vector(int ind,int DIM,int Size,double F) // pbest formula 
    {
        int Pbest = FIND_Pbest();

        int r1 = rand() % ( (Size -1) - 0 + 1) + 0;
        while( r1 == ind || r1 == Pbest)
        {
            r1 = rand() % ( (Size -1) - 0 + 1) + 0;
        }

        int Archieve_size = Archieve.size();

        int r2 = rand() % ( (Size + Archieve_size -1) - 0 + 1) + 0;
        while( r2 == ind || r2 ==r1 || r2 == Pbest )
        {
            r2 = rand() % ( (Size -1) - 0 + 1) + 0;
        }
        double r2_bar;
        if (r2 < Size )
        {
            r2_bar = Particle[r2][DIM];
        }
        else
        {
            r2_bar = Archieve[r2-Size][DIM]; 
        }


        double V = Particle[ind][DIM] + F * (Particle[Pbest][DIM] - Particle[ind][DIM])\
                 + F * (Particle[r1][DIM] - r2_bar) ;

        if (V > max)        
            V = (Particle[ind][DIM]+max)/2;
        else if (V < min)
            V = (Particle[ind][DIM]+min)/2;
        return V;
    }
    void Linear_Reduction(int DIM,int NFE,int MAX_NFE)
    {
        double N_min = 4.0;
        double N_init = DIM*18.0;

        double NEW_N =  N_min - N_init/MAX_NFE *NFE + N_init;
        if ( round(NEW_N) < Particle.size() )
        {
            int DELETE = Particle.size() - NEW_N;
            d1d DELETE_INDEX(DELETE,0);
            for(int i=0;i<DELETE;i++)
            {
                DELETE_INDEX[i] = Rank_index[Rank_index.size()-i-1];
            }
            sort(DELETE_INDEX.begin(),DELETE_INDEX.end(),greater<int>());
            
            for(int i=0;i<DELETE;i++)
            {
                Particle.erase(Particle.begin()+ DELETE_INDEX[i] );
                Objective.erase(Objective.begin() + DELETE_INDEX[i]);
                Rank_index.erase(Rank_index.begin() + DELETE_INDEX[i]);
            }
                Rank();
        }
        
    }






};