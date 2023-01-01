#include <iostream>
#include <pthread.h>
#include "headers.h"
#include <vector>
#include <string> 
#include <sstream>   
#include <fstream>
#include <queue>
#include <memory>



std::vector<std::string> split(std::string str, char del){
    std::vector<std::string> res;
    std::string temp = "";

    for(int i=0; i<(int)str.size(); i++)
    {
        if(str[i] != del){
            temp += str[i];
            if(i == (int)str.size() - 1){
                res.emplace_back(temp);
            }
        }
        else{
            res.emplace_back(temp);
            temp = "";
        }
    }
    return res;
}

int main(int argc, char* argv[]){

    companies.emplace_back(std::shared_ptr<Company>(std::make_shared<Company>("Kevin")));
    companies.emplace_back(std::shared_ptr<Company>(std::make_shared<Company>("Bob")));
    companies.emplace_back(std::shared_ptr<Company>(std::make_shared<Company>("Stuart")));
    companies.emplace_back(std::shared_ptr<Company>(std::make_shared<Company>("Otto")));
    companies.emplace_back(std::shared_ptr<Company>(std::make_shared<Company>("Dave")));

    std::string filepath = argv[1];

    std::fstream inputFile;

    inputFile.open(filepath, std::ios::in);
    if(inputFile.is_open()){
        std::string tp;
        while(getline(inputFile, tp)){
            std::vector<std::string> splitted_vector = split(tp, ',');
            inputs.emplace_back(splitted_vector);
        }
    }   

    for(int i = 0 ; i < 10; i++){
        VendingMachine* mach = new VendingMachine();
        mach->create();
        machines.emplace_back(mach);
    }

    int customer_size;

    std::istringstream(inputs.at(0).at(0)) >> customer_size;


    for(int i = 1 ; i < customer_size + 1 ; i++){
        Customer* customer = new Customer();
        customer->create(std::stoi(inputs.at(i).at(0)), i);
        customers.emplace_back(customer);
    }

    while(payed_customer_count != customer_size ){

    }    
    std::vector<std::string> input_folder_path = split(argv[1], '/');
    std::ofstream output("output_of_" + input_folder_path.at(input_folder_path.size() - 1));
    for(auto machine : machines){
        machine->stop();
        for(auto outputstr : machine->getOutput()){
            output << outputstr + "\n"; 
        }
    }
    
    //Create output.
    output << "All payments completed.\n";
    for(auto company : companies)
    {
        output << company->getName() +":" + std::to_string(company->getTotal()) + "\n";
    }

    output.close();
    return 0;
}