#include <iostream>
#include <fstream>
#include <cstring>
#include "vector.h"

#include <chrono>

int main(int argc, char* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    if (argc != 2) {
        std::cout << "Usage: input-file output-file\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Couldn't open input file\n";
        return 2;
    }

    Vector<int> deck(52);

    // Priority: A K Q J 10 9 8 7 6 5 4 3 2
    char c1;
    char c2;
    for (int i = 0; i < 52; i++)
    {
        int int1 = 0;
        input >> c1;
        input >> c2;
        if (c2 == '0') { 
            int1 = 10;
            input.ignore();
        }
        input.ignore();

        if (int1 == 0)
        {
            switch (c1) {
            case 'A':
                int1 = 14;
                break;
            case 'K':
                int1 = 13;
                break;
            case 'Q':
                int1 = 12;
                break;
            case 'J':
                int1 = 11;
                break;
            default:
                int1 = static_cast<int>(c1 - '0');
            }
        }

        deck.push_back(int1);
    }
    
    // Max amount of cards any player can hold is 52
    Vector<int> p1_RS(52);  // player1 right stack
    Vector<int> p2_RS(52);  // player2 right stack
    for (uint64_t i = 0; i < 26; i++)
    {
        p1_RS.push_back(deck[i]);
    }
    for (uint64_t i = 26; i < 52; i++)
    {
        p2_RS.push_back(deck[i]);
    }

    p1_RS.reverse();  // player1 left stack
    p2_RS.reverse();  // player2 left stack

    Vector<int> p1_LS(52);
    Vector<int> p2_LS(52);

    // FIFO via Two Stacks
    uint64_t i = 0;
    double limit = 1e6;
    const char* res = "unknown";
    while (limit >= 0)
    {
        --limit;

        int f = 0;
        int s = 0;
        if (!p1_RS.empty())
        {
            f = p1_RS.top();
            p1_RS.pop();
        }
        else 
        {
            if (!p1_LS.empty()) {
                while (!p1_LS.empty())
                {
                    p1_RS.push_back(p1_LS.top());
                    p1_LS.pop();
                }

                if (!p1_RS.empty())
                {
                    f = p1_RS.top();
                    p1_RS.pop();
                }
            }
            else
            {
                res = "second";
            }
        }

        if (!p2_RS.empty())
        {
            s = p2_RS.top();
            p2_RS.pop();
        }
        else
        {
            if (!p2_LS.empty()) {
                while (!p2_LS.empty())
                {
                    p2_RS.push_back(p2_LS.top());
                    p2_LS.pop();
                }

                if (!p2_RS.empty())
                {
                    s = p2_RS.top();
                    p2_RS.pop();
                }
            }
            else {
                if (std::strcmp(res, "second") == 0) { res = "draw"; }  // both players are empty
                else { res = "first"; }
                break;
            }
        }

        if ((f == 2 && s == 14) || (f > s && !(f == 14 && s == 2))) {
            p1_LS.push_back(f);
            p1_LS.push_back(s);
        }
        else if ((f == 14 && s == 2) || (f < s && !(f == 2 && s == 14))) {
            p2_LS.push_back(f);
            p2_LS.push_back(s);
        }
        else {
            // draw
            Vector<int> store;
            store.push_back(f);
            store.push_back(s);

            bool finish = false;
            while (!finish) {
                --limit;

                int f1 = 0;
                int s1 = 0;

                if (!p1_RS.empty())
                {
                    f1 = p1_RS.top();
                    p1_RS.pop();
                }
                else
                {
                    if (!p1_LS.empty()) {
                        while (!p1_LS.empty())
                        {
                            p1_RS.push_back(p1_LS.top());
                            p1_LS.pop();
                        }

                        if (!p1_RS.empty())
                        {
                            f1 = p1_RS.top();
                            p1_RS.pop();
                        }
                    }
                    else
                    {
                        res = "second";
                    }
                }

                if (!p2_RS.empty())
                {
                    s1 = p2_RS.top();
                    p2_RS.pop();
                }
                else
                {
                    if (!p2_LS.empty()) {
                        while (!p2_LS.empty())
                        {
                            p2_RS.push_back(p2_LS.top());
                            p2_LS.pop();
                        }

                        if (!p2_RS.empty())
                        {
                            s1 = p2_RS.top();
                            p2_RS.pop();
                        }
                    }
                    else {
                        if (std::strcmp(res, "second") == 0) { res = "draw"; } 
                        else { res = "first"; }
                        finish = true;
                        break;
                    }
                }

                store.push_back(f1);
                store.push_back(s1);

                if ((f1 == 2 && s1 == 14) || (f1 > s1 && !(f1 == 14 && s1 == 2))) {
                    for (uint64_t i = 0; i < store.get_size(); i++) {
                        p1_LS.push_back(store[i]);
                    }
                    finish = true;  // draw is resolved, continue main while loop
                }
                else if ((f1 == 14 && s1 == 2) || (f1 < s1 && !(f1 == 2 && s1 == 14))) {
                    for (uint64_t i = 0; i < store.get_size(); i++) {
                        p2_LS.push_back(store[i]);
                    }
                    finish = true;
                }
                else {
                    continue;  // draw again
                }
            }

            if (std::strcmp(res, "unknown") != 0) { break; }
        }
    }

    std::cout << res;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "\nExecution time: " << duration.count() << " milliseconds" << std::endl;

	return 0;
}