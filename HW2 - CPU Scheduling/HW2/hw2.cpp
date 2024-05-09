#include <iostream>
#include <vector>
#include <queue>
#define N_MAX 5
using namespace std;
int n, m;
int total_bust = 0;
int total_wait = 0;
int current_time = 0; 
vector<int> queue_mode;
vector<int> time_quantum;

struct content{
    int id;
    int arrival;
    int bust;
};

vector<vector<int> > process;

bool mode_0(vector<int> temp_1, vector<int> temp_2) {
    return temp_1[4] > temp_2[4]; 
}

bool mode_1(vector<int> temp_1, vector<int> temp_2){
   if(temp_1[5] == temp_2[5]){ 
        return temp_1[1] > temp_2[1];
    }
    return temp_1[5] > temp_2[5];  
}

bool mode_2(vector<int> temp_1, vector<int> temp_2){
    if(temp_1[4] == temp_2[4]){ 
        return temp_1[1] < temp_2[1]; 
    }
    return temp_1[4] > temp_2[4]; 
}

bool (*which(int i))(vector<int>, vector<int>){
    bool (*comparator)(vector<int>, vector<int>) = nullptr;
    if(queue_mode[i] == 0)
        comparator = mode_0;
    else if(queue_mode[i] == 1)
        comparator = mode_1;
    else
        comparator = mode_2;
    return comparator;
}

void update_context(vector<int>& context, int processed){
    context[3] += processed;
    context[4] = current_time;
    context[5] -= processed;
}

void output(vector<int> wait){
    // cout << "--OUTPUT--\n";
    for(int i=0; i<m; i++){
        cout << wait[i] << " " << wait[i] + process[i][1] << endl;
        total_wait += wait[i];
    }
    int total_turn = total_wait + total_bust;
    cout << total_wait << endl << total_turn << endl;
}

void start_program(vector<vector<int> > process){
    int num = 0;
    int current_time_quantum = -1;
    vector<int> wait(m, 0);
    vector<priority_queue<vector<int>, vector<vector<int> >, bool (*)(vector<int>, vector<int>)> > calculate_vector;

    for (int i=0; i<m; i++){
        bool (*comparator)(vector<int>, vector<int>) = which(i);
        priority_queue<vector<int>, vector<vector<int> >, bool (*)(vector<int>, vector<int>)> pq(comparator);
        calculate_vector.push_back(pq);
    }

    while(1){
        loop:
        vector<int> context;
        int current_num;
        int mode = -1;
        int processed;

        int which_arrive = 0;
        int temp_size = int(process.size());
        // check new process to push in queue
        int i = 0;
        while(i<temp_size){
            int arrive = process[i][0];
            int bust = process[i][1];
            if(process[i][0] <= current_time){ 
                which_arrive++;
                vector<int> add;
                add.emplace_back(num);
                add.emplace_back(arrive);
                add.emplace_back(bust);
                add.emplace_back(0);
                add.emplace_back(arrive);
                add.emplace_back(bust);
                calculate_vector[0].push(add); 
                num++;
            }
            i++;
        }
        for(int i=0; i<which_arrive; i++){
            process.erase(process.begin());
        }
        
        // find the highest priority, non-empty queue
        temp_size = int(calculate_vector.size());
        i = 0;
        while(i<temp_size){
            if(!calculate_vector[i].empty()){
                context = calculate_vector[i].top();
                calculate_vector[i].pop();
                mode = queue_mode[i];
                if(mode == 2)
                    current_time_quantum = time_quantum[i];
                current_num = i+1;
                break;
            }
            if(i == temp_size-1){
                if(process.empty())
                    goto print;
                else{
                    current_time++;
                    goto loop;
                }
            }
            i++;
        }

        processed = context[2] - context[3]; 
        if((mode == 2) && (processed > current_time_quantum)){
            processed = current_time_quantum;
        }

        temp_size = int(process.size());
        if(((mode == 1) && (current_num == 1)) || current_num != 1){
            i = 0;
            while(i<temp_size){
                if(process[i][0] < current_time + processed){ 
                    if((current_num == 1) && (process[i][0] + process[i][1] >= current_time + processed))
                        continue;
                    processed = process[i][0] - current_time; 
                    break;
                }
                i++;
            }
        }
        wait[context[0]] += current_time - context[4]; 
        current_time += processed;
        update_context(context, processed);
        temp_size = int(calculate_vector.size());
        if(context[2] != context[3]){ 
            if (current_num < temp_size)
                calculate_vector[current_num].push(context); 
            else
                calculate_vector[current_num - 1].push(context);
        }
    }
    print:
    output(wait);
}

int main(void){

    cin >> n >> m;

    queue_mode.resize(n);
    time_quantum.resize(n);
    process.resize(m);

    for(int i=0; i<n; i++){
        cin >> queue_mode[i] >> time_quantum[i];
    }
    for(int i=0; i<m; i++){
        process[i].resize(2);
        cin >> process[i][0] >> process[i][1];
        total_bust += process[i][1];
    }
    
    start_program(process);

    return 0;
}