#include <iostream>
#include <vector>
#include "../sat.h"
#include <fstream>
#include <ctime>

using namespace std;

class Gate
{
public:
   Gate(unsigned i): _gid(i) {}
   Gate() {}
   ~Gate() {}

   Var getVar() const { return _var; }
   void setVar(const Var& v) { _var = v; }
   int getGroup() const { return group; }
   void setGroup(const int& _g) { group = _g; }
   int getIntervieweeNumber() const { return interviewee_number; }
   void setIntervieweeNumber(const int& _interviewee) { interviewee_number = _interviewee; }
   int getInterviewerNumber() const { return interviewer_number; }
   void setInterviewerNumber(const int& _interviewer) { interviewer_number = _interviewer; }
   int getDay() const { return day; }
   void setDay(const int& _day) { day = _day; }
   bool getFreeDay() const { return free_day; }
   void setFreeDay(const bool& _free) { free_day = _free; }

private:
   unsigned   _gid;  // for debugging purpose...
   Var        _var;
   int        group;
   int        interviewee_number;
   int        day;
   bool       free_day;
   int        interviewer_number;
};

vector<vector<Gate *> > interviewer;
vector<vector<Gate *> > interviewee;
vector<vector<Gate *> > interviewee_come;

void
genProofModel(SatSolver& s, int y)
{
   for(int i=0; i < interviewer.size(); i++){
      for(int j=0; j < interviewer[i].size(); j++){
         Var v = s.newVar();
         // cout << "Var: " << v << " ";
         interviewer[i][j]->setVar(v);
         // cout << interviewer[i][j]->getFreeDay() << " ";
         if(!interviewer[i][j]->getFreeDay())
            s.addInitial(v);
      }
      // cout << endl;
   }
   for(int i=0; i < interviewee.size(); i++){
      for(int j=0; j < interviewee[i].size(); j++){
         Var v = s.newVar();
         interviewee[i][j]->setVar(v);
         // cout << interviewee[i][j]->getFreeDay() << " ";
         if(!interviewee[i][j]->getFreeDay())
            s.addInitial(v);
      }
      // cout << endl;
   }
   for(int i=0; i < interviewee_come.size(); i++){
      for(int j=0; j < 5; j++){
         Var v = s.newVar();
         interviewee_come[i][j]->setVar(v);
         
         // cout << "Var: " << v << " ";
         // cout << interviewee_come[i][j]->getFreeDay() << " ";
         if(!interviewee_come[i][j]->getFreeDay())
            s.addInitial(v);
      }
      // cout << endl;
   }


   for(int i=0; i < interviewee.size(); i++){
      for(int j=0; j < interviewee[i].size(); j++){
         int temp_group = interviewee[i][j]->getGroup() - 1;
         int temp_day = interviewee[i][j]->getDay() - 1; 
         // cout << "interviewee number: " << i << endl; 
         
         // cout << "group: " << interviewee[i][j]->getGroup() << endl;
         // cout << "Day: "   << interviewee[i][j]->getDay() << endl;
         // cout << "interviewer group: " << interviewer[temp_group][temp_day]->getGroup() << endl;
         // cout << "interviewer day: " << interviewer[temp_group][temp_day]->getDay() << endl;
         s.addCondition1(interviewee[i][j]->getVar(), interviewer[temp_group][temp_day]->getVar());
      }
      for(int j=0; j < 3; j++){
         s.addCondition2(interviewee[i][5*j]->getVar(),interviewee[i][5*j+1]->getVar(),interviewee[i][5*j+2]->getVar(),interviewee[i][5*j+3]->getVar(),interviewee[i][5*j+4]->getVar());
         s.addCondition3(interviewee[i][5*j]->getVar(),interviewee[i][5*j+1]->getVar(),interviewee[i][5*j+2]->getVar(),interviewee[i][5*j+3]->getVar(),interviewee[i][5*j+4]->getVar());
      }
      for(int j=0; j < 5; j++){
         s.addCondition4(interviewee_come[i][j]->getVar(),interviewee[i][j]->getVar(),interviewee[i][j+5]->getVar(),interviewee[i][j+10]->getVar());
      }
   }
   vector<Var> vec_var;
   for(int j = 0; j < 5; j++){
      for(int i = 0; i < interviewee_come.size(); i++){
         vec_var.push_back(interviewee_come[i][j]->getVar());
      }
      s.addCondition5(vec_var, y, 0, 0);
      vec_var.clear();
      s.MapClear();
   }
}


