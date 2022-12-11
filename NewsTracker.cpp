
#include <queue>
#include <stack>
#include "NewsTracker.h"

NewsTracker::NewsTracker() {
}

void NewsTracker::OnDirectData(const DirectData& data){
    for(TopicSource it : data.ts){
        std::unordered_map<int, std::unordered_set<int>>::iterator mit = sources.find(it.sourceId);
        if(mit != sources.end()){
            mit->second.insert(it.topicId);
        }else{
            std::unordered_set<int> topics;
            topics.insert(it.topicId);
            sources[it.sourceId] = topics;
        }
        auto itm = applications.find(data.appId);
        if(itm != applications.end()){
            //data.appId is subscriber, it.sourceId is publisher
            itm->second->subSources.push_back(it);
        }else{
            auto sp = std::make_shared<Application>(int(data.appId));
            //std::shared_ptr<Application> sp(new Application(data.appId));
            sp->subSources.push_back(it);
            applications.emplace(int(data.appId), sp);
        }
    }
}

void NewsTracker::OnRepublishData(const RepublishData& data){
    auto mit = applications.find(data.appId);
    if(mit == applications.end()){
        auto sp = std::make_shared<Application>(int(data.appId));
        applications.emplace(int(data.appId), sp);
    }
    std::shared_ptr<Application> application = applications.at(data.appId);
    for(TopicApplication it : data.ta){
        auto itm = applications.find(it.appId);
        if(itm != applications.end()){
            //only for upward traverse: data.appId is publisher, it.appId is subscriber
            //itm->second->subApplications.push_back(std::make_pair(it.topicId, application));
        }else{
            auto sp = std::make_shared<Application>(int(it.appId));
            applications.emplace(int(it.appId), sp);
        }
        std::shared_ptr<Application> app = applications.at(it.appId);
        application->pubApplications.push_back(std::make_pair(it.topicId, app));
    }
}

std::vector<TopicSource> NewsTracker::GetCurrentDataUsage(int appId){
    //for brevity here assume this appId already exists, otherwise use find instead to avoid exception
    std::shared_ptr<Application> app = applications.at(appId);
    return app->subSources;
}

std::vector<int> NewsTracker::GetCurrentSources(int topicId){
    std::vector<int> src;
    for(auto it : sources){
        std::unordered_set<int> topics = it.second;
        if(topics.find(topicId)!=topics.end()){
            src.push_back(it.first);
        }
    }
    return src;
}

std::vector<int> NewsTracker::GetCurrentTopics(int sourceId){
    std::vector<int> tpc;
    //for brevity here assume this sourceId already exists
    std::unordered_set<int> topics = sources.at(sourceId);
    for(int it : topics){
        tpc.push_back(it);
    }
    return tpc;
}

std::vector<int> NewsTracker::GetTopicUsage(int topicId){
    //Breath First Search on each topic from root
    std::vector<int> subApp;
    std::queue<std::shared_ptr<Application>> appQueue;
    for(auto it : applications){
        std::shared_ptr<Application> app = it.second;
        for(TopicSource ts : app->subSources){
            if(ts.topicId == topicId){
                appQueue.push(app);
            }
        }
    }
    while(!appQueue.empty()){
        std::shared_ptr<Application> application = appQueue.front();
        appQueue.pop();
        subApp.push_back(application->appId);
        for (auto ta : application->pubApplications){
            if(ta.first == topicId){
                appQueue.push(ta.second);
            }
        }
    }
    return subApp;
}

int NewsTracker::GetLongestRepublishChain(){
    int maxDist = 0;
    for(auto it : applications){
        std::shared_ptr<Application> app = it.second;
        for(TopicSource ts : app->subSources){
            //recursive traverse
            int dist = maxDepths(app, ts.topicId);
            if (dist > maxDist){
                maxDist = dist;
            }
        }
    }
    return maxDist;
}

int NewsTracker::GetMostSubscribedTopic(){
    int maxSub = 0;
    int maxTopic = 0;
    std::unordered_map<int, int> subCount;
    for(auto it : applications){
        std::shared_ptr<Application> app = it.second;
        for(TopicSource ts : app->subSources){
            //since subscriber never listen to the same topic from multiple publishers
            //max depth is the count of subscribers
            int count = maxDepths(app, ts.topicId);
            //the same topic maybe published by multiple sources
            subCount[ts.topicId] += count;
            if(subCount[ts.topicId] > maxSub){
                maxSub = subCount[ts.topicId];
                maxTopic = ts.topicId;
            }
        }
    }
    return maxTopic;
}

int NewsTracker::DistanceFromSource(int appId, int topicId){
    int dist = 0;
    //stack application with its distance to the source
    std::stack<std::pair<std::shared_ptr<Application>,int>> appStack;
    for(auto it : applications){
        std::shared_ptr<Application> app = it.second;
        for(TopicSource ts : app->subSources){
            if(ts.topicId == topicId){
                appStack.push(std::make_pair(app,1));
            }
        }
    }
    //Depth First Search on each topic to the application
    while(!appStack.empty()){
        std::pair<std::shared_ptr<Application>,int> appDist = appStack.top();
        std::shared_ptr<Application> application = appDist.first;
        dist = appDist.second;
        appStack.pop();
        /*
        if(application->appId == appId){
            return dist;
        }
        */
        for (auto ta : application->pubApplications){
            if(ta.second->appId == appId){
                //return as soon as the appId found by publisher, earlier than above commented out
                return dist+1;
            }
            if(ta.first == topicId){
                dist += 1;
                appStack.push(std::make_pair(ta.second, dist));
            }
        }
    }
    return dist;
}

/*upward traverse version that requires subscriber to keep TopicApplication to publisher, faster but use more memory
int NewsTracker::DistanceFromSource2(int appId, int topicId){
    int dist = 0;
    //for brevity here assume this appId already exists
    std::shared_ptr<Application> application = applications.at(appId);
    while(true) {
        for (TopicSource ts : application->subSources) {
            if (ts.topicId == topicId) {
                return dist;
            }
        }
        bool found = false;
        for (auto ta : application->subApplications) {
            if (ta.first == topicId) {
                application = ta.second;
                dist+=1;
                found = true;
                break;
            }
        }
        if (!found){
            return 0;
        }
    }
}
*/

std::vector<int> NewsTracker::FindUnusedPublishers(int topicId){
    std::vector<int> publishers;
    std::stack<std::shared_ptr<Application>> appStack;
    for(auto it : applications){
        std::shared_ptr<Application> app = it.second;
        for(TopicSource ts : app->subSources){
            if(ts.topicId == topicId){
                appStack.push(app);
            }
        }
    }
    //Depth First Search on each topic to leaves
    while(!appStack.empty()){
        std::shared_ptr<Application> application = appStack.top();
        appStack.pop();
        bool found = false;
        for (auto ta : application->pubApplications){
            if(ta.first == topicId){
                found = true;
                appStack.push(ta.second);
            }
        }
        if(!found){
            publishers.push_back(application->appId);
        }
    }
    return publishers;
}

int NewsTracker::maxDepths(const std::shared_ptr<Application>& application, int topicId){
    int curMax=0;
    bool found = false;
    for(auto ta : application->pubApplications){
        if (ta.first == topicId) {
            found = true;
            int curDepth = maxDepths(ta.second, topicId);
            if (curDepth > curMax) {
                curMax = curDepth;
            }
        }
    }
    if (found) {
        return curMax + 1;
    }else{
        return 1;
    }
}
