#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#define KEY_UP    72
#define KEY_DOWN  80
#define KEY_ENTER 13
#define COLOR_DEFAULT 7     // 기본 (밝은 회색)
#define COLOR_GRAY    8     // 어두운 회색
#define COLOR_YELLOW  14    // 노란색
#define ANIM_FRAMES 15      // 애니메이션 프레임 수
#define COLOR_BLUE 11       // 남
#define COLOR_PINK 13       // 여
#define COLOR_GREEN 10      //분단 구분선 

int g_use_names = 0;        // 0: 번호만 표시 / 1: 이름·성별 표시
char g_name[101][13];       // 번호별 이름 (1~100번 사용)
int g_gender[101];          // 번호별 성별 (1: 남, 2: 여)
int auto_mode = 0;       // 0: 꺼짐 / 1: 자동 추첨 중
int t;
int g_group = 0;            // 몇 열마다 분단 구분선을 넣을지 (0 = 없음)

int start_screen(void);
void show_help(void);
void set_color(int color);
int seat_arrangement(int row, int column);
int input_int_range(const char *msg, int min, int max);
void shuffle_number(int sit_number[], int total, int fixed_index);
void draw_check02(int c, int r, int x0, int y0); 
void gotoxy(int x, int y);
void swap_seats(int sit_number[], int total, int fixed_number);
void save_result(int sit_number[], int row, int column,
                 int fixed_number, int fixed_row, int fixed_column,
                 int exclude_number);
void play_select_sound(void);
void play_done_sound(void);      
void print_numbers(int sit_number[], int row, int column, int fixed_number, int x0);  
void shuffle_animation(int sit_number[], int total, int fixed_index,
                       int row, int column, int fixed_number);     
int select_menu(const char *items[], int count, int x, int y);
void set_cursor_visible(int visible);  
int get_console_width(void);
int center_x(int width);
void print_center(int y, const char *text);    
int confirm(const char *msg);
void find_seat(int sit_number[], int total, int column);
void pick_presenter(int sit_number[], int total, int column);
void make_teams(int sit_number[], int total);
void rotate_seats(int sit_number[], int total, int fixed_index);

int cell_w(void);
int choose_display_mode(void);
void input_students(int total);

void show_history(int history[][100], int hist_count, int draw_count, int row, int column);
void put_border(const char *s, int j, int c);

int main(void)
{
    int row, column;
    int restart = 1;

    srand((unsigned int)time(NULL));
    system("title 자리 배치 프로그램");

if(start_screen() == 0)          // 시작 화면에서 "종료"를 고른 경우
{
    printf("\n프로그램을 종료합니다.\n");
    return 0;
}

    printf("학생수는 최대 100명입니다.(10X10)\n");

    while(1)
    {
        do {
            printf("행의 수를 입력하고 Enter>");

            if(scanf("%d", &row) != 1)
            {
                printf("오류! 숫자를 입력하세요.\n");
                while(getchar() != '\n');
                row = 0;
                continue;
            }

            if(row < 1 || row > 10)
                printf("오류! 행은 1~10 사이로 입력하세요.\n");

        } while(row < 1 || row > 10);

        do {
            printf("열의 수를 입력하고 Enter>");

            if(scanf("%d", &column) != 1)
            {
                printf("오류! 숫자를 입력하세요.\n");
                while(getchar() != '\n');
                column = 0;
                continue;
            }

            if(column < 1 || column > 10)
                printf("오류! 열은 1~10 사이로 입력하세요.\n");

        } while(column < 1 || column > 10);

        restart = seat_arrangement(row, column);

		if(restart == 0)
    		break;
    }

    printf("\n프로그램을 종료합니다.\n");

    return 0;
}


