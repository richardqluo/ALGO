
#include <unordered_set>
#include <unordered_map>

#include "Application.h"

class NewsTracker
{
public:

    NewsTracker();

    // Incoming data
    void OnDirectData(const DirectData& data);
    void OnRepublishData(const RepublishData& data);

    /*** Queries ***/

    // return all topics and sources used by this application
    std::vector<TopicSource> GetCurrentDataUsage(int appId);

    // return all sources of data for this topic
    std::vector<int> GetCurrentSources(int topicId);

    // return all topics published by this source
    std::vector<int> GetCurrentTopics(int sourceId);

    // return all applications subscribed to this topic
    std::vector<int> GetTopicUsage(int topicId);

    // return the length of the longest publisher chain
    int GetLongestRepublishChain();

    // return the topic with the most subscribers
    int GetMostSubscribedTopic();

    // returns the number of hops from the source to this application for this topic
    int DistanceFromSource(int appId, int topicId);

    // return applications with no subcribers for this topic
    std::vector<int> FindUnusedPublishers(int topicId);

private:
    std::unordered_map<int, std::unordered_set<int>> sources;
    std::unordered_map<int, std::shared_ptr<Application>> applications;

    int maxDepths(const std::shared_ptr<Application>& application, int topicId);
};


#include <memory>
#include "Messages.h"

struct Application {
public:
    Application(int id) : appId(id){};
    int appId;
    std::vector<TopicSource> subSources;
    std::vector<std::pair<int, std::shared_ptr<Application>>> pubApplications;
    //only for upward traverse
    //std::vector<std::pair<int, std::shared_ptr<Application>>> subApplications;
};

