// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <queue>
#include <algorithm>
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <stdio.h>


//declare the method
void threadCreation();

bool expectVCommand = true;
bool expectECommand = false;
int vertexes = 0;

//below are global varialbes as thread when called can reference it. and not 2 varialbes used by same thread. so not race
//will be there 
std::set<std::pair<int,int>> edgeInputA1;
std::vector<std::pair<int,int>> edgeInputA2;
std::vector<std::pair<int,int>> edgeInputA3;
std::string CNF_SAT;
std::string APPROX_VC1;
std::string APPROX_VC2;


//this is the algo for picking randomly edges and then removing from list
void* callApproxVC2(void *arg){
    std::vector<int> listofCovers;


    clockid_t cid;

    //get cpu time clock and put its ref to data pointed by cid
    pthread_getcpuclockid(pthread_self(), &cid);

    timespec startTime;
    clock_gettime(cid,&startTime);

    //std::cout<<startTime.tv_nsec<<std::endl;
    //std::cout<<startTime.tv_sec<<std::endl;
    

    while(edgeInputA3.size()!=0){

        int x = edgeInputA3[0].first;
        int y =edgeInputA3[0].second;

        listofCovers.push_back(x);
        listofCovers.push_back(y);

        for(int i=0;i<edgeInputA3.size();i++){
            if(edgeInputA3[i].first==x||edgeInputA3[i].second==x||edgeInputA3[i].first==y||edgeInputA3[i].second==y){
                edgeInputA3.erase(edgeInputA3.begin()+i);
                i--;
            }
        }

    }
    
    timespec endTime;
    clock_gettime(cid,&endTime);

    //std::cout<<endTime.tv_nsec<<std::endl;
    //std::cout<<endTime.tv_sec<<std::endl;
   // std::cout<<"Time diff: "<<(endTime.tv_nsec-startTime.tv_nsec)<<std::endl;
   // std::cout<<"Time diff in sec: "<<(endTime.tv_sec-startTime.tv_sec)<<std::endl;


    std::sort(listofCovers.begin(), listofCovers.end());    

          
    std::string A3Response = "APPROX-VC-2: ";    
    for(int i=0;i<listofCovers.size()-1;i++){
        A3Response=A3Response+std::to_string(listofCovers.at(i))+",";
    }

    A3Response=A3Response+std::to_string(listofCovers.at(listofCovers.size()-1));
    //std::cout<<A3Response<<std::endl;
    APPROX_VC2=A3Response;

    return nullptr;
}



//this is the algo for picking highest vertex cover and then removing that vertex and corresponding edge from input
void* callApproxVC1(void *arg){
    std::vector<int> listofCovers;
        // for (auto elem : edgeinput)
        //         {
        //             int x = elem.first;
        //             int y = elem.second;
                   
        //             Minisat::vec<Minisat::Lit> clause4;
        //             for(int i=0;i<currk;i++){
        //                 clause4.push(variables[(x-1)*currk+i]);
        //                 clause4.push(variables[(y-1)*currk+i]);
        //             }
        //             solver->addClause(clause4);  

        //         } 

    clockid_t cid;

    //get cpu time clock and put its ref to data pointed by cid
    pthread_getcpuclockid(pthread_self(), &cid);

    timespec startTime;
    clock_gettime(cid,&startTime);

    //std::cout<<startTime.tv_nsec<<std::endl;
    //std::cout<<startTime.tv_sec<<std::endl;

        while(edgeInputA2.size()!=0){

        //    std::cout<<"EdgeINput size"<<edgeinput.size()<<std::endl;
            int highestVertexIndex = -1;
            int localCount=0;
            int maxCount=0;

            // for(auto elem:edgeinput){
            //     int x =elem.first;
            //     int y = elem.second;
                 
                
            //         std::cout<<"<printing"<<x<<","<<y<<std::endl;
                
                
            // }
            
            //for finding the max vertex cover
            for(int i=1;i<=vertexes;i++){

                  for(int j =0;j<edgeInputA2.size();j++){

                      int x =edgeInputA2[j].first;
                      int y =edgeInputA2[j].second;

                      if(x==i||y==i){
                           localCount++; 
                      }  
                  }
                    if(localCount>maxCount){
                        maxCount=localCount;
                        highestVertexIndex=i;
                    }
                    localCount=0;
            }
           // std::cout<<"element slected is "<<highestVertexIndex<<"maxcount is "<<maxCount<<std::endl;
            for(int j =0;j<edgeInputA2.size();j++){
                int x =edgeInputA2[j].first;
                int y = edgeInputA2[j].second;
                // if(x==13||y==13){
                //     std::cout<<"((("<<x<<" "<<y<<std::endl;
                //     std::cout<<highestVertexIndex<<std::endl;
                // }    
                if(x==highestVertexIndex||y==highestVertexIndex){
                    edgeInputA2.erase(edgeInputA2.begin()+j);
                    // std::cout<<"<"<<x<<","<<y<<std::endl;
                    j--;
                }
                
            }
            listofCovers.push_back(highestVertexIndex);
              

        }


    timespec endTime;
    clock_gettime(cid,&endTime);

    //std::cout<<endTime.tv_nsec<<std::endl;
    //std::cout<<endTime.tv_sec<<std::endl;
   // std::cout<<"Time diff: "<<(endTime.tv_nsec-startTime.tv_nsec)<<std::endl;
   // std::cout<<"Time diff in sec: "<<(endTime.tv_sec-startTime.tv_sec)<<std::endl;

        std::sort(listofCovers.begin(), listofCovers.end());    
        std::string A2Response = "APPROX-VC-1: ";    
            for(int i=0;i<listofCovers.size()-1;i++){
                A2Response=A2Response+std::to_string(listofCovers.at(i))+",";
            }

            A2Response=A2Response+std::to_string(listofCovers.at(listofCovers.size()-1));
           // std::cout<<A2Response<<std::endl;
        APPROX_VC1=A2Response;

        return nullptr;

}