int seat_arrangement(int row, int column)
{
    int sit_number[100] = {0};
    int total, i, j, count;
    int choice;
    int need_shuffle = 1;
    int shuffled;

    int fixed_number, fixed_row, fixed_column;
    int fixed_index, number_index;
    int exclude_number;
    int temp;
    
    int x0;
	char buf[100];
	
	int draw_count = 0;          // 추첨 횟수
	char time_str[40];           // 추첨 시각 문자열
	time_t now;
	
	int backup[100];            // 직전 배치 백업
	int has_backup = 0;         // 백업이 있는지 (0 없음 / 1 있음)
	int same_count = 0;         // 직전 배치와 같은 자리에 앉은 학생 수
	
	int seed;
	
	int history[10][100];       // 최근 10회 추첨 결과
	int hist_count = 0;
    
    const char *menu_items[12] {"재추첨", "재실행", "자리 맞바꾸기", "파일 저장", "자리 조회", 
								"발표자 뽑기", "되돌리기", "조 편성", "한 칸 이동", "추첨 이력", "자동 추첨", "종료"};

    total = row * column;

    for(i = 0; i < total; i++)
        sit_number[i] = i + 1;
        
    g_use_names = choose_display_mode();    // 0: 번호만, 1: 이름·성별
	if(g_use_names)
    	input_students(total);    

    
    printf("\n");
    fixed_number = input_int_range("고정할 번호를 입력하세요", 1, total);
    fixed_row    = input_int_range("고정할 행을 입력하세요", 1, row);
    fixed_column = input_int_range("고정할 열을 입력하세요", 1, column);

    fixed_index  = (fixed_row - 1) * column + (fixed_column - 1);
    number_index = fixed_number - 1;   // 아직 안 섞였으니 번호-1이 곧 위치

    // 고정할 번호를 지정한 위치로 이동
    temp = sit_number[fixed_index];
    sit_number[fixed_index] = sit_number[number_index];
    sit_number[number_index] = temp;
    sprintf(buf, "title 자리 배치 프로그램 - %d행 %d열", row, column);
	system(buf);
	
	g_group = input_int_range("분단 구분선: 몇 열마다 표시할까요 (0=없음)", 0, column);

	seed = input_int_range("랜덤 시드 (0=매번 다르게)", 0, 99999);
	if(seed != 0)
    	srand((unsigned int)seed);
    // 제외할 번호 입력 (0이면 제외 안 함)
    while(1)
    {
        exclude_number = input_int_range("제외할 숫자를 입력하시오(0=제외 안 함)", 0, total);

        if(exclude_number != 0 && exclude_number == fixed_number)
        {
            printf("오류! 고정된 번호는 제외할 수 없습니다.\n");
            continue;
        }
        break;
    }

    // 제외 번호를 빈 자리(0)로 바꿈
    if(exclude_number != 0)
    {
        for(i = 0; i < total; i++)
        {
            if(sit_number[i] == exclude_number)
            {
                sit_number[i] = 0;
                break;
            }
        }
    }

    while(1)
    {
        shuffled = 0;
		if(need_shuffle)
		{
			if(draw_count > 0)                                   // 첫 추첨 전 상태는 백업 안 함
		    {
        		memcpy(backup, sit_number, sizeof(sit_number));
        		has_backup = 1;
    		}

    		shuffle_animation(sit_number, total, fixed_index,
                      		  row, column, fixed_number);
    		shuffled = 1;
    		
			if(hist_count == 10)                             // 가득 차면 가장 오래된 것 삭제
			{
    			memmove(history[0], history[1], sizeof(history[0]) * 9);
    			hist_count = 9;
			}
			memcpy(history[hist_count], sit_number, sizeof(sit_number));
			hist_count++;
    		
    		same_count = 0;                                      // ▼ 비교 계산
    		if(has_backup)
    		{
        		for(i = 0; i < total; i++)
            		if(i != fixed_index && sit_number[i] != 0 && sit_number[i] == backup[i])
                		same_count++;
    		}

    		draw_count++;                                   // 횟수 +1
    		now = time(NULL);                               // 현재 시각
    		strftime(time_str, sizeof(time_str),
             "%Y-%m-%d %H:%M:%S", localtime(&now)); // 문자열로 변환
		}
		need_shuffle = 1;

        system("cls");

		sprintf(buf, "자리 배치 결과 (%d행 %d열)", row, column);
		print_center(1, buf);

		sprintf(buf, "고정 번호: %d번 (%d행 %d열)", fixed_number, fixed_row, fixed_column);
		set_color(COLOR_YELLOW);
		print_center(2, buf);
		set_color(COLOR_DEFAULT);

		if(exclude_number != 0)
		{
	    	sprintf(buf, "제외 번호: %d번", exclude_number);
    		set_color(COLOR_GRAY);
    		print_center(3, buf);
    		set_color(COLOR_DEFAULT);
		}
		else
    	print_center(3, "제외 번호: 없음");
    	
    	if(draw_count > 1)                                       // 2회차부터 표시
		{
    		sprintf(buf, "직전 배치와 같은 자리: %d명", same_count);
    		print_center(4, buf);
		}
    	
    	sprintf(buf, "%d회차 추첨  (%s)", draw_count, time_str);
			print_center(5, buf);
		
		if(g_use_names)
    		print_center(6, "남: 하늘색  /  여: 분홍색  /  고정: 노란색");	

		x0 = center_x(column * cell_w() + 1);             // 격자 가로 폭 = 열 수 × 5 + 1
		draw_check02(column, row, x0, 7);
		print_numbers(sit_number, row, column, fixed_number, x0);
		
		
		if(seed != 0)
		{
    		sprintf(buf, "랜덤 시드: %d", seed);
    		print_center(8 + row * 2, buf);
		}

        for(i = 1; i <= row; i++)
        {
            for(j = 1; j <= column; j++)
            {
                gotoxy(j * 5 - 2, 6 + i * 2);

                if(sit_number[count] == 0)
                {
                    set_color(COLOR_GRAY);          // 제외된 자리
                    printf("--");
                }
                else if(sit_number[count] == fixed_number)
                {
                    set_color(COLOR_YELLOW);        // 고정 번호
                    printf("%2d", sit_number[count]);
                }
                else
                {
                    printf("%2d", sit_number[count]);
                }

                set_color(COLOR_DEFAULT);           // 반드시 원래 색으로 복구
                count++;
            }
        }

        if(shuffled && !auto_mode)           // 자동 모드에서는 소리 생략
    play_done_sound();

	if(auto_mode)
	{
    	print_center(9 + row * 2, "자동 추첨 중... (아무 키나 누르면 멈춤)");

    	for(t = 0; t < 30 && !_kbhit(); t++)
        	Sleep(50);                   // 약 1.5초 대기, 키 입력이 오면 바로 중단

    	if(_kbhit())
    		{
        		_getch();                    // 눌린 키 버리기
        		auto_mode = 0;
    	    	need_shuffle = 0;            // 멈춘 화면 그대로 유지
    		}
    	continue;                        // 키가 없으면 need_shuffle=1 그대로 → 다음 추첨
	}	

    	if(shuffled)
    	play_done_sound();

		print_center(9 + row * 2, "추가 기능 선택");
		choice = select_menu(menu_items, 12, -1, 10 + row * 2);

		if(choice == -1)           // 창 크기가 바뀜 → 섞지 않고 다시 그리기만
		{
    		need_shuffle = 0;
    		continue;
		}

		choice++;                  // 1~5로 변환 (이하 분기는 그대로)
		if(choice == 1)
    		continue;
		else if(choice == 2){
    		if(confirm("처음부터 다시 시작하시겠습니까?"))
        		return 1;
    		need_shuffle = 0;          // "아니오"면 섞지 않고 화면 복귀
    	}
		else if(choice == 3)
		{
			memcpy(backup, sit_number, sizeof(sit_number));
    		has_backup = 1;
    		swap_seats(sit_number, total, fixed_number);
    		need_shuffle = 0;
		}
		else if(choice == 4)
		{
    		save_result(sit_number, row, column,
                fixed_number, fixed_row, fixed_column, exclude_number);
    		system("pause");       // 저장 메시지가 cls로 바로 지워지지 않게 잠깐 멈춤
    	need_shuffle = 0;      // 저장 후 다시 그릴 때 셔플하지 않음
		}
		else if(choice == 5)
		{
    		find_seat(sit_number, total, column);
    		need_shuffle = 0;
		}
		else if(choice == 6)
		{
    		pick_presenter(sit_number, total, column);
    		need_shuffle = 0;
		}
		else if(choice == 7)                    // 되돌리기 (종료 else 바로 앞)
		{
    		if(has_backup)
    		{
        		memcpy(sit_number, backup, sizeof(sit_number));
        		has_backup = 0;                 // 한 단계만 되돌릴 수 있음
    		}
    		else
    		{
        		printf("\n되돌릴 배치가 없습니다.\n");
        		system("pause");
    		}
    	need_shuffle = 0;
		}
		else if(choice == 8)                    // 조 편성 (종료 else 바로 앞)
		{
    		make_teams(sit_number, total);
    		need_shuffle = 0;
		}
		else if(choice == 9)                    // 한 칸 이동 (종료 else 바로 앞)
		{
    		memcpy(backup, sit_number, sizeof(sit_number));
    		has_backup = 1;
    		rotate_seats(sit_number, total, fixed_index);
    		need_shuffle = 0;
		}
		else if(choice == 10)                // 종료 else 바로 앞
		{
 	   		show_history(history, hist_count, draw_count, row, column);
    		need_shuffle = 0;
		}
		else if(choice == 11)                // 종료 else 바로 앞
		{
    		auto_mode = 1;                   // 다음 반복부터 자동 추첨 시작
		}	
		else
    		if(confirm("프로그램을 종료하시겠습니까?"))
        	return 0;
    		need_shuffle = 0;
    	}
	}

