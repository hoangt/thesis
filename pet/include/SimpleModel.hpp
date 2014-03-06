#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <atomic>

#include "PowerModel.hpp"

class SimpleModel : public PowerModel {
    public:
        SimpleModel(
                boost::lockfree::spsc_queue<std::string*> *q,
                std::atomic<bool> *done,
                std::map<unsigned long, std::string> *annotations,
                std::map<std::string, unsigned long> *weights,
                unsigned long bucket_size = 10000000, unsigned long long numTicks = 0L);
        virtual ~SimpleModel();
        virtual int calculate(const SimEvent *inst);
    private:
        boost::mutex m;
};
