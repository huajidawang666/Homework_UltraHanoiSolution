/* 班级 学号 姓名 */

#include <iostream>
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include "cmd_console_tools.h"
#include "cmd_hdc_tools.h"
#include "hanoi.h"
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

void wait() {
    if (delay == 0)
        _getch();
    else {
        Sleep(delay);
    }
}

void returnSuspend(int choice) {
    switch (choice) {
    case 0:
        return;
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
        cout << endl << "按回车键继续";
        break;
    default:
        break;
	case 4: // match with Demo
        cct_gotoxy(0, 13);
        cout << "按回车键继续";
        break;
    }

    char ch;
    while ((ch = _getch()) == '\n');
}

void graph_init() {
    cct_cls();
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    hdc_init();
    hdc_cls();
}

/*
* Start of Print Functions
*/

/* choice = 1 */
void basic_solution(char from, char to) {
    cout << setw(2) << disk[to - 'A'][top[to - 'A'] - 1] << "# " << from << "-->" << to << endl;
}

/* choice = 2 */
void basic_solution_with_step_record(char from, char to) {
    cout << "第" << setw(4) << step << " 步(" << setw(2) << disk[to - 'A'][top[to - 'A'] - 1] << "#: " << from << "-->" << to << ") " << endl;
}

/* choice = 3*/
void horizontal_array_display(char from, char to) {
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

/* choice = 4 */
void horizontal_graphical_display_print_init(char from, char to, int level) {
    cct_cls();
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << from << " 移动到 " << to << "，共 " << level << " 层，延时设置为 " << delay << "ms";

    cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
    cout << "初始:" << setw(17) << "";

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

    cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET - 2, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 1);
    for (int i = 1; i <= 2 * Underpan_Distance + 5; i++) {
		cout << "=";
    }

    cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET, MenuItem4_Start_Y + Underpan_A_Y_OFFSET);
    cout << "A" << endl;
    cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem4_Start_Y + Underpan_A_Y_OFFSET);
    cout << "B" << endl;
    cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem4_Start_Y + Underpan_A_Y_OFFSET);
    cout << "C" << endl;

    for (int i = 0; i < top[from - 'A']; i ++) {
        cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * (from - 'A'), MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2 - i);
        cout << disk[from - 'A'][i];
	}

    wait();
}

void horizontal_graphical_display_print(char from, char to) {
    cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
    cout << "第" << setw(4) << step << "步(" << disk[to - 'A'][top[to - 'A'] - 1] << "#: " << from << "-->" << to << ") ";

	horizontal_array_display(from, to);

    int fromX = 0, fromY = 0, toX = 0, toY = 0;
	fromX = MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * (from - 'A');
	fromY = MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2 - top[from - 'A'];
	toX = MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * (to - 'A');
	toY = MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 1 - top[to - 'A'];
    cct_gotoxy(fromX, fromY);
    cout << " ";
    cct_gotoxy(toX, toY);
    cout << disk[to - 'A'][top[to - 'A'] - 1];

    wait();
}

/* choice = 5 */
void graphic_preliminary_display() {
    // draw underpan
    for (int i = 1; i <= 3; i++) {
        hdc_rectangle(
            HDC_Start_X + (i - 1) * (HDC_Underpan_Distance + HDC_Base_Width * 23),
            HDC_Start_Y, HDC_Base_Width * 23,
            HDC_Base_High,
            HDC_COLOR[MAX_LAYER + 1]
        );
        Sleep(HDC_Init_Delay);
	}

	// draw pillars
    for (int i = 1; i <= 3; i++) {
        hdc_rectangle(
            HDC_Start_X + (i - 1) * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * 11,
            HDC_Start_Y - HDC_Base_High * 12,
            HDC_Base_Width, HDC_Base_High * 12,
            HDC_COLOR[MAX_LAYER + 1]
        );
		Sleep(HDC_Init_Delay);
    }
}