void shuffle_number(int sit_number[], int total, int fixed_index){
    int i, rnd, temp;

    for(i = total - 1; i > 0; i--)
    {
        // 고정된 좌석은 건너뜀
        if(i == fixed_index)
            continue;

        // 고정된 위치가 선택되지 않도록 함! 
        do {
            rnd = rand() % (i + 1);
        } while(rnd == fixed_index);

        temp = sit_number[i];
        sit_number[i] = sit_number[rnd];
        sit_number[rnd] = temp;
    }
}

void draw_check02(int c, int r, int x0, int y0)
{
    int i, j, k, y = y0;
    int cw = cell_w() - 1;

    gotoxy(x0, y++);
    for(j = 0; j < c; j++)
    {
        put_border("+", j, c);
        for(k = 0; k < cw; k++)
            printf("-");
    }
    put_border("+", c, c);

    for(i = 0; i < r; i++)
    {
        gotoxy(x0, y++);
        for(j = 0; j < c; j++)
        {
            put_border("|", j, c);
            printf("%*s", cw, "");
        }
        put_border("|", c, c);

        gotoxy(x0, y++);
        for(j = 0; j < c; j++)
        {
            put_border("+", j, c);
            for(k = 0; k < cw; k++)
                printf("-");
        }
        put_border("+", c, c);
    }
}

