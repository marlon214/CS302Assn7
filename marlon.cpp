#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

template <class t1, class t2>
class priorityQ
{
    public:
        priorityQ ();
        void push_back(const t1&, const t2&);
        void pop_front ();
        void increaseKey(const t1&, const t2&);
        t2 get_front_priority () const;
        t1 get_front_key () const;
        bool isEmpty () const;
    private:
        struct priorityType
        {
            t1 key;
            t2 priority;
        };

    void bubbleUp(std:: size_t );
    void bubbleDown(std:: size_t );
    std::vector <priorityType > heapArray;
    std:: unordered_map <t1 , std::size_t > itemToPQ;
};

template <class t1, class t2>
priorityQ<t1, t2>::priorityQ(){
    //INITIALIZE EMPTY ARRAY
    heapArray.resize(1); 
}
//maps key to the last index of the heap and bubblesUp from that after adding new elements 
template <class t1, class t2>
void priorityQ<t1, t2>::push_back(const t1& key, const t2& value){
    //PUSH BACK KEY VALUE IN ARRAY AND 
    heapArray.push_back({key, value}); 
    itemToPQ[key] = heapArray.size() - 1; 
    bubbleUp(heapArray.size() - 1); 
}
//assigne last elment as root of array and update the map using index
//delete the root before replacing the root 
template<class t1, class t2>
void priorityQ<t1, t2>::pop_front()
{   
    //to get the last element in the vector 
    size_t lastElement = heapArray.size()-1; 
    //if empty do nothing
    if (isEmpty()) 
        return;
    itemToPQ.erase(heapArray[1].key); 
    heapArray[1] = heapArray.back(); 
    itemToPQ[heapArray[1].key] = 1; 
    heapArray.pop_back(); 
    bubbleDown(1); //bubbling down the new root element
}
//find index by calling map[key] and increase priority, then bubbleup said index 
template<class t1, class t2>
void priorityQ<t1, t2>::increaseKey(const t1& key, const t2& value)
{
    size_t index = itemToPQ[key]; 
    heapArray[index].priority += value; 
    bubbleUp(index);
}

//the priority field of the root element
template<class t1 , class t2>
t2 priorityQ<t1, t2>::get_front_priority() const
{
    return heapArray[1].priority; 
}

//key field of the root element
template<class t1, class t2> 
t1 priorityQ<t1, t2>::get_front_key() const
{
    return heapArray[1].key; 
}

//true if there is nothing in the priority queue and false otherwise
template<class t1 , class t2>
bool priorityQ<t1, t2>::isEmpty() const
{
    return heapArray.empty(); 
}

template<class t1 , class t2>
void priorityQ<t1, t2>::bubbleUp(std::size_t index)
{
    while(index > 1)
    {
        size_t parentI = index/2; 
        if(heapArray[parentI].priority < heapArray[index].priority)
        {
            //swap the location using the index
            swap(heapArray[parentI], heapArray[index]);
            
            //update the mapping at the key as well
            swap(itemToPQ[heapArray[parentI].key], itemToPQ[heapArray[index].key]);

            //update index 
            index = parentI;  
        }
        else
            //no more to bubble up so break
            break; 
    }
}

template <class t1, class t2> 
void priorityQ<t1, t2>::bubbleDown(std::size_t index)
{
    //only when one element is present 
    while (2 * index < heapArray.size()) {
        //the left child as the greatest 
        size_t greatest = 2 * index;
        //if the right child is greater, set the right and update to give it the highest priority
        if (greatest + 1 < heapArray.size() && heapArray[greatest + 1].priority > heapArray[greatest].priority) {
            greatest++;
        }
        //if curr index is larger, then break 
        if (heapArray[index].priority > heapArray[greatest].priority) 
        {
            break; 
        }
        //else swap curr with the greatest and update the key val as well
        swap(heapArray[index], heapArray[greatest]);
        swap(itemToPQ[heapArray[index].key], itemToPQ[heapArray[greatest].key]);
        //reset the index as the curr large val, to compare with new vall added
        index = greatest;
    }
}

/******************/
struct mercType {
    int zombies;
    int spiders;
    int drainDeimos;
    int zombieDogs;
    int hunters;
    int nemesis;
    int civilians;

    // Constructor to initialize all stats to 0
    mercType()
    {
        zombies=0;
        spiders=0;  
        drainDeimos=0;
        zombieDogs=0;  
        hunters=0; 
        nemesis=0;
        civilians=0; 
    } 

