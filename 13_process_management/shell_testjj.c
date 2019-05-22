#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>


#define MAX_LEN_LINE    10
#define LEN_HOSTNAME	30


int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
  
    while (true) {
        char *s;
        char ss[10][30];
        int len;
        int count=0; //a;b;c와같이 입력받을경우 입력받은 프로그램의 수를 나타내기 위함;
        int c_count =0;

    ///////////////////////////////////////////////////////////////////////////////
    //호스트네임,유저네임 받아서 표시하는 부분
        char hostname[LEN_HOSTNAME + 1];
        memset(hostname, 0x00, sizeof(hostname));
        gethostname(hostname, LEN_HOSTNAME);
            
    //username@hostname $ 형식으로 표현하기위함 

        //printf("@@여러개의 프로그램을 입력하는경우 a; b; c와 같은 형식으로 입력해주세요@@ \n 종료를 원하시면 exit 를 입력하세요\n\n");
        printf("%s@%s $ ",getpwuid(getuid())->pw_name,hostname);
        
        //입력한내용 s에 들어감
        s = fgets(command, MAX_LEN_LINE, stdin);
        len = strlen(command);// 3번째 과제------------정우

          ///////////////////////////////////////////////////////////////////////////////
        //exit 입력시 프로그램종료
        if(strcmp(s,"exit\n") == 0)
        {
            printf("Shell을 종료합니다. \n");
            break;//아예 메인함수 종료
        }
         ///////////////////////////////////////////////////////////////////////////////

       for(int i =0;i<len;i++)
       {
          if( s[i] == ';')
           count++;//;을 받을때마다 카운트 하나씩 추가 = 프로그램 갯수
       }
       
       // count =0; //count 초기화

///
        char *ptr = strtok(s,"; ");
        while(ptr != NULL)
        {
            printf("  %s \n" ,ptr);
            strcpy(ss[c_count],ptr);
            c_count++;
            ptr = strtok(NULL,"; ");
        }
       

for(int i=0;i<c_count;i++)
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    s = ss[i];
    strcpy(command,s);
    len =strlen(command);

        
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);//if문만 탈출
        }
       

        
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);
      
     

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
}
    
    }
    return 0;
}





