게임프로그래밍 1주차
1. 개발 환경
VS Code 또는 **Dev-C++**을 사용한다.
2. 커서 위치 제어
2.1 기본 코드

gotoxy() 함수를 사용하여 콘솔 화면에서 커서의 위치를 지정할 수 있다.

void gotoxy(int x, int y) {
    COORD Pos = {x - 1, y - 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

2.2 gotoxy() 함수의 의미
gotoxy(x, y);

x : 가로 좌표
y : 세로 좌표
지정한 (x, y) 위치로 콘솔 커서를 이동한다.
x - 1, y - 1을 사용하는 이유는 콘솔의 좌표가 0부터 시작하기 때문이다.
3. 좌표를 지정하여 Hello 출력하기

다음 코드는 (2, 4)와 (40, 20) 위치에 각각 Hello를 출력한다.

#include <stdio.h>
#include <windows.h>

void gotoxy(int x, int y);

int main(void) {
    gotoxy(2, 4);
    printf("Hello");

    gotoxy(40, 20);
    printf("Hello");

    return 0;
}

void gotoxy(int x, int y) {
    COORD Pos = {x - 1, y - 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}


핵심: gotoxy(x, y)를 이용하면 원하는 좌표에 문자를 출력할 수 있다.

4. 화면 지우기
4.1 <conio.h>
#include <conio.h>


콘솔 입출력을 제어하기 위한 헤더 파일이다.

대표적으로 다음과 같은 함수를 사용할 수 있다.

getch()
getche()
4.2 system("cls")
system("cls");


콘솔 화면에 출력되어 있는 내용을 전체 삭제한다.

즉, 현재 화면을 깨끗하게 지우고 다시 출력할 수 있다.

참고: system("cls")는 Windows 콘솔 환경에서 사용하는 명령이다.

5. 1주차 핵심 정리
기능	코드	역할
커서 이동	gotoxy(x, y)	지정한 좌표로 커서 이동
화면 지우기	system("cls")	콘솔 화면 전체 삭제
콘솔 입출력	#include <conio.h>	getch(), getche() 등의 함수 사용
Windows 기능	#include <windows.h>	COORD, SetConsoleCursorPosition() 등의 기능 사용
기억할 코드
gotoxy(10, 5);
printf("Hello");


→ 콘솔의 (10, 5) 위치에 Hello 출력

system("cls");


→ 콘솔 화면 전체 삭제
