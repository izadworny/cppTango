//
// Created by ingvord on 10/31/16.
//

#pragma once

#include <thread>

namespace Tango {
    namespace thread {
        class named_thread {
            std::thread thread_;
            std::string name_;
        public:
            template <typename T, typename... Args>
            named_thread(T&& name, Args&&... args):
                    name_{std::forward(name)},
                    thread_{std::forward(std::forward<Args>(args)...)}
            {}
            ~named_thread();
            std::string name();
            const std::thread& as_thread();
        };
    }//thread
}//Tango