void reportResult(const SatSolver& solver, bool result, fstream& fout)
{
   solver.printStats();
   cout << (result? "SAT" : "UNSAT") << endl;
   if (result) {
      for(int i = 0; i < interviewer.size(); i++){
         fout << "interviewer group: " << interviewer[i][0]->getGroup() << endl;
         for(int j = 0; j < interviewer[i].size(); j++){           
            fout << solver.getValue(interviewer[i][j]->getVar()) << " ";
         }
         fout << endl;
      }
      for(int i = 0; i < interviewee.size(); i++){
         fout << "interviewee number: " << interviewee[i][0]->getIntervieweeNumber() << endl;
         for(int j = 0; j < interviewee[i].size(); j++){
            if(j == 0){
               fout << "interviewee group: " << interviewee[i][j]->getGroup() << endl;
            }
            else if(j == 5){
               fout << endl;
               fout << "interviewee group: " << interviewee[i][j]->getGroup() << endl;
            }
            else if(j == 10){
               fout << endl;
               fout << "interviewee group: " << interviewee[i][j]->getGroup() << endl;
            }
            fout << solver.getValue(interviewee[i][j]->getVar()) << " ";
         }
         fout << endl;
      }
      for (size_t i = 0; i < interviewee_come.size(); i++){
         fout << "interviewee day to come: " << interviewee[i][0]->getIntervieweeNumber() << endl;
         for (size_t j = 0; j < interviewee_come[i].size(); j++){
            fout << solver.getValue(interviewee_come[i][j]->getVar()) << " ";
         }
         fout << endl;
      }
   }
}

