#include "PowerModel.hpp"

#include "TraceLine.hpp"

using namespace boost;

#define DELETE_STACK_SIZE 128
PowerModel::PowerModel(lockfree::queue<std::string*> *q, atomic<bool> *done, unsigned long bucket_size) : q(q), done(done), output(), bucket_size(bucket_size) { }

PowerModel::~PowerModel() { };

OutputVector PowerModel::getOutput() const
{
    return output;
}

int inline clean_stack(std::string *delete_stack[DELETE_STACK_SIZE], int ds) {
    for (int i = 0; i < ds; i++)
        delete delete_stack[i];
    return 0;
}

int PowerModel::run()
{
    std::string *s = 0;
    std::string *delete_stack[DELETE_STACK_SIZE];
    int ds = 0;
    while (!(*done) || !q->empty()) {
        while (q->pop(s)) {
            TraceLine tr = TraceLine(*s);
            
            calculate(tr.getSimEvent());
            if (ds < DELETE_STACK_SIZE) {
                delete_stack[ds++] = s;
            } else {
                ds = clean_stack(delete_stack, ds);
                delete s;
            }
        }
        ds = clean_stack(delete_stack, ds);
        this_thread::yield();
    }
    ds = clean_stack(delete_stack, ds);
    std::cout << "Thread " << this_thread::get_id() << std::endl;
    return 0;
}
