#include <iostream>
#include <fstream>
#include <random>

using namespace std;

int main(int argc, char* argv[]) {
    fstream fout(argv[1]);
    int number_group;
    int number_interviewee;
    random_device rd;
    mt19937 gen(rd());
    cout << "please input number of groups: ";
    cin >> number_group;
    cout << "please input number of interviewee: ";
    cin >> number_interviewee;
    fout << number_group << endl << number_interviewee << endl;
    uniform_int_distribution<int> distrib1(1,number_group);
        uniform_int_distribution<int> distrib(1,15);
    for (int i = 0; i < number_group; i++){
        for (int j = 0; j < 5; j++){
            if(distrib(gen) < 12)
                fout << 1 << " ";
            else
                fout << 0 << " ";
        }
        fout << endl;        
    }
    for (int i = 0; i < number_interviewee; i++){
        int temp1 = 0;
        int temp2 = 0;
        int temp3 = 0;
        for (int k = 0; k < 5; k++){
            if(distrib(gen) < 14)
                fout << 1 << " ";
            else
                fout << 0 << " ";
        }
        for (int j = 0; j < 3; j++){
            if(j == 0){
                temp1 = distrib1(gen);
                fout << temp1 << " ";
            }
            else if(j == 1){
                temp2 = distrib1(gen);
                while(temp1 == temp2){
                    temp2 = distrib1(gen);
                }
                fout << temp2 << " ";
            }
            else{
                temp3 = distrib1(gen);
                while(temp1 == temp3 || temp2 == temp3){
                    temp3 = distrib1(gen);
                }
                fout << temp3 << " ";
            }
        }
        fout << endl;
    }
    fout.close();
    return 0;
}