//this is the minsat using vertex cover problem
void* callVertexCoverCheck(void *arg){
            // -- allocate on the heap so that we can reset later if needed

            // if(edgeInputA1.size()==0){
            //     std::cout<<std::endl;
            //     return;
            // }

//            std::cout<<"here"<<std::endl;

            clockid_t cid;

            //get cpu time clock and put its ref to data pointed by cid
            pthread_getcpuclockid(pthread_self(), &cid);

            timespec startTime;
            clock_gettime(cid,&startTime);

            //std::cout<<startTime.tv_nsec<<std::endl;
            //std::cout<<startTime.tv_sec<<std::endl;

            std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
            int minimum=0;
            Minisat::vec<Minisat::Lit> variables;


            for(int currk=1;currk<=vertexes;currk++){
                
                int totalVar = currk*vertexes;
                

                //preparingall the variables according to currk size
                for(int i=0;i<totalVar;i++){
                    Minisat::Lit l;
                    l = Minisat::mkLit(solver->newVar());
                    variables.push(l);
                }


                //this is for adding cnf with each k having at least one  vertex associated
                for(int i=0;i<currk;i++){
                    Minisat::vec<Minisat::Lit> clause1;
                    for(int k=0;k<vertexes;k++){
                         clause1.push(variables[k*currk+i]);   
                    }
                    solver->addClause(clause1);
                }   

                
                //no one vertex should have two covers association    
                for(int i=0;i<vertexes;i++){
                    for(int q=0;q<currk;q++){
                        for(int p=0;p<q;p++){
                           Minisat::vec<Minisat::Lit> clause2;
                           clause2.push(~variables[currk*i+p]);
                           clause2.push(~variables[currk*i+q]);
                           solver->addClause(clause2);     
                        }
                    }
                }

                //no one cover should be taken by two vertices
                for(int i=0;i<currk;i++){
                    for(int q=0;q<vertexes;q++){
                        for(int p=0;p<q;p++){
                           Minisat::vec<Minisat::Lit> clause3;
                           clause3.push(~variables[currk*p+i]);
                           clause3.push(~variables[currk*q+i]);
                           solver->addClause(clause3);     
                        }
                    }
                }

                for (auto elem : edgeInputA1)
                {
                    int x = elem.first;
                    int y = elem.second;
                   
                    Minisat::vec<Minisat::Lit> clause4;
                    for(int i=0;i<currk;i++){
                        clause4.push(variables[(x-1)*currk+i]);
                        clause4.push(variables[(y-1)*currk+i]);
                    }
                    solver->addClause(clause4);  

                }    
                // auto val = solver->solve();
                // std::cout<<"Value of for "<<currk <<"is "<<val<<std::endl;
                if(solver->solve()){
                    minimum=currk;
                    break;
                }else{
                    solver.reset (new Minisat::Solver());
                    variables.clear();
                    minimum=0;
                }    

            }
           // std::cout<<"here2"<<minimum<<std::endl;
            std::vector<int> listofCovers;
            for(int i=0;i<vertexes;i++){
                for(int j=0;j<minimum;j++){
                    if(solver->modelValue(variables[i*minimum+j])==Minisat::l_True){
                        listofCovers.push_back(i+1);
                    }
                }
            }

    timespec endTime;
    clock_gettime(cid,&endTime);

    //std::cout<<endTime.tv_nsec<<std::endl;
    //std::cout<<endTime.tv_sec<<std::endl;
   // std::cout<<"Time diff: "<<(endTime.tv_nsec-startTime.tv_nsec)<<std::endl;
   // std::cout<<"Time diff in sec: "<<(endTime.tv_sec-startTime.tv_sec)<<std::endl;

            std::sort(listofCovers.begin(), listofCovers.end());    

            std::string A1Response = "CNF-SAT-VC: ";    
            for(int i=0;i<listofCovers.size()-1;i++){
                A1Response=A1Response+std::to_string(listofCovers.at(i))+",";
            }

            A1Response=A1Response+std::to_string(listofCovers.at(listofCovers.size()-1));
           // std::cout<<A1Response<<std::endl;
           CNF_SAT=A1Response;

           return nullptr;

}

