//
// Created by ingvord on 11/1/16.
//

#pragma once

#include <queue>
#include <list>
#include <functional>
#include <experimental/optional>


namespace Tango {
    class WorkItem;
    namespace polling {
        bool compare_work_items(WorkItem&,WorkItem&);
        //TODO extract template
        class PollingQueue : public std::priority_queue<WorkItem, std::list<WorkItem>, std::function<bool(WorkItem&, WorkItem&)>> {
        public:
            PollingQueue();

            void erase(std::function<bool(const WorkItem&)>);

            std::experimental::optional<WorkItem>
            remove_if(std::function<bool(const WorkItem&)>);

            void for_each(std::function<void(const WorkItem&)>);

            std::experimental::optional<WorkItem>
            find_if(std::function<bool(const WorkItem & )>);
        };
    }//polling
}//Tango


