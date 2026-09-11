정사각형 출력 프로그램 수정
1. 과제 내용

이번 주차에는 정사각형의 크기를 입력받아 화면에 정사각형을 출력하는 C 코드가 주어졌다.
원본 코드는 특정 환경에서는 정상적으로 출력되지만, Visual Studio 등의 UTF-8 환경에서 정사각형의 우측 모서리가 정상적으로 이어지지 않는 문제가 발생한다.

따라서 해당 문제의 원인을 파악하고, 다양한 환경에서도 정사각형이 정상적으로 출력되도록 코드를 수정하였다.

2. 원본 코드
#include <stdio.h>

void draw_square(int size);

int main(void)
{
    int n;

    printf("정사각형 그리기\n\n");
    printf("정사각형의 길이(최대 37)를\n");
    printf("입력하고 Enter>");
    scanf("%d", &n);

    draw_square(n);

    return 0;
}

void draw_square(int size)
{
    int i, j;
    unsigned char a = 0xa6;
    unsigned char b[7];

    for(i = 1; i < 7; i++)
        b[i] = 0xa0 + i;

    printf("%c%c", a, b[3]);

    for(i = 0; i < size; i++)
        printf("%c%c", a, b[1]);

    printf("%c%c", a, b[4]);
    printf("\n");

    for(i = 0; i < size; i++)
    {
        printf("%c%c", a, b[2]);

        for(j = 0; j < size; j++)
            printf(" ");

        printf("%c%c", a, b[2]);
        printf("\n");
    }

    printf("%c%c", a, b[6]);

    for(i = 0; i < size; i++)
        printf("%c%c", a, b[1]);

    printf("%c%c", a, b[5]);
    printf("\n");
}

3. 원본 코드의 문제점

원본 코드에서는 ┌, ─, ┐, │, └, ┘ 등의 특수문자를 직접 사용하는 대신, 다음과 같이 16진수 코드값을 이용하여 두 개의 바이트를 출력하고 있다.

unsigned char a = 0xa6;
unsigned char b[7];

for(i = 1; i < 7; i++)
    b[i] = 0xa0 + i;


이후 다음과 같은 방식으로 특수문자를 출력한다.

printf("%c%c", a, b[3]);


이 방식은 특정 문자 인코딩 환경에서는 정상적으로 동작한다. 예를 들어 CP949 계열 환경에서는 해당 바이트 조합이 상자 문자로 정상적으로 해석될 수 있다.

하지만 UTF-8 환경에서는 동일한 방식으로 문자를 처리하지 않기 때문에, 특수문자의 출력 폭이 예상과 달라지거나 정사각형의 오른쪽 테두리가 어긋나 보이는 문제가 발생할 수 있다.

즉, 정사각형을 그리는 반복문 자체에 문제가 있는 것이 아니라, 특수문자를 출력하는 방식이 특정 문자 인코딩 환경에 의존한다는 점이 문제이다.

4. 수정 방법

문자 코드를 이용하여 특수문자를 출력하는 대신, 특수문자를 문자 그대로 소스 코드에 작성하여 출력하도록 수정하였다.

예를 들어 기존의

printf("%c%c", a, b[3]);


대신 다음과 같이 작성한다.

printf("┌");


같은 방식으로 정사각형을 구성하는 각 문자를 직접 출력한다.

┌ : 왼쪽 위 모서리
─ : 가로선
┐ : 오른쪽 위 모서리
│ : 세로선
└ : 왼쪽 아래 모서리
┘ : 오른쪽 아래 모서리

이렇게 하면 코드가 특정 바이트 조합에 의존하지 않고, UTF-8 환경에서 해당 문자를 직접 출력할 수 있다.

5. 수정 코드
#include <stdio.h>

void draw_square(int size);

int main(void)
{
    int n;

    printf("정사각형 그리기\n\n");
    printf("정사각형의 길이(최대 37)를\n");
    printf("입력하고 Enter>");
    scanf("%d", &n);

    draw_square(n);

    return 0;
}

void draw_square(int size)
{
    int i, j;

    // 상단 테두리
    printf("┌");

    for(i = 0; i < size; i++)
        printf("─");

    printf("┐\n");

    // 좌우 테두리와 내부
    for(i = 0; i < size; i++)
    {
        printf("│");

        for(j = 0; j < size; j++)
            printf(" ");

        printf("│\n");
    }

    // 하단 테두리
    printf("└");

    for(i = 0; i < size; i++)
        printf("─");

    printf("┘\n");
}

6. 실행 결과

예를 들어 정사각형의 길이를 5로 입력하면 다음과 같이 출력된다.

┌─────┐
│     │
│     │
│     │
│     │
│     │
└─────┘

7. 결과 및 느낀 점

이번 과제를 통해 C 언어에서 문자를 출력할 때 문자 인코딩에 따라 출력 결과가 달라질 수 있다는 점을 알게 되었다.

원본 코드는 0xA6 등의 바이트 값을 이용하여 특수문자를 출력하기 때문에 특정 인코딩 환경에서는 정상적으로 동작하지만, UTF-8과 같은 다른 환경에서는 출력 결과가 달라질 수 있다.

따라서 특수문자를 직접 작성하여 출력하도록 코드를 수정하였으며, 이를 통해 코드의 가독성을 높이고 UTF-8 환경에서도 정사각형이 정상적으로 출력되도록 수정하였다.