void parseVCommand(std::string &cmdLine){


        expectECommand =false;
       

        std::string vCommandArg = cmdLine.substr(1,cmdLine.size()-1);
    
        //trim forward whitespace and ending space
        vCommandArg = std::regex_replace(vCommandArg, std::regex("^ +"), "");
        vCommandArg = std::regex_replace(vCommandArg, std::regex(" +$"), "");

        for(int i=0;i<vCommandArg.size();i++){
            if(vCommandArg.at(i)<'0'||vCommandArg.at(i)>'9')
                throw "not valid char in input";
        }


        try{
              int value = std::stoi(vCommandArg);  
             

              if(value<=0)
                throw "argument is less than 1 in v command";

              vertexes  = value;
              expectECommand =true;
            
                
        }catch(...){
            throw "argument of V command not proper";
        }

}

void parseECommand(std::string &cmdLine){

       
        if(expectECommand==false){
            expectVCommand=true;
            expectECommand=false;
          
            throw "V command is expected first";
        
        }


        std::string eCommandArg = cmdLine.substr(1,cmdLine.size()-1);

        //trim forward and ending whitespace
        eCommandArg = std::regex_replace(eCommandArg, std::regex("^ +"), "");
        eCommandArg = std::regex_replace(eCommandArg, std::regex(" +$"), "");

        for(int i=0;i<eCommandArg.size();i++){
            if(eCommandArg.at(i)=='<'|| eCommandArg.at(i)=='>'|| eCommandArg.at(i)=='{'||eCommandArg.at(i)=='}'||
            eCommandArg.at(i)==','||(eCommandArg.at(i)>='0'&& eCommandArg.at(i)<='9')||eCommandArg.at(i)==' ')
                continue;
            else
                throw "not proper characters in arguments of E";    
        }

        if(eCommandArg.find('{')==std::string::npos)
            throw "open curly not found";

        if(eCommandArg.find('}')==std::string::npos)    
            throw "close curly not found";

        int start = eCommandArg.find('{');
        int end = eCommandArg.find('}');
        
        if(start>end ||start!=0 ||end!=eCommandArg.size()-1)
            throw "curly bracket not properly inputted";

        
        std::set<std::pair<int,int>> edgeInput;

        bool openBracket=false;
        bool edgeSeparation=true;
        std::string contentInBracket= "";

        for(int i=start+1;i<end;i++){

            char c =eCommandArg.at(i);

            if(c=='<'){

                if(openBracket!=false ||edgeSeparation!=true)
                    throw "issue in open bracket input for E command";

                openBracket=true;    

            }else if(c=='>'){

                if(openBracket!=true)
                    throw "issue in close bracket input for E command";

                std::regex reg(",");

                std::sregex_token_iterator iter(contentInBracket.begin(), contentInBracket.end(), reg, -1);
                std::sregex_token_iterator end;

                std::vector<std::string> edgePoints(iter, end);

                //std::cout<<edgePoints.size()<<std::endl;   
                
                if(edgePoints.size()!=2)
                    throw "more than two points in edge input";

                edgePoints[0] = std::regex_replace(edgePoints[0], std::regex("^ +"), "");
                edgePoints[0] = std::regex_replace(edgePoints[0], std::regex(" +$"), "");

                edgePoints[1] = std::regex_replace(edgePoints[1], std::regex("^ +"), "");
                edgePoints[1] = std::regex_replace(edgePoints[1], std::regex(" +$"), "");

                for(int i=0;i<edgePoints[0].size();i++){
                    if(edgePoints[0].at(i)<'0'||edgePoints[0].at(i)>'9')
                        throw "Vertex in e command not given proper";
                }

                for(int i=0;i<edgePoints[1].size();i++){
                    if(edgePoints[1].at(i)<'0'||edgePoints[1].at(i)>'9')
                        throw "Vertex in e command not given proper";
                }    
                
                int x=0;
                int y=0;    

                try{
                 x = std::stoi(edgePoints[0]);
                 y = std::stoi(edgePoints[1]);
                }catch(...){
                    throw "parsing error in vertex in edge input";
                }

                if(x<1||x>vertexes||y<1||y>vertexes||x==y)
                    throw "value of V in E command is wrong";

                if(edgeInput.count(std::make_pair(x,y))||edgeInput.count(std::make_pair(y,x))){
                    throw "repetative edges in argument of E command";
                }

                edgeInputA1.insert(std::make_pair(x,y));
                edgeInputA2.push_back(std::make_pair(x,y));
                edgeInputA3.push_back(std::make_pair(x,y));    

                openBracket=false;
                edgeSeparation=false;
                contentInBracket="";

            }else if(c==','&& edgeSeparation==false){
                edgeSeparation=true;

            }else if(openBracket==true){
                   contentInBracket+=c; 

            }else if(c==' '&& openBracket==false){
                    continue;

            }else
                throw "improper content formatting in E argument";        
        }
        
        expectECommand=false;
        
        // callVertexCoverCheck(edgeInputA1,vertexes);
        // callApproxVC1(edgeInputA2,vertexes);
        // callApproxVC2(edgeInputA3,vertexes);

        

}

