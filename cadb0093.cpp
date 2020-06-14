#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

struct Line{
    double start_x, start_y, end_x, end_y;
};

struct Arc{
    double start_x, start_y, end_x, end_y;
    double center_x, center_y;
    double radius;
    bool direction;//CW = 0, CCW = 1;
};

struct Operation{
    string name;
    double expand, notchsize;
    vector<int>commands;//line = 0, arc = 1;
    vector<Line>lines;
    vector<Arc>arcs;
};

struct Answer{
    double max_x = -50;
    double max_y = -50;
    double min_x = 50;
    double min_y = 50;
};

void read_file(char argv[], vector<Operation> &operations){
    ifstream file;
    string str;
    stringstream ss;
    bool flag = false;
    file.open(argv, ifstream::in);
    if(!file.is_open()){
        perror("Can't open the txt file\n");
        exit(1);
    }
    int i = -1;
    while(!file.eof()){
        string str;
        getline(file, str);
        if(str.compare(0,3, "End")==0){
            flag = false;
        }
        if(flag){
            str.replace(str.find(';'), 1, "");
            stringstream ss(str);
            getline(ss, str, ',');
            if(str == "Expand"){
                while(!ss.eof()){
                    getline(ss, str, ',');
                    operations[i].expand = atof(str.c_str());
                }
            }
            else if(str == "NotchSize"){
                while(!ss.eof()){
                    getline(ss, str, ',');
                    operations[i].notchsize = atof(str.c_str());
                }
            }
            else if(str == "Line"){
                Line line;
                getline(ss, str, ',');
                line.start_x = atof(str.c_str());
                getline(ss, str, ',');
                line.start_y = atof(str.c_str());
                getline(ss, str, ',');
                line.end_x = atof(str.c_str());
                getline(ss, str, ',');
                line.end_y = atof(str.c_str());
                operations[i].lines.push_back(line);
                operations[i].commands.push_back(0);
            }
            else if(str == "Arc"){
                Arc arc;
                getline(ss, str, ',');
                arc.start_x = atof(str.c_str());
                getline(ss, str, ',');
                arc.start_y = atof(str.c_str());
                getline(ss, str, ',');
                arc.end_x = atof(str.c_str());
                getline(ss, str, ',');
                arc.end_y = atof(str.c_str());
                getline(ss, str, ',');
                arc.center_x = atof(str.c_str());
                getline(ss, str, ',');
                arc.center_y = atof(str.c_str());
                arc.radius = sqrt(pow(arc.start_x-arc.center_x,2)+pow(arc.start_y-arc.center_y,2));
                getline(ss, str, ',');
                if(str.compare(0,2,"CW")==0)
                    arc.direction = false;
                else if(str.compare(0,3,"CCW")==0)
                    arc.direction = true;
                operations[i].arcs.push_back(arc);
                operations[i].commands.push_back(1);
            }
        }
        if(str.compare(0,4, "Data")==0){
            flag = true;
            str = str.substr(5, str.length()-7);
            Operation temp;
            temp.name = str;
            operations.push_back(temp);
            i++;
        }
    }
}

void show_operation(const vector<Operation> &operations){
    for(int j = 0 ; j<operations.size() ; j++){
        cout<<operations[j].name<<endl;
        cout<<operations[j].expand<<endl;
        cout<<operations[j].notchsize<<endl;
        int l=0, a=0;
        for(int i=0 ; i<operations[j].commands.size() ; i++){
            if(operations[j].commands[i]==0){
                cout<<operations[j].lines[l].start_x<<" ";
                cout<<operations[j].lines[l].start_y<<" ";
                cout<<operations[j].lines[l].end_x<<" ";
                cout<<operations[j].lines[l].end_y<<"\n";
                l++;
            }
            else{
                cout<<operations[j].arcs[a].start_x<<" ";
                cout<<operations[j].arcs[a].start_y<<" ";
                cout<<operations[j].arcs[a].end_x<<" ";
                cout<<operations[j].arcs[a].end_y<<" ";
                cout<<operations[j].arcs[a].center_x<<" ";
                cout<<operations[j].arcs[a].center_y<<" ";
                cout<<operations[j].arcs[a].radius<<" ";
                cout<<operations[j].arcs[a].direction<<"\n";
                a++;
            }
        }
    }
}

