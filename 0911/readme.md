# 커서의 위치 제어

## 화면 지우기

```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char ch;

    printf("문자를 입력하고 Enter>");
    scanf("%c", &ch);

    system("cls");

    printf("입력된 문자 %c\n", ch);

    return 0;
}
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main(void)
{
    int i, j;

    for(j = 1; j <= 9; j++)
    {
        system("cls");

        for(i = 1; i <= 9; i++)
            printf("%d*%d=%d\n", j, i, j * i);

        printf("아무키나 누르시오.\n");
        getch();
    }

    return 0;
}
```

# 문자열 문제

```c
#include <stdio.h>

int main()
{
    char string[20];
    char c;

    scanf("%s", string);
    scanf("%c", &c);

    printf("%s\n", string);
    printf("!!%c!!\n", c);

    return 0;
}
```

다음으로 입력하면 문자열만 입력받고 끝나버린다.

원인은 `scanf("%s", string)` 문자열 입력이 실행되고 나서 입력버퍼에 개행문자가 그대로 남아 있는 것인데,

해결법은 두 개의 `scanf` 사이에 입력버퍼를 비워주면 되는데, `getchar();`을 사이에 추가하거나, `scanf("%c", &c);`를 `scanf(" %c", &c);`로 바꿔서 한 칸 띄워보면 공백을 구분자로 인식한다.

`scanf("%*c", c);`의 경우 입력은 받으나 저장하지는 않는다.

# 아스키 코드 & 스캔 코드

아스키코드: 컴퓨터 내부에서 문자 처리하기 위한 규칙. 전 세계가 동일하다.

스캔 코드: 각각의 키보드 키에 대한 코드값으로, 키보드마다 따로 입력되어 있음. 그래서 키보드의 모든 버튼에는 아스키와 스캔 2가지가 저장되어 있음.

스캔 코드는 확장 코드를 의미하며, 아스키코드에 해당하지 않는 키를 포함한다. 예를 들어 방향키, pgup, home, end 등이 있다.

```c
#include <stdio.h>
#include <conio.h>

int main(void)
{
    int chr;

    do
    {
        chr = getch();

        if (chr == 0 || chr == 0xe0)
        {
            // 의미: 0 또는 224를 반환한다.
            chr = getch();
            printf("확장키 code=%d\n", chr);
        }
        else
            printf("아스키 code=%d\n", chr);

    } while(1);

    return 0;
}
```

이 코드 예시처럼, 확장키가 입력될 경우 0, 224를 반환하므로 이걸 통해 확장키를 얻을 수 있다. 그렇지 않은 경우는 아스키이다.

또한 스캔과 아스키코드를 모두 저장하려면 2바이트가 필요하다.

# 방향키를 이용한 이동

```c
void move_arrow_key(char key, int *x1, int *y1, int x_b, int y_b)
{
    switch(key)
    {
        case 72: // 위쪽(상) 방향의 화살표 키 입력
            *y1 = *y1 - 1;
            if (*y1 < 1) *y1 = 1; // y좌표의 최소값
            break;

        case 75: // 왼쪽(좌) 방향의 화살표 키 입력
            *x1 = *x1 - 1;
            if (*x1 < 1) *x1 = 1; // x좌표의 최소값
            break;

        case 77: // 오른쪽(우) 방향의 화살표 키 입력
            *x1 = *x1 + 1;
            if (*x1 > x_b) *x1 = x_b; // x좌표의 최대값
            break;

        case 80: // 아래쪽(하) 방향의 화살표 키 입력
            *y1 = *y1 + 1;
            if (*y1 > y_b) *y1 = y_b; // y좌표의 최대값
            break;

        default:
            return;
    }
}
```

이 코드 예시에서 알 수 있는 것은:

위쪽 화살표는 72에, 아래쪽은 80에, 좌는 75에, 우는 77에 대응되는 걸 알 수 있다.

그리고 실행 시 방향키 위쪽을 누르면 y가 y--가 되어서 1 줄어드는 등 실제 화살표 키의 방향대로 한 칸씩 움직이는 걸 볼 수 있다.

# 정사각형의 표현

모서리를 나타내는 기호로 출력해야 하는데, 확장코드 `0xa3`(163), `0xa4`(164), `0xa5`(165), `0xa6`(166)이 각 네 모서리를 나타내는 기호로 표현되기에 이들을 활용한다.
