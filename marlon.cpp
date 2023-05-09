/*
Name: <Marlon Alejandro>
NSHE ID: <5002573038>
Section: <1002>
Assignment: 7
Description: Resident Evil Priority Queue Leader Board
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <unordered_map>

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
    // intialize as an empty array
    heapArray.resize(1); 
}

template <class t1, class t2>
void priorityQ<t1, t2>::push_back(const t1& key, const t2& value){
    // add element to the end of the array, and update hashmap in accordance to its position to the array position
    heapArray.push_back({key, value}); 
    itemToPQ[key] = heapArray.size() - 1; 
    bubbleUp(heapArray.size() - 1); 
}

template<class t1, class t2>
void priorityQ<t1, t2>::pop_front(){
    // remove higest priority item, which is the first item of our PQ   
    //base case: empty array, no elements to remove
    if (isEmpty()){
        return;
    }
    itemToPQ.erase(heapArray[1].key); 
    heapArray[1] = heapArray[heapArray.size()-1]; 
    itemToPQ[heapArray[1].key] = 1; 
    heapArray.pop_back(); 
    bubbleDown(1); 
}
template<class t1, class t2>
void priorityQ<t1, t2>::increaseKey(const t1& key, const t2& value){
    //increase position in queue
    int index = itemToPQ[key]; 
    heapArray[index].priority += value; 
    bubbleUp(index);
}

template<class t1 , class t2>
t2 priorityQ<t1, t2>::get_front_priority() const{
    return heapArray[1].priority; 
}

template<class t1, class t2> 
t1 priorityQ<t1, t2>::get_front_key() const{
    return heapArray[1].key; 
}

template<class t1 , class t2>
bool priorityQ<t1, t2>::isEmpty() const{
    return heapArray.empty(); 
}

template<class t1 , class t2>
void priorityQ<t1, t2>::bubbleUp(std::size_t index){
    //bubble up item in array in accordance to their priority
    while (index > 1 && heapArray[index/2].priority < heapArray[index].priority) {
            size_t index2=index/2;
            swap(heapArray[index2], heapArray[index]);
            swap(itemToPQ[heapArray[index2].key], itemToPQ[heapArray[index].key]);
            index = index2;
        }
}

template <class t1, class t2> 
void priorityQ<t1, t2>::bubbleDown(std::size_t index){
    while (2 * index < heapArray.size()) {
        size_t index2 = 2 * index;
        //check priority status
        if (index2 + 1 < heapArray.size()) {
                if(heapArray[index2 + 1].priority > heapArray[index2].priority){
                    index2++;
                }
        }
        //parent node has the hiest priority
        if (heapArray[index].priority > heapArray[index2].priority) {
            break; 
        }
        //swap in accordance to their priority and update hash map
        swap(heapArray[index], heapArray[index2]); 
        swap(itemToPQ[heapArray[index].key], itemToPQ[heapArray[index2].key]); 
        index = index2;
    }
}

/**************************************************************************************************/
struct mercType {
    int zombies;
    int spiders;
    int drainDeimos;
    int zombieDogs;
    int hunters;
    int nemesis;
    int civilians;


    bool operator>(const mercType& rhs) const {
        // Calculate and compare scores
        float left = zombies + spiders * 1.1 + drainDeimos * 1.2 + zombieDogs * 1.3 + hunters * 1.4 + 
                        nemesis * 1.5 + civilians * 2.0;
        float right = rhs.zombies + rhs.spiders * 1.1 + rhs.drainDeimos * 1.2 + rhs.zombieDogs * 1.3 + 
                       rhs.hunters * 1.4 + rhs.nemesis * 1.5 + rhs.civilians * 2.0;
        if(left > right){
            return true; 
        }
        return false;
    }

    //add score to exisiting mercType and return updated class
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

    // Calculate score
    float score() {
        float score = zombies + spiders * 1.1 + drainDeimos * 1.2 + zombieDogs * 1.3 + hunters * 1.4 + nemesis * 1.5 + civilians * 2.0;
        return score;
    }
};

/**************************************************************************************************/

void fileOpen(ifstream& mFile, ifstream& sFile);
//make a queue corresponding to the score
void rankQueue(priorityQ<string, double>& rank, unordered_map<string, mercType>& stats);
//print leaderboard
void leaderBoard(priorityQ<string, double>& rank);
// Store mercenary file info into our hashmap
void scoreTracker(mercType& monster,ifstream& mFile, ifstream& sFile,unordered_map<string, mercType>& stats);

int main() {
    ifstream mFile;
    ifstream sFile;
    priorityQ<string, double> mercenaries;
    unordered_map<string, mercType> stats;
    mercType monster;

    fileOpen(mFile,sFile);
    scoreTracker(monster,mFile, sFile,stats);
    rankQueue(mercenaries, stats);
    leaderBoard(mercenaries);  

    mFile.close();
    sFile.close(); 
    
    return 0;
}

void fileOpen(ifstream& mFile, ifstream& sFile){
    string fileName;
    cout << "\nEnter mercenaries file: \n";
    cin >> fileName;
    mFile.open(fileName);
    cout << "\nEnter simulation file: ";
    cin >> fileName;
    cout<<endl; 
    sFile.open(fileName); 
}

void rankQueue(priorityQ<string, double>& rank, unordered_map<string, mercType>& stats){
    for(auto it = stats.begin(); it != stats.end(); it++){
        rank.push_back(it->first, it->second.score());
    }
}

void scoreTracker(mercType& monster,ifstream& mFile, ifstream& sFile,unordered_map<string, mercType>& stats){
    string name;
    string leader;
    string monType;
    float topScore = 0.0;
    int count=0; 

    // Store mercenary file info into our hashmap
    while (mFile >> name >> monster.zombies >> monster.spiders >> monster.drainDeimos >> monster.zombieDogs >> monster.hunters >> monster.nemesis >> monster.civilians){
        stats[name] = monster;
    }

    //register player name and keep track of the number of monsters of eachtypes
    while (sFile >> name >>monType >>count) {
        if (monType == "zombies"){
            stats[name].zombies += count;
        } 
        else if(monType == "spiders") {
            stats[name].spiders += count;
        } 
        else if(monType == "drainDeimos") {
            stats[name].drainDeimos += count;
        } 
        else if(monType == "zombieDogs") {
            stats[name].zombieDogs += count;
        } 
        else if(monType == "hunters") {
            stats[name].hunters += count;
        } 
        else if(monType == "nemesis") {
            stats[name].nemesis += count;
        } 
        else if(monType == "civilians") {
            stats[name].civilians += count;
        }

        //update top score and prompt if new player
        float score = stats[name].score(); 
        if (score > topScore) {
            topScore = score;
            if (leader != name){
                cout << "We have a new leader: " << name << " Score: "<< fixed << setprecision(1)<<topScore << "\n\n";
            }
            leader = name;
        }
    }
}

void leaderBoard(priorityQ<string, double>& rank){
    //print leader board
    int position = 1; 
    while (!rank.isEmpty()){
        string name = rank.get_front_key();
        float score = rank.get_front_priority();  
        cout << "\nRank " << position << endl;
        cout << "Name: " << name << " Score: " << score << endl;
        position++;
        rank.pop_front();
        if(position == 15){
            break;
        }
    }
    cout<< endl;
}