void gotoxy(int x, int y)
{
   COORD Pos = {x - 1, y - 1};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int input_int_range(const char *msg, int min, int max)
{
    int value;

    while(1)
    {
        printf("%s (%d~%d): ", msg, min, max);

        if(scanf("%d", &value) != 1)
        {
            printf("오류! 숫자를 입력하세요.\n");
            while(getchar() != '\n');
            continue;
        }

        if(value < min || value > max)
        {
            printf("오류! %d~%d 사이로 입력하세요.\n", min, max);
            continue;
        }

        return value;
    }
}

void swap_seats(int sit_number[], int total, int fixed_number)
{
    int a, b, i, temp;
    int idx_a, idx_b;

    while(1)
    {
        printf("\n변경할 두 번호를 입력하세요(취소: 0 0)>");

        if(scanf("%d %d", &a, &b) != 2)
        {
            printf("오류! 숫자 두 개를 입력하세요.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');   // 줄 끝까지 비우기

        if(a == 0 && b == 0)        // 취소
            return;

        if(a < 1 || a > total || b < 1 || b > total)
        {
            printf("오류! 1~%d 사이로 입력하세요.\n", total);
            continue;
        }

        if(a == b)
        {
            printf("오류! 서로 다른 두 번호를 입력하세요.\n");
            continue;
        }

        if(a == fixed_number || b == fixed_number)
        {
            printf("오류! 고정된 번호는 바꿀 수 없습니다.\n");
            continue;
        }

        // 두 번호가 현재 어느 자리에 있는지 찾기
        idx_a = -1;
        idx_b = -1;
        for(i = 0; i < total; i++)
        {
            if(sit_number[i] == a) idx_a = i;
            if(sit_number[i] == b) idx_b = i;
        }

        // 제외된 번호는 배열에 없으니 -1로 남음
        if(idx_a == -1 || idx_b == -1)
        {
            printf("오류! 제외된 번호는 바꿀 수 없습니다.\n");
            continue;
        }

        temp = sit_number[idx_a];
        sit_number[idx_a] = sit_number[idx_b];
        sit_number[idx_b] = temp;
        return;
    }
}
void save_result(int sit_number[], int row, int column,
                 int fixed_number, int fixed_row, int fixed_column,
                 int exclude_number)
{
    FILE *fp;
    int i, j, count = 0;

    fp = fopen("seat_result.txt", "w");
    if(fp == NULL)
    {
        printf("\n파일을 열 수 없습니다.\n");
        return;
    }

    // 머리말
    fprintf(fp, "자리 배치 결과 (%d행 %d열)\n", row, column);
    fprintf(fp, "고정 번호: %d번 (%d행 %d열)\n", fixed_number, fixed_row, fixed_column);

    if(exclude_number != 0)
        fprintf(fp, "제외 번호: %d번\n\n", exclude_number);
    else
        fprintf(fp, "제외 번호: 없음\n\n");

    // 위쪽 테두리
    for(j = 0; j < column; j++)
        fprintf(fp, "+----");
    fprintf(fp, "+\n");

    // 좌석 + 아래쪽 테두리
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < column; j++)
        {
            if(sit_number[count] == 0)
                fprintf(fp, "|    ");            // 빈 자리
            else
                fprintf(fp, "| %2d ", sit_number[count]);
            count++;
        }
        fprintf(fp, "|\n");

        for(j = 0; j < column; j++)
            fprintf(fp, "+----");
        fprintf(fp, "+\n");
    }

    fclose(fp);
    printf("\nseat_result.txt 파일에 저장했습니다.\n");
}

void set_color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int start_screen(void)
{
    const char *items[3] = {"실행", "도움말", "종료"};
    int choice;

    while(1)
    {
        system("cls");

        print_center(1, "==============================");
        print_center(2, "자리 배치 프로그램");
        print_center(3, "==============================");

        choice = select_menu(items, 3, -1, 5);

        if(choice == -1)           // 창 크기가 바뀜 → 처음부터 다시 그림
            continue;

        choice++;                  // 1~3으로 변환 (이하 분기는 그대로)

        if(choice == 1)
        {
            system("cls");
            return 1;
        }
        else if(choice == 2)
            show_help();
        else
            return 0;
    }
}

void show_help(void)
{
    FILE *fp;
    char line[200];

    system("cls");

    fp = fopen("help.txt", "r");
    if(fp == NULL)
    {
        printf("도움말 파일(help.txt)을 찾을 수 없습니다.\n");
        printf("실행 파일과 같은 폴더에 help.txt가 있는지 확인하세요.\n\n");
        system("pause");
        return;
    }

    while(fgets(line, sizeof(line), fp) != NULL)   // 한 줄씩 읽어서 그대로 출력
        printf("%s", line);

    fclose(fp);

    printf("\n\n");
    system("pause");
}

void play_select_sound(void)
{
    Beep(800, 50);          // 짧은 "띡" 소리
}

void play_done_sound(void)
{
    Beep(523, 80);          // 도
    Beep(659, 80);          // 미
    Beep(784, 150);         // 솔  (상승음으로 완료 느낌)
}

// 격자 안의 숫자만 찍는 함수 (색상 규칙 포함)
void print_numbers(int sit_number[], int row, int column,
                   int fixed_number, int x0)
{
    int i, j, n, count = 0, len, left;
    int w = cell_w() - 1;               // 칸 안쪽 글자 폭
    char text[16];

    for(i = 1; i <= row; i++)
    {
        for(j = 1; j <= column; j++)
        {
            n = sit_number[count++];
            gotoxy(x0 + (j - 1) * cell_w() + 1, 6 + i * 2);

            if(n == 0)                              // 제외된 자리
            {
                strcpy(text, "--");
                set_color(COLOR_GRAY);
            }
            else
            {
                if(g_use_names)
                    strcpy(text, g_name[n]);        // 이름 모드
                else
                    sprintf(text, "%2d", n);        // 번호 모드 (기존과 동일)

                if(n == fixed_number)
                    set_color(COLOR_YELLOW);        // 고정 학생은 노란색 우선
                else if(g_use_names)
                    set_color(g_gender[n] == 1 ? COLOR_BLUE : COLOR_PINK);
            }

            len = (int)strlen(text);
            left = (w - len) / 2;                   // 가운데 맞추기용 왼쪽 공백
            printf("%*s%s%*s", left, "", text, w - len - left, "");

            set_color(COLOR_DEFAULT);
        }
    }
}
// 여러 번 섞으며 보여주는 애니메이션 (마지막 프레임이 최종 결과)
void shuffle_animation(int sit_number[], int total, int fixed_index,
                       int row, int column, int fixed_number)
{
    int x0 = center_x(column * cell_w() + 1);

	system("cls");
	print_center(1, "추첨 중입니다...");

	draw_check02(column, row, x0, 7);

	for(int frame = 0; frame < ANIM_FRAMES; frame++)
	{
    	shuffle_number(sit_number, total, fixed_index);
    	print_numbers(sit_number, row, column, fixed_number, x0);   // x0 추가
    	Sleep(40 + frame * 12);
	}

    Sleep(300);                         // 결과 확정 전 잠깐 멈춤
	}

// 콘솔 깜빡이는 커서를 숨기거나 보이게 함
void set_cursor_visible(int visible)
{
    CONSOLE_CURSOR_INFO info;

    info.dwSize = 1;
    info.bVisible = visible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

// 방향키로 항목을 고르는 메뉴. 선택한 항목의 번호(0부터)를 반환
int select_menu(const char *items[], int count, int x, int y)
{
    int cursor = 0;
    int i, key, len, maxlen = 0;
    int width = get_console_width();      // 처음 그릴 때의 창 너비

    if(x < 0)                              // x가 -1이면 가운데 정렬
    {
        for(i = 0; i < count; i++)
        {
            len = (int)strlen(items[i]);
            if(len > maxlen)
                maxlen = len;
        }
        x = center_x(maxlen + 3);          // ▷와 공백 3칸 + 가장 긴 항목
    }

    set_cursor_visible(0);

    for(i = 0; i < count; i++)
    {
        gotoxy(x + 3, y + i);
        printf("%s", items[i]);
    }

    while(1)
    {
        for(i = 0; i < count; i++)
        {
            gotoxy(x, y + i);
            if(i == cursor)
                printf("▷");
            else
                printf("  ");
        }

        while(!_kbhit())                   // 키를 기다리는 동안 창 크기 감시
        {
            if(get_console_width() != width)
            {
                set_cursor_visible(1);
                return -1;                 // 크기가 바뀌었으니 다시 그려 달라는 신호
            }
            Sleep(30);
        }

        key = _getch();

        if(key == 224 || key == 0)
        {
            key = _getch();

            if(key == KEY_UP)
                cursor = (cursor - 1 + count) % count;
            else if(key == KEY_DOWN)
                cursor = (cursor + 1) % count;
        }
        else if(key == KEY_ENTER)
        {
            play_select_sound();
            gotoxy(1, y + count + 1);
            set_cursor_visible(1);
            return cursor;
        }
    }
}
// 현재 보이는 콘솔 창의 가로 칸 수
int get_console_width(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

// 가로 width칸짜리 내용을 가운데에 놓으려면 시작해야 할 x좌표 (1부터)
int center_x(int width)
{
    int x = (get_console_width() - width) / 2 + 1;

    if(x < 1)
        x = 1;          // 창이 내용보다 좁으면 왼쪽 끝에서 시작
    return x;
}

// y번째 줄에 글자를 가운데 정렬로 출력
void print_center(int y, const char *text)
{
    gotoxy(center_x((int)strlen(text)), y);
    printf("%s", text);
}

int confirm(const char *msg)
{
    const char *items[2] = {"예", "아니오"};
    int result;

    do {
        system("cls");
        print_center(5, msg);                    // 질문을 가운데에 출력
        result = select_menu(items, 2, -1, 7);   // 방향키 메뉴 재활용
    } while(result == -1);                       // 창 크기가 바뀌면 다시 그림

    return (result == 0);                        // "예"(0번)를 골랐을 때만 1
}

void find_seat(int sit_number[], int total, int column)
{
    int n, i, found = 0;

    n = input_int_range("조회할 번호를 입력하세요", 1, total);

    for(i = 0; i < total; i++)
    {
        if(sit_number[i] == n)
        {
            printf("\n%d번 학생의 자리: %d행 %d열\n",
                   n, i / column + 1, i % column + 1);
            found = 1;
            break;
        }
    }

    if(!found)
        printf("\n%d번은 제외된 번호라 자리가 없습니다.\n", n);

    system("pause");
}

void pick_presenter(int sit_number[], int total, int column)
{
    int i, count = 0, n = 0, target;

    for(i = 0; i < total; i++)          // ① 배치된 학생 수 세기 (빈 자리 0 제외)
        if(sit_number[i] != 0)
            count++;

    target = rand() % count;            // ② 몇 번째 학생을 뽑을지 결정

    for(i = 0; i < total; i++)          // ③ 그 학생의 위치 찾기
    {
        if(sit_number[i] != 0)
        {
            if(n == target)
                break;
            n++;
        }
    }

    printf("\n발표자: %d번 학생 (%d행 %d열)\n",
           sit_number[i], i / column + 1, i % column + 1);
    play_done_sound();
    system("pause");
}

void make_teams(int sit_number[], int total)
{
    int list[100], count = 0;
    int i, j, t, k, tmp, teams;

    for(i = 0; i < total; i++)               // ① 배치된 학생만 모으기 (빈 자리 제외)
        if(sit_number[i] != 0)
            list[count++] = sit_number[i];

    teams = input_int_range("조의 수를 입력하세요", 1, count);

    for(i = count - 1; i > 0; i--)           // ② 학생 목록을 무작위로 섞기
    {
        j = rand() % (i + 1);
        tmp = list[i];
        list[i] = list[j];
        list[j] = tmp;
    }

    for(t = 0; t < teams; t++)               // ③ 번갈아 나눠주며 조별 출력
    {
        printf("\n%d조:", t + 1);
        for(k = t; k < count; k += teams)
            printf(" %d", list[k]);
    }
    printf("\n\n");

    play_done_sound();
    system("pause");
}
void rotate_seats(int sit_number[], int total, int fixed_index)
{
    int list[100], count = 0;
    int i, last;

    for(i = 0; i < total; i++)               // ① 고정 자리를 뺀 값들을 순서대로 모음
        if(i != fixed_index)
            list[count++] = sit_number[i];

    if(count < 2)                            // 움직일 학생이 없으면 종료
        return;

    last = list[count - 1];                  // ② 맨 마지막 값을 기억해 두고
    for(i = count - 1; i > 0; i--)           //    나머지를 한 칸씩 뒤로 밀고
        list[i] = list[i - 1];
    list[0] = last;                          //    마지막 값을 맨 앞에 넣음

    count = 0;
    for(i = 0; i < total; i++)               // ③ 다시 배열에 채워 넣기
        if(i != fixed_index)
            sit_number[i] = list[count++];
}
// 격자 한 칸의 가로 폭 (테두리 포함)
int cell_w(void)
{
    return g_use_names ? 9 : 5;
}

// 표시 방식 선택: 0 = 번호만, 1 = 이름·성별
int choose_display_mode(void)
{
    const char *items[2] = {"번호로만 표시 (기존 방식)", "이름과 성별 입력"};
    int result;

    do {
        system("cls");
        print_center(5, "표시 방식을 선택하세요");
        result = select_menu(items, 2, -1, 7);
    } while(result == -1);              // 창 크기가 바뀌면 다시 그림

    return result;
}

// 1번부터 total번까지 이름과 성별을 한 명씩 입력
void input_students(int total)
{
    int i;
    char buf[100];

    system("cls");
    printf("학생 %d명의 이름과 성별을 입력하세요.\n", total);
    printf("(이름은 한글 4자 / 영문 8자 이내, 공백 없이)\n\n");

    for(i = 1; i <= total; i++)
    {
        while(1)
        {
            printf("%d번 이름: ", i);
            scanf("%99s", buf);
            while(getchar() != '\n');   // 같은 줄에 남은 입력 버리기

            if(strlen(buf) <= 8)
                break;
            printf("오류! 이름은 한글 4자 / 영문 8자 이내로 입력하세요.\n");
        }

        strcpy(g_name[i], buf);
        g_gender[i] = input_int_range("성별 1:남 2:여", 1, 2);
    }
}

void show_history(int history[][100], int hist_count, int draw_count,
                  int row, int column)
{
    int h, i, j;

    system("cls");
    printf("추첨 이력 (최근 %d회)\n", hist_count);

    for(h = 0; h < hist_count; h++)
    {
        printf("\n[%d회차]\n", draw_count - hist_count + 1 + h);

        for(i = 0; i < row; i++)
        {
            for(j = 0; j < column; j++)
            {
                if(history[h][i * column + j] == 0)
                    printf(" --");
                else
                    printf("%3d", history[h][i * column + j]);
            }
            printf("\n");
        }
    }

    printf("\n");
    system("pause");
}
// 테두리 기호 하나를 출력. 분단 경계(j열 앞)이면 초록색으로 출력
void put_border(const char *s, int j, int c)
{
    if(g_group > 0 && j > 0 && j < c && j % g_group == 0)
        set_color(COLOR_GREEN);

    printf("%s", s);
    set_color(COLOR_DEFAULT);
}