    // > operator overload to comp scores
    bool operator>(const mercType& rhs) const {
        // Calculate scores for both mercenaries
        double leftScore = zombies * 1.0 + spiders * 1.1 + drainDeimos * 1.2 + zombieDogs * 1.3 + hunters * 1.4 + nemesis * 1.5 + civilians * 2.0;
        double rightScore = rhs.zombies * 1.0 + rhs.spiders * 1.1 + rhs.drainDeimos * 1.2 + rhs.zombieDogs * 1.3 + rhs.hunters * 1.4 + rhs.nemesis * 1.5 + rhs.civilians * 2.0;
        
        //true if leftScore is greater than right, else false 
        if(leftScore > rightScore)
        {
            return true; 
        }
        return false;
    }

    //operator overload for += to return object *this for each update stats of monsters based on simulation
    mercType& operator+=(const mercType& rhs) {
        zombies += rhs.zombies;
        spiders += rhs.spiders;
        drainDeimos += rhs.drainDeimos;
        zombieDogs += rhs.zombieDogs;
        hunters += rhs.hunters;
        nemesis += rhs.nemesis;
        civilians += rhs.civilians;
        return *this;
    }

    // Calculate and return the total score
    double score() const {
        double result = zombies * 1.0 + spiders * 1.1 + drainDeimos * 1.2 + zombieDogs * 1.3 + hunters * 1.4 + nemesis * 1.5 + civilians * 2.0;
        return result;
    }
};

//make a queue to rank and push back name and score to un map
void rankingQ(priorityQ<string, double>& ranking, unordered_map<string, mercType>& Data)
{
    for(auto it = Data.begin(); it != Data.end(); it++)
    {
        ranking.push_back(it->first, it->second.score());
    }
}
//print the top names from queue and delete after printing
void printRanking( priorityQ<string, double>& ranking)
{
    //count rank 
    int rank = 1; 
    //print the ranking and pop from queue ones name is printed
    string name = ""; 
    while (!ranking.isEmpty() && rank != 15){
        //call get front function to get top name and key 
        name = ranking.get_front_key();
        double score = ranking.get_front_priority();  
        cout << "\nRank " << rank << '\n';
        cout << "Name: " << name << " Score: " << score << "\n";
        rank++;
        ranking.pop_front();
    }
}
int main() {
    ifstream mercenaries;
    ifstream simmulation;
    string mFile;
    string sFile;
    cout << "Enter mercenaries file: ";
    cin >> mFile;
    cout << "\nEnter simulation file: ";
    cin >> sFile;
    cout<<endl; 
    // Open both files
    mercenaries.open(mFile);
    simmulation.open(sFile); 
    unordered_map<string, mercType> Data;

    string name;
    mercType monster;
    // Read the mercenaries file and store the data in Data
    while (mercenaries >> name >> monster.zombies >> monster.spiders >> monster.drainDeimos >> monster.zombieDogs >> monster.hunters >> monster.nemesis >> monster.civilians){
        Data[name] = monster;
    }

    double topScore = 0.0;
    string leaderName = "";
    string pName = ""; 
    string monType="";
    int count=0; 
    //get sim file and load name, then check for monster and update map 
    while (simmulation >> name >>monType >>count) 
    {
        //simmulation >> monType >> count;
        if (monType == "zombies")
        {
            Data[name].zombies += count;
        } 
        else if(monType == "spiders") 
        {
            Data[name].spiders += count;
        } 
        else if(monType == "drainDeimos") 
        {
            Data[name].drainDeimos += count;
        } 
        else if(monType == "zombieDogs") 
        {
            Data[name].zombieDogs += count;
        } 
        else if(monType == "hunters") 
        {
            Data[name].hunters += count;
        } 
        else if(monType == "nemesis") 
        {
            Data[name].nemesis += count;
        } 
        else if(monType == "civilians") 
        {
            Data[name].civilians += count;
        }

        // if curr score is higher, update && if names dont match top name, reset to curr parent name(pName)
        double currScore = Data[name].score(); 
        if (currScore > topScore) {
            topScore = currScore;
            if (leaderName != name)
            {
                cout << "We have a new leader: " << name << " Score: "<< fixed << setprecision(1)<<topScore << "\n";
            }
            pName = leaderName;
            leaderName = name;
        }
    }

    // Create a priority queue to rank the mercenaries by their scores
    priorityQ<string, double> ranking;
    //load ranking into map 
    rankingQ(ranking, Data);
    //print ranking 
    printRanking(ranking);  
    
    return 0;
}
