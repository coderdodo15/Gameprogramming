**숫자를 문자로 처리**: 문자열 길이를 순서대로 혹은 역순으로 처리.  
(number(0) = '7')

```c
#include <stdio.h>
#include <string.h>
int main(void)
{
char number[20];
int length, i;
printf("금액을 입력하고 Enter>");
scanf("%s", number);
length=strlen(number);
for(i=length;i>=0;i--)
printf("%c\n", number[i]);
return 0;
}
```  
  
**출력 로직**: 예를 들어 입력 정수가 756이라면 다음과 같이 나눈다.  
 | 구분| 
| --- |  
|756/100=7|  
|56/10=5|  
|6/1=6|  


**디지털 숫자**: 입력된 숫자를 픽셀 형태로 5줄을 출력한다. 예를 들어 123이라면   
