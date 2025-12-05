/* 班级 学号 姓名 */

#include <iostream>
#include <iomanip>
#include <conio.h>
using namespace std;


/* ----------------------------------------------------------------------------------

     本文件功能：
     1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

     本文件要求：
     1、不允许定义外部全局变量（const及#define不在限制范围内）
     2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
     3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
     4、按需加入系统头文件、自定义头文件、命名空间等
   ----------------------------------------------------------------------------------- */


static int delay = 0; // miliseconds
static int top[3] = { 0 };
static int disk[3][10] = { 0 };
static int step = 0;

void basic_solution(char from, char to) {
    cout << setw(2) << disk[to - 'A'][top[to - 'A'] - 1] << "# " << from << "-->" << to << endl;
}

void basic_solution_with_step_record(char from, char to) {
    cout << "第" << setw(4) << step << " 步(" << setw(2) << disk[to - 'A'][top[to - 'A'] - 1] << "#: " << from << "-->" << to << ") " << endl;
}

void horizonal_array_display(char from, char to) {
    cout << "第" << setw(4) << step << " 步("
        << setw(2) << disk[to - 'A'][top[to - 'A'] - 1] << "): "
        << from << "-->" << to;

    for (int i = 0; i < 3; i++) {
        cout << " ";
        cout << static_cast<char>('A' + i) << ":";
        for (int j = 0; j < 10; j++) {
            if (disk[i][j] == 0)
                cout << setw(2) << " ";
            else
                cout << setw(2) << disk[i][j];
        }
    }

    cout << endl;
}

void printStatus(char from, char to, int choice) {
    switch (choice) {
    case 1:
        basic_solution(from, to);
        break;
    case 2:
        basic_solution_with_step_record(from, to);
		break;
    case 3:
		horizonal_array_display(from, to);
        break;
    }
}

void returnSuspend(int choice) {
	switch (choice) {
    case 1:
    case 2:
    case 3:
        cout << endl << "按回车键继续";
        break;
    default:
        break;
    }

    char ch;
    while ((ch = _getch()) == '\n');
}

void move(char from, char to, int depth, int choice) {
    // move disk from "from" to "to"
    disk[to - 'A'][top[to - 'A']++] = disk[from - 'A'][--top[from - 'A']];
    disk[from - 'A'][top[from - 'A']] = 0;
    step++;
	printStatus(from, to, choice);
}

void hanoi(char from, char to, int depth, int choice) {
    char mid = 'A' + 'B' + 'C' - from - to;

    if (depth == 1) {
		move(from, to, depth, choice);
        return;
    }

    hanoi(from, mid, depth - 1, choice);
	move(from, to, depth, choice);
    hanoi(mid, to, depth - 1, choice);
}

void getHanoiConf(int choice, int* level, char* src, char* dest) {

	int _level, _delay;
    char _src, _dst;

    // get level
    while (1) {
        cout << "请输入汉诺塔的层数(1-10)：" << endl;
        cin >> _level;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (_level < 1 || _level > 10) {
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');
        break;
    }

    // get _src and dest
    while (1) {
        cout << "请输入起始柱(A-C)" << endl;
        cin >> _src;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (_src >= 'a' && _src <= 'c')
            _src -= 32;
        if (_src < 'A' || _src > 'C') {
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');
        break;
    }

    while (1) {
        cout << "请输入目标柱(A-C)" << endl;
        cin >> _dst;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (_dst >= 'a' && _dst <= 'c')
            _dst -= 32;
        if (_dst < 'A' || _dst > 'C') {
            cin.ignore(10000, '\n');
            continue;
        }
        if (_dst == _src) {
            cout << "目标柱(" << _dst << ")不能与起始柱(" << _src << ")相同" << endl;
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');
        break;
    }

    *level = _level;
    *src = _src;
    *dest = _dst;

	// get delay
    switch (choice) {
    case 4:
        while (true) {
            cout << "请输入移动速度(0-200：0-按回车单步演示 1-200:延时1-200ms) " << endl;
            cin >> _delay;
            if (cin.fail() || _delay < 0 || _delay > 200) {
                cin.clear();
                cin.ignore(10000, '\n');
                _delay = 0;
            }
            cin.ignore(10000, '\n');
            break;
        }

        delay = _delay;
        break;

    case 7:
    case 8:
    case 9:
        while (true) {
            cout << "请输入移动速度(0-20：0-按回车单步演示 1-20:延时1-20ms) " << endl;
            cin >> _delay;
            if (cin.fail() || _delay < 0 || _delay > 20) {
                cin.clear();
                cin.ignore(10000, '\n');
                _delay = 0;
            }
            cin.ignore(10000, '\n');
            break;
        }

        delay = _delay;
		break;

    default:
        break; // do nothing.
    }
}

void solve(int choice) {    
    int level = 0;
	char src = 'A', tmp = 'B', dest = 'C'; // by default
    
    // get hanoi configuration
    if (choice != 5) {
        getHanoiConf(choice, &level, &src, &dest);
		tmp = 'A' + 'B' + 'C' - src - dest;
	}
    
    // init
    step = 0;
    top[src - 'A'] = level;
	top[tmp - 'A'] = 0;
	top[dest - 'A'] = 0;
    for (int i = 0; i < level; i++) {
        disk[src - 'A'][i] = level - i;
		disk[tmp - 'A'][i] = 0;
		disk[dest - 'A'][i] = 0;
    }

    hanoi(src, dest, level, choice);

    returnSuspend(choice);
}
   /***************************************************************************
     函数名称：
     功    能：
     输入参数：
     返 回 值：
     说    明：
   ***************************************************************************/
