[3주차]
메뉴 만들기.

```c 
#include <stdio.h>  
#include <conio.h>  
#include <stdlib.h>  
int menu_display(void);  
void hamburger(void);  
void spaghetti(void);  
void press_any_key(void);  
int main(void){ //프로토타입  
	int c;  
	while((c=menu_display()) != 3){  
	switch(c){  
		case 1 : hamburger();  
		break;  
		case 2 : spaghetti();  
		break;  
		default : break;  
		}  
	}  
	return 0;  
}  
int menu_display(void){  
int select;  
system("cls");  
printf("간식 만들기\n\n");  
printf("1. 햄버거 \n");
printf("2. 스파게티\n");
printf("3. 프로그램 종료\n\n");
printf("메뉴번호 입력>");
select=getch()-48;
return select;
}
void hamburger(void){//여기부터 모듈화
system("cls");
printf("햄버거 만드는 방법\n");
printf("중략\n");
press_any_key();
}
void spaghetti(void) {
	system("cls");
	printf("스파게티 만드는 방법\n");
	printf("중략\n");
	press_any_key();
}
void press_any_key(void){
	printf("\n\n");
	printf("아무키나 누르면 메인 메뉴로...");
	getch();  //pos
}
```

//실제 실무에서 main 안에서는 코드가 그렇게 많이 들어가있지는 않음.
수식의 구성요소: assingment statement
```C
a = 1(literal)
a =b(variable)
a = b+1(operator)
a= sum(1,2)*6(function)
```
select = getch()-48이 있는 경우 아스키 코드값 49(=1)을 받기 때문이다.

random = 범위 내에서 난수 생성.
(ex. 1<= 난수 <= 6은 rand()%6 +1로 씀. 6으로 나눈 나머지 값 의 범위는 0-5인데 거기에 1을 더하기 때문에 1~6 사이의 난수가 생성된다) 
(a <= x <= b 라 하면 일반적으로 rand()%(b-a+1) + a;로 생각하면 된다.)
* 엑셀의 RAND, RANDBETWEEN 등도 비슷하게 구현되 있다.

seed: 
srandom(seed random): 랜덤 숫자를 뽑기 전에 시작점을 생성하라는 뜻.
srand(time(NULL)): srand 자체는 시드값이 고정되기 때문에, 매번 바뀌는 time(NULL), 즉 시간 개념을 넣어 시드값이 매번 바뀔수 있도록 한다.

중복없는 난수: 
vba(viaual basic): 

가변인수: 인수의 갯수가 고정되지 않고 변하는 인수. 예를 들어 printf를 사용할때 값 출력 변수는 경우에 따라 달라질수 있고, scanf 역시 입력 변수의 갯수는 고정되어 있지 않음.


[트럼프 카드 표시]
- 트럼프 카드는 4개 모양(♠◆♥♣)과 13장(1~10, J, Q, K)의 카드로 총 52장이 사용됨.
숫자와 모양 두 개의 데이터로 표시되므로 구조체로 표현됨.
```C
structure card{
	char order; // 우선순위
	char number;  // 카드 숫자 혹은 문자.
}

structure trump{
	char order; // 우선순위
	char number;  // 카드의 숫자.
	char shape[3]; //카드의 모양. 3바이트로 지정한 이유는
}

trump card[52];
```

화면 표시가 A,♠ / 2,◆ 이렇다 할때, 실제 저장된 멤버의 값은 1,0 / 2,1 이런식이다.

각 모양의 순위를 지정한 뒤, 2차원 배열에 저장하여 구분. 반복문으로 카드 순위를 특정 멤버에 저장.
카드 만드는 함수는 void makecard(trump m_card)라 할때, int i,j로 2차원 배열을 만듬.
for(i=0;i<4;i++){for(j=i*13;j=i*13;j++){카드 제작}. 이렇게 이중 반복문을 사용한다. i는 카드의 모양을, j는 카드의 숫자를 지정한다(J=11, Q=12, K=13에 해당된다.)
이때 case 11: m_card[j].number='J'; break; 이런 식으로 문자형 카드를 지정할수도 있음.
마찬가지로 case 1: m_card[j].number='A'; break; 이렇게 1은 A(ace)로 표현할수도 있다. ♠️♦️♥️🍀

카드 출력의 경우 void display card(trump m_card[])라고 할때, int i, count=0을 지정.

카드 섞기는 위치를 교환한다. rnd를 발생해 a(0}, a(rnd) 값을 서로 바꾸는 식이다. 이를 a값을 늘려가며 계속 반복한다.
