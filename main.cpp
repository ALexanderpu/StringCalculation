#include<bits/stdc++.h>
using namespace std;
class StringCalculator{
public:
    int Add(string nums){
        if(nums.length() == 0) return 0;
        replace(nums.begin(), nums.end(), ',',  ' ');
        stringstream ss(nums);
        string tmp;
        vector<int> container;
        while(ss >> tmp) container.push_back(stoi(tmp));
        /*
        int result = accumulate(container.begin(), container.end(), 0);
        return result;
         */
        return AddWithConstraints(container);
    }
    // more general one
    int Add(string nums, char delimiter){
        if(nums.length() == 0) return 0;
        replace(nums.begin(), nums.end(), delimiter,  ' ');
        stringstream ss(nums);
        string tmp;
        vector<int> container;
        while(ss >> tmp) container.push_back(stoi(tmp));
        /*
        int result = accumulate(container.begin(), container.end(), 0);
        return result;
         */
        return AddWithConstraints(container);
    }

    vector<string> Add(string nums, string delimiter){
        std::vector<std::string> splittedString;
        int startIndex = 0;
        int  endIndex = 0;
        while( (endIndex = nums.find(delimiter, startIndex)) < nums.size() ){

            std::string val = nums.substr(startIndex, endIndex - startIndex);
            splittedString.push_back(val);
            startIndex = endIndex + delimiter.length();

        }
        if(startIndex < nums.length()){
            std::string val = nums.substr(startIndex);
            splittedString.push_back(val);
        }
        return splittedString;

    }

    int AddWithDelimiter(string numsWithDelimiter){
        // format is  //x\n
        if(numsWithDelimiter.length() == 0) return 0;
        char delimiter = numsWithDelimiter[2];
        string nums(numsWithDelimiter.begin() + 4, numsWithDelimiter.end());
        return Add(nums, delimiter);
    }

    // implement a wrapper for adding constraints when sum them up
    int AddWithConstraints(vector<int>& nums){
        // negative not allow
        int min_val = *min_element(nums.begin(), nums.end());
        if(min_val < 0) {
            try {
                throw min_val;
            } catch (int n) {
                cout << "Negative not allowed. The number is: " << n << endl;
            }
        }
        // larger than 1000 should be ignored
        int result = accumulate(nums.begin(), nums.end(), 0, [](int x, int y){if(y <= 1000){ return x+y; }});
        return result;
    }


    int AddWithMultiDelimiters(string numsWithMultiDelimiter){
        // two parts: delimiters and numbers
        string delimiters, nums;
        auto pos = numsWithMultiDelimiter.find('\n');
        if(pos != string::npos) {
            delimiters = numsWithMultiDelimiter.substr(2, pos-1);
            cout << " delimiters: " << delimiters << endl;
            nums = numsWithMultiDelimiter.substr(pos + 1); // to the end of string
            cout << " nums: " << nums << endl;
            // check how many delimiters

            replace(delimiters.begin(), delimiters.end(), ',', ' ');
            stringstream fss(delimiters);
            string tmp;
            unordered_map<size_t, vector<string> > delimiterContainer;
            vector<size_t> delimiterSize;
            while (fss >> tmp) {
                delimiterContainer[tmp.size()].push_back(tmp);
                delimiterSize.push_back(tmp.size());
            }
            sort(delimiterSize.rbegin(), delimiterSize.rend());

            // then use these delimiters to separate integers from the longest to the shortest
            vector<string> numsParts;
            numsParts.push_back(nums);
            for (auto size: delimiterSize) {
                for (auto delimiter: delimiterContainer[size]) {
                    vector<string> tmp;
                    for (auto part: numsParts) {
                        vector<string> res = Add(part, delimiter);
                        tmp.insert(tmp.end(), res.begin(), res.end());
                    }
                    swap(tmp, numsParts);
                }
            }

            vector<int> nums;
            transform(numsParts.begin(), numsParts.end(), std::back_inserter(nums), [](const string& str) { return stoi(str); });

            return AddWithConstraints(nums);


        }else{
            cout << "The program cannot recognize the format of the input string" << endl;
            return 0;
        }


    }


};

int main(){
    StringCalculator sc;
    // test cases;
    cout << "Pass input test (1,2,5): " << (sc.Add("1,2,5") == 8) << endl;
    cout << "Pass input test (1\\n,2,5): " << (sc.Add("1\n,2,3") == 6) << endl;
    cout << "Pass input test (//$\\n1$2$3): " << (sc.AddWithDelimiter("//$\n1$2$3") == 6) << endl;
    cout << "Pass input test (//@\\n2@3@8): " << (sc.AddWithDelimiter("//@\n2@3@8") == 13) << endl;

    cout << "Negative input test: (2,-101, 3.-22): " << endl;
    sc.Add("2,-101, 3.-22");
    cout << "Pass input test (2,1001): " << (sc.Add("2,1001") == 2) << endl;

    cout << "Pass input test (//***,@@,$\\n12***34$59@@4): " << (sc.AddWithMultiDelimiters("//***,@@,$\n12***34$59@@4") == 109) << endl;


}
