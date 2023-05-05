#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256
#define STRG 256

int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in echo_serv_addr;
  char echo_buffer[MAX_BUFFER_SIZE + 1];
  char echo_string[STRG];
  int echo_string_len;
  char return_message[MAX_BUFFER_SIZE + 1];
  int return_message_len;
  int mode_set = -1;

  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <Server IP> <Port>\n", argv[0]);
    exit(1);
  }

  while (1)
  {
    printf("Prease choose mode: \n");
    printf("1. PC / 2. CPU / 3. EXIT\n > ");
    scanf("%d", &mode_set);

    if (mode_set == 1)
    {
      if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      {
        perror("socket() failed");
        exit(1);
      }
      bzero(&echo_serv_addr, sizeof(echo_serv_addr));
      echo_serv_addr.sin_family = AF_INET;
      echo_serv_addr.sin_port = inet_addr(argv[2]);

      if (inet_pton(AF_INET, argv[1], &echo_serv_addr.sin_addr) <= 0)
      {
        perror("inet_pton() failed");
        exit(1);
      }

      if (connect(sockfd, (struct sockaddr *)&echo_serv_addr, sizeof(echo_serv_addr)) < 0)
      {
        perror("connect() failed");
        exit(1);
      }

      printf("Connected to %s\n", argv[1]);
      do
      {
        printf("Please enter four-digit pen code: ");
        scanf("%s", echo_string);

        if (strlen(echo_string) != 4)
        {
          printf("Please enter four-digit pen code\n");
        }
      } while (strlen(echo_string) != 4);
      write(sockfd, echo_string, strlen(echo_string));

      while (1)
      {
        if ((return_message_len = read(sockfd, return_message, MAX_BUFFER_SIZE)) < 0)
        {
          perror("read() failed");
          exit(1);
        }
        return_message[return_message_len] = '\0';
        printf("%s\n", return_message);

        if (return_message[0] == '!')
        {
          printf("\n");
          close(sockfd);
          break;
        }

        do
        {
          scanf("%s", echo_string);
          if (strlen(echo_string) != 4)
          {
            printf("Please enter four-digit pen code\n");
          }
        } while (strlen(echo_string) != 4);
        {
          write(sockfd, echo_string, strlen(echo_string));

          if ((return_message_len = read(sockfd, return_message, MAX_BUFFER_SIZE)) < 0)
          {
            perror("read() failed");
            exit(1);
          }
          return_message[return_message_len] = '\0';
          printf("%s\n", return_message);

          if (return_message[0] == '!')
          {
            close(sockfd);
            break;
          }
        }
      }
    }
    else if (mode_set == 2)
    {
      printf("CPU mode\n");
      printf("Not yet implemented\n");
    }
    else if (mode_set == 3)
    {
      printf("EXIT\n");
      exit(1);
    }
    else
    {
      printf("Wrong input\n");
    }
  }
}