/* choice = 6 */
void graphic_disk_display(char from, char to) {

    // cct_cls & hdc_init & hdc_cls included.
	graphic_preliminary_display();

    // draw disks
    for (int i = 0; i < top[from - 'A']; i++) {
        hdc_rectangle(
            HDC_Start_X + (from - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * (11 - disk[from - 'A'][i]),
            HDC_Start_Y - HDC_Base_High * (i + 1),
            HDC_Base_Width * (disk[from - 'A'][i] * 2 + 1),
            HDC_Base_High,
            HDC_COLOR[disk[from - 'A'][i]]
		);

		Sleep(HDC_Init_Delay);
    }
}

/* choice = 7 */
void graphic_first_move_init(char from, char to) {
	graphic_disk_display(from, to);
}

/* choice = 7 & 8 */

/*
* This drives me MAD... I hate u SJ.
*/
void graphic_move(char from, char to) {    

    // move upwards within pillar
    for (

        /*
        * Note : why "+1" ? cuz disk have been actually removed in array disk[3][10]
        * print function is always called after actual move.
        * 
		* So all top[from - 'A'] - 1 => top[from - 'A']
		* And all disk[from - 'A'][top[from - 'A'] - 1] => disk[to - 'A'][top[to - 'A'] - 1]
        * 
        * This should be precisely calculated.
        * 
        * ** BTW ** 
        * These coordinations fuck my brains.
        */

        int y = HDC_Start_Y - HDC_Base_High * (top[from - 'A'] + 1);
        y - HDC_Step_Y >= HDC_Top_Y; // Note: y is decreasing when moving UPWARDS
		y -= HDC_Step_Y
        ) {
		// clear previous position
        hdc_rectangle(
            HDC_Start_X + (from - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * (11 - disk[to - 'A'][top[to - 'A'] - 1]),
            y + HDC_Base_High - HDC_Step_Y,
            HDC_Base_Width * (disk[to - 'A'][top[to - 'A'] - 1] * 2 + 1),
            HDC_Step_Y,
            HDC_COLOR[0]
		);

        // restore pillar

        // Fill the pillar only when overlapped.
		if (y + HDC_Base_High > HDC_Start_Y - HDC_Base_High * 12) { 

            /*
            * Worthy of Note:
            * Why getting a "(A>B) ? A:B" here?
            *
            * Normal Case:
            *
            * ========== (Pillar Top)
            * ↑
            * Far from Each Other
            * ↓
            * ========== (New Disk Bottom)      ┬ To be Filled
            * ========== (Former Disk Bottom)   ┘
            *
            *
            * A troublesome case:
            *
            * ========== (New Disk Bottom)
            * ========== (Pillar Top)           ┬ To be Filled
            * ========== (Former Disk Bottom)   ┘
            *
            */

            hdc_rectangle(
                HDC_Start_X + (from - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * 11,
				// to be more precise, there might be a tiny gap between the top of the pillar and the bottom of the new disk
                (y + HDC_Base_High - HDC_Step_Y > HDC_Start_Y - HDC_Base_High * 12) ?
                    y + HDC_Base_High - HDC_Step_Y : HDC_Start_Y - HDC_Base_High * 12,
                HDC_Base_Width,
                (y + HDC_Base_High - HDC_Step_Y > HDC_Start_Y - HDC_Base_High * 12) ?
                    HDC_Step_Y : (HDC_Start_Y - HDC_Base_High * 12) - (y + HDC_Base_High - HDC_Step_Y),
                HDC_COLOR[MAX_LAYER + 1]
            );
        }
		// draw disk at new position
        hdc_rectangle(
            HDC_Start_X + (from - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * (11 - disk[to - 'A'][top[to - 'A'] - 1]),
            y - HDC_Step_Y,
            HDC_Base_Width * (disk[to - 'A'][top[to - 'A'] - 1] * 2 + 1),
            HDC_Step_Y,
			HDC_COLOR[disk[to - 'A'][top[to - 'A'] - 1]]
        );

        wait();
    }

    // step for x-cord might be negative.

    for (
        int x = HDC_Start_X + (from - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * (11 - disk[to - 'A'][top[to - 'A'] - 1]);
        (x + ((from < to) ? HDC_Step_X : -HDC_Step_X))
            != (HDC_Start_X + (to - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * (11 - disk[to - 'A'][top[to - 'A'] - 1]))
               + ((from < to) ? HDC_Step_X : -HDC_Step_X);
        x += (from < to) ? HDC_Step_X : -HDC_Step_X
		) {

        // clear previous position
        hdc_rectangle(
            x + ((from < to) ? 0 : (HDC_Base_Width * (disk[to - 'A'][top[to - 'A'] - 1] * 2 + 1) - HDC_Step_X)),
            HDC_Top_Y,
            HDC_Step_X,
            HDC_Base_High,
            HDC_COLOR[0]
        );

        // draw disk at new position
        hdc_rectangle(
            x + ((from < to) ? (HDC_Base_Width * (disk[to - 'A'][top[to - 'A'] - 1] * 2 + 1)) : -HDC_Step_X),
            HDC_Top_Y,
			HDC_Step_X,
            HDC_Base_High,
			HDC_COLOR[disk[to - 'A'][top[to - 'A'] - 1]]
        );
        wait();
    }

	// move downwards within pillar
    for (
        int y = HDC_Top_Y;
		y + HDC_Base_High + HDC_Step_Y <= HDC_Start_Y - HDC_Base_High * (top[to - 'A'] - 1);
        y += HDC_Step_Y
        ) {
        // clear previous position
        hdc_rectangle(
            HDC_Start_X + (to - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * (11 - disk[to - 'A'][top[to - 'A'] - 1]),
            y,
            HDC_Base_Width * (disk[to - 'A'][top[to - 'A'] - 1] * 2 + 1),
            HDC_Step_Y,
            HDC_COLOR[0]
        );
		// restore pillar

		// Fill the pillar only when overlapped.
		if (y + HDC_Step_Y > HDC_Start_Y - HDC_Base_High * 12) {

            /*
            * Worthy of Note AS WELL:
            * Why getting a "(A>B) ? A:B" here?
            *
            * Normal Case:
            *
            * ========== (Pillar Top)
            * ↑
            * Far from Each Other
            * ↓
            * ========== (Former Disk Top)  ┬ To be Filled
            * ========== (New Disk Top)     ┘
            *
            *
            * A troublesome case:
            *
            * ========== (Former Disk Top)
            * ========== (Pillar Top)       ┬ To be Filled
            * ========== (New Disk Top)     ┘
            *
            */

            hdc_rectangle(
                HDC_Start_X + (to - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * 11,
                (y > HDC_Start_Y - HDC_Base_High * 12) ? y : HDC_Start_Y - HDC_Base_High * 12,
                HDC_Base_Width,
                (y > HDC_Start_Y - HDC_Base_High * 12) ? HDC_Step_Y : (y + HDC_Step_Y) - (HDC_Start_Y - HDC_Base_High * 12),
                HDC_COLOR[MAX_LAYER + 1]
            );
        }
        // draw disk at new position
        hdc_rectangle(
            HDC_Start_X + (to - 'A') * (HDC_Underpan_Distance + HDC_Base_Width * 23) + HDC_Base_Width * (11 - disk[to - 'A'][top[to - 'A'] - 1]),
            y + HDC_Base_High,
            HDC_Base_Width * (disk[to - 'A'][top[to - 'A'] - 1] * 2 + 1),
            HDC_Step_Y,
            HDC_COLOR[disk[to - 'A'][top[to - 'A'] - 1]]
        );

        wait();
	}
}

/*
* Hub of Print Functions
*/

void printInit(char src, char dest, char level, int choice) {
    switch (choice) {
    case 4:
        horizontal_graphical_display_print_init(src, dest, level);
        break;
    default:
        break;
    }
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
		horizontal_array_display(from, to);
        break;
    case 4:
        horizontal_graphical_display_print(from, to);
		break;
    case 5:
        graphic_preliminary_display();
        break;
    }
}

/*
* End of Print Functions
*/

void move(char from, char to, int depth, int choice) {
    // move disk from "from" to "to"
    disk[to - 'A'][top[to - 'A']++] = disk[from - 'A'][--top[from - 'A']];
    disk[from - 'A'][top[from - 'A']] = 0;
    step++;
	printStatus(from, to, choice);
}

void hanoi(char from, char to, int depth, int choice) {
    char mid = 'A' + 'B' + 'C' - from - to;

    if (depth <= 1) {
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

	// specialty solution configuration: when choice == 0
	if (choice == 0) {
        return;
    }

    int level = 0;
	char src = 'A', tmp = 'B', dest = 'C'; // by default
    
    // specialty solution configuration: when choice == 5
    if (choice == 5) {
        graph_init();

		graphic_preliminary_display();

        hdc_release();
        return;
    }

    getHanoiConf(choice, &level, &src, &dest);

    // init
    tmp = 'A' + 'B' + 'C' - src - dest;
    step = 0;
    top[src - 'A'] = level;
	top[tmp - 'A'] = 0;
	top[dest - 'A'] = 0;
    for (int i = 0; i < level; i++) {
        disk[src - 'A'][i] = level - i;
		disk[tmp - 'A'][i] = 0;
		disk[dest - 'A'][i] = 0;
    }

    // specialty solution configuration: when choice == 6

    if (choice == 6) {
		graph_init();

		graphic_disk_display(src, dest);

		hdc_release();
        return;
    }

    if (choice == 7) {
        graph_init();

		graphic_first_move_init(src, dest);

        disk[dest - 'A'][top[dest - 'A']++] = disk[src - 'A'][--top[src - 'A']];
        disk[src - 'A'][top[src - 'A']] = 0;
        step++;

		// print is always called after actual move.
        graphic_move(src, dest);

        hdc_release();
        return;
    }

    // specialty solution configuration: when choice == 7

    printInit(src, dest, level, choice);

    hanoi(src, dest, level, choice);
}
