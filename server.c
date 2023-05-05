#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256
#define BACKLOG 5

int main(int argc, char *argv[])
{
  int server_sockfd;
  int client_sockfd_p1;
  int client_sockfd_p2;

  struct sockaddr_in echo_server_addr;
  struct sockaddr_in echo_client_addr_p1;
  struct sockaddr_in echo_client_addr_p2;

  int client_len_p1;
  int client_len_p2;

  char echo_message[MAX_BUFFER_SIZE + 1];
  char return_message[MAX_BUFFER_SIZE + 1];
  int return_message_len;

  char pan_cord_1p[MAX_BUFFER_SIZE + 1];
  char pan_cord_2p[MAX_BUFFER_SIZE + 1];

#ifdef NOREUSEADDR
  int nonzero = 1;
#endif

  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  if ((server_sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    perror("socket() failed.");
    exit(1);
  }

  bzero(&echo_server_addr, sizeof(echo_server_addr));
  echo_server_addr.sin_family = AF_INET;
  echo_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  echo_server_addr.sin_port = htons(atoi(argv[1]));

#ifdef NOREUSEADDR
  if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &nonzero, sizeof(nonzero)) < 0)
  {
    perror("setsockopt() failed.");
    exit(1);
  }
#endif

  if (bind(server_sockfd, (struct sockaddr *)&echo_server_addr, sizeof(echo_server_addr)) < 0)
  {
    perror("bind() failed.");
    exit(1);
  }

  if (listen(server_sockfd, BACKLOG) < 0)
  {
    perror("listen() failed.");
    exit(1);
  }

  while (1)
  {
    client_len_p1 = sizeof(echo_client_addr_p1);
    if ((client_sockfd_p1 = accept(server_sockfd, (struct sockaddr *)&echo_client_addr_p1, &client_len_p1)) < 0)
    {
      perror("accept() failed to player 1.");
      exit(1);
    }

    client_len_p2 = sizeof(echo_client_addr_p2);
    if ((client_sockfd_p2 = accept(server_sockfd, (struct sockaddr *)&echo_client_addr_p2, &client_len_p2)) < 0)
    {
      perror("accept() failed to player 2.");
      exit(1);
    }

    if (return_message_len = recv(client_sockfd_p1, return_message, MAX_BUFFER_SIZE, 0) < 0)
    {
      perror("recv() failed.");
      exit(1);
    }
    return_message[return_message_len] = '\0';
    strcpy(pan_cord_1p, return_message);

    if (return_message_len = recv(client_sockfd_p2, return_message, MAX_BUFFER_SIZE, 0) < 0)
    {
      perror("recv() failed.");
      exit(1);
    }
    return_message[return_message_len] = '\0';
    strcpy(pan_cord_2p, return_message);

    int battle_flag = 1;
    int win_flag_p1 = 1;
    int win_flag_p2 = 1;
    while (battle_flag)
    {
      int i, j;

      int hit = 0;
      int blow = 0;
      char rival_pen_cord[MAX_BUFFER_SIZE + 1];

      if (win_flag_p1 == 1)
      {
        snprintf(echo_message, MAX_BUFFER_SIZE, "Your turn.\n");
        write(client_sockfd_p1, echo_message, strlen(echo_message));
        snprintf(echo_message, MAX_BUFFER_SIZE, "Set rival pen cord: ");
        write(client_sockfd_p1, echo_message, strlen(echo_message));

        if (return_message_len = recv(client_sockfd_p1, return_message, MAX_BUFFER_SIZE, 0) < 0)
        {
          perror("recv() failed.");
          exit(1);
        }
        strcpy(rival_pen_cord, return_message);

        for (i = 0; i < 4; i++)
        {
          for (j = 0; j < 4; j++)
          {
            if (pan_cord_2p[i] == rival_pen_cord[j])
            {
              if (i == j)
              {
                hit++;
              }
              else
              {
                blow++;
              }
            }
          }
        }

        if (hit == 4)
        {
          snprintf(echo_message, MAX_BUFFER_SIZE, "You win!\n");
          write(client_sockfd_p1, echo_message, strlen(echo_message));
          snprintf(echo_message, MAX_BUFFER_SIZE, "You lose!\n");
          write(client_sockfd_p2, echo_message, strlen(echo_message));
          battle_flag = 0;
          win_flag_p2 = 0;
        }
        else
        {
          snprintf(echo_message, MAX_BUFFER_SIZE, "Hit: %d, Blow: %d\n", hit, blow);
          write(client_sockfd_p1, echo_message, strlen(echo_message));
        }
      }

      hit = 0;
      blow = 0;
      if (win_flag_p2 == 1)
      {
        snprintf(echo_message, MAX_BUFFER_SIZE, "Your turn.\n");
        write(client_sockfd_p2, echo_message, strlen(echo_message));
        snprintf(echo_message, MAX_BUFFER_SIZE, "Set rival pen cord: ");
        write(client_sockfd_p2, echo_message, strlen(echo_message));

        if (return_message_len = recv(client_sockfd_p2, return_message, MAX_BUFFER_SIZE, 0) < 0)
        {
          perror("recv() failed.");
          exit(1);
        }
        strcpy(rival_pen_cord, return_message);

        for (i = 0; i < 4; i++)
        {
          for (j = 0; j < 4; j++)
          {
            if (pan_cord_1p[i] == rival_pen_cord[j])
            {
              if (i == j)
              {
                hit++;
              }
              else
              {
                blow++;
              }
            }
          }
        }

        if (hit == 4)
        {
          snprintf(echo_message, MAX_BUFFER_SIZE, "You win!\n");
          write(client_sockfd_p2, echo_message, strlen(echo_message));
          snprintf(echo_message, MAX_BUFFER_SIZE, "You lose!\n");
          write(client_sockfd_p1, echo_message, strlen(echo_message));
          battle_flag = 0;
          win_flag_p1 = 0;
        }
        else
        {
          snprintf(echo_message, MAX_BUFFER_SIZE, "Hit: %d, Blow: %d\n", hit, blow);
          write(client_sockfd_p2, echo_message, strlen(echo_message));
        }
      }
    }

    close(client_sockfd_p1);
    close(client_sockfd_p2);
  }

  return 0;
}