void init(fstream& fin){
   int number_groups;
   int number_interviewees;
   fin >> number_groups;
   fin >> number_interviewees;
   for(int i=1; i <= number_groups; i++){
      vector<Gate *> temp_vec;
      for(int j=1; j <= 5; j++){
         int free;
         Gate* temp_gate = new Gate();
         temp_gate->setGroup(i);
         temp_gate->setInterviewerNumber(i);
         temp_gate->setDay(j);
         fin >> free;
         if(free)
            temp_gate->setFreeDay(true);
         else
            temp_gate->setFreeDay(false);
         temp_vec.push_back(temp_gate);
      }
      interviewer.push_back(temp_vec);
   }
   for(int i=1; i <= number_interviewees; i++){
      int d1, d2, d3, d4, d5, g1, g2, g3;
      vector<Gate *> temp_vec;
      vector<Gate *> temp_vec_free;
      fin >> d1;
      fin >> d2;
      fin >> d3;
      fin >> d4;
      fin >> d5;
      fin >> g1;
      fin >> g2;
      fin >> g3;
      for(int j=0; j < 5; j++){
         Gate* temp_gate_free = new Gate();
         temp_gate_free->setDay(j+1);
         switch(j){
            case 0:
               if(d1)
                  temp_gate_free->setFreeDay(true);
               else
                  temp_gate_free->setFreeDay(false);
               break;
            case 1:
               if(d2)
                  temp_gate_free->setFreeDay(true);
               else
                  temp_gate_free->setFreeDay(false);
               break;
            case 2:
               if(d3)
                  temp_gate_free->setFreeDay(true);
               else
                  temp_gate_free->setFreeDay(false);
               break;
            case 3:
               if(d4)
                  temp_gate_free->setFreeDay(true);
               else
                  temp_gate_free->setFreeDay(false);
               break; 
            case 4:
               if(d5)
                  temp_gate_free->setFreeDay(true);
               else
                  temp_gate_free->setFreeDay(false);
               break;
         }
         temp_vec_free.push_back(temp_gate_free);
      }
      interviewee_come.push_back(temp_vec_free);
      for(int j=0; j < 15; j++){
         Gate* temp_gate = new Gate();
         temp_gate->setIntervieweeNumber(i);
         if(j/5 == 0){
            temp_gate->setGroup(g1);
            if(j%5 == 0){
               temp_gate->setDay(1);
               if(d1)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 1){
               temp_gate->setDay(2);
               if(d2)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 2){
               temp_gate->setDay(3);
               if(d3)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 3){
               temp_gate->setDay(4);
               if(d4)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 4){
               temp_gate->setDay(5);
               if(d5)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
         }
         else if(j/5 == 1){
            temp_gate->setGroup(g2);
            if(j%5 == 0){
               temp_gate->setDay(1);
               if(d1)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 1){
               temp_gate->setDay(2);
               if(d2)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 2){
               temp_gate->setDay(3);
               if(d3)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 3){
               temp_gate->setDay(4);
               if(d4)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 4){
               temp_gate->setDay(5);
               if(d5)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
         }
         else if(j/5 == 2){
            temp_gate->setGroup(g3);
            if(j%5 == 0){
               temp_gate->setDay(1);
               if(d1)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 1){
               temp_gate->setDay(2);
               if(d2)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 2){
               temp_gate->setDay(3);
               if(d3)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 3){
               temp_gate->setDay(4);
               if(d4)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
            else if(j%5 == 4){
               temp_gate->setDay(5);
               if(d5)
                  temp_gate->setFreeDay(true);
               else
                  temp_gate->setFreeDay(false);
               temp_vec.push_back(temp_gate);
            }
         }
      }
      interviewee.push_back(temp_vec);
   }
}

bool proof(SatSolver& solver, int y){
   bool check = true;
   for(int i = 0; i < interviewee.size(); i++){
      bool come = false;
      for(int j = 0; j < interviewee[i].size(); j++){
         if(j%5 == 0)
            come = false;
         if(solver.getValue(interviewee[i][j]->getVar()) == 1 && solver.getValue(interviewer[interviewee[i][j]->getGroup()-1][j%5]->getVar()) == 0){
            cout << "condition1 doesn't match." << endl;
            check = false;
         }
         if(!come && solver.getValue(interviewee[i][j]->getVar())){
            come = true;
         }
         else if(come && solver.getValue(interviewee[i][j]->getVar())){
            cout << "condition2 doesn't match." << endl;
            check = false;
         }
         if(!come && j%5 == 4){
            cout << "condition3 doesn't match." << endl;
            check = false;
         }
         if(j < 5){
            if((solver.getValue(interviewee[i][j]->getVar()) == 1 || solver.getValue(interviewee[i][j+5]->getVar()) == 1 || solver.getValue(interviewee[i][j+10]->getVar()) == 1) && solver.getValue(interviewee_come[i][j%5]->getVar()) != 1){
               cout << "condition4 doesn't match." << endl;
               check = false;
            }
         }
      }
   }
   for (int i = 0; i < 5; i++){
      int temp = 0;
      for (int j = 0; j < interviewee_come.size(); j++){  
         if(solver.getValue(interviewee_come[j][i]->getVar()))
            temp++;
      }
      if(temp > y){
         cout << "condition5 doesn't match." << endl;
         cout << temp << endl;
         check = false;
      }
   }
   
   if(check)
      cout << "all condition pass." << endl;
   return check;
}


int main(int argc, char* argv[])
{
   fstream fin(argv[1]);
   fstream fout(argv[2]);
   // initCircuit();
   init(fin);

   clock_t start, stop;
   
   start = clock();

   SatSolver solver;
   solver.initialize();

   int y;
   cout << "input number of limit: ";
   cin >> y;
   genProofModel(solver,y);
   bool result;
   result = solver.solve();

   /*
   int num = 1;
   while(result){
      bool check_condition = true;
      for (int i = 0; i < 5; i++){
         int temp = 0;
         for (int j = 0; j < interviewee_come.size(); j++){  
            if(solver.getValue(interviewee_come[j][i]->getVar()))
               temp++;
         }
         if(temp > y){
            vec<Lit> lits;
            for (int j = 0; j < interviewee_come.size(); j++){ 
               if(solver.getValue(interviewee_come[j][i]->getVar()) == 1)
                  lits.push(~Lit(interviewee_come[j][i]->getVar()));
               else
                  lits.push(Lit(interviewee_come[j][i]->getVar()));
            }
            solver.addClause(lits);
            lits.clear();
            check_condition = false;
         }   
      }
      if(check_condition)
         break;
      result = solver.solve();
      num++;
   }
   
   cout << "limit = " << y << endl;
   cout << num << endl;
   */
/*
   if(result){
      if(!proof(solver,y)){
         vector<Var> vec_var;
         for(int j = 0; j < 5; j++){
            for(int i = 0; i < interviewee_come.size(); i++){
               vec_var.push_back(interviewee_come[i][j]->getVar());
            }
            solver.addCondition5(vec_var, y, 0, 0);
            vec_var.clear();
            solver.MapClear();
         }
         result = solver.solve(); 
      }  
   }
   */
   reportResult(solver, result, fout);
   
   if(result)
      proof(solver,y);
   stop = clock();

   cout << double(stop - start) / CLOCKS_PER_SEC <<endl;

}
