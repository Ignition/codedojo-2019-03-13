#include <iostream>
#include <vector>
#include <array>

struct machine
{

    std::array<std::vector<char>, 10> stacks;

    static constexpr int PUSH = 0;
    static constexpr int SET_OPERATION = 1;
    static constexpr int SET_ARG2 = 2;
    static constexpr int SET_ARG1 = 3;
    static constexpr int DO_A_THING = 4;

    static constexpr int END_STATE = 5;

    int state = PUSH;

    enum {
        ADD, SUB, MUL, DIV, CLEAR
    } op = CLEAR;
    int arg2 = 0;
    int arg1 = 0;

    void push_op(char i)
    {
        stacks[arg1].push_back(i);
    }

    void set_op(char i)
    {
        switch (i)
        {
            case 'c':
                op = CLEAR;
                break;
            case '+':
                op = ADD;
                break;
            case '-':
                op = SUB;
                break;
            case '*':
                op = MUL;
                break;
            case '/':
                op = DIV;
                break;
        }
    }

    void set_arg2(char i)
    {
        arg2 = i - '0';
    }

    void set_arg1(char i)
    {
        arg1 = i - '0';
    }

    void eval()
    {
        if (op == CLEAR) return;
        auto a1 = stacks[arg1].back();
        auto a2 = stacks[arg2].back();
        stacks[arg1].pop_back();
        stacks[arg2].pop_back();

        switch (op)
        {
            case ADD:
                stacks[arg1].push_back(a1 + a2);
                break;
            case SUB:
                stacks[arg1].push_back(a1 - a2);
                break;
            case MUL:
                stacks[arg1].push_back(a1 * a2);
                break;
            case DIV:
                stacks[arg1].push_back(a1 / a2);
                break;
        }
    }

    void move()
    {
        stacks[arg2].push_back(stacks[arg1].back());
        stacks[arg1].pop_back();
    }

    void print()
    {
        auto end = stacks[arg1].rend();
        for (auto it = stacks[arg1].rbegin(); it != end; ++it){
            char c = *it;
            std::cout << c;
        }
        stacks[arg1].clear();
        std::cout << std::flush;
    }

    void print_num()
    {
        auto val = stacks[arg1].back();
        stacks[arg1].pop_back();
        std::cout << (int)val << std::flush;

    }

    void copy()
    {
        stacks[arg2].push_back(stacks[arg1].back());
    }

    void run_stack()
    {
        auto end = stacks[arg1].rend();
        for (auto it = stacks[arg1].rbegin(); it != end; ++it){
            char c = *it;
            next_step(c);
        }

    }

    void do_thing(char i)
    {
        switch (i)
        {
            case 'm': // MOVE between stacks
                move();
                break;
            case 'c': // COPY between stacks
                copy();
                break;
            case 'p': // PRINT a whole stack
                print();
                break;
            case 'e': // EVAL the op
                eval();
                break;
            case '#': // NUMBER print
                print_num();
                break;
            case '%':
                run_stack();
        }

    }

    void next_step(char i)
    {
        auto current_state = state;
        state++;
        state %= END_STATE;
        if (i != 'n') // NOT noop
        {

            switch (current_state)
            {
                case PUSH:
                    push_op(i);
                    break;
                case SET_OPERATION:
                    set_op(i);
                    break;
                case SET_ARG2:
                    set_arg2(i);
                    break;
                case SET_ARG1:
                    set_arg1(i);
                    break;
                case DO_A_THING:
                    do_thing(i);
                    break;

            }
        }

    }

};

int main()
{
    machine m;
    std::string s = "innnnhnnnp"; // print hi
    //std::string s = "5-nnn0n10mnnnnennnn#"; //print 5
    /*
    std::string s = "1nn1n0nnnnn-10enn10c"
                    "7nn3n7+03e"
                    "nnn2n nnnpnn11cnnn1#nn00cn+10enn10mnn20mnn01mnn12m"
                    "nnn2n nnnpnn11cnnn1#nn00cn+10enn10mnn20mnn01mnn12m"
                    "nnn2n nnnpnn11cnnn1#nn00cn+10enn10mnn20mnn01mnn12m"
                    "nnn2n nnnpnn11cnnn1#nn00cn+10enn10mnn20mnn01mnn12m"
                    "nnn2n nnnpnn11cnnn1#nn00cn+10enn10mnn20mnn01mnn12m"
                    "nnn2n nnnpnn11cnnn1#nn00cn+10enn10mnn20mnn01mnn12m"
                    "nnn2n nnnpnn11cnnn1#nn00cn+10enn10mnn20mnn01mnn12m";
                    */


    /*
     * store '1' in s0   1nn1n
     * store '0' in s1   0nnnn
     * sub s1 from s0    n-10e
     * copy s0 to s1     nn10c
     *
     * load ' ' in s2 then print s2   "nnn2n nnnp"
     *
     * copy s1 s1      [{1},{1,1}]   nn11c
     * num print s1    [{1},{1}]     nnn1#
     * copy s0 s0      [{1,1},{1}]   nn00c
     * add s0 s1       [{2,1},{}]    n+10e
     * move s0 s1      [{1},{2}]     nn10m
     *
     * move s0 s2     nn20m
     * move s1 s0     nn01m
     * move s2 s1     nn12m
     *
     *
     * */


    //std::cin >> s;
    for (auto c : s)
    {
        m.next_step(c);
    }
}