void threadCreation(){

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    pthread_create(&thread1,nullptr,&callVertexCoverCheck,nullptr);
    pthread_create(&thread2,nullptr,&callApproxVC1,nullptr);
    pthread_create(&thread3,nullptr,&callApproxVC2,nullptr);

    // ref::https://linux.die.net/man/3/pthread_timedjoin_np
    timespec waitTime;
    clock_gettime(CLOCK_REALTIME,&waitTime);
    waitTime.tv_sec+=10;  //having wait time of 10 sec for this graph cnf-sat solver thread

    pthread_timedjoin_np(thread1,nullptr,&waitTime);
    pthread_join(thread2,nullptr);
    pthread_join(thread3,nullptr);
    
    std::cout<<CNF_SAT<<std::endl;
    std::cout<<APPROX_VC1<<std::endl;
    std::cout<<APPROX_VC2<<std::endl;

}


char parseCommandLine(std::string &cmdLine){

    if(cmdLine.find("V ")==0)
        return 'V';
    else if(cmdLine.find("E ")==0)
        return 'E';
    else
        throw "input command not proper";    
}

int main(int argc, char** argv) {

 
    while (!std::cin.eof()) {
      
        
        try{
            std::string cmdLine;
            std::getline(std::cin, cmdLine);
            

            if(std::cin.fail()==1)
                break;

            //trim forward whitespace and ending space
            cmdLine = std::regex_replace(cmdLine, std::regex("^ +"), "");
            cmdLine = std::regex_replace(cmdLine, std::regex(" +$"), "");

            char order=parseCommandLine(cmdLine);

            if(order=='V')
                parseVCommand(cmdLine);
            else if(order=='E'){    
                parseECommand(cmdLine);
                threadCreation();

                
            }
            edgeInputA1.clear();
            edgeInputA2.clear();
            edgeInputA3.clear();
            CNF_SAT="CNF-SAT-VC: timeout";


        }catch(const char * exception){
            std::cout<<"Error: "<<exception<<std::endl;
        }
    

    }


}
