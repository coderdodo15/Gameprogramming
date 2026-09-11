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

    // 중간 빈 공간과 좌우 벽
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
