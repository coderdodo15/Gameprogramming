게임프로그래밍 1주차 수업.

[개발 환경]
-vscode나 Dev C++을 사용한다.

[커서 위치 제어 코드]
<기본>
void gotoxy(int x, int y) {
  COORD Pos = {x - 1, y - 1};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

<x, y 좌표를 지정하여 Hello를 출력하기>
#include <stdio.h>
#include <windows.h>
void gotoxy(int x, int y);

int main(void){
  gotoxy(2,4);
  printf("Hello");
  gotoxy(40, 20);
  printf("Hello");
  return 0;
}

void gotoxy(int x, int y){
  COORD Pos = {x - 1, y - 1};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
//커서 위치 제어

<화면 지우기>
#include <conio.h> - 화면 콘솔 입출력 제어 헤더. getch, getche 등을 사용 가능하게 한다.
system("cls") 사용시 프롬포트의 모든 화면이 깨끗하게 지워진다.
