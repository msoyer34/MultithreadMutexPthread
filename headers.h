#include <iostream>
#include "pthread.h"
#include <unistd.h>
#include <vector>
#include <queue>
#include <memory>


typedef void * (*THREADFUNCPTR)(void *);
static int payed_customer_count;

class ICommand{
    public:
        virtual void *execute(void * args) = 0;
};

class IRunnableVending{
    public:
        virtual void create() = 0;
        pthread_t thread_id;

};

class IRunnableCustomer{
    public:
        virtual void create(int sleep_time, int customer_id) = 0;
        pthread_t thread_id;

};

//Company class
class Company {
    public: 
        Company(std::string name){
            company_name_ = name;
        }
        
        ~Company() = default;
        //Get Payment
        void payMe(int payment_val){
            pthread_mutex_lock(&mutex);
            total_ += payment_val;
            pthread_mutex_unlock(&mutex);
        }
        std::string getName(){
            return company_name_;
        }
        int getTotal(){
            return total_;
        }
    private:
        std::string company_name_;
        int total_{0};            
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        std::queue<int> payments_;
};

static std::vector<std::shared_ptr<Company>> companies;

class VendingMachine : public ICommand, public IRunnableVending{
    public:
        void * execute(void * args) override{
            is_initialized = true;
            while(is_initialized){
                if(queue_customer_.size()){
                   pthread_mutex_lock(&mutex);
                   auto pair = queue_customer_.front();
                   for(auto company : companies){
                        if(company->getName() == pair.second){
                            company->payMe(pair.first);
                            payed_customer_count++;
                            break;
                        } 
                   }
                   queue_customer_.pop();
                   pthread_mutex_unlock(&mutex);
                }
            }
            return NULL;
        }

        void create() override
        {
            pthread_create(&thread_id, NULL, (THREADFUNCPTR)&VendingMachine::execute,this);
        }
        void stop(){
            is_initialized = false;
        }
        void appendCustomerPayment(int payment, std::string company_name, int id){
            //lock while appending a customer payment to queue and lock.
            pthread_mutex_lock(&mutex);
            queue_customer_.emplace(std::make_pair(payment, company_name));
            output_.emplace_back("Customer" + std::to_string(id) +"," + std::to_string(payment)+ "," + company_name);
            pthread_mutex_unlock(&mutex);
        }
        std::vector<std::string> getOutput(){
            return output_;
        }
        private:
            pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
            std::queue<std::pair<int, std::string>> queue_customer_;
            bool is_initialized;
            std::vector<std::string> output_;
};

static std::vector<VendingMachine *> machines; 

static std::vector<std::vector<std::string>> inputs; 

pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
class Customer : public ICommand, public IRunnableCustomer{
    public:
        void * execute(void * args) override
        {
            sleep(sleep_time_);
            pthread_mutex_lock(&mutex);
            machines.at(std::stoi(inputs.at(customer_id_).at(1)) - 1)->appendCustomerPayment(std::stoi(inputs.at(customer_id_).at(3)), inputs.at(customer_id_).at(2), customer_id_);
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        void create(int sleep_time, int customer_id) override
        {
            customer_id_ = customer_id;
            pthread_create(&thread_id, NULL, (THREADFUNCPTR)&Customer::execute,this);
        }
        private:
            int sleep_time_;
            pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
            int customer_id_;
    
};

static std::vector<Customer *> customers; 