void csv_operation(const vector<Operation> &operations){
    ofstream line, arc;
    //cout<<operations.size()<<endl;
    for(int j = 0 ; j<operations.size() ; j++){
        line.open(operations[j].name+"_line.csv");
        line<<"start_x,start_y,end_x,end_y\n";
        arc.open(operations[j].name+"_arc.csv");
        arc<<"start_x,start_y,end_x,end_y,center_x,center_y,radius,direction\n";
        int l=0, a=0;
        for(int i=0 ; i<operations[j].commands.size() ; i++){
            if(operations[j].commands[i]==0){
                line<<operations[j].lines[l].start_x<<",";
                line<<operations[j].lines[l].start_y<<",";
                line<<operations[j].lines[l].end_x<<",";
                line<<operations[j].lines[l].end_y<<"\n";
                l++;
            }
            else{
                arc<<operations[j].arcs[a].start_x<<",";
                arc<<operations[j].arcs[a].start_y<<",";
                arc<<operations[j].arcs[a].end_x<<",";
                arc<<operations[j].arcs[a].end_y<<",";
                arc<<operations[j].arcs[a].center_x<<",";
                arc<<operations[j].arcs[a].center_y<<",";
                arc<<operations[j].arcs[a].radius<<",";
                arc<<operations[j].arcs[a].direction<<"\n";
                a++;
            }
        }
        line.close();
        arc.close();
    }
}

Answer find_contour(Operation operations){
    Answer answer;
    int l=0, a= 0;
    for(int i=0 ; i<operations.commands.size() ; i++){
        if(operations.commands[i] == 0){
            if(operations.lines[l].start_x>answer.max_x){
                answer.max_x = operations.lines[l].start_x;
            }
            if(operations.lines[l].end_x>answer.max_x){
                answer.max_x = operations.lines[l].start_x;
            }
            if(operations.lines[l].start_y>answer.max_y){
                answer.max_y = operations.lines[l].start_y;
            }
            if(operations.lines[l].end_y>answer.max_y){
                answer.max_y = operations.lines[l].start_y;
            }
            if(operations.lines[l].start_x<answer.min_x){
                answer.min_x = operations.lines[l].start_x;
            }
            if(operations.lines[l].end_x<answer.min_x){
                answer.min_x = operations.lines[l].start_x;
            }
            if(operations.lines[l].start_y<answer.min_y){
                answer.min_y = operations.lines[l].start_y;
            }
            if(operations.lines[l].end_y<answer.min_y){
                answer.min_y = operations.lines[l].start_y;
            }
            l++;
        }
        else{
            double circle_max_x = operations.arcs[a].center_x + operations.arcs[a].radius;
            double circle_min_x = operations.arcs[a].center_x - operations.arcs[a].radius;
            double circle_max_y = operations.arcs[a].center_y + operations.arcs[a].radius;
            double circle_min_y = operations.arcs[a].center_y - operations.arcs[a].radius;
            if(circle_max_x>answer.max_x){
                answer.max_x = circle_max_x;
            }
            if(circle_min_x<answer.min_x){
                answer.min_x = circle_min_x;
            }
            if(circle_max_y>answer.max_y){
                answer.max_y = circle_max_y;
            }
            if(circle_min_y<answer.min_y){
                answer.min_y = circle_min_y;
            }
            a++;
        }
    }
    cout<<answer.max_x<<" "<<answer.max_y<<" "<<answer.min_x<<" "<<answer.min_y<<endl;
    return answer;
}

int main(int argc, char *argv[]){
    vector<Operation> operations;
    read_file(argv[1], operations);
    //csv_operation(operations);
    //show_operation(operations);
    vector<Answer> answers;
    for(int i=0 ; i<operations.size() ; i++){
        Answer temp;
        temp = find_contour(operations[i]);
        temp.max_x += operations[i].expand;
        temp.max_y += operations[i].expand;
        temp.min_x += operations[i].expand;
        temp.min_y += operations[i].expand;
        answers.push_back(temp);
    }
    ofstream output;
    output.open("output.txt");
    output<<"Operation";
    for(int i=0 ; i<operations.size() ; i++){
        output<<","<<operations[i].name;
    }
    output<<";\n\n";
    for(int i=0 ; i<answers.size() ; i++){
        output<<"Data,"<<operations[i].name<<";\n";
        output<<"Line,"<<answers[i].max_x<<","<<answers[i].max_y<<","<<answers[i].max_x<<","<<answers[i].min_y<<";\n";
        output<<"Line,"<<answers[i].max_x<<","<<answers[i].min_y<<","<<answers[i].min_x<<","<<answers[i].min_y<<";\n";
        output<<"Line,"<<answers[i].min_x<<","<<answers[i].min_y<<","<<answers[i].min_x<<","<<answers[i].max_y<<";\n";
        output<<"Line,"<<answers[i].min_x<<","<<answers[i].max_y<<","<<answers[i].max_x<<","<<answers[i].max_y<<";\n";
        output<<"End,"<<operations[i].name<<";\n\n";
    }
    return 0;
}