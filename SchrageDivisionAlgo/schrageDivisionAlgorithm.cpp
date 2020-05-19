#include <iostream>
#include <fstream>
#include <queue>
#include <chrono> 

struct Task
{
    unsigned int r;
    unsigned int p; 
    unsigned int q;
};

auto comparisonR = [](Task &task1, Task &task2){return task1.r > task2.r;};
auto comprasionQ = [](Task &task1, Task &task2){return task1.q < task2.q;};

void getDataFromFile
(
    const std::string &fileName,
    unsigned int &numberOfTasks,
    std::priority_queue<Task, std::vector<Task>, decltype(comparisonR)> &N,
    std::priority_queue<Task, std::vector<Task>, decltype(comprasionQ)> &G
)
{
    std::fstream file;
    file.open(fileName);
    if(file.good())
    {
        file >> numberOfTasks;
        Task task;
        for(int i=0; i<numberOfTasks; ++i)
        {
            file >> task.r;
            file >> task.p;
            file >> task.q;
            N.push(task);
        }
        file.close();
    }
    else
    {
        std::cerr << "ERROR - CAN NOT OPEN FILE!" << std::endl;
    }
}

unsigned int schrageAlgorithm
(
    std::priority_queue<Task, std::vector<Task>, decltype(comparisonR)> &N,
    std::priority_queue<Task, std::vector<Task>, decltype(comprasionQ)> &G
)
{
    unsigned int t = 0;
    unsigned int cmax = 0;
    Task taskE;
    Task taskL;
    taskL.q = 1000000;

    while (!G.empty() || !N.empty())
    {    
        while (!N.empty() && N.top().r<=t)
        {
            taskE = N.top();
            G.push(taskE);
            N.pop();
            if(taskE.q > taskL.q)
            {
                taskL.p = t - taskE.r;
                t = taskE.r;
                if(taskL.p > 0)
                    G.push(taskL);
            }
        }

        if(G.empty())
        {
            t = N.top().r;
        }
        else
        {
            taskE = G.top();
            G.pop();
            taskL = taskE;
            t += taskE.p;
            cmax = std::max(cmax, t + taskE.q);
        }
    }
    return cmax;
}

void performAlgorithm
(
    const std::string &dataSetName,
    unsigned int &numberOfTasks,
    std::priority_queue<Task, std::vector<Task>, decltype(comparisonR)> &N,
    std::priority_queue<Task, std::vector<Task>, decltype(comprasionQ)> &G
)
{
    std::string fileName;
    unsigned int counter;
    unsigned int cmax;

    if(dataSetName == "DATA")
        counter = 6;
    else
        counter = 9;

    for(int i=1; i<=counter; ++i)
    {
        fileName = "Data/" + dataSetName + std::to_string(i) + ".DAT";
        getDataFromFile(fileName, numberOfTasks, N, G);
        auto start = std::chrono::high_resolution_clock::now();
        cmax = schrageAlgorithm(N,G);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Cmax: " << cmax << "\t Time duration: " << duration.count() << std::endl;
    }
}

int main()
{
    std::string fileName;
    unsigned int numberOfTasks;
    std::priority_queue<Task, std::vector<Task>, decltype(comparisonR)> N(comparisonR);
    std::priority_queue<Task, std::vector<Task>, decltype(comprasionQ)> G(comprasionQ);

    std::cout << "Dataset 1: " << std::endl;
    performAlgorithm("SCHRAGE", numberOfTasks, N, G);

    std::cout << "Dataset 2: " << std::endl;
    performAlgorithm("DATA", numberOfTasks, N, G);
}
