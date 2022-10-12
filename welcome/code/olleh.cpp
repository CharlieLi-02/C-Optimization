#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;


void test(string s){
    string str = s;
    int n = str.size();
    char temp [n];

    for (int i = 0; i < n; i ++)
        {
            if ( isalnum(str[i]) ) {
                temp [i] = str[i];
            }
            else {
                temp [i] = ' ';
            }
        }
    
    string sample(temp);

    int i = 0, j = 0;
        
    while (j <= n) {
        if (sample[j] == ' '){
            reverse(sample.begin() + i, sample.begin() + j);
            j ++;
            i = j;
        }
        else if (j == n - 1){
            reverse(sample.begin() + i, sample.begin() + j + 1);
            j ++;
            i = j;
        }
        else {
            j ++;
        }
    }
    
    for (int i = 0; i < n; i ++) {
        {
            if (isalnum (str[i])){
                str [i] = sample [i];
            }
        }
    }
    cout << str << endl;
}


int main() {
    
    string input = "";
    getline(cin, input);
    test(input);
    return 0;
}
